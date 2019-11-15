#include <Disk/AHCI/SATADevice.h>

SATADevice::SATADevice(AHCIController* controller,uint8_t port) : StorageDevice(controller,port)
{
    this->transfer_mode = DefaultTransferMode;
    this->command_set = ATACommandSet;
}
uint16_t SATADevice::get_hardware_protocol()
{
    return SATA_AHCI;
}