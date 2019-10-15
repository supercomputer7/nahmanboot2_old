#pragma once

#include <stdint.h>
#include <LibC/stdstring.h>


#include <Memory/Allocator.h>
#include <Memory/malloc.h>
#include <Memory/Node.h>

#include <Disk/IDEController.h>
#include <Disk/AHCIController.h>

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

PCI::Access* boot_acpi(RSDP* rsdp);
PCI::Access* boot_nonacpi();