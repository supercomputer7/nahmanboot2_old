#include <PCI/Access.h>
#include <PCI/defs.h>
#include <PCI/PCI.h>
#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>
#include <PCI/List.h>

#include <stdint.h>
#include <stddef.h>
#include <Memory/Node.h>

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

PCI::List* PCI::find_storage_devices(PCI::List* pci_devices)
{
    Node* node = new Node();
    Node* tmp_node = node;

    Node* device = pci_devices->get_devices();
    int count = 0;
    for(int i=0; i<(int)pci_devices->get_count();++i)
    {
        PCI::Device* device_node = (PCI::Device*)device->get_object();
        if(device_node->get_class_code() == 0x1)
        {
            tmp_node->initialize((void*) device_node,NULL);
            tmp_node->set_next(new Node());
            tmp_node = tmp_node->get_next();
            count++;
        }
        if((void*)device->get_next() == NULL)
            break;
        device = device->get_next();
    }
    PCI::List* list =  new PCI::List();
    list->initialize(node,count);
    return list;
}

PCI::List* PCI::enum_devices(PCI::Access* access)
{
    Node* node = new Node();
    Node* node_tmp = (Node*)node;
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
                            PCI::Device* tmp = new PCI::Device();
                            node_tmp->initialize((void*)tmp,NULL);
                            tmp->initialize(access,seg,bus,device,func);
                            node_tmp->set_next(new Node());
                            node_tmp = node_tmp->get_next();
                            count++;
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
                        PCI::Device* tmp = new PCI::Device();
                        node_tmp->initialize((void*)tmp,NULL);
                        tmp->initialize(access,0,bus,device,func);
                        node_tmp->set_next(new Node());
                        node_tmp = node_tmp->get_next();
                        count++;
                    }
                }
            }
        }
        
    }
    PCI::List* list =  new PCI::List();
    list->initialize(node,count);
    return list;
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