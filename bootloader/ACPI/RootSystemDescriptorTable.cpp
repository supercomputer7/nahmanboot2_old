#include <ACPI/RootSystemDescriptorTable.h>

RootSystemDescriptorTable::RootSystemDescriptorTable(ACPI_RSDT* rsdt) : SystemDescriptorTable()
{
    this->sdt = rsdt;
}
ACPITableHeader* RootSystemDescriptorTable::get_table(const char* signature)
{
    for (int i = 0; i < (get_rsdt()->h.Length - sizeof(get_rsdt()->h)) / sizeof(uint32_t); ++i)
    {
        ACPITableHeader* h = (ACPITableHeader *)get_rsdt()->PointerToOtherSDT[i];
        if (strncmp(h->Signature, signature, 4))
            return (ACPITableHeader*) h;
    }
    return nullptr;
}
ACPI_RSDT* RootSystemDescriptorTable::get_rsdt()
{
    return reinterpret_cast<ACPI_RSDT*>(this->sdt);
}