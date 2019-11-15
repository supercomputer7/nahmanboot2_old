#include <Disk/IDE/IDEController.h>
#include <IO/IO.h>
#include <Memory/malloc.h>

IDEController::IDEController(PCI::Device* device,PCI::Access* access) : GenericDiskController()
{
    this->initialize(device,access);
}
uint16_t IDEController::get_controller_type()
{
	return IDEStorageController;
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
    this->prdt = (ATA_DMA_PRDT*)dma_calloc();
    this->default_transfer_mode = PIOTransferMode;
}
bool IDEController::ata_identify(bool is_primary,bool is_slave,uint16_t* buf)
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
        IO::repeated_inw(port + ATA_REG_DATA,buf,256);   
    }
    return true;
}
bool IDEController::probe_port_connected(uint8_t port)
{
    switch (port)
    {
        case 0:
            return this->ata_identify(true,false,(uint16_t*)&this->cached_identify_data);
        case 1:
            return this->ata_identify(true,true,(uint16_t*)&this->cached_identify_data);
        case 2:
            return this->ata_identify(false,false,(uint16_t*)&this->cached_identify_data);
        case 3:
            return this->ata_identify(false,true,(uint16_t*)&this->cached_identify_data);
    }
    return false;
}

bool IDEController::read(uint8_t transfer_mode,uint8_t commandset,uint8_t port_number,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    if(commandset == ATACommandSet)
    {
        bool primary, slave;
        switch(port_number)
        {
            case 0:
                primary = true;
                slave = false;
                break;
            case 1:
                primary = true;
                slave = true;
                break;
            case 2:
                primary = false;
                slave = false;
                break;
            case 3:
                primary = false;
                slave = true;
                break;
            default:
                return false;
        }
        this->native_read(transfer_mode,primary,slave,lbal,lbah,buf,sectors_count,sector_size);
        return true;
    }
    return false;
}
void IDEController::native_read(uint8_t transfer_mode,bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    if(lbah != 0)
        read_48bit(transfer_mode,is_primary,is_slave,lbal,lbah,buf,sectors_count,sector_size);
    else
        read_28bit(transfer_mode,is_primary,is_slave,lbal,buf,sectors_count,sector_size);
}
void IDEController::read_48bit(uint8_t transfer_mode,bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    if(transfer_mode == DefaultTransferMode)
    {
        if(this->default_transfer_mode == PIOTransferMode)
        {
            this->read_ata_pio_lba48(is_primary,is_slave,lbal,lbah,buf,sectors_count,sector_size);
        }
        else if(this->default_transfer_mode == DMATransferMode)
        {
            this->read_ata_dma_lba48(is_primary,is_slave,lbal,lbah,buf,sectors_count,sector_size);
        }
    }
    else if (transfer_mode == PIOTransferMode)
    {
        this->read_ata_pio_lba48(is_primary,is_slave,lbal,lbah,buf,sectors_count,sector_size);
    }
    else if (transfer_mode == DMATransferMode)
    {
        this->read_ata_dma_lba48(is_primary,is_slave,lbal,lbah,buf,sectors_count,sector_size);
    }
}
void IDEController::read_28bit(uint8_t transfer_mode,bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    if(transfer_mode == 0)
    {
        if(this->default_transfer_mode == PIOTransferMode)
        {
            this->read_ata_pio_lba28(is_primary,is_slave,lbal,buf,sectors_count,sector_size);
        }
        else if(this->default_transfer_mode == DMATransferMode)
        {
            this->read_ata_dma_lba28(is_primary,is_slave,lbal,buf,sectors_count,sector_size);
        }
    }
    else if (transfer_mode == PIOTransferMode)
    {
        this->read_ata_pio_lba28(is_primary,is_slave,lbal,buf,sectors_count,sector_size);
    }
    else if (transfer_mode == DMATransferMode)
    {
        this->read_ata_dma_lba28(is_primary,is_slave,lbal,buf,sectors_count,sector_size);
    }
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

void IDEController::read_ata_dma_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    /* TODO: Check if bus master register is memory mapped or IO mapped */
    this->enable_pci_bus_master();
    prdt->byte_count = (sectors_count * sector_size);
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
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectors_count & 0xff));
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

void IDEController::read_ata_dma_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    /* TODO: Check if bus master register is memory mapped or IO mapped */
    this->enable_pci_bus_master();
    prdt->byte_count = (sectors_count * sector_size);
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

    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectors_count >> 8));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbah));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbah >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbah >> 16));
    IO::outb(port + 0x1, 0x00);
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectors_count & 0xff));
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

void IDEController::read_ata_pio_lba28(bool is_primary,bool is_slave,uint32_t lbal,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    uint16_t words_count = sector_size/sizeof(uint16_t);

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xE0 | ((lbal >> 24) & 0x0F));
    else
        IO::outb(port + ATA_REG_DATA, 0xF0 | ((lbal >> 24) & 0x0F));
    
    IO::outb(port + 0x1, 0x00);
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)sectors_count);
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
            for(int sector=0; sector<sectors_count; ++sector)
            {
                IO::repeated_inw(port + ATA_REG_DATA,(buf + (words_count*sector)),words_count);
                this->do_400ns_delay();
                input = IO::inb(port + ATA_REG_STATUS);
                while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
                {
                    input = IO::inb(port + ATA_REG_STATUS);
                }
            }
        }
    }
}
void IDEController::read_ata_pio_lba48(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t sectors_count,uint16_t sector_size)
{
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    uint16_t words_count = sector_size/sizeof(uint16_t);

    if(is_slave)
        IO::outb(port + ATA_REG_DATA, 0xE0 | ((lbal >> 24) & 0x0F));
    else
        IO::outb(port + ATA_REG_DATA, 0xF0 | ((lbal >> 24) & 0x0F));
    
    IO::outb(port + 0x1, 0x00);

    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectors_count >> 8));
    IO::outb(port + ATA_REG_LBA0, (uint8_t)(lbah));
    IO::outb(port + ATA_REG_LBA1, (uint8_t)(lbah >> 8));
    IO::outb(port + ATA_REG_LBA2, (uint8_t)(lbah >> 16));
    IO::outb(port + ATA_REG_SECCOUNT0,(uint8_t)(sectors_count & 0xff));
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
            for(int sector=0; sector<sectors_count; ++sector)
            {
                IO::repeated_inw(port + ATA_REG_DATA,(buf + (words_count*sector)),words_count);
                this->do_400ns_delay();
                input = IO::inb(port + ATA_REG_STATUS);
                while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
                {
                    input = IO::inb(port + ATA_REG_STATUS);
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


uint16_t IDEController::get_logical_sector_size(uint8_t port)
{
    switch (port)
    {
        case 0:
            return this->native_get_logical_sector_size(true,false);
        case 1:
            return this->native_get_logical_sector_size(true,true);
        case 2:
            return this->native_get_logical_sector_size(false,false);
        case 3:
            return this->native_get_logical_sector_size(false,true);
    }
    return 0x0;
}

uint16_t IDEController::native_get_logical_sector_size(bool is_primary,bool is_slave)
{
    this->ata_identify(is_primary,is_slave,(uint16_t*)&this->cached_identify_data);
    if((this->cached_identify_data.physical_logical_sector & (1 << 12)) == 0)
        return ATA_LOGICAL_SECTOR_SIZE;
    if(this->cached_identify_data.logical_sector_size[0] == 0)
        return ATA_LOGICAL_SECTOR_SIZE;
    else
        return this->cached_identify_data.logical_sector_size[0] << 1;
}
