#pragma once
#include <Disk/StorageDevice.h>
#include <Disk/AHCI/AHCIController.h>
class SATAPIDevice : public StorageDevice {

public:
    SATAPIDevice(AHCIController* controller,uint8_t port);
    ~SATAPIDevice();
    void read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount) override;
    uint16_t get_hardware_protocol() override;
private:
    
};