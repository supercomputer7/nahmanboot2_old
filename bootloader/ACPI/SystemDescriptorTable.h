#pragma once
#include <stdint.h>
#include <ACPI/defs.h>

class SystemDescriptorTable {

public:
    ~SystemDescriptorTable();
    SystemDescriptorTable()
    {

    }
    virtual ACPITableHeader* get_table(__attribute__((unused)) const char* signature)
    {
        return nullptr;
    }
protected:
    void* sdt;
};