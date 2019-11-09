#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>
#include <ACPI/SystemDescriptorTable.h>
#include <ACPI/RootSystemDescriptionPointer.h>

class ACPIManager {

public:
    ~ACPIManager();
    ACPIManager();
    ACPITableHeader* get_table(const char* signature);
private:
    SystemDescriptorTable* sdt;
    RootSystemDescriptionPointer* rsdp;
};