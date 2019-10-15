#pragma once
#include <stdint.h>
#include <PCI/defs.h>
#include <ACPI/defs.h>

namespace PCI
{
    class Access;
    class MemoryAccess;
    class IOAccess;
    class Device;
    class List;
    uint32_t count(PCI::Access* access);
}