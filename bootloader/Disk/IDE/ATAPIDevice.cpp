#include <Disk/IDE/ATAPIDevice.h>

ATAPIDevice::ATAPIDevice(IDEController* controller,uint8_t port) : StorageDevice(controller,port)
{
    this->transfer_mode = DefaultTransferMode;
    this->command_set = ATAPICommandSet;
}
uint16_t ATAPIDevice::get_hardware_protocol()
{
    return IDEATAPI;
}