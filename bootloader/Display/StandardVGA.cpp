#include "StandardVGA.h"

StandardVGA::~StandardVGA()
{
}

bool StandardVGA::initialize(uint32_t addr,uint32_t pitch)
{
    this->addr = (void*)addr;
    return true;
}

void StandardVGA::clear()
{
    for(int i=0;i<MaxBytesPerScreen/2;++i)
        *(uint16_t*)((uint32_t)this->addr + (uint32_t)(i << 1)) = 0x1f00 | 00;
}

uint32_t StandardVGA::get_color(uint32_t foreground_color,uint32_t background_color)
{
    return ((foreground_color << 4) | background_color);
}

uint32_t StandardVGA::get_pitch()
{
    return this->pitch;
}

void StandardVGA::write(char ch, uint32_t color,uint32_t offset)
{
    *(uint16_t*)((uint32_t)this->addr + (uint32_t)(offset << 1)) = color << 8 | ch;
}