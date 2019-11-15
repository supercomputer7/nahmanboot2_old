#include <Disk/IDE/ATADevice.h>
ATADevice::ATADevice(IDEController* controller,uint8_t port) : StorageDevice(controller,port)
{
    this->transfer_mode = DefaultTransferMode;
    this->command_set = ATACommandSet;
}
uint16_t ATADevice::get_hardware_protocol()
{
    return IDEATA;
}