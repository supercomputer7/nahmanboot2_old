#pragma once
#include <stdint.h>
#include <PCI/defs.h>
#include <ACPI/defs.h>

#define PCIeDeviceConfigSpaceSize 4096
#define PCIDeviceConfigSpaceSize 256
#define DevicesPerBus 32
#define FunctionsPerDevice 8

#define PCIMemoryAccess 2
#define PCIIOAccess 1


namespace PCI
{
    class Access;
    class MemoryAccess;
    class IOAccess;
}