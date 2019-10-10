#include <stdint.h>
#include "Memory/Allocator.h"
#include "Display/StandardVGA.h"
#include "Display/TerminalDriver.h"
#include "LibC/stdstring.h"
extern "C" void kmain()
{

    BumpAllocator allocator;
    allocator.initialize(0x300000);
    StandardVGA* screen = (StandardVGA*)allocator.allocate(sizeof(StandardVGA));
    screen->initialize(0xb8000,80);

    TerminalDriver* terminal = (TerminalDriver*)allocator.allocate(sizeof(TerminalDriver));
    terminal->initialize((StandardVGA*)screen,1,0xf);
    terminal->clear();
    const char* hello = "Nahmanboot, Loading";
    terminal->write(hello);
    terminal->write(hello);
    

}