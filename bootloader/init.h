#pragma once

#include <stdint.h>
#include <LibC/stdstring.h>


#include <Memory/Allocator.h>
#include <Memory/malloc.h>
#include <Memory/Node.h>
#include <Memory/List.h>

#include <Disk/StorageDevice.h>
#include <Disk/GenericDiskController.h>
#include <Disk/IDEController.h>
#include <Disk/AHCIController.h>
#include <Disk/Disk.h>

#include <Filesystem/Partition.h>
#include <Filesystem/Ext2Filesystem.h>

#include <Parsers/ELF/ELF32Parser.h>

#include <Kernels/MultibootKernel.h>

#include <Display/Print.h>

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