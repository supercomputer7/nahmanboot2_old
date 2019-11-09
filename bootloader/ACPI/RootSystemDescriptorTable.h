#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <ACPI/defs.h>
#include <ACPI/SystemDescriptorTable.h>

class RootSystemDescriptorTable : public SystemDescriptorTable {

public:
    ~RootSystemDescriptorTable();
    explicit RootSystemDescriptorTable(ACPI_RSDT* rsdt);
    ACPITableHeader* get_table(const char* signature) override;
private:
    ACPI_RSDT* get_rsdt();
};