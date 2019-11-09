#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>

class SystemDescriptorTable {

public:
    ~SystemDescriptorTable();
    SystemDescriptorTable()
    {

    }
    virtual ACPITableHeader* get_table(const char* signature)
    {
        return nullptr;
    }
protected:
    void* sdt;
};