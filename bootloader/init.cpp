#include "init.h"

extern "C" void kmain()
{
    kmalloc_init();
    dma_alloc_init(E820_MemoryMapLocationDescriptor);
    cache_alloc_init(E820_MemoryMapLocationDescriptor);
    stdout::init();
    init();
}

void init()
{
    
    stdout::print("Nahmanboot (VGA 80x25), Initializing...\n");

    RSDP* rsdp = new RSDP();
    PCI::Access* access;
    if(rsdp->initialize())
        access = boot_acpi(rsdp);
    else
        access = boot_nonacpi();
    
    stdout::print("Probing PCI for Mass Storage Controllers...\n");
    List<PCI::Device>* pci_devices = PCI::enum_devices(access);
    List<PCI::Device>* pci_storage_devices = PCI::find_storage_devices(pci_devices);
    stdout::print("Preparing Devices...\n");

    List<GenericDiskController>* controllers = Disk::enum_storage_controllers(pci_storage_devices,access);
    List<StorageDevice>* storage_devices = Disk::enum_storage_devices(controllers);
    
    stdout::exit();
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

