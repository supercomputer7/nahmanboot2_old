#include <Partition/Partition.h>
#include <Filesystem/LinuxFilesystems/Ext2Filesystem.h>
#include <Filesystem/GenericFilesystems/GenericFilesystem.h>

void DevicePartition::initialize(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,bool long_partition_type,uint32_t* partition_type,bool long_partition_uuid,uint32_t* partition_uuid)
{
    this->device = device;
    this->lba_limit1 = lbal_limit;
    this->lba_limit2 = lbah_limit;

    this->lba_offset1 = lbal_offset;
    this->lba_offset2 = lbah_offset;
    this->long_partition_type = long_partition_type;
    this->long_partition_uuid = long_partition_uuid;

    if(this->long_partition_type)
        for(int i=0; i<4;++i)
            this->partition_type[i] = partition_type[i];
    else
        this->partition_type[0] = partition_type[0];

    if(this->long_partition_uuid)
        for(int i=0; i<4;++i)
            this->uuid[i] = partition_uuid[i];
    else
        this->uuid[0] = partition_uuid[0];
    this->fs = Partition::probe_filesystem(this,device,partition_type,long_partition_type,lbal_offset,lbah_offset);
}
uint16_t DevicePartition::get_sector_size()
{
    return this->device->get_sector_size();
}
void DevicePartition::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    if((lbal+(bytesOffset/this->get_sector_size())) <= this->lba_limit1 && lbah <= this->lba_limit2)
        this->device->read((lbal+this->lba_offset1), (lbah+this->lba_offset2),bytesOffset,buf,bytesCount);
}
bool DevicePartition::read_file(const char* filename,uint16_t* buf,uint32_t bytesCount)
{
    return this->fs->read(filename,buf,bytesCount);
}

GenericFilesystem* Partition::probe_filesystem(DevicePartition* created_partition,StorageDevice* device,uint32_t* partition_type,bool long_partition_type,uint32_t lbal,uint32_t lbah)
{
    if(Partition::probe_ext2(device,lbal,lbah))
    {
        Ext2Filesystem* fs = new Ext2Filesystem();
        fs->initialize(created_partition);
        return (GenericFilesystem*)fs;
    }
    return new GenericFilesystem();
}

List<DevicePartition>* Partition::enum_storage_device(Node<StorageDevice>* device)
{
    uint32_t* buf = new uint32_t[4];
    

}
List<DevicePartition>* Partition::enum_storage_devices(List<StorageDevice>* devices)
{

}

uint32_t Partition::get_universal_partition_type(uint32_t* partition_type,bool long_partition_type)
{

}
bool Partition::probe_ext2(StorageDevice* device,uint32_t lbal,uint32_t lbah)
{
    uint16_t* buf = (uint16_t* )kmalloc(sizeof(uint16_t));
    device->read(lbal,lbah,1024+56,buf,2);
    if(buf[0] == 0xef53)
        return true;
    else
        return false;
}