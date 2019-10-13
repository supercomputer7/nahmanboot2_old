#pragma once
#include <PCI/PCI.h>
#include <PCI/Access.h>
#include <stdint.h>

class PCI::MemoryAccess : public PCI::Access {

public:
    ~MemoryAccess();
    void initialize(ACPI_MCFG* mcfg_table);
    void write(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value);
    uint16_t read(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset);
private:
    ACPI_MCFG* mcfg_table;
};