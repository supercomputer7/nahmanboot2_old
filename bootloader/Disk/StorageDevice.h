#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Disk/AHCIController.h>
#include <Disk/GenericDiskController.h>
#include <Disk/IDEController.h>
#include <Disk/NVMeController.h>

class StorageDevice {

public:
    ~StorageDevice();
    void initialize(GenericDiskController* controller,uint32_t port);
    void read(uint32_t lbal,uint32_t lbah,uint16_t* buf,uint16_t bytesCount);
    uint16_t get_sector_size();
private:
    char cached_mbr[512];
    GenericDiskController* disk_controller;
    uint32_t port;
};