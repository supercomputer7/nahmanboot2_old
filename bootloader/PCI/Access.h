#pragma once
#include <PCI/PCI.h>
#include <stdint.h>
class PCI::Access{

public:
    ~Access();
    uint8_t get_access_type();
protected:
    uint8_t access_type;
};