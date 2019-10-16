#include <Disk/StorageDevice.h>
void StorageDevice::initialize(GenericDiskController* controller,uint32_t port)
{
    this->disk_controller = controller;
    this->port = port;
}
void StorageDevice::read(uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount)
{
    if(this->disk_controller->type == AHCI_DiskController)
    {
        AHCIController* controller = (AHCIController*)this->disk_controller;
        controller->read(this->port,lbal,lbah,buf,bytesCount);
    }
    else if(this->disk_controller->type == NVMe_DiskController)
    {
        /* TODO: Implement read from NVMe Device */
    }
    else if(this->disk_controller->type == IDE_DiskController)
    {
        IDEController* controller = (IDEController*)this->disk_controller;
        switch(this->port)
        {
            case 0:
                controller->read(true,false,lbal,lbah,buf,bytesCount);
            case 1:
                controller->read(true,true,lbal,lbah,buf,bytesCount);
            case 2:
                controller->read(false,false,lbal,lbah,buf,bytesCount);
            case 3:
                controller->read(false,true,lbal,lbah,buf,bytesCount);
        } 
    }
}
uint16_t StorageDevice::get_sector_size()
{
    if(this->disk_controller->type == AHCI_DiskController)
    {
        AHCIController* controller = (AHCIController*)this->disk_controller;
        return controller->get_sector_size(this->port);
    }
    else if(this->disk_controller->type == NVMe_DiskController)
    {
        return 0xffff; /* TODO: Implement getting sector size from NVMe Device */
    }
    else if(this->disk_controller->type == IDE_DiskController)
    {
        IDEController* controller = (IDEController*)this->disk_controller;
        switch(this->port)
        {
            case 0:
                return controller->get_sector_size(true,false);
            case 1:
                return controller->get_sector_size(true,true);
            case 2:
                return controller->get_sector_size(false,false);
            case 3:
                return controller->get_sector_size(false,true);
        }
        
    }
    return 0xffff;
}