#include "Allocator.h"


BumpAllocator::~BumpAllocator()
{
}

bool BumpAllocator::initialize(uint32_t addr)
{
    this->addr = addr;
    return true;
}

uint32_t BumpAllocator::allocate(uint32_t size)
{
    uint32_t tmp = this->addr;
    this->addr += size;
    return tmp;
}
uint32_t BumpAllocator::get_addr()
{
    return this->addr;
}
