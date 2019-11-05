#pragma once

#include <stdint.h>
#include <Display/Protocols/GenericScreen.h>
#include <Display/Protocols/FramebufferResolution.h>
#include <CPU/RealModeExtender.h>
#include <Memory/List.h>

namespace VESA
{
    typedef struct {
        char VbeSignature[4];       // == "VESA"
        uint16_t VbeVersion;        // == 0x0300 for VBE 3.0
        uint16_t OemStringPtr[2];   // isa vbeFarPtr
        uint8_t Capabilities[4];
        uint16_t VideoModePtr[2];   // isa vbeFarPtr
        uint16_t TotalMemory;       // as # of 64KB blocks
        uint16_t software_rev;		// software revision
        uint32_t vendor;			// segment:offset to card vendor string
        uint32_t product_name;		// segment:offset to card model name
        uint32_t product_rev;		// segment:offset pointer to product revision
        char reserved[222];         // reserved for future expansion
        char oem_data[256];         // OEM BIOSes store their strings in this area
    } __attribute__((packed)) InfoBlock;

    typedef struct {
        uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    	uint8_t window_a;			// deprecated
    	uint8_t window_b;			// deprecated
    	uint16_t granularity;		// deprecated; used while calculating bank numbers
    	uint16_t window_size;
    	uint16_t segment_a;
    	uint16_t segment_b;
    	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
    	uint16_t pitch;			// number of bytes per horizontal line
    	uint16_t width;			// width in pixels
    	uint16_t height;			// height in pixels
    	uint8_t w_char;			// unused...
    	uint8_t y_char;			// ...
    	uint8_t planes;
    	uint8_t bpp;			// bits per pixel in this mode
    	uint8_t banks;			// deprecated; total number of banks in this mode
    	uint8_t memory_model;
    	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    	uint8_t image_pages;
    	uint8_t reserved0;
    
    	uint8_t red_mask;
    	uint8_t red_position;
    	uint8_t green_mask;
    	uint8_t green_position;
    	uint8_t blue_mask;
    	uint8_t blue_position;
    	uint8_t reserved_mask;
    	uint8_t reserved_position;
    	uint8_t direct_color_attributes;
    
    	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
    	uint32_t off_screen_mem_off;
    	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
    	uint8_t reserved1[206];
    } __attribute__((packed)) ModeInfoBlock;
}



class StandardVESA : public GenericScreen {

public:
    ~StandardVESA();
    StandardVESA();
    void initialize() override;
    uint16_t get_current_videomode() override;
    uint32_t get_resolution_number(uint32_t width,uint32_t height,uint32_t bpp) override;
    bool set_resolution(uint32_t width,uint32_t height,uint32_t bpp) override;
    void write(char ch, uint32_t color,uint32_t offset) override;
    void clear() override;
    void draw_rectangle(uint32_t topleft_offset,uint32_t bottomright_offset,uint32_t color) override;
private:

    void get_infoblock();
    void enumerate_resolutions(uint16_t* buf);
    void set_vesa_resolution(uint32_t modenumber);

    void clear_4bpp();
    void clear_8bpp();
    void clear_15bpp();
    void clear_16bpp();
    void clear_24RGBbpp();
    void clear_24BGRbpp();
    void clear_32bpp();

    RealModeExtender* vbe_caller;
    VESA::InfoBlock* vbe_infoblock;

    List<FramebufferResolution>* resolutions_available;
    FramebufferResolution* current_resolution;
};