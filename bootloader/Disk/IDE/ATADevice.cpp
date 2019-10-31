#include <Disk/IDE/ATADevice.h>

ATADevice::ATADevice(IDEController* controller,uint8_t port) : StorageDevice(controller,port)
{
    uint16_t sector_size = controller->get_logical_sector_size(port);
    this->set_sector_size(sector_size);
}
void ATADevice::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    reinterpret_cast<IDEController*>(this->disk_controller)->read(ATACommandSet,this->port,lbal,lbah,bytesOffset,buf,bytesCount);
}
uint16_t ATADevice::get_hardware_protocol()
{
    return IDEATA;
}