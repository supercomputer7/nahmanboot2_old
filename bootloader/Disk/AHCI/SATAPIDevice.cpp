#include <Disk/AHCI/SATAPIDevice.h>

SATAPIDevice::SATAPIDevice(AHCIController* controller,uint8_t port) : StorageDevice(controller,port)
{
    this->port = port;
    uint16_t sector_size = controller->get_logical_sector_size(port);
    this->set_sector_size(sector_size);
}
void SATAPIDevice::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    reinterpret_cast<AHCIController*>(this->disk_controller)->read(ATAPICommandSet,this->port,lbal,lbah,bytesOffset,buf,bytesCount);
}
uint16_t SATAPIDevice::get_hardware_protocol()
{
    return SATAPI_AHCI;
}