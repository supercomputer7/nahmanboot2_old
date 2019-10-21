#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>

#include <stdint.h>
#include <LibC/stdbool.h>
#include <Disk/StorageDevice.h>
#include <Filesystem/GenericFilesystem.h>

class DevicePartition {

public:
    ~DevicePartition();
    void initialize(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,bool long_partition_type,uint32_t* partition_type);
    uint16_t get_sector_size();
    void read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    void read_file(const char* filename,uint16_t* buf,uint32_t bytesCount);
private:
    uint32_t lba_limit1;
    uint32_t lba_limit2;
    uint32_t lba_offset1;
    uint32_t lba_offset2;
    bool long_partition_type;
    uint32_t partition_type[4];
    StorageDevice* device;

};

namespace Partition {
    uint16_t find();
}