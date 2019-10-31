#pragma once

#include <stdint.h>
#include <LibC/stdstring.h>


#include <Memory/Allocator.h>
#include <Memory/malloc.h>
#include <Memory/Node.h>
#include <Memory/List.h>

#include <Disk/Disk.h>

#include <Partition/Partition.h>
#include <Filesystem/LinuxFilesystems/Ext2Filesystem.h>

#include <Parsers/ELF/ELF32Parser.h>

#include <Kernels/MultibootKernel.h>

#include <Display/Monitors/Print.h>

#include <ACPI/RSDP.h>
#include <ACPI/XSDT.h>
#include <ACPI/RSDT.h>

#include <PCI/IOAccess.h>
#include <PCI/MemoryAccess.h>
#include <PCI/Device.h>

extern "C" void kmain();
void init();

void halt();

PCI::Access* boot_acpi(RSDP* rsdp);
PCI::Access* boot_nonacpi();