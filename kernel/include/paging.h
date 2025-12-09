#ifndef PAGING_H
#define PAGING_H

#include "stdint.h"
#include "isr.h"

#define PAGE_PRESENT    0x001  // present in memory
#define PAGE_WRITE      0x002  // writable
#define PAGE_USER       0x004  // accessible from user mode
#define PAGE_WRITETHROUGH 0x008
#define PAGE_CACHE_DISABLE 0x010
#define PAGE_ACCESSED   0x020  // been accessed
#define PAGE_DIRTY      0x040  // been written to
#define PAGE_SIZE_4MB   0x080  // 4MB
#define PAGE_GLOBAL     0x100  // Global page

typedef uint32_t page_table_entry_t;
typedef uint32_t page_directory_entry_t;

typedef struct {
    page_table_entry_t entries[1024];
} page_table_t;

typedef struct {
    page_directory_entry_t entries[1024];
} page_directory_t;

void paging_init(void);

page_directory_t *paging_get_directory(void);

void paging_switch_directory(page_directory_t *dir);

void paging_map_page(uint32_t virt, uint32_t phys, uint32_t flags);
void paging_unmap_page(uint32_t virt);
uint32_t paging_get_physical(uint32_t virt);

void page_fault_handler(struct registers *regs);

#endif
