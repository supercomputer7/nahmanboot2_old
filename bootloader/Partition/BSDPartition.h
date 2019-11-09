#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Partition/GenericStoragePartition.h>
#include <Memory/List.h>
#include <Memory/Node.h>

class BSDPartition : public GenericStoragePartition {

public:
    ~BSDPartition();
    BSDPartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,char partition_type,uint16_t partition_id);
    uint8_t get_label() override;
    uint32_t get_id(int index) override;
    uint32_t get_type(int index) override;
private:
    uint16_t id;
    char partition_type;
};