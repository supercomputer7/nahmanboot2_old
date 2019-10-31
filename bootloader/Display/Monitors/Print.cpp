#include <Display/Monitors/Print.h>


void stdout::init()
{
    StandardVGA* screen = new StandardVGA();
    screen->initialize(0xb8000,80,0,0xf);
    void *address = reinterpret_cast<void *>(STD_TERMINAL_DRIVER_ADDRESS);
    TerminalDriver* terminal = new (address) TerminalDriver();
    terminal->initialize((StandardVGA*)screen,0,0xf,80,25);
    terminal->clear();
}
void stdout::print(const char* str)
{
    reinterpret_cast<TerminalDriver*>(STD_TERMINAL_DRIVER_ADDRESS)->write(str);
}
void stdout::exit()
{
    reinterpret_cast<TerminalDriver*>(STD_TERMINAL_DRIVER_ADDRESS)->exit();
}
void stdout::clear()
{
    reinterpret_cast<TerminalDriver*>(STD_TERMINAL_DRIVER_ADDRESS)->clear();
}
void stdout::printf(const char* str)
{

}