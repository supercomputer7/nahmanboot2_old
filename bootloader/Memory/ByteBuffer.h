#pragma once
#include <stdint.h>
#include <LibC/stdbool.h>
#include <Memory/malloc.h>

class ByteBuffer {

public:
    ByteBuffer(void* addr,size_t size)
    {
        this->ptr = addr;
        this->buffer_size = size;
        this->byte_pointer = 0;
    }
    bool is_null()
    {
        return !(ptr == nullptr);
    }
    size_t get_buffer_size()
    {
        return buffer_size;
    }
    void null_it()
    {
        ptr = nullptr;
    }
    void* get_ptr()
    {
        return ptr;
    }
    uint8_t operator[](int i)
    {
        if(i < static_cast<int>(buffer_size))
            return static_cast<uint8_t*>(this->ptr)[i];
        else
            return 0;
    }
    uint8_t get_byte()
    {
        uint8_t byte;
        if(byte_pointer < buffer_size)
        {
            byte = static_cast<uint8_t*>(this->ptr)[byte_pointer];
            this->byte_pointer++;
        }
        else
        {
            reset_offset_pointer();
            byte = static_cast<uint8_t*>(this->ptr)[byte_pointer];
        }
        return byte;
    }
    void reset_offset_pointer()
    {
        this->byte_pointer = 0;
    }
private:
    void* ptr;
    uint32_t buffer_size;
    uint32_t byte_pointer;
};