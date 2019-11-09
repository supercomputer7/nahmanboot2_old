#include <ACPI/RootSystemDescriptionPointer.h>

RootSystemDescriptionPointer::~RootSystemDescriptionPointer()
{

}
RootSystemDescriptionPointer::RootSystemDescriptionPointer()
{
    this->initialize();
}
void RootSystemDescriptionPointer::initialize()
{
    RSDPDescriptor20* p = this->find();
    if(p != nullptr)
    {
        found_valid_rsdp = true;
        strcopy((char*)p,(char*)&this->cached_rsdp,sizeof(RSDPDescriptor20));
    }
    else
    {
        found_valid_rsdp = false;
    }
    
}
RSDPDescriptor20* RootSystemDescriptionPointer::find()
{
    uint16_t count = 64;
    uint32_t addr = 0x80000;
    
    RSDPDescriptor20* p = this->search_rsdt(addr,count);
    if(p != nullptr)
        return p;
    else
    {
        count = 8191;
        addr = 0xE0000;
        return this->search_rsdt(addr,count);        
    }
}
RSDPDescriptor20* RootSystemDescriptionPointer::get_rsdp()
{
    if(found_valid_rsdp)
        return reinterpret_cast<RSDPDescriptor20*>(this->cached_rsdp);
    else 
        return nullptr;
}

ACPI_RSDT* RootSystemDescriptionPointer::get_rsdt()
{
    if(found_valid_rsdp)
        return reinterpret_cast<ACPI_RSDT*>(this->get_rsdp()->firstPart.RsdtAddress);
    else
        return nullptr;
}
ACPI_XSDT* RootSystemDescriptionPointer::get_xsdt()
{
    if(found_valid_rsdp)
        return reinterpret_cast<ACPI_XSDT*>(this->get_rsdp()->XsdtAddress);
    else
        return nullptr;
}

RSDPDescriptor20* RootSystemDescriptionPointer::search_rsdt(uint32_t addr,uint16_t count)
{
    char *p = (char*)addr;
    for(int i=0; i<count; ++i)
    {
        if(strncmp((char*)p,"RSD PTR ",strlen("RSD PTR ")))
            return (RSDPDescriptor20*)p;
        p += 0x10;
    }
    return nullptr;
}