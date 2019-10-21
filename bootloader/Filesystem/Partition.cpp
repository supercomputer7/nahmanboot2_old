#include <Filesystem/Partition.h>

void DevicePartition::initialize(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,bool long_partition_type,uint32_t* partition_type)
{
    this->device = device;
    this->lba_limit1 = lbal_limit;
    this->lba_limit2 = lbah_limit;

    this->lba_offset1 = lbal_offset;
    this->lba_offset2 = lbah_offset;
    this->long_partition_type = long_partition_type;
    if(this->long_partition_type)
        for(int i=0; i<4;++i)
            this->partition_type[i] = partition_type[i];
    else
        this->partition_type[0] = partition_type[0];
}
uint16_t DevicePartition::get_sector_size()
{
    return this->device->get_sector_size();
}
void DevicePartition::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    //if((lbal+(bytesOffset/this->get_sector_size())) <= this->lba_limit1 && lbah <= this->lba_limit2)
    this->device->read((lbal+this->lba_offset1), (lbah+this->lba_offset2),bytesOffset,buf,bytesCount);
}

uint16_t Partition::find()
{
    return 0;
}