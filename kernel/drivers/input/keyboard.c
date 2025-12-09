#include "drivers/input/keyboard.h"
#include "drivers/video/vga.h"
#include "irq.h"
#include "pic.h"

static const char scancode_to_ascii[] = {
    0,    27,  '1',  '2',  '3',  '4', '5', '6', '7', '8', '9',  '0',
    '-',  '=', '\b', '\t', 'q',  'w', 'e', 'r', 't', 'y', 'u',  'i',
    'o',  'p', '[',  ']',  '\n',
    0, // Left Ctrl
    'a',  's', 'd',  'f',  'g',  'h', 'j', 'k', 'l', ';', '\'', '`',
    0, // Left Shift
    '\\', 'z', 'x',  'c',  'v',  'b', 'n', 'm', ',', '.', '/',
    0, // Right Shift
    '*',
    0,                                                  // Left Alt
    ' ',                                                // Space
    0,                                                  // Caps Lock
    0,    0,   0,    0,    0,    0,   0,   0,   0,   0, // F1-F10
    0,                                                  // Num Lock
    0,                                                  // Scroll Lock
    0,                                                  // Home
    0,                                                  // Up Arrow
    0,                                                  // Page Up
    '-',
    0, // Left Arrow
    0,
    0, // Right Arrow
    '+',
    0,                        // End
    0,                        // Down Arrow
    0,                        // Page Down
    0,                        // Insert
    0,                        // Delete
    0,    0,   0,    0,    0, // F11, F12
};

static const char scancode_to_ascii_shift[] = {
    0,   27,  '!',  '@',  '#',  '$', '%', '^', '&', '*', '(', ')',
    '_', '+', '\b', '\t', 'Q',  'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{',  '}',  '\n',
    0, // Left Ctrl
    'A', 'S', 'D',  'F',  'G',  'H', 'J', 'K', 'L', ':', '"', '~',
    0, // Left Shift
    '|', 'Z', 'X',  'C',  'V',  'B', 'N', 'M', '<', '>', '?',
    0, // Right Shift
    '*',
    0,   // Left Alt
    ' ', // Space
};

static uint8_t shift_pressed = 0;
static char last_char = 0;

static inline uint8_t inb(uint16_t port) {
  uint8_t ret;
  asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
  return ret;
}

static void keyboard_handler(struct registers *regs) {
  (void)regs;

  uint8_t scancode = inb(KEYBOARD_DATA_PORT);

  if (scancode == 0x2A || scancode == 0x36) {
    shift_pressed = 1;
    return;
  } else if (scancode == 0xAA || scancode == 0xB6) {
    shift_pressed = 0;
    return;
  }

  if (scancode & 0x80) {
    return;
  }

  char ascii = 0;
  if (scancode < sizeof(scancode_to_ascii)) {
    if (shift_pressed && scancode < sizeof(scancode_to_ascii_shift)) {
      ascii = scancode_to_ascii_shift[scancode];
    } else {
      ascii = scancode_to_ascii[scancode];
    }
  }

  if (ascii != 0) {
    last_char = ascii;

    if (ascii == '\b') {
      uint8_t x, y;
      vga_get_cursor(&x, &y);
      if (x > 0) {
        vga_set_cursor(x - 1, y);
        vga_putchar(' ');
        vga_set_cursor(x - 1, y);
      }
    } else {
      vga_putchar(ascii);
    }
  }
}

void keyboard_init(void) {
  irq_register_handler(1, keyboard_handler);

  pic_clear_mask(1);
}

char keyboard_getchar(void) {
  char c = last_char;
  last_char = 0;
  return c;
}
