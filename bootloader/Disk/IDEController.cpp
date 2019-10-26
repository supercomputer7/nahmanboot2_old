#include <Disk/IDEController.h>
IDEController::IDEController(PCI::Device* device,PCI::Access* access) : GenericDiskController(IDE_DiskController)
{
    this->initialize(device,access);
}
void IDEController::initialize(PCI::Device* device,PCI::Access* access)
{
    this->device = device;
    this->access = access;
    uint16_t port1 = PCI::read(access,
                            device->get_segment(),
                            device->get_bus(),
                            device->get_device_number(),
                            device->get_function_number(),
                            0x10
                            );
    uint16_t port2 = PCI::read(access,
                            device->get_segment(),
                            device->get_bus(),
                            device->get_device_number(),
                            device->get_function_number(),
                            0x14
                            );
    if(port1 == 0x1 || port1 == 0x0)
        this->primary_bus_io_port = ATA_PRIMARY_BUS_BASEIO;
    else
        this->primary_bus_io_port = port1;
    if(port2 == 0x1 || port2 == 0x0)
        this->secondary_bus_io_port = ATA_SECONDARY_BUS_BASEIO;
    else
        this->secondary_bus_io_port = port2;

    this->bus_master_register = PCI::read(access,
                                device->get_segment(),
                                device->get_bus(),
                                device->get_device_number(),
                                device->get_function_number(),        
                                0x20) | 
                                PCI::read(access,
                                device->get_segment(),
                                device->get_bus(),
                                device->get_device_number(),
                                device->get_function_number(),        
                                0x22);
    this->tmpbuffer = (uint8_t*)dma_calloc();
    this->tmpbuffer_size = DMARegionSize;
    this->prdt = (ATA_DMA_PRDT*)dma_calloc();
}
bool IDEController::identify(bool is_primary,bool is_slave,uint16_t* buf)
{
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;
    

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xB0);
    else
        IO::outb(port + ATA_REG_DATA, 0xA0);
    
    IO::outb(port + ATA_REG_SECCOUNT0, 0);
    IO::outb(port + ATA_REG_LBA0, 0);
    IO::outb(port + ATA_REG_LBA1, 0);
    IO::outb(port + ATA_REG_LBA2, 0);
    IO::outb(port + ATA_REG_COMMAND, 0xEC);

    uint8_t input = IO::inb(port + ATA_REG_STATUS);
    
    if (input)
    {
        input = IO::inb(port + ATA_REG_STATUS);
        if(((input & 0x1) != 0) || ((input & 0x20) != 0))
        {
            return false;
        }
        while(((input & ATA_SR_BSY) != 0) && ((input & ATA_SR_DRQ) == 0))
        {
            if(((input & ATA_SR_DF) != 0) || ((input & ATA_SR_ERR) != 0))
            {
                return true;
            }
            input = IO::inb(port + ATA_REG_STATUS);
        }
        for(int i=0; i < 256;++i)
        {
            buf[i] = IO::inw(port + ATA_REG_DATA);
            this->do_400ns_delay();            
        }
    }
    return true;
}
bool IDEController::probe_numbered_port_connected(uint8_t port)
{
    switch (port)
    {
        case 0:
            return this->identify(true,false,(uint16_t*)&this->cached_identify_data);
        case 1:
            return this->identify(true,true,(uint16_t*)&this->cached_identify_data);
        case 2:
            return this->identify(false,false,(uint16_t*)&this->cached_identify_data);
        case 3:
            return this->identify(false,true,(uint16_t*)&this->cached_identify_data);
    }
    return false;
}
bool IDEController::probe_port_connected(bool is_primary,bool is_slave)
{
    return this->identify(is_primary,is_slave,(uint16_t*)&this->cached_identify_data);
}

