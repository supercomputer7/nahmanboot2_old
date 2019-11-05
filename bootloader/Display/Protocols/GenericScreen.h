#pragma once
#include <stdint.h>
#include <LibC/stdstring.h>
#include <LibC/stdbool.h>

#define BitsPerByte 8
#define WriteTuple char ch, uint32_t color,uint32_t offset
#define WriteTupleArgs ch, color, offset

class GenericScreen {

public:
    virtual void initialize()
    {

    }
    virtual uint16_t get_current_videomode()
    {
        return 0xffff;
    }
    virtual uint32_t get_resolution_number(__attribute__((unused)) uint32_t width,__attribute__((unused)) uint32_t height,__attribute__((unused)) uint32_t bpp)
    {
        return 0xffffffff;
    }
    virtual bool set_resolution(__attribute__((unused)) uint32_t width,__attribute__((unused)) uint32_t height,__attribute__((unused)) uint32_t bpp)
    {
        return false;
    }
    virtual void set_color(uint32_t foreground_color,uint32_t background_color)
    {
        this->foreground_color = foreground_color;
        this->background_color = background_color;
    }
    virtual void write(__attribute__((unused)) char ch,__attribute__((unused)) uint32_t color,__attribute__((unused)) uint32_t offset)
    {

    }
    virtual void clear()
    {
        
    }
    virtual void draw_rectangle(__attribute__((unused)) uint32_t topleft_offset,__attribute__((unused)) uint32_t bottomright_offset,__attribute__((unused)) uint32_t color)
    {
        
    }
    virtual uint32_t get_foreground_color()
    {
        return this->foreground_color;
    }
    virtual uint32_t get_background_color()
    {
        return this->background_color;
    }
    virtual uint32_t get_resolution_width()
    {
        return this->resolution_width;
    }
    virtual uint32_t get_resolution_pitch()
    {
        return this->resolution_pitch;
    }
    virtual uint32_t get_resolution_height()
    {
        return this->resolution_height;
    }
    virtual uint16_t get_resolution_bpp()
    {
        return this->resolution_bpp;
    }
    virtual void* get_resolution_buffer()
    {
        return this->buffer;
    }
    virtual int get_resolution_units_count()
    {
        return (this->resolution_pitch*this->resolution_height) / (this->get_resolution_bpp() / BitsPerByte);
    }

protected:
    void* buffer;
    uint32_t resolution_pitch;
    uint32_t resolution_bpp;
    uint32_t resolution_width;
    uint32_t resolution_height;
    uint32_t foreground_color;
    uint32_t background_color;
};