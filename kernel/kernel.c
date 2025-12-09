#include "kernel.h"

#include "drivers/input/keyboard.h"
#include "drivers/video/vga.h"

#include "string.h"

#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "pic.h"
#include "pmm.h"

void kernel_main() {
  vga_init();

  vga_clear();

  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_println("<><><><><><><><><><><><><><><><><><><><>");
  vga_set_color(VGA_COLOR_BRIGHT_WHITE, VGA_COLOR_BLACK);
  vga_println("       Welcome to packOS!");
  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_println("<><><><><><><><><><><><><><><><><><><><>");
  vga_println("");

  // init interrupts

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Initializing IDT... ");
  idt_init();
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Initializing ISRs... ");
  isr_init();
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Remapping PIC... ");
  pic_init();
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Initializing IRQs... ");
  irq_init();
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  // enable them now

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Enabling interrupts... ");
  asm volatile("sti");
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Initializing keyboard... ");
  keyboard_init();
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Initializing memory manager... ");
  pmm_init(16 * 1024 * 1024);
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  vga_println("");
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("Bootloader executed successfully");

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_println("");
  vga_println("System ready! Type:");
  vga_println("");

  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_print("> ");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

  while (1) {
    asm volatile("hlt");
  }
}
