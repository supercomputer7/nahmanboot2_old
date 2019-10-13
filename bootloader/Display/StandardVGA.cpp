#include "StandardVGA.h"

StandardVGA::~StandardVGA()
{
}

void StandardVGA::initialize(uint32_t addr,uint32_t pitch, uint32_t foreground_color,uint32_t background_color)
{
    this->addr = (void*)addr;
    this->pitch = pitch;
    this->background_color = background_color;
    this->foreground_color = foreground_color;
}

void StandardVGA::clear()
{
    for(int i=0;i<MaxBytesPerScreen/2;++i)
        *(uint16_t*)((uint32_t)this->addr + (uint32_t)(i << 1)) = (uint16_t)this->foreground_color << 8 | (uint16_t)this->background_color;
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