#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>

#include <stdint.h>
#include <LibC/stdbool.h>
#include <Filesystem/GenericFilesystems/GenericFilesystem.h>
#include <Disk/StorageDevice.h>
#include <Memory/List.h>
#include <Memory/Node.h>

class GenericFilesystem;
class DevicePartition {

public:
    ~DevicePartition();
    void initialize(StorageDevice* device,uint32_t lbal_offset,uint32_t lbah_offset,uint32_t lbal_limit,uint32_t lbah_limit,bool long_partition_type,uint32_t* partition_type,bool long_partition_uuid,uint32_t* partition_uuid);
    uint16_t get_sector_size();
    void read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount);
    bool read_file(const char* filename,uint16_t* buf,uint32_t bytesCount);
private:
    uint32_t lba_limit1;
    uint32_t lba_limit2;
    uint32_t lba_offset1;
    uint32_t lba_offset2;
    bool long_partition_type;
    uint32_t partition_type[4];
    bool long_partition_uuid;
    uint32_t uuid[4];
    uint16_t partition_container_type;
    
    StorageDevice* device;
    GenericFilesystem* fs;
};

namespace Partition {
    bool probe_ext2(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ext3(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ext4(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_iso9660(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_fat32(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_vfat(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_bfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ufs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ufs2(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_hfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_xfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_zfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_btrfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_f2fs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_hammerfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_hammer2fs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    GenericFilesystem* probe_filesystem(DevicePartition* created_partition,StorageDevice* device,uint32_t* partition_type,bool long_partition_type,uint32_t lbal,uint32_t lbah);
    uint32_t get_universal_partition_type(uint32_t* partition_type,bool long_partition_type);
    List<DevicePartition>* enum_storage_device(Node<StorageDevice>* device);
    List<DevicePartition>* enum_storage_devices(List<StorageDevice>* devices);
}