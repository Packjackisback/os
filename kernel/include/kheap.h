#ifndef KHEAP_H
#define KHEAP_H

#include "stdint.h"


//simple heap with a linked list

#define KHEAP_START        0xC0000000
#define KHEAP_INITIAL_SIZE 0x100000
#define KHEAP_MAX_SIZE     0x10000000
#define HEAP_MIN_SIZE      16

typedef struct heap_block {
	uint32_t size;
	uint32_t magic;
	struct heap_block *next;
	uint8_t is_free;
} heap_block_t;

void kheap_init(void);

void *kmalloc(size_t size);

void *kmalloc_a(size_t size, uint32_t align);

void *kmalloc_p(size_t size, uint32_t *phys);

void *kmalloc_ap(size_t size, uint32_t align, uint32_t *phys);

void kfree(void *ptr);

void kheap_stats(uint32_t *total, uint32_t *used, uint32_t *free);

#endif
