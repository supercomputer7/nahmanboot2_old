#include <Disk/StorageDevice.h>
#include <Display/Print.h>
StorageDevice::StorageDevice(GenericDiskController* disk_controller,uint32_t port)
{
    this->disk_controller = disk_controller;
    this->port = port;
}
void StorageDevice::set_sector_size(uint16_t sector_size)
{
    this->sector_size = sector_size;
}
void StorageDevice::read(__attribute__((unused)) uint32_t lbal, __attribute__((unused)) uint32_t lbah,__attribute__((unused)) uint32_t bytesOffset,__attribute__((unused)) uint16_t* buf,__attribute__((unused)) uint16_t bytesCount)
{
    stdout::print("Undefined read\n");
}
uint16_t StorageDevice::get_sector_size()
{
    return this->sector_size;
}
uint16_t StorageDevice::get_hardware_protocol()
{
    return 0;
}