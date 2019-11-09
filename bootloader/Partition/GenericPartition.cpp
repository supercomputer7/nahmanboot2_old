#include <Partition/GenericPartition.h>
GenericPartition::GenericPartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit)
{
    this->lba_limit1 = lbal_limit;
    this->lba_limit2 = lbah_limit;
    this->lba_offset1 = lbal_offset;
    this->lba_offset2 = lbah_offset;
    this->device = device;
}
uint16_t GenericPartition::get_sector_size()
{
    if(device != nullptr)
        return this->device->get_sector_size();
    return 0xffff;
}
uint32_t GenericPartition::get_lbal_offset()
{
    return this->lba_offset1;
}
uint32_t GenericPartition::get_lbah_offset()
{
    return this->lba_offset2;
}
uint32_t GenericPartition::get_lbal_limit()
{
    return this->lba_limit1;
}
uint32_t GenericPartition::get_lbah_limit()
{
    return this->lba_limit2;
}