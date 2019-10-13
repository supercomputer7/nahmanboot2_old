#pragma once
#include <stdint.h>
#include <Display/StandardVGA.h>
#include <LibC/stdstring.h>

class TerminalDriver {

public:
    ~TerminalDriver();
    bool initialize(StandardVGA* vga,uint32_t foreground_color,uint32_t background_color,uint32_t col_limit,uint32_t row_limit);
    void write(const char* str);
    void clear();
    void exit();
private:

    void new_line();
    void first_line();
    void next_char();

    StandardVGA* vga;
    uint32_t foreground_color;
    uint32_t background_color;

    uint32_t column_limit;
    uint32_t row_limit;

    uint32_t column;
    uint32_t row;
};