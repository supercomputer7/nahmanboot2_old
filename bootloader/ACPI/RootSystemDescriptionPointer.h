#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>

class RootSystemDescriptionPointer {

public:
    ~RootSystemDescriptionPointer();
    
    RootSystemDescriptionPointer();
    RSDPDescriptor20* get_rsdp();
    ACPI_RSDT* get_rsdt();
    ACPI_XSDT* get_xsdt();
private:
    bool found_valid_rsdp;
    void initialize();
    RSDPDescriptor20* find();
    RSDPDescriptor20* search_rsdt(uint32_t addr,uint16_t count);
    
    uint8_t cached_rsdp[sizeof(RSDPDescriptor20)];
};