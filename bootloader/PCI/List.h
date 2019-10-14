#pragma once
#include <PCI/PCI.h>
#include <PCI/Device.h>
#include <stdint.h>
class PCI::List{

public:
    ~List();
    void initialize(PCI::Device* devices,uint32_t count);
    PCI::Device* get_devices();
    uint32_t get_count();
private:
    PCI::Device* devices;
    uint32_t count;
};