#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>

class XSDT {

public:
    ~XSDT();
    void initialize(uint32_t xsdt);
    ACPISDTHeader* get_table(const char* signature);
    uint32_t get_sdts();
private:
    ACPI_XSDT* xsdt_table;
};