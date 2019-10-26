#include <Disk/AHCIController.h>
AHCIController::AHCIController(PCI::Device* device,PCI::Access* access) : GenericDiskController(AHCI_DiskController)
{
	this->initialize(device,access);
}
void AHCIController::initialize(PCI::Device* device,PCI::Access* access)
{
    this->access = access;
    this->device = device;
    this->hba = (AHCI::HBA_MEM*)(
                        PCI::read(access,
                        device->get_segment(),
                        device->get_bus(),
                        device->get_device_number(),
                        device->get_function_number(),
                        AHCI_ABAR_BASE) | 
                        (PCI::read(access,
                        device->get_segment(),
                        device->get_bus(),
                        device->get_device_number(),
                        device->get_function_number(),
                        AHCI_ABAR_BASE+2) << 16)
                    );
	this->tmpbuffer = (uint8_t*)dma_calloc();
	this->tmpbuffer_size = DMARegionSize;
}
bool AHCIController::probe_port_connected(uint8_t port)
{
    if(port > 31)
        return false;
    return (((this->hba->pi) >> port) & 0x1) == 1;
}
bool AHCIController::read(uint8_t port_number,uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
	uint32_t lbal_offseted = lbal + (bytesOffset / this->get_logical_sector_size(port_number));
    uint16_t offset_in_first_lba = bytesOffset % this->get_logical_sector_size(port_number);

    if((offset_in_first_lba + bytesCount) <= this->tmpbuffer_size)
        return this->small_read(port_number,lbal_offseted,lbah,offset_in_first_lba,buf,bytesCount);
    else
    {
        uint32_t buffer_addr = (uint32_t)buf;

        this->small_read(port_number,lbal_offseted,lbah,offset_in_first_lba,buf,(this->tmpbuffer_size - offset_in_first_lba));
        uint16_t bytes_to_read = (bytesCount - this->tmpbuffer_size) + offset_in_first_lba;
        while(bytes_to_read > 0)
        {
            lbal_offseted += (this->tmpbuffer_size / this->get_logical_sector_size(port_number));
            buffer_addr += this->tmpbuffer_size;
            if(bytes_to_read >= this->tmpbuffer_size)
            {
                if(this->small_read(port_number,lbal_offseted,lbah,0,(uint16_t*)buffer_addr,this->tmpbuffer_size) == false)
					return false;
            }
            else
            {
                return this->small_read(port_number,lbal_offseted,lbah,0,(uint16_t*)buffer_addr,bytes_to_read);
            }
            bytes_to_read = bytes_to_read - this->tmpbuffer_size;
        }
    }
	return true;
}
bool AHCIController::small_read(uint8_t port_number,uint32_t lbal,uint32_t lbah,uint16_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{

	if(this->read_ata(port_number,lbal,lbah,(uint16_t*)this->tmpbuffer,this->tmpbuffer_size))
	{
		this->transfer_data(bytesOffset,bytesCount,(uint8_t*)buf);
		return true;
	}
	return false;
}
void AHCIController::transfer_data(uint16_t offset,uint16_t bytesCount,uint8_t* buf)
{
    for(int i=0; i<bytesCount;++i)
        buf[i] = this->tmpbuffer[offset + i];
}
bool AHCIController::read_ata(uint8_t port_number,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount)
{
    AHCI::HBA_PORT* port = this->get_port(port_number);
    port->is = (uint32_t) -1;
	int spin = 0; // Spin lock timeout counter
	int slot = this->find_freeslot(port);
	if (slot == -1)
		return false;

    uint32_t sectorcount = this->get_sector_count(port_number,bytesCount);
 
	AHCI::HBA_CMD_HEADER *cmdheader = (AHCI::HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->command = sizeof(AHCI::FIS_REG_H2D)/sizeof(uint32_t);	// Command FIS size
	cmdheader->prdtl = (uint16_t)((sectorcount-1)>>4) + 1;	// PRDT entries count
 
	AHCI::HBA_CMD_TBL *cmdtbl = (AHCI::HBA_CMD_TBL*)(cmdheader->ctba);
	int i=0;
	// 8K bytes (16 sectors) per PRDT
	for (i=0; i<cmdheader->prdtl-1; i++)
	{
		cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
		cmdtbl->prdt_entry[i].info = ((8*1024-1) & (0x3FFFFF)) | (1<<31);	// 8K bytes (this value should always be set to 1 less than the actual value)
		buf += 4*1024;	// 4K words
		sectorcount -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
	cmdtbl->prdt_entry[i].info = (((sectorcount<<9)-1) & (0x3FFFFF)) | (1<<31);	// 512 bytes per sector
 
	// Setup command
	AHCI::FIS_REG_H2D *cmdfis = (AHCI::FIS_REG_H2D*)(&cmdtbl->cfis);
	memset((uint8_t*)cmdfis,0,sizeof(AHCI::FIS_REG_H2D));
	cmdfis->h.fis_type = AHCI::FIS_TYPE_REG_H2D;
	cmdfis->h.attrs = 0x80;	// Command
	cmdfis->command = ATA_CMD_READ_DMA_EXT;
 
	cmdfis->lba0 = (uint8_t)lbal;
	cmdfis->lba1 = (uint8_t)(lbal>>8);
	cmdfis->lba2 = (uint8_t)(lbal>>16);
	cmdfis->device = 1<<6;	// LBA mode
 
	cmdfis->lba3 = (uint8_t)(lbal>>24);
	cmdfis->lba4 = (uint8_t)lbah;
	cmdfis->lba5 = (uint8_t)(lbah>>8);
 
	cmdfis->countl = sectorcount & 0xFF;
	cmdfis->counth = (sectorcount >> 8) & 0xFF;
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_SR_BSY | ATA_SR_DRQ)) && spin < 1000000)
	{
		spin++;
		if (spin == 1000000)
		{
			return false;
		}
	}
	
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & AHCI_HBA_PxIS_TFES)	// Task file error
		{
			break;
		}
	}
 
	// Check again
	if (port->is & AHCI_HBA_PxIS_TFES)
	{
		return false;
	}
	return true;
}
AHCI::HBA_PORT* AHCIController::get_port(uint8_t port)
{
    return &this->hba->ports[port];
}
int AHCIController::find_freeslot(AHCI::HBA_PORT* port)
{
    uint32_t slots = (port->sact | port->ci);
	for (int i=0; i < AHCI_MAXIMUM_COMMAND_SLOTS; i++)
	{
		if ((slots&1) == 0)
			return i;
		slots >>= 1;
	}
    return -1;
}
uint16_t AHCIController::get_logical_sector_size(uint8_t port)
{
	this->identify(port,(uint16_t*)this->cached_identify_data);
    if((this->cached_identify_data[106] & (1 << 12)) == 0)
        return ATA_LOGICAL_SECTOR_SIZE;
    if(this->cached_identify_data[117] == 0)
        return ATA_LOGICAL_SECTOR_SIZE;
    else
        return this->cached_identify_data[117] << 1;
}
uint16_t AHCIController::get_physical_sector_size(uint8_t port)
{
	this->identify(port,(uint16_t*)&this->cached_identify_data);
    uint16_t alignment = this->cached_identify_data[106] & 0xf;
    uint16_t logical_sector_size = this->get_logical_sector_size(port);
    return logical_sector_size << alignment;
}
uint16_t AHCIController::get_sector_count(uint8_t port,uint16_t bytesCount)
{
    uint16_t sector_size =  this->get_logical_sector_size(port);
    if((bytesCount % sector_size) == 0)
        return (bytesCount/sector_size);
    else
        return (bytesCount/sector_size) + 1;
}
bool AHCIController::identify(uint8_t port_number,uint16_t* buf)
{
	AHCI::HBA_PORT* port = this->get_port(port_number);

    port->is = (uint32_t) -1;
	int spin = 0; // Spin lock timeout counter
	int slot = this->find_freeslot(port);
	if (slot == -1)
		return false;
 
	AHCI::HBA_CMD_HEADER *cmdheader = (AHCI::HBA_CMD_HEADER*)port->clb;
	cmdheader += slot;
	cmdheader->command = sizeof(AHCI::FIS_REG_H2D)/sizeof(uint32_t); // Command FIS size
	cmdheader->prdtl = 1; // PRDT entries count


	AHCI::HBA_CMD_TBL *cmdtbl = (AHCI::HBA_CMD_TBL*)(cmdheader->ctba);
	cmdtbl->prdt_entry[0].dba = (uint32_t)buf;
    cmdtbl->prdt_entry[0].info = ATA_LOGICAL_SECTOR_SIZE-1;
 
	// Setup command
	AHCI::FIS_REG_H2D *cmdfis = (AHCI::FIS_REG_H2D*)(&cmdtbl->cfis);
	memset((uint8_t*)cmdfis,0,sizeof(AHCI::FIS_REG_H2D));
	cmdfis->h.fis_type = AHCI::FIS_TYPE_REG_H2D;
	cmdfis->command = ATA_CMD_IDENTIFY;
	cmdfis->device = 0;
	cmdfis->h.attrs |= (1 << 7);
 
	// The below loop waits until the port is no longer busy before issuing a new command
	while ((port->tfd & (ATA_SR_BSY | ATA_SR_DRQ)) && spin < 1000000)
	{
		spin++;
		if (spin == 1000000)
		{
			return false;
		}
	}

	port->cmd |= (1 << 4) | (1 << 0);
	port->ci = 1<<slot;	// Issue command
 
	// Wait for completion
	while (1)
	{
		// In some longer duration reads, it may be helpful to spin on the DPS bit 
		// in the PxIS port field as well (1 << 5)
		if ((port->ci & (1<<slot)) == 0) 
			break;
		if (port->is & AHCI_HBA_PxIS_TFES)	// Task file error
		{
			break;
		}
	}
 
	// Check again
	if (port->is & AHCI_HBA_PxIS_TFES)
	{
		return false;
	}
	return true;
}
uint8_t AHCIController::get_physical_logical_sector_alignment(uint8_t port)
{
    return this->get_physical_sector_size(port) / this->get_logical_sector_size(port);
}