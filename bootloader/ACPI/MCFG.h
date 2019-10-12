#pragma once
#include <stdint.h>
#include "../LibC/stdstring.h"
#include "defs.h"

class MCFG {

public:
    ~MCFG();
    void initialize(ACPI_MCFG* rsdt);
    ACPI_MCFG* get_table();
private:
    ACPI_MCFG* mcfg_table;
};