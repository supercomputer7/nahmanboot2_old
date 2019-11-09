#include <Partition/PartitionProbing.h>
#include <Filesystem/LinuxFilesystems/Ext2Filesystem.h>
GenericFilesystem* Partition::probe_filesystem(GenericStoragePartition* created_partition,StorageDevice* device,uint32_t lbal,uint32_t lbah)
{
    if(Partition::probe_ext2(device,lbal,lbah))
    {
        GenericFilesystem* fs = new Ext2Filesystem();
        fs->initialize(created_partition);
        return fs;
    }
    return new GenericFilesystem();
}
bool Partition::probe_ext2(StorageDevice* device,uint32_t lbal,uint32_t lbah)
{
    uint16_t* buf = (uint16_t* )kmalloc(sizeof(uint32_t));
    device->read(lbal,lbah,1024+56,buf,2);
    if(buf[0] == 0xef53)
    {
        device->read(lbal,lbah,1024+92,buf,4);
        uint32_t optional_features = buf[0] | (buf[1] << 16);
        device->read(lbal,lbah,1024+96,buf,4);
        uint32_t required_features = buf[0] | (buf[1] << 16);
        if((required_features & Ext2::Filesystem_uses_journal_device) == 0 && (optional_features & Ext2::File_system_has_a_journal) == 0)
            return true;
    }
    return false;
}
bool Partition::probe_ext3(StorageDevice* device,uint32_t lbal,uint32_t lbah)
{
    uint16_t* buf = (uint16_t* )kmalloc(sizeof(uint32_t));
    device->read(lbal,lbah,1024+56,buf,2);
    if(buf[0] == 0xef53)
    {
        device->read(lbal,lbah,1024+92,buf,4);
        uint32_t optional_features = buf[0] | (buf[1] << 16);
        device->read(lbal,lbah,1024+96,buf,4);
        uint32_t required_features = buf[0] | (buf[1] << 16);
        if((required_features & Ext2::Filesystem_uses_journal_device) != 0 && (optional_features & Ext2::File_system_has_a_journal) != 0 && (optional_features & Ext2::Directories_use_hash_index) != 0)
            return true;
    }
    return false;
}