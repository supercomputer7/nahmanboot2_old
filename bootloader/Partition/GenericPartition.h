#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Disk/StorageDevice.h>

class GenericPartition {

public:
    ~GenericPartition();
    GenericPartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit);
    uint16_t get_sector_size();
    uint32_t get_lbal_offset();
    uint32_t get_lbah_offset();
    uint32_t get_lbal_limit();
    uint32_t get_lbah_limit();
protected:
    StorageDevice* device;
private:
    uint32_t lba_limit1;
    uint32_t lba_limit2;
    uint32_t lba_offset1;
    uint32_t lba_offset2;
};