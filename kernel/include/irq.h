#ifndef IRQ_H
#define IRQ_H

#include "stdint.h"
#include "isr.h"

typedef void (*irq_handler_t)(struct registers*);

void irq_init(void);

void irq_register_handler(uint8_t irq, irq_handler_t handler);

extern void irq0(void);   // Timer
extern void irq1(void);   // Keyboard
extern void irq2(void);   // Cascade
extern void irq3(void);   // COM2
extern void irq4(void);   // COM1
extern void irq5(void);   // LPT2
extern void irq6(void);   // Floppy
extern void irq7(void);   // LPT1
extern void irq8(void);   // RTC
extern void irq9(void);   // Free
extern void irq10(void);  // Free
extern void irq11(void);  // Free
extern void irq12(void);  // PS/2 Mouse
extern void irq13(void);  // Coprocessor
extern void irq14(void);  // Primary ATA
extern void irq15(void);  // Secondary ATA

#endif
