#pragma once

#include <stdint.h>
#define MaxBytesPerScreen 4000

class StandardVGA {

public:
    ~StandardVGA();
    bool initialize(uint32_t addr,uint32_t pitch);
    void write(char ch, uint32_t color,uint32_t offset);
    uint32_t get_color(uint32_t foreground_color,uint32_t background_color);
    uint32_t get_pitch();
    void clear();
private:
    void* addr;
    uint32_t pitch;
};