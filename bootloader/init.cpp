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
        access = boot_acpi(terminal,rsdp);
    else
        access = boot_nonacpi(terminal);
    
    terminal->write("Probing PCI for Mass Storage Controllers...\n");
    PCI::List* list_devices = enum_devices(access);


    terminal->write("Preparing Devices...\n");
    terminal->exit();
}

void halt()
{
    asm volatile ("cli;"
    "hlt");
}

PCI::List* enum_devices(PCI::Access* access)
{
    PCI::Device* devices[0];
    int count = 0;
    if(access->access_type == PCIMemoryAccess)
    {
        PCI::MemoryAccess* pcie_access = (PCI::MemoryAccess*)access;
        uint8_t start_bus;
        uint8_t end_bus;
        for(int seg=0;seg<pcie_access->get_segments_count();++seg)
        {
            start_bus = pcie_access->get_segment_start_bus(seg);
            end_bus = pcie_access->get_segment_end_bus(seg);
            for(uint8_t bus=start_bus; bus < end_bus; ++bus)
            {
                for(uint8_t device=0; device < DevicesPerBus; ++device)
                {
                    for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                    {
                        if(pcie_access->read(seg,bus,device,func,0) != 0xffff && pcie_access->read(seg,bus,device,func,2) != 0xffff)
                        {
                            devices[count] = new PCI::Device();
                            devices[count]->initialize(access,seg,bus,device,func);
                            count++;
                        }
                    }
                }
            }
        }
        
    }
    else if(access->access_type == PCIIOAccess)
    {
        PCI::IOAccess* pci_access = (PCI::IOAccess*)access;
        uint8_t start_bus = 0x0;
        uint8_t end_bus = 0xff;
        for(uint8_t bus=start_bus; bus < end_bus; ++bus)
        {
            for(uint8_t device=0; device < DevicesPerBus; ++device)
            {
                for(uint8_t func=0; func < FunctionsPerDevice; ++func)
                {
                    if(pci_access->read(bus,device,func,0) != 0xffff && pci_access->read(bus,device,func,2) != 0xffff)
                    {
                        devices[count] = new PCI::Device();
                        devices[count]->initialize(access,0,bus,device,func);
                        count++;
                    }
                }
            }
        }
        
    }
    PCI::List* list =  new PCI::List();
    list->initialize((PCI::Device*)devices[0],count);
    return list;
}

PCI::Access* boot_acpi(TerminalDriver* terminal,RSDP* rsdp)
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
        return get_nonpcie_interface();
        
    }
    else
    {
        return get_pcie_interface(mcfg);
    }
}
PCI::Access* boot_nonacpi(TerminalDriver* terminal)
{
    return (PCI::Access*)get_nonpcie_interface();
}

PCI::Access* get_nonpcie_interface()
{
    PCI::IOAccess* tmp = new PCI::IOAccess();
    tmp->initialize();
    return (PCI::Access*)tmp;
}
PCI::Access* get_pcie_interface(ACPI_MCFG* mcfg)
{
    PCI::MemoryAccess* tmp = new PCI::MemoryAccess();
    tmp->initialize(mcfg);
    return (PCI::Access*)tmp;
}