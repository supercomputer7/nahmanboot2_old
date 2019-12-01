#pragma once
#include <stdint.h>
class ProgrammableInterruptController {

public:
    static ProgrammableInterruptController& the();
    static void initialize();
    void reset_protected_mode();
    void EOI(uint8_t irq);
    void reset_real_mode();
    void mask_all();
    void unmask_all();
    void enable_interrupts();
    void disable_interrupts();
    void disable_irq_line(uint8_t irq);
    void enable_irq_line(uint8_t irq);
private:
    ~ProgrammableInterruptController();
    ProgrammableInterruptController();
};
inline static void halt_cpu()
{
    asm volatile("hlt");
}