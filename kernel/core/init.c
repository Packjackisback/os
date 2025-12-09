#include "init.h"

#include "drivers/input/keyboard.h"
#include "drivers/video/vga.h"

#include "idt.h"
#include "irq.h"
#include "isr.h"
#include "kheap.h"
#include "paging.h"
#include "pic.h"
#include "pmm.h"

#include "string.h"

void kernel_init(void) {
  vga_clear();

  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_println("<><><><><><><><><><><><><><><><><><><><>");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_println("       Welcome to packOS!");
  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_println("<><><><><><><><><><><><><><><><><><><><>");
  vga_println("");

  INIT_COMPONENT("IDT", idt_init());
  INIT_COMPONENT("ISRs", isr_init());
  INIT_COMPONENT("PIC", pic_init());
  INIT_COMPONENT("IRQs", irq_init());

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_print("Enabling interrupts ");
  asm volatile("sti");
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK]");

  INIT_COMPONENT("keyboard", keyboard_init());
  INIT_COMPONENT("PMM (16MB)", pmm_init(16 * 1024 * 1024));
  INIT_COMPONENT("paging", paging_init());
  INIT_COMPONENT("kernel heap", kheap_init());

  vga_println("");
  vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK);
  vga_println("[OK] Bootloader executed successfully");
  vga_println("[OK] Kernel loaded at 0x10000");
  vga_println("[OK] All subsystems initialized");
}

void display_system_info(void) {
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_println("");

  memory_info_t mem_info = pmm_get_info();
  vga_set_color(VGA_COLOR_CYAN, VGA_COLOR_BLACK);
  vga_print("Memory: ");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

  char buffer[32];
  utoa(mem_info.total_memory / 1024 / 1024, buffer, 10);
  vga_print(buffer);
  vga_print("MB total, ");

  utoa(mem_info.free_pages * 4, buffer, 10);
  vga_print(buffer);
  vga_print("KB free, ");

  utoa(mem_info.used_pages * 4, buffer, 10);
  vga_print(buffer);
  vga_println("KB used");

  vga_println("");
  vga_println("System ready! Type:");
  vga_println("");

  vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
  vga_print("> ");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
}
