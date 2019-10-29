#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Disk/GenericDiskController.h>

#define IDEATA 1
#define IDEATAPI 2
#define IDEUNKNOWN 3
#define SATA_AHCI 4
#define SATAPI_AHCI 5
#define NVMe 6
#define USB 7

class StorageDevice {

public:
    StorageDevice(GenericDiskController* disk_controller,uint32_t port);
    ~StorageDevice();
    virtual void read(__attribute__((unused)) uint32_t lbal, __attribute__((unused)) uint32_t lbah,__attribute__((unused)) uint32_t bytesOffset,__attribute__((unused)) uint16_t* buf,__attribute__((unused)) uint16_t bytesCount);
    virtual uint16_t get_hardware_protocol();
    uint16_t get_sector_size();
protected:
    void set_sector_size(uint16_t sector_size);
    uint16_t sector_size;
    char cached_mbr[512];
    GenericDiskController* disk_controller;
    uint32_t port;
};