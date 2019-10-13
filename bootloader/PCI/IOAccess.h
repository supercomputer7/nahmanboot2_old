#pragma once
#include <PCI/PCI.h>
#include <PCI/Access.h>
#include <stdint.h>
#include <IO/IO.h>

class PCI::IOAccess : public PCI::Access {

public:
    ~IOAccess();
    void initialize();
    void write(uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value);
    uint16_t read(uint8_t bus,uint8_t device,uint8_t func,uint8_t offset);
    void save_cache_descriptor(uint8_t bus,uint8_t device,uint8_t func);
    uint16_t read_cache(uint8_t offset);
private:
    uint8_t cached_descriptor[256];
};