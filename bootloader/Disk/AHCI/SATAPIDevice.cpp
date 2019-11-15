#include <Disk/AHCI/SATAPIDevice.h>

SATAPIDevice::SATAPIDevice(AHCIController* controller,uint8_t port) : StorageDevice(controller,port)
{
    this->transfer_mode = DefaultTransferMode;
    this->command_set = ATAPICommandSet;
}
uint16_t SATAPIDevice::get_hardware_protocol()
{
    return SATAPI_AHCI;
}