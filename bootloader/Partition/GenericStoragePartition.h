#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Partition/GenericPartition.h>

#define MBRLabel 0x1
#define GPTLabel 0x2
#define BSDLabel 0x4
#define LBA48BitMBR 0xEC //see - https://wiki.osdev.org/Partition_Table#.22Unofficial.22_48_bit_LBA_Proposed_MBR_Format 

class GenericStoragePartition : public GenericPartition {
public:
    ~GenericStoragePartition();
    GenericStoragePartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit);
    virtual uint8_t get_label();
    virtual uint32_t get_id(__attribute__((unused)) int index);
    virtual uint32_t get_type(__attribute__((unused)) int index);
    void read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    bool read_file(const char* filename,uint16_t* buf,uint32_t bytesCount);
protected:
    void* fs;
};