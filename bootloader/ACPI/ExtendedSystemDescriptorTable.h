#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>
#include <ACPI/SystemDescriptorTable.h>

class ExtendedSystemDescriptorTable : public SystemDescriptorTable {

public:
    ~ExtendedSystemDescriptorTable();
    explicit ExtendedSystemDescriptorTable(ACPI_XSDT* xsdt);
    ACPITableHeader* get_table(const char* signature) override;
private:
    ACPI_XSDT* get_xsdt();
};