void IDEController::read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    uint32_t lbal_offseted = lbal + (bytesOffset / this->get_logical_sector_size(is_primary,is_slave));
    uint16_t offset_in_first_lba = bytesOffset % this->get_logical_sector_size(is_primary,is_slave);

    if((offset_in_first_lba + bytesCount) <= this->tmpbuffer_size)
        this->small_read(is_primary,is_slave,lbal_offseted,lbah,offset_in_first_lba,buf,bytesCount);
    else
    {
        uint32_t buffer_addr = (uint32_t)buf;

        this->small_read(is_primary,is_slave,lbal_offseted,lbah,offset_in_first_lba,buf,(this->tmpbuffer_size - offset_in_first_lba));
        uint16_t bytes_to_read = (bytesCount - this->tmpbuffer_size) + offset_in_first_lba;
        while(bytes_to_read > 0)
        {
            lbal_offseted += (this->tmpbuffer_size / this->get_logical_sector_size(is_primary,is_slave));
            buffer_addr += this->tmpbuffer_size;
            if(bytes_to_read >= this->tmpbuffer_size)
            {
                this->small_read(is_primary,is_slave,lbal_offseted,lbah,0,(uint16_t*)buffer_addr,this->tmpbuffer_size);
            }
            else
            {
                this->small_read(is_primary,is_slave,lbal_offseted,lbah,0,(uint16_t*)buffer_addr,bytes_to_read);
                break;
            }
            bytes_to_read = bytes_to_read - this->tmpbuffer_size;
        }
    }
}

void IDEController::small_read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    if(lbah != 0)
    {
        this->read_dma_lba48(is_primary,is_slave,lbal,lbah,(uint16_t*)this->tmpbuffer,this->tmpbuffer_size);
        this->transfer_data(bytesOffset,bytesCount,(uint8_t*)buf);
    }
    else
    {
        this->read_dma_lba28(is_primary,is_slave,lbal,(uint16_t*)this->tmpbuffer,this->tmpbuffer_size);
        this->transfer_data(bytesOffset,bytesCount,(uint8_t*)buf);
    }
    
}

void IDEController::transfer_data(uint16_t offset,uint16_t bytesCount,uint8_t* buf)
{
    for(int i=0; i<bytesCount;++i)
        buf[i] = this->tmpbuffer[offset + i];
}


void IDEController::enable_pci_bus_master()
{
    uint16_t enabled_pci_bus_master = PCI::read(access,
            device->get_segment(),
            device->get_bus(),
            device->get_device_number(),
            device->get_function_number(),
            0x4) | (1 << 2);

    PCI::write(access,
            device->get_segment(),
            device->get_bus(),
            device->get_device_number(),
            device->get_function_number(),
            0x4,enabled_pci_bus_master);

}

void IDEController::disable_pci_bus_master()
{
    uint16_t enabled_pci_bus_master = PCI::read(access,
            device->get_segment(),
            device->get_bus(),
            device->get_device_number(),
            device->get_function_number(),
            0x4) & (~(1 << 2));

    PCI::write(access,
            device->get_segment(),
            device->get_bus(),
            device->get_device_number(),
            device->get_function_number(),
            0x4,enabled_pci_bus_master);
}

void IDEController::read_dma_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount)
{
    /* TODO: Check if bus master register is memory mapped or IO mapped */
    this->enable_pci_bus_master();
    uint16_t sectorcount = this->get_sector_count(is_primary,is_slave,bytesCount);
    prdt->byte_count = bytesCount;
    prdt->data_buffer = (uint32_t)buf;
    prdt->reserved = 0x8000;
    uint32_t prdt_addr = (uint32_t)this->prdt;

    uint16_t base = this->bus_master_register & (0xFFF0);
    if(is_primary == false)
        base += 8;
    IO::outl((base+4),prdt_addr);    
    IO::outb(base,0);
    IO::outb(base,0x8);
    IO::outb((base+2),IO::inb((base+2)) | 0x6);
    
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xE0 | ((lbal >> 24) & 0x0F));
    else
        IO::outb(port + ATA_REG_DATA, 0xF0 | ((lbal >> 24) & 0x0F));
    this->do_400ns_delay();

    //IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectorcount >> 8));
    //IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbah));
    //IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbah >> 8));
    //IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbah >> 16));
    IO::outb(port + 0x1, 0x00);
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectorcount & 0xff));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbal));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbal >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbal >> 16));
    IO::outb(port + ATA_REG_COMMAND, ATA_CMD_READ_DMA);
    for (;;) {
        uint8_t status = IO::inb(port + ATA_REG_STATUS);
        if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRDY))
            break;
    }    
    this->do_400ns_delay();
    IO::outb(base,(0x1 | 0x8));

    while (1) {
        int status = IO::inb(base+2);
        int dstatus = IO::inb(port + ATA_REG_STATUS);
        if (!(status & 0x04)) {
            continue;
        }
        if (!(dstatus & 0x80)) {
            break;
        }
    }

    IO::outb(base + 2, IO::inb(base + 2) | 0x6);
}

