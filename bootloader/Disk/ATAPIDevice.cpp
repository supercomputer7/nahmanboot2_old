#include <Disk/ATAPIDevice.h>

ATAPIDevice::ATAPIDevice(IDEController* controller,uint8_t port) : StorageDevice(controller,port)
{
    uint16_t sector_size = controller->get_logical_sector_size(port);
    this->set_sector_size(sector_size);
}
void ATAPIDevice::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    reinterpret_cast<IDEController*>(this->disk_controller)->read(ATAPICommandSet,this->port,lbal,lbah,bytesOffset,buf,bytesCount);
}
uint16_t ATAPIDevice::get_hardware_protocol()
{
    return IDEATAPI;
}