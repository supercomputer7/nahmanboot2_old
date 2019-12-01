#include <CPU/PIC.h>
#include <IO/IO.h>
#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)
#define PIC_EOI		0x20		/* End-of-interrupt command code */
#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */

#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

static ProgrammableInterruptController* pic;

ProgrammableInterruptController& ProgrammableInterruptController::the()
{
    return *pic;
}

void ProgrammableInterruptController::initialize()
{
	pic = new ProgrammableInterruptController();
}

ProgrammableInterruptController::ProgrammableInterruptController()
{
	pic->reset_protected_mode();
}

void ProgrammableInterruptController::reset_protected_mode()
{
	/* 
		TODO: Add IO wait procedures somehow...
	*/
    unsigned char a1, a2;
    char offset1 = 0x20;
    char offset2 = 0x28;
	a1 = IO::inb(PIC1_DATA);                        // save masks
	a2 = IO::inb(PIC2_DATA);
 
	IO::outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	IO::outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	IO::outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	IO::outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	IO::outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	IO::outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	IO::outb(PIC1_DATA, ICW4_8086);
	IO::outb(PIC2_DATA, ICW4_8086);
	IO::outb(PIC1_DATA, a1);   // restore saved masks.
	IO::outb(PIC2_DATA, a2);
    asm volatile("nop; nop; nop");

    pic->mask_all();
	pic->enable_interrupts();
}

void ProgrammableInterruptController::EOI(uint8_t irq)
{
	if(irq >= 8)
		IO::outb(PIC2_COMMAND,PIC_EOI);
	IO::outb(PIC1_COMMAND,PIC_EOI);
}
void ProgrammableInterruptController::disable_irq_line(uint8_t irq)
{
	if(irq < 8)
		IO::outb(PIC1_DATA,(IO::inb(PIC1_DATA) | (1 << irq)));
	else
		IO::outb(PIC1_DATA,(IO::inb(PIC2_DATA) | (1 << (irq - 8))));
}
void ProgrammableInterruptController::enable_irq_line(uint8_t irq)
{
	if(irq < 8)
		IO::outb(PIC1_DATA,(IO::inb(PIC1_DATA) & ~(1 << irq)));
	else
		IO::outb(PIC1_DATA,(IO::inb(PIC2_DATA) & ~(1 << (irq - 8))));
}
void ProgrammableInterruptController::mask_all()
{
	IO::outb(PIC1_DATA,0xff);                     
	IO::outb(PIC2_DATA,0xff);
}
void ProgrammableInterruptController::unmask_all()
{
	IO::outb(PIC1_DATA,0x0);                     
	IO::outb(PIC2_DATA,0x0);
}
void ProgrammableInterruptController::enable_interrupts()
{
	asm volatile ("sti");
}
void ProgrammableInterruptController::disable_interrupts()
{
	asm volatile ("cli");
}
