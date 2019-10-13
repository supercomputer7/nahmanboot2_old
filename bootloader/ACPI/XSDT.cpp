#include <ACPI/XSDT.h>

XSDT::~XSDT()
{

}
void XSDT::initialize(uint32_t xsdt)
{
    this->xsdt_table = (ACPI_XSDT*)xsdt;
}
uint32_t XSDT::get_sdts()
{
    return (uint32_t)this->xsdt_table->PointerToOtherSDT;
}
ACPISDTHeader* XSDT::get_table(const char* signature)
{
    int entries = (this->xsdt_table->h.Length - sizeof(this->xsdt_table->h)) / sizeof(uint64_t);
 
    for (int i = 0; i < entries; i++)
    {
        ACPITableHeader* h = (ACPITableHeader *) (uint32_t)this->xsdt_table->PointerToOtherSDT[i];
        if (strncmp(h->Signature, signature, 4))
            return (ACPISDTHeader*) h;
    }
    return 0;
}