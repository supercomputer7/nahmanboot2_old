#pragma once
#include <Disk/StorageDevice.h>
#include <Disk/IDE/IDEController.h>
class ATAPIDevice : public StorageDevice {

public:
    ATAPIDevice(IDEController* controller,uint8_t port);
    ~ATAPIDevice();
    uint16_t get_hardware_protocol() override;
private:
    
};