void IDEController::read_dma_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount)
{
    /* TODO: Check if bus master register is memory mapped or IO mapped */
    this->enable_pci_bus_master();
    uint16_t sectorcount = this->get_sector_count(is_primary,is_slave,bytesCount);
    prdt->byte_count = bytesCount;
    prdt->data_buffer = (uint32_t)buf;
    prdt->reserved = 0x8000;
    uint32_t prdt_addr = (uint32_t)this->prdt;

    uint16_t base = this->bus_master_register & (0xFFF0);
    if(is_primary == false)
        base += 8;
    IO::outl((base+4),prdt_addr);    
    IO::outb(base,0);
    IO::outb(base,0x8);
    IO::outb((base+2),IO::inb((base+2)) | 0x6);
    
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xE0 | ((lbal >> 24) & 0x0F));
    else
        IO::outb(port + ATA_REG_DATA, 0xF0 | ((lbal >> 24) & 0x0F));
    this->do_400ns_delay();

    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectorcount >> 8));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbah));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbah >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbah >> 16));
    IO::outb(port + 0x1, 0x00);
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectorcount & 0xff));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbal));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbal >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbal >> 16));
    IO::outb(port + ATA_REG_COMMAND, ATA_CMD_READ_DMA_EXT);
    for (;;) {
        uint8_t status = IO::inb(port + ATA_REG_STATUS);
        if (!(status & ATA_SR_BSY) && (status & ATA_SR_DRDY))
            break;
    }    
    this->do_400ns_delay();
    IO::outb(base,(0x1 | 0x8));

    while (1) {
        int status = IO::inb(base+2);
        int dstatus = IO::inb(port + ATA_REG_STATUS);
        if (!(status & 0x04)) {
            continue;
        }
        if (!(dstatus & 0x80)) {
            break;
        }
    }

    IO::outb(base + 2, IO::inb(base + 2) | 0x6);
}

