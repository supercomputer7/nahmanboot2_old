#pragma once

#include <stdint.h>

typedef struct 
{
    uint32_t sig;
    uint32_t returnaddr;
    uint32_t reserved_stack;
    uint16_t interrupt_vector;
    uint32_t eax;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t esi;
    uint32_t edi;
    uint16_t es;
    uint16_t gs;
    uint16_t fs;
    uint32_t jump_addr;
} __attribute__((__packed__)) RealModeSwitch;

class RealModeExtender {

public:
    ~RealModeExtender();

    void invoke_bios_interrupt(uint16_t interrupt_vector,uint32_t eax,uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi,uint16_t es);
private:
    
};