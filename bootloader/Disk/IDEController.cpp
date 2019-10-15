#include <Disk/IDEController.h>

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
    *(uint32_t*)(0x1080) = this->primary_bus_io_port; 
    if(port2 == 0x1 || port2 == 0x0)
        this->secondary_bus_io_port = ATA_SECONDARY_BUS_BASEIO;
    else
        this->secondary_bus_io_port = port2;
    
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
    
    if (input != 0)
    {
        input = IO::inb(port + ATA_REG_STATUS);
        if(((input & 0x1) != 0) || ((input & 0x20) != 0))
        {
            return false;
        }
        while(((input & 0x80) != 0) && ((input & 0x8) == 0))
        {
            if(((input & 0x1) != 0) || ((input & 0x20) != 0))
            {
                return true;
            }
            input = IO::inb(port + ATA_REG_STATUS);
        }
        for(int i=0; i < 256;i++)
        {
            buf[i] = IO::inw(port + ATA_REG_DATA);            
        }
    }
    return true;
}
void IDEController::read(bool is_primary,bool is_slave,uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount)
{
    uint16_t port;
    if(is_primary)
        port = this->primary_bus_io_port;
    else
        port = this->secondary_bus_io_port;

    uint16_t sectorcount = this->get_sector_count(is_primary,is_slave,bytesCount);
    uint16_t words = this->get_sector_size(is_primary,is_slave)/2;

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
                this->do_400ns_delay();
                input = IO::inb(port + ATA_REG_STATUS);
                while(((input & 0x80) != 0) && ((input & 0x8) == 0)) // wait for DRQ to set, BSY to clear
                {
                    input = IO::inb(port + ATA_REG_STATUS);
                }
                for(int k=0; k<3; ++k)
                {
                    this->do_400ns_delay();
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
uint16_t IDEController::get_sector_size(bool is_primary,bool is_slave)
{
    if(this->identify(is_primary,is_slave,(uint16_t*)&this->cached_identify_data))
        return ATA_LOGICAL_SECTOR_SIZE << (this->cached_identify_data.physical_logical_sector & 0xf);
    else
        return ATA_LOGICAL_SECTOR_SIZE;
}