#include <Display/Protocols/StandardVESA.h>
StandardVESA::StandardVESA()
{
    this->initialize();
}
void StandardVESA::initialize()
{
    this->vbe_caller = new RealModeExtender();
    this->resolutions_available = new List<FramebufferResolution>(nullptr,0);
    this->get_infoblock();
}
void StandardVESA::get_infoblock()
{
    vbe_caller->invoke_bios_interrupt(0x10,0x4f00,0x0,0x0,0x0,0x0,0x0,0x7000);
    this->vbe_infoblock = (VESA::InfoBlock*)kmalloc(sizeof(VESA::InfoBlock));
    strcopy((char*)0x70000,(char*)this->vbe_infoblock,sizeof(VESA::InfoBlock));
    uint16_t* modes_buf = (uint16_t*)((this->vbe_infoblock->VideoModePtr[1] * 0x10) + (this->vbe_infoblock->VideoModePtr[0]));
    this->enumerate_resolutions((uint16_t*)modes_buf);
}
void StandardVESA::enumerate_resolutions(uint16_t* buf)
{
    VESA::ModeInfoBlock* resolution = (VESA::ModeInfoBlock*)0x80000;
    if(buf[0] != 0xffff)
    {
        int i=0;
        while(1)
        {

            vbe_caller->invoke_bios_interrupt(0x10,0x4f01,0x0,(uint32_t)(buf[i] & 0xffff),0x0,0x0,0x0,0x8000);
            FramebufferResolution* resolution_new = new FramebufferResolution(
                                buf[i],
                                resolution->pitch,
                                resolution->width,
                                resolution->height,
                                resolution->bpp,
                                resolution->framebuffer,
                                ((resolution->attributes & (1 << 5)) != 0),
                                resolution->memory_model,
                                0xffffffff);
            this->resolutions_available->insert_node(resolution_new);
            if(buf[i+1] == 0xffff)
                break;
            i++;
        }
    }
}
uint16_t StandardVESA::get_current_videomode()
{
    if(this->current_resolution != nullptr)
        return this->current_resolution->get_modenumber();
    return 0xffff;
}
uint32_t StandardVESA::get_resolution_number(uint32_t width,uint32_t height,uint32_t bpp)
{
    for(uint32_t i=0;i<this->resolutions_available->get_count();i++)
        if(this->resolutions_available->get_node(i)->get_object()->get_width() == width && 
            this->resolutions_available->get_node(i)->get_object()->get_height() == height &&
            this->resolutions_available->get_node(i)->get_object()->get_bpp() == bpp)
            return i;
    return 0xffffffff;
}
bool StandardVESA::set_resolution(uint32_t width,uint32_t height,uint32_t bpp)
{
    uint32_t resolution_number = this->get_resolution_number(width, height, bpp);
    if(resolution_number != 0xffffffff)
    {
        this->set_vesa_resolution(this->resolutions_available->get_node(resolution_number)->get_object()->get_modenumber());
        this->current_resolution = this->resolutions_available->get_node(resolution_number)->get_object();
        this->resolution_width = this->current_resolution->get_width();
        this->resolution_pitch = this->current_resolution->get_pitch();
        this->resolution_bpp = this->current_resolution->get_bpp();
        this->resolution_height = this->current_resolution->get_height();
        this->buffer = (void*)this->current_resolution->get_linearframebuffer();
        return true;
    }
    return false;
}
void StandardVESA::set_vesa_resolution(uint32_t modenumber)
{
    vbe_caller->invoke_bios_interrupt(0x10,
                                0x4f02,
                                modenumber | 0x4000,
                                0x0,0x0,0x0,0x0,0x0);
}
void StandardVESA::write(char ch, uint32_t color,uint32_t offset)
{

}
void StandardVESA::clear()
{
    switch(this->resolution_bpp)
    {

    }
}
void StandardVESA::clear_4bpp()
{

}
void StandardVESA::clear_8bpp()
{
    char* buf = (char*)this->buffer;
    for(int i=0; i < this->resolution_height; i++)
    {
        for(int j=0; i < this->resolution_width; j++)
        {
            buf[j] = (char)this->background_color;
        }
        buf = (char*)((uint32_t)buf + this->resolution_pitch);
    }
}
void StandardVESA::clear_15bpp()
{

}
void StandardVESA::clear_16bpp()
{
    uint16_t* buf = (uint16_t*)this->buffer;
    for(int i=0; i < this->resolution_height; i++)
    {
        for(int j=0; i < this->resolution_width; j++)
        {
            buf[j] = (uint16_t)this->background_color;
        }
        buf = (uint16_t*)((uint32_t)buf + this->resolution_pitch);
    }
}
void StandardVESA::clear_24RGBbpp()
{
    char* buf = (char*)this->buffer;
    for(int i=0; i < (int)this->resolution_height; i++)
    {
        char* buf2 = (char*)buf;
        for(int j=0; i < (int)this->resolution_width; j++)
        {
            buf2[j] = this->background_color ;
            buf2 = (char*)((uint32_t)buf2 + 3);
        }
        buf = (char*)((uint32_t)buf + this->resolution_pitch);
    }
}
void StandardVESA::clear_32bpp()
{

}
void StandardVESA::draw_rectangle(uint32_t topleft_offset,uint32_t bottomright_offset,uint32_t color)
{

}