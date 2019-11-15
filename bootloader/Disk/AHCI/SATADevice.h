#pragma once
#include <Disk/StorageDevice.h>
#include <Disk/AHCI/AHCIController.h>
class SATADevice : public StorageDevice {

public:
    SATADevice(AHCIController* controller,uint8_t port);
    ~SATADevice();
    uint16_t get_hardware_protocol() override;
private:
    
};