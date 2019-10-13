#include <PCI/MemoryAccess.h>
void PCI::MemoryAccess::initialize(ACPI_MCFG* mcfg_table)
{
    this->mcfg_table = mcfg_table;
    this->access_type = PCIMemoryAccess;
}
void PCI::MemoryAccess::write(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value)
{
    uint16_t* access = (uint16_t*)this->mcfg_table->spaces[seg].base_addr;
    access[(PCIeDeviceConfigSpaceSize * DevicesPerBus * FunctionsPerDevice * bus + 
            PCIeDeviceConfigSpaceSize * FunctionsPerDevice * device + 
            PCIeDeviceConfigSpaceSize * func + offset)] = value;
}
uint16_t PCI::MemoryAccess::read(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset)
{
    uint16_t* access = (uint16_t*)this->mcfg_table->spaces[seg].base_addr;
    return access[(PCIeDeviceConfigSpaceSize * DevicesPerBus * FunctionsPerDevice * bus + 
            PCIeDeviceConfigSpaceSize * FunctionsPerDevice * device + 
            PCIeDeviceConfigSpaceSize * func + offset) >> 1];
}