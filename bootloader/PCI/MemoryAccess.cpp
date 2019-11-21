#include <PCI/MemoryAccess.h>
PCI::MemoryAccess::MemoryAccess(ACPI_MCFG& mcfg_table)
{
    this->initialize(mcfg_table);
}
void PCI::MemoryAccess::initialize(ACPI_MCFG& mcfg_table)
{
    this->mcfg_table = &mcfg_table;
}
void PCI::MemoryAccess::write(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value)
{
    uint16_t* access = (uint16_t*)this->mcfg_table->spaces[seg].base_addr;
    access[(PCIeDeviceConfigSpaceSize * DevicesPerBus * FunctionsPerDevice * bus + 
            PCIeDeviceConfigSpaceSize * FunctionsPerDevice * device + 
            PCIeDeviceConfigSpaceSize * func + offset) >> 1] = value;
}
uint16_t PCI::MemoryAccess::read(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset)
{
    uint16_t* access = (uint16_t*)this->mcfg_table->spaces[seg].base_addr;
    return access[(PCIeDeviceConfigSpaceSize * DevicesPerBus * FunctionsPerDevice * bus + 
            PCIeDeviceConfigSpaceSize * FunctionsPerDevice * device + 
            PCIeDeviceConfigSpaceSize * func + offset) >> 1];
}
uint8_t PCI::MemoryAccess::get_access_type()
{
    return PCIMemoryAccess;
}
uint32_t PCI::MemoryAccess::get_segments_count()
{
    return (this->mcfg_table->h.Length - sizeof(mcfg_table->h) - sizeof(mcfg_table->Reserved)) / sizeof(PCIeSpaceConfig);
}
uint8_t PCI::MemoryAccess::get_segment_start_bus(uint32_t segment)
{
    return this->mcfg_table->spaces[segment].pci_start_bus;
}
uint8_t PCI::MemoryAccess::get_segment_end_bus(uint32_t segment)
{
    return this->mcfg_table->spaces[segment].pci_end_bus;
}