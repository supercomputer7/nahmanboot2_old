#include <Disk/StorageDevice.h>
#include <Display/Monitors/Print.h>
#include <LibC/stdstring.h>
#define SECTORS_COUNT_BUFFER (this->data_buffer->get_buffer_size() / this->sector_size)

StorageDevice::StorageDevice(GenericDiskController* disk_controller,uint32_t port)
{
    this->data_buffer = new ByteBuffer(dma_calloc(),DMARegionSize);
    this->disk_controller = disk_controller;
    this->port = port;
    this->sector_size = disk_controller->get_logical_sector_size(this->port);
}
void StorageDevice::set_sector_size(uint16_t sector_size)
{
    this->sector_size = sector_size;
}
void StorageDevice::small_read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    disk_controller->read(this->transfer_mode,this->command_set,this->port,lbal,lbah,(uint16_t*)this->data_buffer->get_ptr(),SECTORS_COUNT_BUFFER,this->sector_size);
    strcopy((char*)((uint32_t)get_buffer_data()+bytesOffset),(char*)buf,bytesCount);
}
void StorageDevice::read(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset,uint16_t* buf,uint16_t bytesCount)
{
    uint32_t lbal_offsetted = lbal + (bytesOffset / this->sector_size);
    uint16_t offset_in_first_sector = (bytesOffset % this->sector_size);

    if((offset_in_first_sector + bytesCount) <= this->data_buffer->get_buffer_size())
    {   
        this->small_read(lbal_offsetted,lbah,offset_in_first_sector,buf,bytesCount);
    }
    else
    {
        this->small_read(lbal_offsetted,lbah,offset_in_first_sector,buf,(this->data_buffer->get_buffer_size() - offset_in_first_sector));
        uint16_t bytes_to_read = (bytesCount - this->data_buffer->get_buffer_size()) + offset_in_first_sector;
        int i = 0;
        while(bytes_to_read > 0)
        {
            lbal_offsetted += (this->data_buffer->get_buffer_size() / this->sector_size);
            if(bytes_to_read >= this->data_buffer->get_buffer_size())
            {
                
                this->small_read(lbal_offsetted,lbah,0,(uint16_t*)(buf + (SECTORS_COUNT_BUFFER*i)),this->data_buffer->get_buffer_size());
            }
            else
            {
                this->small_read(lbal_offsetted,lbah,0,(uint16_t*)(buf +  (SECTORS_COUNT_BUFFER*i)),bytes_to_read);
            }
            i++;
            bytes_to_read -= this->data_buffer->get_buffer_size();
        }
    }
}
void StorageDevice::read_cache_one_sector(uint32_t lbal,uint32_t lbah,uint32_t bytesOffset)
{
    this->read(lbal,lbah,bytesOffset,(uint16_t*)this->data_buffer->get_ptr(),sector_size);
}
void* StorageDevice::get_buffer_data()
{
    return this->data_buffer->get_ptr();
}
uint16_t StorageDevice::get_sector_size()
{
    return this->sector_size;
}
uint16_t StorageDevice::get_hardware_protocol()
{
    return 0;
}
uint32_t StorageDevice::get_low_dword_cache()
{
    return reinterpret_cast<uint32_t*>(this->data_buffer)[0];
}
uint32_t StorageDevice::get_high_dword_cache()
{
    return reinterpret_cast<uint32_t*>(this->data_buffer)[1];
}
uint32_t StorageDevice::get_port()
{
    return this->port;
}