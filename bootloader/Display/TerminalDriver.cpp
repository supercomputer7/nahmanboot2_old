#include "TerminalDriver.h"
#include "../LibC/stdstring.h"

TerminalDriver::~TerminalDriver()
{
}

bool TerminalDriver::initialize(StandardVGA* vga,uint32_t foreground_color,uint32_t background_color)
{
    this->vga = vga;
    this->background_color = background_color;
    this->foreground_color = foreground_color;
    return true;
}
void TerminalDriver::clear()
{
    this->vga->clear();
}

void TerminalDriver::new_line()
{
    this->column = 0;
    this->row += 1;
}
void TerminalDriver::first_line()
{
    this->column = 0;
    this->row = 0;
}
void TerminalDriver::next_char()
{
    this->column += 1;
}
void TerminalDriver::write(const char* str)
{
    for (int i = 0; i<(int)strlen(str);++i)
    {
        
        if(strlen(str) >= 2)
        {
            if(str[i] == '/' && str[i+1] == 'n')
            {
                i+=2;
                if(this->row == this->row_limit)
                {
                    this->first_line();
                }
                else
                {
                    this->new_line();
                }
                continue;
            }
        }
        if(this->column == this->column_limit)
        {
            this->new_line();
        }
        this->vga->write(str[i],this->vga->get_color(this->foreground_color,this->background_color),this->column + (this->row * 80));
        this->next_char();
    }
    this->new_line();
}