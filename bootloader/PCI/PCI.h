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

    PCI::Access* get_nonpcie_interface();
    PCI::Access* get_pcie_interface(ACPI_MCFG* mcfg);

    PCI::List* find_storage_devices(PCI::List* pci_devices);
    PCI::List* enum_devices(PCI::Access* access);

    uint32_t count(PCI::Access* access);
    uint16_t read(PCI::Access* access, uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset);
    void write(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value);
}