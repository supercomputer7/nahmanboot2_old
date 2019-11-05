#include <Display/Protocols/FramebufferResolution.h>
FramebufferResolution::FramebufferResolution(uint32_t modenumber,uint32_t resolution_pitch,uint32_t resolution_width,uint32_t resolution_height,uint32_t resolution_bpp,uint32_t framebuffer,bool vga_compatible,uint32_t resolution_memory_model,uint32_t resolution_pixel_format)
{
    this->resolution_bpp = resolution_bpp;
    this->resolution_height = resolution_height;
    this->resolution_width = resolution_width;
    this->resolution_pitch = resolution_pitch;
    this->mode_number = modenumber;
    this->lfb = framebuffer;
    this->resolution_memory_model = resolution_memory_model;
    this->resolution_vga_compatible = vga_compatible;
    this->resolution_pixel_format = resolution_pixel_format;
}
uint32_t FramebufferResolution::get_width()
{
    return this->resolution_width;
}
uint32_t FramebufferResolution::get_pitch()
{
    return this->resolution_pitch;
}
uint32_t FramebufferResolution::get_bpp()
{
    return this->resolution_bpp;
}
uint32_t FramebufferResolution::get_height()
{
    return this->resolution_height;
}
uint32_t FramebufferResolution::get_modenumber()
{
    return this->mode_number;
}
uint32_t FramebufferResolution::get_linearframebuffer()
{
    return this->lfb;
}
bool FramebufferResolution::is_vga_compatible()
{
    return this->resolution_vga_compatible;
}
uint8_t FramebufferResolution::get_memory_model()
{
    return this->resolution_memory_model;
}
uint32_t FramebufferResolution::get_pixel_format()
{
    return this->resolution_pixel_format;
}