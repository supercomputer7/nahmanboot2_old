#include "RSDT.h"

RSDT::~RSDT()
{

}
void RSDT::initialize(ACPI_RSDT* rsdt)
{
    this->rsdt_table = (ACPI_RSDT*)rsdt;
}

uint32_t RSDT::get_sdts()
{
    return (uint32_t)this->rsdt_table->PointerToOtherSDT;
}
ACPITableHeader* RSDT::get_table(const char* signature)
{
    return (ACPITableHeader*)this->rsdt_table->PointerToOtherSDT[0];
    int entries = (this->rsdt_table->h.Length - sizeof(this->rsdt_table->h)) / sizeof(uint32_t);
 
    for (int i = 0; i < entries; ++i)
    {
        ACPITableHeader* h = (ACPITableHeader *)this->rsdt_table->PointerToOtherSDT[i];
        if (strncmp(h->Signature, signature, 4))
            return (ACPITableHeader*) h;
    }
    return 0;
}