#include "idt.h"

struct idt_entry idt[256];
struct idt_ptr idtp;

extern void idt_load(uint32_t);

void idt_set_gate(uint8_t num, uint32_t handler, uint16_t selector,
                  uint8_t flags) {
  idt[num].base_low = handler & 0xFFFF;
  idt[num].base_high = (handler >> 16) & 0xFFFF;
  idt[num].selector = selector;
  idt[num].always0 = 0;
  idt[num].flags = flags;
}

void idt_init(void) {
  idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
  idtp.base = (uint32_t)&idt;

  for (int i = 0; i < 256; i++) {
    idt_set_gate(i, 0, 0, 0);
  }

  idt_load((uint32_t)&idtp);
}
