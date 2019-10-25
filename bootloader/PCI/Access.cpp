#include <PCI/Access.h>
#include <PCI/Device.h>
#include <PCI/defs.h>
#include <PCI/PCI.h>
#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>

#include <stdint.h>
#include <stddef.h>
#include <Memory/Node.h>

uint8_t PCI::Access::get_access_type()
{
    return this->access_type;
}

uint16_t PCI::read(PCI::Access* access, uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset)
{
    if(access->get_access_type() == PCIMemoryAccess)
        return reinterpret_cast<PCI::MemoryAccess*>(access)->read(seg,bus,device,func,offset);
    else if(access->get_access_type() == PCIIOAccess)
        return reinterpret_cast<PCI::IOAccess*>(access)->read(bus,device,func,offset);
    return 0xffff;
}
void PCI::write(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value)
{
    if(access->get_access_type() == PCIMemoryAccess)
        reinterpret_cast<PCI::MemoryAccess*>(access)->write(seg,bus,device,func,offset,value);
    else if(access->get_access_type() == PCIIOAccess)
        reinterpret_cast<PCI::IOAccess*>(access)->write(bus,device,func,offset,value);
}

List<PCI::Device>* PCI::find_storage_devices(List<PCI::Device>* pci_devices)
{
    List<PCI::Device>* tmp = new List<PCI::Device>(nullptr,0);
    for(int i=0; i<(int)pci_devices->get_count();++i)
    {
        PCI::Device* device = pci_devices->get_node(i)->get_object();
        if(device->get_class_code() == 0x1)
            tmp->insert_node(device);
    }
    return tmp;
}

List<PCI::Device>* PCI::enum_devices(PCI::Access* access)
{
    List<PCI::Device>* tmp_list = new List<PCI::Device>(nullptr,0);
    if(access->get_access_type() == PCIMemoryAccess)
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
                            PCI::Device* tmp = new PCI::Device();
                            tmp->initialize(access,seg,bus,device,func);
                            tmp_list->insert_node(tmp);
                        }
                    }
                }
            }
        }
        
    }
    else if(access->get_access_type() == PCIIOAccess)
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
                        PCI::Device* tmp = new PCI::Device();
                        tmp->initialize(access,0,bus,device,func);
                        tmp_list->insert_node(tmp);
                    }
                }
            }
        }
        
    }
    return tmp_list;
}

PCI::Access* PCI::get_nonpcie_interface()
{
    PCI::IOAccess* tmp = new PCI::IOAccess();
    tmp->initialize();
    return (PCI::Access*)tmp;
}
PCI::Access* PCI::get_pcie_interface(ACPI_MCFG* mcfg)
{
    PCI::MemoryAccess* tmp = new PCI::MemoryAccess();
    tmp->initialize(mcfg);
    return (PCI::Access*)tmp;
}