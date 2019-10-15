#include "init.h"

extern "C" void kmain()
{
    kmalloc_init();
    init();
}

void init()
{
    StandardVGA* screen = new StandardVGA();
    screen->initialize(0xb8000,80,0,0xf);

    TerminalDriver* terminal = new TerminalDriver();
    terminal->initialize((StandardVGA*)screen,0,0xf,80,25);
    terminal->clear();

    terminal->write("Nahmanboot (VGA 80x25), Initializing...\n");

    RSDP* rsdp = new RSDP();
    PCI::Access* access;
    if(rsdp->initialize())
        access = boot_acpi(rsdp);
    else
        access = boot_nonacpi();
    
    terminal->write("Probing PCI for Mass Storage Controllers...\n");
    PCI::List* list_devices = PCI::enum_devices(access);
    PCI::List* storage_devices = PCI::find_storage_devices(list_devices);
    terminal->write("Preparing Devices...\n");
    
    terminal->exit();
}

void halt()
{
    asm volatile ("cli;"
    "hlt");
}

PCI::Access* boot_acpi(RSDP* rsdp)
{
    ACPI_MCFG* mcfg;

    if(rsdp->get_rsdp()->firstPart.Revision >= 2 || rsdp->get_xsdt() != 0)
    {
        XSDT* xsdt = new XSDT();
        xsdt->initialize((uint32_t)rsdp->get_xsdt());
        mcfg = (ACPI_MCFG*)xsdt->get_table("MCFG");
    }
    else
    {
        RSDT* rsdt = new RSDT();
        rsdt->initialize(rsdp->get_rsdt());
        mcfg = (ACPI_MCFG*)rsdt->get_table("MCFG");
    }

    if (mcfg == 0)
    {
        return PCI::get_nonpcie_interface();
        
    }
    else
    {
        return PCI::get_pcie_interface(mcfg);
    }
}
PCI::Access* boot_nonacpi()
{
    return (PCI::Access*)PCI::get_nonpcie_interface();
}

