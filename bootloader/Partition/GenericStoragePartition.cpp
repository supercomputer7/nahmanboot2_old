#include <Partition/GenericStoragePartition.h>
#include <Partition/PartitionProbing.h>
#include <Filesystem/GenericFilesystems/GenericFilesystem.h>

GenericStoragePartition::GenericStoragePartition(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit) 
    : GenericPartition(device,lbal_offset,lbah_offset,lbal_limit,lbah_limit)
{
    this->fs = Partition::probe_filesystem(this,device,lbal_offset,lbah_offset);
}
uint8_t GenericStoragePartition::get_label()
{
    return 0xff;
}
uint32_t GenericStoragePartition::get_id(__attribute__((unused)) int index)
{
    return 0xffff;
}
uint32_t GenericStoragePartition::get_type(__attribute__((unused)) int index)
{
    return 0xffff;
}
void GenericStoragePartition::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    if(((lbal+(bytesOffset/this->get_sector_size())) <= this->get_lbal_limit()) && (lbah <= this->get_lbah_limit()))
        this->device->read((lbal+this->get_lbal_offset()), (lbah+this->get_lbah_offset()),bytesOffset,buf,bytesCount);
}
bool GenericStoragePartition::read_file(const char* filename,uint16_t* buf,uint32_t bytesCount)
{
    if(this->fs != nullptr)
        return reinterpret_cast<GenericFilesystem*>(this->fs)->read(filename,buf,bytesCount);
    else
        return false; 
}