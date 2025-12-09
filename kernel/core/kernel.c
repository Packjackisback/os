#include "kernel.h"
#include "drivers/video/vga.h"
#include "init.h"

void kernel_main(void) {
  vga_init();

  kernel_init();

  display_system_info();

  while (1) {
    asm volatile("hlt");
  }
}
