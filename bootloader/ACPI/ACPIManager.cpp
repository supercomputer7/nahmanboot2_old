#include <ACPI/ACPIManager.h>
#include <ACPI/ExtendedSystemDescriptorTable.h>
#include <ACPI/RootSystemDescriptorTable.h>
ACPIManager::ACPIManager()
{
    rsdp = new RootSystemDescriptionPointer();
    if(rsdp->get_xsdt() != nullptr)
        sdt = new ExtendedSystemDescriptorTable(rsdp->get_xsdt());
    else
        if(rsdp->get_rsdt() != nullptr)
            sdt = new RootSystemDescriptorTable(rsdp->get_rsdt());
}
ACPITableHeader* ACPIManager::get_table(const char* signature)
{
    if(sdt != nullptr)
        return sdt->get_table(signature);
    else
        return nullptr;
}