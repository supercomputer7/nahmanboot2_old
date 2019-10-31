#pragma once
#include <Disk/StorageDevice.h>
#include <Disk/AHCI/AHCIController.h>
class SATADevice : public StorageDevice {

public:
    SATADevice(AHCIController* controller,uint8_t port);
    ~SATADevice();
    void read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount) override;
    uint16_t get_hardware_protocol() override;
private:
    
};