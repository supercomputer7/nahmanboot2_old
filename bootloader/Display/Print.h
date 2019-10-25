#pragma once
#include <Memory/malloc.h>
#include <Display/TerminalDriver.h>
namespace stdout {

    #define STD_TERMINAL_DRIVER_ADDRESS 0x4000
    void init();
    void print(const char* str);
    void printf(const char* str);
    void exit();
    void clear();
}