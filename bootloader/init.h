#pragma once

#include <stdint.h>
#include <Memory/malloc.h>
#include <Memory/List.h>
#include <Disk/Disk.h>
#include <Partition/PartitionProbing.h>
#include <Partition/GenericStoragePartition.h>
#include <Partition/DOSPartition.h>
#include <Parsers/ELF/ELF32Parser.h>
#include <Kernels/MultibootKernel.h>
#include <Kernels/LinuxKernel.h>
#include <Display/Monitors/Print.h>
#include <ACPI/ACPIManager.h>
#include <PCI/Access.h>
#include <CPU/PIC.h>

extern "C" void kmain();
void init();
void halt();
