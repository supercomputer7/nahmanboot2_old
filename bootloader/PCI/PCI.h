#pragma once
#include <stdint.h>
#include <PCI/defs.h>
#include <ACPI/defs.h>
#include <Memory/List.h>

namespace PCI
{
    class Access;
    class MemoryAccess;
    class IOAccess;
    class Device;

    PCI::Access* get_nonpcie_interface();
    PCI::Access* get_pcie_interface(ACPI_MCFG* mcfg);

    List<PCI::Device>* enum_devices(PCI::Access* access);
    List<PCI::Device>* find_storage_devices(List<PCI::Device>* pci_devices);

    uint32_t count(PCI::Access* access);
    uint16_t read(PCI::Access* access, uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset);
    void write(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value);
}