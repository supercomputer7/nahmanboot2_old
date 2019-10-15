#pragma once
#include <stdint.h>
#include <PCI/defs.h>
#include <ACPI/defs.h>

namespace PCI
{
    class Access;
    class MemoryAccess;
    class IOAccess;
    class Device;
    class List;
    uint32_t count(PCI::Access* access);
    uint16_t read(PCI::Access* access, uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset);
    void write(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value);
}