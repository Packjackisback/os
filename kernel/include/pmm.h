#ifndef PMM_H
#define PMM_H

#include "stdint.h"

#define PAGE_SIZE 4096

typedef struct {
    uint32_t total_memory;      // Total memory in bytes
    uint32_t total_pages;       // Total number of pages
    uint32_t used_pages;        // Number of used pages
    uint32_t free_pages;        // Number of free pages
} memory_info_t;

void pmm_init(uint32_t mem_size);

uint32_t pmm_alloc_page(void);

void pmm_free_page(uint32_t page_addr);

memory_info_t pmm_get_info(void);

#endif
