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
    PCI::List* list_devices = enum_devices(access);

    PCI::List* storage_devices = find_storage_devices(list_devices);


    terminal->write("Preparing Devices...\n");
    terminal->exit();
}

void halt()
{
    asm volatile ("cli;"
    "hlt");
}

PCI::List* find_storage_devices(PCI::List* pci_devices)
{
    Node* node = new Node();
    Node* tmp_node = node;

    Node* device = pci_devices->get_devices();
    int count = 0;
    for(int i=0; i<(int)pci_devices->get_count();++i)
    {
        PCI::Device* device_node = (PCI::Device*)device->get_object();
        if(device_node->get_class_code() == 0x1)
        {
            tmp_node->initialize((void*) device_node,NULL);
            tmp_node->set_next(new Node());
            tmp_node = tmp_node->get_next();
            count++;
        }
        if((void*)device->get_next() == NULL)
            break;
        device = device->get_next();
    }
    PCI::List* list =  new PCI::List();
    list->initialize(node,count);
    return list;
}

PCI::List* enum_devices(PCI::Access* access)
{
    Node* node = new Node();
    Node* node_tmp = (Node*)node;
    int count = 0;
    if(access->access_type == PCIMemoryAccess)
    {
        PCI::MemoryAccess* pcie_access = (PCI::MemoryAccess*)access;
        uint8_t start_bus;
        uint8_t end_bus;
        for(int seg=0;seg<(int)pcie_access->get_segments_count();++seg)
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
                            PCI::Device* tmp = new PCI::Device();
                            node_tmp->initialize((void*)tmp,NULL);
                            tmp->initialize(access,seg,bus,device,func);
                            node_tmp->set_next(new Node());
                            node_tmp = node_tmp->get_next();
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
                        PCI::Device* tmp = new PCI::Device();
                        node_tmp->initialize((void*)tmp,NULL);
                        tmp->initialize(access,0,bus,device,func);
                        node_tmp->set_next(new Node());
                        node_tmp = node_tmp->get_next();
                        count++;
                    }
                }
            }
        }
        
    }
    PCI::List* list =  new PCI::List();
    list->initialize(node,count);
    return list;
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
        return get_nonpcie_interface();
        
    }
    else
    {
        return get_pcie_interface(mcfg);
    }
}
PCI::Access* boot_nonacpi()
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