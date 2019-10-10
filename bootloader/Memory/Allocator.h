#pragma once
#include <stdint.h>

class BumpAllocator {

public:
    ~BumpAllocator();
    bool initialize(uint32_t addr);
    uint32_t allocate(uint32_t size);
    uint32_t get_addr();
private:
    uint32_t addr;
};