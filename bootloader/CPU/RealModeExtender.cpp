#include <CPU/RealModeExtender.h>
void RealModeExtender::invoke_bios_interrupt(uint16_t interrupt_vector,uint32_t eax,uint32_t ebx,uint32_t ecx,uint32_t edx,uint32_t esi,uint32_t edi,uint16_t es)
{
    RealModeSwitch* realmode_switch = (RealModeSwitch*)(0xc00);
    realmode_switch->interrupt_vector = 0x10;
    realmode_switch->eax = eax;
    realmode_switch->ebx = ebx;
    realmode_switch->ecx = ecx;
    realmode_switch->edx = edx;
    realmode_switch->esi = esi;
    realmode_switch->edi = edi;
    realmode_switch->es = es;
    asm volatile(
    "cli;"
    "movl $0xF000,%edi;"
    "movl %esp,(%edi);"
    "movl $0x600,%esp;"
    "movl $0xc30,%eax;"
    "call %eax;"
    "movl $0xF000,%edi;"
    "movl (%edi),%esp;"
    );
}