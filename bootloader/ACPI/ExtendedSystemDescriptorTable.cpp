#include <ACPI/ExtendedSystemDescriptorTable.h>

ExtendedSystemDescriptorTable::ExtendedSystemDescriptorTable(ACPI_XSDT* xsdt) : SystemDescriptorTable()
{
    this->sdt = xsdt;
}
ACPITableHeader* ExtendedSystemDescriptorTable::get_table(const char* signature)
{
    for (int i = 0; i < static_cast<int>((get_xsdt()->h.Length - sizeof(get_xsdt()->h)) / sizeof(uint64_t)); i++)
    {
        ACPITableHeader* h = (ACPITableHeader *) (uint32_t)get_xsdt()->PointerToOtherSDT[i];
        if (strncmp(h->Signature, signature, 4))
            return (ACPISDTHeader*) h;
    }
    return nullptr;
}
ACPI_XSDT* ExtendedSystemDescriptorTable::get_xsdt()
{
    return reinterpret_cast<ACPI_XSDT*>(this->sdt);
}