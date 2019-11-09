#include <PCI/Access.h>
#include <PCI/Device.h>
#include <PCI/defs.h>
#include <PCI/PCI.h>
#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>

#include <stdint.h>
#include <stddef.h>
#include <Memory/Node.h>

#include <ACPI/ACPIManager.h>

PCI::Access* PCI::get_access()
{
    ACPIManager* acpi_manager = new ACPIManager();
    ACPI_MCFG* mcfg = (ACPI_MCFG*)acpi_manager->get_table("MCFG");
    if(mcfg != nullptr)
        return PCI::get_pcie_interface(mcfg);
    else
        return PCI::get_nonpcie_interface();
}

uint16_t PCI::read(PCI::Access* access, uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset)
{
    return access->read(seg,bus,device,func,offset);
}
void PCI::write(PCI::Access* access,uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value)
{
    access->write(seg,bus,device,func,offset,value);
}

List<PCI::Device>* PCI::find_storage_devices(List<PCI::Device>* pci_devices)
{
    List<PCI::Device>* tmp = new List<PCI::Device>(nullptr,0);
    for(int i=0; i<(int)pci_devices->get_count();++i)
        if(pci_devices->get_node(i)->get_object()->get_class_code() == 0x1)
            tmp->insert_node(pci_devices->get_node(i)->get_object());
    return tmp;
}

List<PCI::Device>* PCI::enum_devices(PCI::Access* access)
{
    List<PCI::Device>* tmp_list = new List<PCI::Device>(nullptr,0);
    for(uint32_t i=0; i < access->get_segments_count(); i++)
        for(uint8_t bus=access->get_segment_start_bus(i); bus < access->get_segment_end_bus(i); ++bus)
            for(uint8_t device=0; device < DevicesPerBus; ++device)
                for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                    if(access->read(i,bus,device,func,0) != 0xffff && access->read(i,bus,device,func,2) != 0xffff)
                        tmp_list->insert_node(new PCI::Device(access,i,bus,device,func));
    return tmp_list;
}

PCI::Access* PCI::get_nonpcie_interface()
{
    return new PCI::IOAccess();
}
PCI::Access* PCI::get_pcie_interface(ACPI_MCFG* mcfg)
{
    return new PCI::MemoryAccess(mcfg);
}