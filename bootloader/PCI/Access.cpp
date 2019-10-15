#include <PCI/Access.h>
#include <PCI/defs.h>
#include <PCI/PCI.h>
#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>

uint16_t PCI::read(PCI::Access* access, uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset)
{
    if(access->access_type == PCIMemoryAccess)
    {
        PCI::MemoryAccess* pcie_access = (PCI::MemoryAccess*)access;
        return pcie_access->read(seg,bus,device,func,offset);
    }
    else if(access->access_type == PCIIOAccess)
    {
        PCI::IOAccess* pci_access = (PCI::IOAccess*)access;
        return pci_access->read(bus,device,func,offset);
    }
    return 0xffff;
}
void PCI::write(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value)
{
    if(access->access_type == PCIMemoryAccess)
    {
        PCI::MemoryAccess* pcie_access = (PCI::MemoryAccess*)access;
        pcie_access->write(seg,bus,device,func,offset,value);
    }
    else if(access->access_type == PCIIOAccess)
    {
        PCI::IOAccess* pci_access = (PCI::IOAccess*)access;
        pci_access->write(bus,device,func,offset,value);
    }
}