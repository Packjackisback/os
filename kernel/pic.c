#include "pic.h"

static inline void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static inline void io_wait(void) { outb(0x80, 0); }

void pic_init(void) {
  uint8_t mask1, mask2;

  mask1 = inb(PIC1_DATA);
  mask2 = inb(PIC2_DATA);

  outb(PIC1_COMMAND, 0x11);
  io_wait();
  outb(PIC2_COMMAND, 0x11);
  io_wait();

  outb(PIC1_DATA, PIC1_OFFSET);
  io_wait();
  outb(PIC2_DATA, PIC2_OFFSET);
  io_wait();

  outb(PIC1_DATA, 0x04);
  io_wait();
  outb(PIC2_DATA, 0x02);
  io_wait();

  outb(PIC1_DATA, 0x01);
  io_wait();
  outb(PIC2_DATA, 0x01);
  io_wait();

  outb(PIC1_DATA, mask1);
  outb(PIC2_DATA, mask2);
}

void pic_send_eoi(uint8_t irq) {
  if (irq >= 8) {
    outb(PIC2_COMMAND, PIC_EOI);
  }
  outb(PIC1_COMMAND, PIC_EOI);
}

void pic_set_mask(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if (irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irq -= 8;
  }

  value = inb(port) | (1 << irq);
  outb(port, value);
}

void pic_clear_mask(uint8_t irq) {
  uint16_t port;
  uint8_t value;

  if (irq < 8) {
    port = PIC1_DATA;
  } else {
    port = PIC2_DATA;
    irq -= 8;
  }

  value = inb(port) & ~(1 << irq);
  outb(port, value);
}
