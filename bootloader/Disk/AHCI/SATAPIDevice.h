#pragma once
#include <Disk/StorageDevice.h>
#include <Disk/AHCI/AHCIController.h>
class SATAPIDevice : public StorageDevice {

public:
    SATAPIDevice(AHCIController* controller,uint8_t port);
    ~SATAPIDevice();
    uint16_t get_hardware_protocol() override;
private:
    
};