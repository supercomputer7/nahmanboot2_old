#include <Partition/GUIDPartition.h>
GUIDPartition::GUIDPartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,uint32_t* partition_type,uint32_t* partition_id)
    : GenericStoragePartition(device,lbal_offset,lbah_offset,lbal_limit,lbah_limit)
{
    for(int i=0; i<4; i++)
    {
        this->uuid[i] = partition_id[i];
        this->partition_type[i] = partition_type[i];
    }
}
uint8_t GUIDPartition::get_label()
{
    return GPTLabel;
}
uint32_t GUIDPartition::get_id(int index)
{
    if(index > 0)
        return 0xffffffff;
    else
        return this->uuid[index];
}
uint32_t GUIDPartition::get_type(int index)
{
    if(index > 0)
        return 0xffffffff;
    else
        return this->partition_type[index];
}