#include <PCI/List.h>

void PCI::List::initialize(Node<PCI::Device>* devices,uint32_t count)
{
    this->devices = devices;
    this->count = count;
}
Node<PCI::Device>* PCI::List::get_devices()
{
    return this->devices;
}
uint32_t PCI::List::get_count()
{
    return this->count;
}

uint32_t PCI::count(PCI::Access* access)
{
    int count = 0;
    if(access->access_type == PCIMemoryAccess)
    {
        PCI::MemoryAccess* pcie_access = (PCI::MemoryAccess*)access;
        uint8_t start_bus;
        uint8_t end_bus;
        for(int seg=0;seg<(int)pcie_access->get_segments_count();++seg)
        {
            start_bus = pcie_access->get_segment_start_bus(seg);
            end_bus = pcie_access->get_segment_end_bus(seg);
            for(uint8_t bus=start_bus; bus < end_bus; ++bus)
            {
                for(uint8_t device=0; device < DevicesPerBus; ++device)
                {
                    for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                    {
                        if(pcie_access->read(seg,bus,device,func,0) != 0xffff && pcie_access->read(seg,bus,device,func,2) != 0xffff)
                        {
                            ++count;
                        }
                    }
                }
            }
        }
        
    }
    else if(access->access_type == PCIIOAccess)
    {
        PCI::IOAccess* pci_access = (PCI::IOAccess*)access;
        uint8_t start_bus = 0x0;
        uint8_t end_bus = 0xff;
        for(uint8_t bus=start_bus; bus < end_bus; ++bus)
        {
            for(uint8_t device=0; device < DevicesPerBus; ++device)
            {
                for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                {
                    if(pci_access->read(bus,device,func,0) != 0xffff && pci_access->read(bus,device,func,2) != 0xffff)
                    {
                        ++count;
                    }
                }
            }
        } 
    }
    return count;
}