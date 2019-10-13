#include <ACPI/RSDP.h>

RSDP::~RSDP()
{

}
bool RSDP::initialize()
{
    uint32_t p = this->find();
    if(p == 0)
    {
        return false;
    }
    strcopy((char*)p,(char*)&this->cached_rsdp,sizeof(RSDPDescriptor20));
    return true;
}
uint32_t RSDP::find()
{
    uint16_t count = 64;
    uint32_t addr = 0x80000;
    
    uint32_t p = this->search_rsdt(addr,count);
    if(p != 0)
        return p;
    else
    {
        count = 8191;
        addr = 0xE0000;
        return this->search_rsdt(addr,count);        
    }
}
RSDPDescriptor20* RSDP::get_rsdp()
{
    return (RSDPDescriptor20*)&this->cached_rsdp;
}

ACPI_RSDT* RSDP::get_rsdt()
{
    return (ACPI_RSDT*)this->get_rsdp()->firstPart.RsdtAddress;
}
ACPI_XSDT* RSDP::get_xsdt()
{
    return (ACPI_XSDT*)this->get_rsdp()->XsdtAddress;
}

uint32_t RSDP::search_rsdt(uint32_t addr,uint16_t count)
{
    char *p = (char*)addr;
    for(int i=0; i<count; ++i)
    {
        if(strncmp((char*)p,"RSD PTR ",strlen("RSD PTR ")))
            return (uint32_t)p;
        p = p + 0x10;
    }
    return 0;
}