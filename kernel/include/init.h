#ifndef INIT_H
#define INIT_H

#define INIT_COMPONENT(name, func) do { \
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK); \
    vga_print("Initializing " name " "); \
    func; \
    vga_set_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK); \
    vga_println("[OK]"); \
} while(0)

void kernel_init(void);

void display_system_info(void);

#endif
