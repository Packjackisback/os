#include "drivers/video/vga.h"

#define VGA_MEMORY 0xB8000

static volatile uint16_t *vga_buffer = (uint16_t *)VGA_MEMORY;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;
static uint8_t current_color = 0x0F; // default to white on black

static inline uint16_t vga_entry(char c, uint8_t color) {
  return (uint16_t)c | (uint16_t)color << 8;
}

static inline uint8_t vga_color(uint8_t fg, uint8_t bg) { return fg | bg << 4; }

static inline void outb(uint16_t port, uint8_t value) {
  asm volatile("outb %0, %1" : : "a"(value), "Nd"(port));
}

void vga_init(void) {
  cursor_x = 0;
  cursor_y = 0;
  current_color = vga_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
  vga_clear();
}

void vga_clear(void) {
  for (int y = 0; y < VGA_HEIGHT; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      const int index = y * VGA_WIDTH + x;
      vga_buffer[index] = vga_entry(' ', current_color);
    }
  }
  cursor_x = 0;
  cursor_y = 0;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
  current_color = vga_color(fg, bg);
}

// up by one line
static void vga_scroll(void) {
  for (int y = 0; y < VGA_HEIGHT - 1; y++) {
    for (int x = 0; x < VGA_WIDTH; x++) {
      vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
    }
  }

  for (int x = 0; x < VGA_WIDTH; x++) {
    vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] =
        vga_entry(' ', current_color);
  }

  cursor_y = VGA_HEIGHT - 1;
}

static void vga_update_cursor(void) {
  uint16_t pos = cursor_y * VGA_WIDTH + cursor_x;

  outb(0x3D4, 0x0F);
  outb(0x3D5, (uint8_t)(pos & 0xFF));
  outb(0x3D4, 0x0E);
  outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void vga_putchar(char c) {
  if (c == '\n') {
    cursor_x = 0;
    cursor_y++;
  } else if (c == '\r') {
    cursor_x = 0;
  } else if (c == '\t') {
    cursor_x = (cursor_x + 4) & ~(4 - 1);
  } else if (c == '\b') {
    if (cursor_x > 0) {
      cursor_x--;
    }
  } else {
    const int index = cursor_y * VGA_WIDTH + cursor_x;
    vga_buffer[index] = vga_entry(c, current_color);
    cursor_x++;
  }

  if (cursor_x >= VGA_WIDTH) {
    cursor_x = 0;
    cursor_y++;
  }

  if (cursor_y >= VGA_HEIGHT) {
    vga_scroll();
  }

  vga_update_cursor();
}

void vga_print(const char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    vga_putchar(str[i]);
  }
}

void vga_println(const char *str) {
  vga_print(str);
  vga_putchar('\n');
}

void vga_set_cursor(uint8_t x, uint8_t y) {
  if (x < VGA_WIDTH && y < VGA_HEIGHT) {
    cursor_x = x;
    cursor_y = y;
    vga_update_cursor();
  }
}

void vga_get_cursor(uint8_t *x, uint8_t *y) {
  if (x)
    *x = cursor_x;
  if (y)
    *y = cursor_y;
}
