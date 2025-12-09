#include "pmm.h"
#include "string.h"

#define BITMAP_ADDR 0x100000
static uint32_t *bitmap = (uint32_t *)BITMAP_ADDR;

static uint32_t total_pages = 0;
static uint32_t used_pages = 0;

extern uint32_t kernel_end;

static inline void bitmap_set(uint32_t page) {
  uint32_t index = page / 32;
  uint32_t bit = page % 32;
  bitmap[index] |= (1 << bit);
}

static inline void bitmap_clear(uint32_t page) {
  uint32_t index = page / 32;
  uint32_t bit = page % 32;
  bitmap[index] &= ~(1 << bit);
}

static inline int bitmap_test(uint32_t page) {
  uint32_t index = page / 32;
  uint32_t bit = page % 32;
  return bitmap[index] & (1 << bit);
}

void pmm_init(uint32_t mem_size) {
  total_pages = mem_size / PAGE_SIZE;

  uint32_t bitmap_size = total_pages / 8;
  if (total_pages % 8)
    bitmap_size++;

  memset(bitmap, 0, bitmap_size);

  uint32_t reserved_pages = 0x100000 / PAGE_SIZE;
  for (uint32_t i = 0; i < reserved_pages; i++) {
    bitmap_set(i);
    used_pages++;
  }

  uint32_t bitmap_pages = bitmap_size / PAGE_SIZE;
  if (bitmap_size % PAGE_SIZE)
    bitmap_pages++;

  for (uint32_t i = 0; i < bitmap_pages; i++) {
    bitmap_set(reserved_pages + i);
    used_pages++;
  }

  for (uint32_t i = total_pages; i < total_pages + 32; i++) {
    if (i / 32 < bitmap_size / 4) {
      bitmap_set(i);
    }
  }
}

uint32_t pmm_alloc_page(void) {
  for (uint32_t i = 0; i < total_pages; i++) {
    if (!bitmap_test(i)) {
      bitmap_set(i);
      used_pages++;
      return i * PAGE_SIZE;
    }
  }

  return 0;
}

void pmm_free_page(uint32_t page_addr) {
  uint32_t page = page_addr / PAGE_SIZE;

  if (page >= total_pages) {
    return;
  }

  if (bitmap_test(page)) {
    bitmap_clear(page);
    used_pages--;
  }
}

memory_info_t pmm_get_info(void) {
  memory_info_t info;
  info.total_memory = total_pages * PAGE_SIZE;
  info.total_pages = total_pages;
  info.used_pages = used_pages;
  info.free_pages = total_pages - used_pages;
  return info;
}
