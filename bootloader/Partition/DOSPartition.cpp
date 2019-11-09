#include <Partition/DOSPartition.h>
DOSPartition::DOSPartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,char partition_type,uint16_t partition_id)
    : GenericStoragePartition(device,lbal_offset,lbah_offset,lbal_limit,lbah_limit)
{
    this->partition_type = partition_type;
    this->id = partition_id;
}
uint8_t DOSPartition::get_label()
{
    return MBRLabel;
}
uint32_t DOSPartition::get_id(int index)
{
    if(index > 0)
        return 0xffffffff;
    else
        return this->id;
}
uint32_t DOSPartition::get_type(int index)
{
    if(index > 0)
        return 0xffffffff;
    else
        return this->partition_type;
}