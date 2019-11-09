#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Partition/GenericStoragePartition.h>

class GUIDPartition : public GenericStoragePartition {

public:
    ~GUIDPartition();
    GUIDPartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,uint32_t* partition_type,uint32_t* partition_id);
    uint8_t get_label() override;
    uint32_t get_id(int index) override;
    uint32_t get_type(int index) override;
private:
    uint32_t partition_type[4];
    uint32_t uuid[4];
};