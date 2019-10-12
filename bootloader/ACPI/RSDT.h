#pragma once
#include <stdint.h>
#include "../LibC/stdstring.h"
#include "defs.h"

class RSDT {

public:
    ~RSDT();
    void initialize(ACPI_RSDT* rsdt);
    ACPITableHeader* get_table(const char* signature);
    uint32_t get_sdts();
private:
    ACPI_RSDT* rsdt_table;
};