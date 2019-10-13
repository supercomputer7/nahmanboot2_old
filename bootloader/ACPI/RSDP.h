#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>

class RSDP {

public:
    ~RSDP();
    
    bool initialize();
    RSDPDescriptor20* get_rsdp();
    ACPI_RSDT* get_rsdt();
    ACPI_XSDT* get_xsdt();
private:
    uint32_t find();
    uint32_t search_rsdt(uint32_t addr,uint16_t count);
    
    uint8_t cached_rsdp[sizeof(RSDPDescriptor20)];
};