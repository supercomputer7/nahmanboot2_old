#include <Display/Monitors/TerminalDriver.h>

TerminalDriver::~TerminalDriver()
{
}

bool TerminalDriver::initialize(StandardVGA* vga,uint32_t foreground_color,uint32_t background_color,uint32_t col_limit,uint32_t row_limit)
{
    this->vga = vga;
    this->background_color = background_color;
    this->foreground_color = foreground_color;
    this->first_line();
    this->column_limit = col_limit;
    this->row_limit = row_limit;
    this->is_writable = true;
    return true;
}
void TerminalDriver::clear()
{
    this->vga->clear();
}

void TerminalDriver::exit()
{
    this->write("Terminal Stopped\n");
    this->is_writable = false;
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
    if(is_writable)
    {
        int i=0;
        while(i<(int)strlen(str))
        {
            if(str[i] == '\n')
            {
                if(this->row == this->row_limit)
                {
                    this->first_line();
                }
                else
                {
                    this->new_line();   
                }
                i+=2;
                continue;
            }
            if(this->column == this->column_limit)
            {
                this->new_line();
            }

            this->vga->write(str[i],this->vga->get_color(this->foreground_color,this->background_color),this->column + (this->row * this->vga->get_pitch()));
            this->next_char();
            ++i;
        }
    }
}