#ifndef IDT_H
#define IDT_H

#include "stdint.h"

struct idt_entry {
    uint16_t base_low;   // lower 16 bits 
    uint16_t selector;   // kernel segment selector
    uint8_t  always0;    // always 0
    uint8_t  flags;
    uint16_t base_high;  //upper 16 bits 
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t handler, uint16_t selector, uint8_t flags);

#endif