//void IDEController::read_atapi(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount)
//{
//    
//}
void IDEController::read_pio_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t bytesCount)
{
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    uint16_t sectorcount = this->get_sector_count(is_primary,is_slave,bytesCount);
    uint16_t words = this->get_logical_sector_size(is_primary,is_slave)/2;

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xE0 | ((lbal >> 24) & 0x0F));
    else
        IO::outb(port + ATA_REG_DATA, 0xF0 | ((lbal >> 24) & 0x0F));
    
    IO::outb(port + 0x1, 0x00);
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)sectorcount);
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbal));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbal >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbal >> 16));
    IO::outb(port + ATA_REG_COMMAND, ATA_CMD_READ_PIO);

    uint8_t input = IO::inb(port + ATA_REG_STATUS);
    bool error = false;
    if (input)
    {
        input = IO::inb(port + ATA_REG_STATUS);
        while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
        {
            input = IO::inb(port + ATA_REG_STATUS);
            if(((input & 0x1) != 0) || ((input & 0x20) != 0)) // error happened!
            {
                error = true;
                break;
            }
        }
        if(!error)
        {
            int count = 0;
            for(int j=0; j<sectorcount; ++j)
            {
                for(uint16_t i=0; i < words; i++)
                {
                    buf[count] = IO::inw(port + ATA_REG_DATA); // transfer data
                    ++count;         
                }
                //this->do_400ns_delay();
                input = IO::inb(port + ATA_REG_STATUS);
                while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
                {
                    input = IO::inb(port + ATA_REG_STATUS);
                }
                for(int k=0; k<3; ++k)
                {
                    //this->do_400ns_delay();
                }
            }
        }
    }
}
void IDEController::read_pio_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount)
{
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    uint16_t sectorcount = this->get_sector_count(is_primary,is_slave,bytesCount);
    uint16_t words = this->get_logical_sector_size(is_primary,is_slave)/2;

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xE0 | ((lbal >> 24) & 0x0F));
    else
        IO::outb(port + ATA_REG_DATA, 0xF0 | ((lbal >> 24) & 0x0F));
    
    IO::outb(port + 0x1, 0x00);

    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectorcount >> 8));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbah));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbah >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbah >> 16));
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectorcount & 0xff));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbal));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbal >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbal >> 16));
    IO::outb(port + ATA_REG_COMMAND, ATA_CMD_READ_PIO_EXT);

    uint8_t input = IO::inb(port + ATA_REG_STATUS);
    bool error = false;
    if (input)
    {
        input = IO::inb(port + ATA_REG_STATUS);
        while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
        {
            input = IO::inb(port + ATA_REG_STATUS);
            if(((input & 0x1) != 0) || ((input & 0x20) != 0)) // error happened!
            {
                error = true;
                break;
            }
        }
        if(!error)
        {
            int count = 0;
            for(int j=0; j<sectorcount; ++j)
            {
                for(uint16_t i=0; i < words; i++)
                {
                    buf[count] = IO::inw(port + ATA_REG_DATA); // transfer data
                    ++count;         
                }
                //this->do_400ns_delay();
                input = IO::inb(port + ATA_REG_STATUS);
                while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
                {
                    input = IO::inb(port + ATA_REG_STATUS);
                }
                for(int k=0; k<3; ++k)
                {
                    //this->do_400ns_delay();
                }
            }
        }
    }
}
void IDEController::do_400ns_delay()
{
    for(int i = 0;i < 4; i++)
		IO::inb(this->primary_bus_io_port + ATA_REG_ALTSTATUS);
}
uint16_t IDEController::get_sector_count(bool is_primary,bool is_slave,uint16_t bytesCount)
{
    if(this->identify(is_primary,is_slave,(uint16_t*)&this->cached_identify_data))
    {
        uint16_t sector_size =  ATA_LOGICAL_SECTOR_SIZE << (this->cached_identify_data.physical_logical_sector & 0xf);
        if(bytesCount % sector_size == 0)
            return bytesCount/sector_size;
        else
            return (bytesCount/sector_size) + 1;
    }
    else
    {
        return 0xffff;
    }
}
uint16_t IDEController::get_logical_sector_size(bool is_primary,bool is_slave)
{
    this->identify(is_primary,is_slave,(uint16_t*)&this->cached_identify_data);
    if((this->cached_identify_data.physical_logical_sector & (1 << 12)) == 0)
        return ATA_LOGICAL_SECTOR_SIZE;
    if(this->cached_identify_data.logical_sector_size[0] == 0)
        return ATA_LOGICAL_SECTOR_SIZE;
    else
        return this->cached_identify_data.logical_sector_size[0] << 1;
}
uint16_t IDEController::get_physical_sector_size(bool is_primary,bool is_slave)
{
    this->identify(is_primary,is_slave,(uint16_t*)&this->cached_identify_data);
    uint16_t alignment = this->cached_identify_data.physical_logical_sector & 0xf;
    uint16_t logical_sector_size = this->get_logical_sector_size(is_primary,is_slave);
    return logical_sector_size << alignment;
}
uint8_t IDEController::get_physical_logical_sector_alignment(bool is_primary,bool is_slave)
{
    return this->get_physical_sector_size(is_primary,is_slave) / this->get_logical_sector_size(is_primary,is_slave);
}