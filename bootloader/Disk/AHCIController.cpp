#include <Disk/AHCIController.h>
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
	this->type = AHCI_DiskController;
}
bool AHCIController::probe_port_connected(uint8_t port)
{
    if(port > 31)
        return false;
    return (((this->hba->pi) >> port) & 0x1) == 1;
}
bool AHCIController::read(uint8_t port_number,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount)
{
	uint64_t lba = (lbal | (lbah << 32));
    lba = lba << this->get_physical_logical_sector_alignment(port_number);
	return this->read_ata(port_number,(uint32_t)lba,(uint32_t)(lba >> 32),buf,bytesCount);
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
		cmdtbl->prdt_entry[i].info = (8*1024-1 & (0x3FFFFF)) | (1<<31);	// 8K bytes (this value should always be set to 1 less than the actual value)
		buf += 4*1024;	// 4K words
		sectorcount -= 16;	// 16 sectors
	}
	// Last entry
	cmdtbl->prdt_entry[i].dba = (uint32_t) buf;
	cmdtbl->prdt_entry[i].info = ((sectorcount<<9)-1 & (0x3FFFFF)) | (1<<31);	// 512 bytes per sector
 
	// Setup command
	AHCI::FIS_REG_H2D *cmdfis = (AHCI::FIS_REG_H2D*)(&cmdtbl->cfis);
 
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
uint16_t AHCIController::get_sector_size(uint8_t port)
{
	return 0; /* TODO: get sector size of device per port */
}
uint16_t AHCIController::get_sector_count(uint8_t port,uint16_t bytesCount)
{
	/* TODO: get count of sectors per bytes counts */
    uint16_t sector_size =  ATA_LOGICAL_SECTOR_SIZE;
    if(bytesCount % sector_size == 0)
        return bytesCount/sector_size;
    else
        return (bytesCount/sector_size) + 1;
}
bool AHCIController::identify(uint8_t port,uint16_t* buf)
{
	return false; /* TODO: read ATA IDENTIFY data */
}
uint8_t AHCIController::get_physical_logical_sector_alignment(uint8_t port)
{
    /* TODO: get how many logical sectors per physical sectors */
    return 0;
}