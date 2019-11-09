#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Partition/GenericPartition.h>
#include <Filesystem/GenericFilesystems/GenericFilesystem.h>
#include <Memory/List.h>
#include <Memory/Node.h>

namespace Partition {
    bool probe_ext2(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ext3(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ext4(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_iso9660(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_fat12(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_fat16(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_fat32(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_ntfs(StorageDevice* device,uint32_t lbal,uint32_t lbah);
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
    GenericFilesystem* probe_filesystem(GenericStoragePartition* created_partition,StorageDevice* device,uint32_t lbal,uint32_t lbah);
    uint32_t get_universal_partition_type(uint32_t* partition_type,bool long_partition_type);
    bool probe_gpt(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_mbr(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_enhancedmbr(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    bool probe_bsdlabel(StorageDevice* device,uint32_t lbal,uint32_t lbah);
    List<GenericStoragePartition>* enum_storage_device(Node<StorageDevice>* device);
    List<GenericStoragePartition>* enum_storage_devices(List<StorageDevice>* devices);
}