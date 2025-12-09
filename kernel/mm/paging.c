#include "paging.h"
#include "drivers/video/vga.h"
#include "isr.h"
#include "pmm.h"
#include "string.h"

static page_directory_t *kernel_directory = 0;
static page_directory_t *current_directory = 0;

static page_table_entry_t *paging_get_page(uint32_t virt, int create,
                                           page_directory_t *dir) {
  uint32_t pd_index = virt >> 22;

  if (!(dir->entries[pd_index] & PAGE_PRESENT)) {
    if (!create) {
      return 0;
    }

    uint32_t pt_phys = pmm_alloc_page();
    if (!pt_phys) {
      return 0;
    }

    memset((void *)pt_phys, 0, PAGE_SIZE);

    dir->entries[pd_index] = pt_phys | PAGE_PRESENT | PAGE_WRITE | PAGE_USER;
  }

  page_table_t *table = (page_table_t *)(dir->entries[pd_index] & ~0xFFF);

  uint32_t pt_index = (virt >> 12) & 0x3FF;

  return &table->entries[pt_index];
}

static void paging_enable(void) {
  uint32_t cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;
  asm volatile("mov %0, %%cr0" : : "r"(cr0));
}

static void paging_load_directory(page_directory_t *dir) {
  asm volatile("mov %0, %%cr3" : : "r"(dir));
}

static void paging_invalidate_page(uint32_t virt) {
  asm volatile("invlpg (%0)" : : "r"(virt) : "memory");
}

void paging_init(void) {
  uint32_t pd_phys = pmm_alloc_page();
  kernel_directory = (page_directory_t *)pd_phys;
  current_directory = kernel_directory;

  memset(kernel_directory, 0, PAGE_SIZE);

  for (uint32_t i = 0; i < 0x400000; i += PAGE_SIZE) {
    paging_map_page(i, i, PAGE_PRESENT | PAGE_WRITE);
  }

  isr_register_handler(14, (isr_handler_t)page_fault_handler);

  paging_load_directory(kernel_directory);
  paging_enable();
}

page_directory_t *paging_get_directory(void) { return current_directory; }

void paging_switch_directory(page_directory_t *dir) {
  current_directory = dir;
  paging_load_directory(dir);
}

void paging_map_page(uint32_t virt, uint32_t phys, uint32_t flags) {
  virt &= 0xFFFFF000;
  phys &= 0xFFFFF000;

  page_table_entry_t *pte = paging_get_page(virt, 1, current_directory);

  if (pte) {
    *pte = phys | (flags & 0xFFF) | PAGE_PRESENT;

    paging_invalidate_page(virt);
  }
}

void paging_unmap_page(uint32_t virt) {
  virt &= 0xFFFFF000;

  page_table_entry_t *pte = paging_get_page(virt, 0, current_directory);

  if (pte && (*pte & PAGE_PRESENT)) {
    *pte = 0;

    paging_invalidate_page(virt);
  }
}

uint32_t paging_get_physical(uint32_t virt) {
  page_table_entry_t *pte = paging_get_page(virt, 0, current_directory);

  if (!pte || !(*pte & PAGE_PRESENT)) {
    return 0;
  }

  return (*pte & 0xFFFFF000) | (virt & 0xFFF);
}

void page_fault_handler(struct registers *regs) {
  uint32_t faulting_addr;
  asm volatile("mov %%cr2, %0" : "=r"(faulting_addr));

  int present = regs->err_code & 0x1;
  int write = regs->err_code & 0x2;
  int user = regs->err_code & 0x4;
  int reserved = regs->err_code & 0x8;
  int fetch = regs->err_code & 0x10;

  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
  vga_print("\n\n!!! PAGE FAULT !!!\n");
  vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);

  char buffer[32];
  vga_print("Faulting address: 0x");
  utoa(faulting_addr, buffer, 16);
  vga_println(buffer);

  vga_print("Error code: ");
  if (!present)
    vga_print("[NOT PRESENT] ");
  if (write)
    vga_print("[WRITE] ");
  if (user)
    vga_print("[USER MODE] ");
  if (reserved)
    vga_print("[RESERVED] ");
  if (fetch)
    vga_print("[FETCH] ");
  vga_println("");

  asm volatile("cli");
  for (;;) {
    asm volatile("hlt");
  }
}
