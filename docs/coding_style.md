# Style Guide

## Principles
- Don't be clever: Write code every c programmer with a brain and google can understand.
- Documentation: Keep it brief, code really should be clear enough without documentation if well written. - Modular: Each function should only do 1 thing, and preferrably call functions on the same level of abstraction (ie: don't run asm and a vga driver function in the same function)
- No magic numbers: Define any random numbers in the header.

## Naming Conventions:

- Functions should be lowercase and snake case:

```c
void kernel_init(void);
void paging_map_page(uint32_t virt, uint32_t phys, uint32_t flags);
```

- Variables should also be lowercase snake case.

```c
uint32_t page_count;
```

- Constants/Macros should be UPPERCASE with underscores

```c
#define PAGE_SIZE 4096
```

- Types should be lowercase with the _t suffix

```c
typedef struct page_directory page_directory_t;
```

- Structs should be snake case like functions

```c
struct heap_block {
    uint32_t size;
    uint8_t is_free;
};
```

- Enums should be lowercase with members UPPERCASE

```c
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_WHITE = 15,
};
```

## Indentation

Use tabs, same-line braces for control structures, braces on new lines for functions.

Lines shouldn't be too long, view 80-100 as a high bound.
