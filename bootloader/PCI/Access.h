#pragma once
#include <PCI/PCI.h>
#include <stdint.h>
class PCI::Access{

public:
    ~Access();
    virtual uint16_t read(__attribute__((unused)) uint32_t seg,__attribute__((unused)) uint8_t bus,__attribute__((unused)) uint8_t device,__attribute__((unused)) uint8_t func,__attribute__((unused)) uint8_t offset)
    {
        return 0xffff;
    }
    virtual void write(__attribute__((unused)) uint32_t seg,__attribute__((unused)) uint8_t bus,__attribute__((unused)) uint8_t device,__attribute__((unused)) uint8_t func,__attribute__((unused)) uint8_t offset,__attribute__((unused)) uint16_t value)
    {

    }
    virtual uint8_t get_access_type()
    {
        return 0;
    }
    virtual uint32_t get_segments_count()
    {
        return 0;
    }
    virtual uint8_t get_segment_start_bus(__attribute__((unused)) uint32_t segment)
    {
        return 0;
    }
    virtual uint8_t get_segment_end_bus(__attribute__((unused)) uint32_t segment)
    {
        return 0;
    }
protected:
};