#pragma once
#include <PCI/PCI.h>
#include <PCI/Access.h>
#include <stdint.h>
#include <IO/IO.h>

class PCI::IOAccess : protected PCI::Access {

public:
    ~IOAccess();
    void write(__attribute__((unused)) uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value) override;
    uint16_t read(__attribute__((unused)) uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset) override;
    uint8_t get_access_type() override;
    uint32_t get_segments_count() override;
    uint8_t get_segment_start_bus(uint32_t segment) override;
    uint8_t get_segment_end_bus(uint32_t segment) override;
    void save_cache_descriptor(uint8_t bus,uint8_t device,uint8_t func);
    uint16_t read_cache(uint8_t offset);
private:
    uint8_t cached_descriptor[256];
};