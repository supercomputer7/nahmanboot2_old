#pragma once
#include <PCI/PCI.h>
#include <PCI/Access.h>
#include <stdint.h>

class PCI::MemoryAccess : protected PCI::Access {

public:
    explicit MemoryAccess(ACPI_MCFG* mcfg_table);
    ~MemoryAccess();
    void write(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset,uint16_t value) override;
    uint16_t read(uint32_t seg,uint8_t bus,uint8_t device,uint8_t func,uint8_t offset) override;
    uint8_t get_access_type() override;
    uint32_t get_segments_count() override;
    uint8_t get_segment_start_bus(uint32_t segment) override;
    uint8_t get_segment_end_bus(uint32_t segment) override;
private:
    void initialize(ACPI_MCFG* mcfg_table);
    ACPI_MCFG* mcfg_table;
};