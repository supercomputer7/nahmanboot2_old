#pragma once

#include <stdint.h>
#include <LibC/stdstring.h>


#include <Memory/Allocator.h>
#include <Memory/malloc.h>

#include <Display/StandardVGA.h>
#include <Display/TerminalDriver.h>
#include <ACPI/RSDP.h>
#include <ACPI/XSDT.h>
#include <ACPI/RSDT.h>

#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>
#include <PCI/List.h>
#include <PCI/Device.h>

extern "C" void kmain();
void init();

void halt();

PCI::List* enum_devices(PCI::Access* access);

PCI::Access* boot_acpi(TerminalDriver* terminal,RSDP* rsdp);
PCI::Access* boot_nonacpi(TerminalDriver* terminal);

PCI::Access* get_nonpcie_interface();
PCI::Access* get_pcie_interface(ACPI_MCFG* mcfg);