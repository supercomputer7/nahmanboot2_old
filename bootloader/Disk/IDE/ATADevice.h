#pragma once
#include <Disk/StorageDevice.h>
#include <Disk/IDE/IDEController.h>
class ATADevice : public StorageDevice {

public:
    ATADevice(IDEController* controller,uint8_t port);
    ~ATADevice();
    uint16_t get_hardware_protocol() override;
private:
    
};