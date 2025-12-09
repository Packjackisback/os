#include "kheap.h"
#include "paging.h"
#include "pmm.h"
#include "string.h"

#define HEAP_MAGIC 0x12345678

static uint32_t heap_start = KHEAP_START;
static uint32_t heap_end = KHEAP_START;
static uint32_t heap_max = KHEAP_START + KHEAP_MAX_SIZE;
static heap_block_t *free_list = 0;

static int expand_heap(size_t size) {
  size_t pages_needed = (size + PAGE_SIZE - 1) / PAGE_SIZE;

  if (heap_end + (pages_needed * PAGE_SIZE) > heap_max) {
    return 0;
  }

  for (size_t i = 0; i < pages_needed; i++) {
    uint32_t phys = pmm_alloc_page();
    if (!phys) {
      return 0;
    }

    paging_map_page(heap_end, phys, PAGE_PRESENT | PAGE_WRITE);
    heap_end += PAGE_SIZE;
  }

  return 1;
}

static void split_block(heap_block_t *block, size_t size) {
  size_t min_split_size = size + sizeof(heap_block_t) + HEAP_MIN_SIZE;

  if (block->size >= min_split_size) {
    heap_block_t *new_block = (heap_block_t *)((uint8_t *)block + size);
    new_block->size = block->size - size;
    new_block->magic = HEAP_MAGIC;
    new_block->is_free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
  }
}

static void merge_blocks(void) {
  heap_block_t *curr = free_list;

  while (curr && curr->next) {
    if (curr->is_free && curr->next->is_free) {
      uint8_t *curr_end = (uint8_t *)curr + curr->size;
      if (curr_end == (uint8_t *)curr->next) {
        curr->size += curr->next->size;
        curr->next = curr->next->next;
        continue;
      }
    }
    curr = curr->next;
  }
}

void kheap_init(void) {
  if (!expand_heap(KHEAP_INITIAL_SIZE)) {
    return;
  }

  free_list = (heap_block_t *)heap_start;
  free_list->size = KHEAP_INITIAL_SIZE;
  free_list->magic = HEAP_MAGIC;
  free_list->is_free = 1;
  free_list->next = 0;
}

void *kmalloc(size_t size) { return kmalloc_ap(size, 0, 0); }

void *kmalloc_a(size_t size, uint32_t align) {
  return kmalloc_ap(size, align, 0);
}

void *kmalloc_p(size_t size, uint32_t *phys) {
  return kmalloc_ap(size, 0, phys);
}

void *kmalloc_ap(size_t size, uint32_t align, uint32_t *phys) {
  if (size == 0) {
    return 0;
  }

  size = (size + 3) & ~3;
  size_t total_size = size + sizeof(heap_block_t);

  if (total_size < sizeof(heap_block_t) + HEAP_MIN_SIZE) {
    total_size = sizeof(heap_block_t) + HEAP_MIN_SIZE;
  }

  heap_block_t *curr = free_list;
  heap_block_t *prev = 0;

  while (curr) {
    if (curr->is_free && curr->magic == HEAP_MAGIC) {
      uint32_t block_addr = (uint32_t)curr + sizeof(heap_block_t);
      uint32_t aligned_addr = block_addr;

      if (align > 0) {
        aligned_addr = (block_addr + align - 1) & ~(align - 1);
      }

      uint32_t alignment_offset = aligned_addr - block_addr;
      size_t required_size = total_size + alignment_offset;

      if (curr->size >= required_size) {
        if (alignment_offset > 0) {
          heap_block_t *padding = curr;
          padding->size = alignment_offset;
          padding->is_free = 1;

          curr = (heap_block_t *)(block_addr + alignment_offset -
                                  sizeof(heap_block_t));
          curr->size = required_size - alignment_offset;
          curr->magic = HEAP_MAGIC;
          curr->next = padding->next;
          padding->next = curr;
        }

        split_block(curr, total_size);

        curr->is_free = 0;

        if (phys) {
          *phys = paging_get_physical((uint32_t)curr + sizeof(heap_block_t));
        }

        return (void *)((uint8_t *)curr + sizeof(heap_block_t));
      }
    }

    prev = curr;
    curr = curr->next;
  }

  size_t expand_size =
      (total_size > KHEAP_INITIAL_SIZE) ? total_size : KHEAP_INITIAL_SIZE;

  if (!expand_heap(expand_size)) {
    return 0;
  }

  heap_block_t *new_block = (heap_block_t *)(heap_end - expand_size);
  new_block->size = expand_size;
  new_block->magic = HEAP_MAGIC;
  new_block->is_free = 1;
  new_block->next = 0;

  if (prev) {
    prev->next = new_block;
  } else {
    free_list = new_block;
  }

  return kmalloc_ap(size, align, phys);
}

void kfree(void *ptr) {
  if (!ptr) {
    return;
  }

  heap_block_t *block = (heap_block_t *)((uint8_t *)ptr - sizeof(heap_block_t));

  if (block->magic != HEAP_MAGIC) {
    return;
  }

  block->is_free = 1;

  merge_blocks();
}

void kheap_stats(uint32_t *total, uint32_t *used, uint32_t *free_mem) {
  uint32_t total_size = heap_end - heap_start;
  uint32_t free_size = 0;

  heap_block_t *curr = free_list;
  while (curr) {
    if (curr->is_free) {
      free_size += curr->size;
    }
    curr = curr->next;
  }

  if (total)
    *total = total_size;
  if (used)
    *used = total_size - free_size;
  if (free_mem)
    *free_mem = free_size;
}
