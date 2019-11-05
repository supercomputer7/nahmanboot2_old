#pragma once

#include <stdint.h>
#include <LibC/stdbool.h>

class FramebufferResolution {

public:
    ~FramebufferResolution();
    FramebufferResolution(uint32_t modenumber,uint32_t resolution_pitch,uint32_t resolution_width,uint32_t resolution_height,uint32_t resolution_bpp,uint32_t framebuffer,bool vga_compatible,uint32_t resolution_memory_model,uint32_t resolution_pixel_format);

    uint32_t get_linearframebuffer();
    uint32_t get_width();
    uint32_t get_bpp();
    uint32_t get_pitch();
    uint32_t get_height();
    uint32_t get_modenumber();
    bool     is_vga_compatible();
    uint8_t  get_memory_model();
    uint32_t get_pixel_format();
private:
    uint32_t mode_number;
    uint32_t resolution_width;
    uint32_t resolution_height;
    uint32_t resolution_bpp;
    uint32_t resolution_pitch;
    uint32_t lfb;
    bool resolution_vga_compatible;
    uint8_t resolution_memory_model;
    uint32_t resolution_pixel_format;
};