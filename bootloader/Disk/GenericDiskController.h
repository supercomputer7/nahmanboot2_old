#pragma once

#include <stdint.h>

#define IDE_DiskController 0x1
#define AHCI_DiskController 0x2
#define NVMe_DiskController 0x3

class GenericDiskController {

public:
    ~GenericDiskController();
    uint16_t type;
private:
    
};