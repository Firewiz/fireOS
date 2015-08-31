#ifndef STATIC_H
#define STATIC_H

#include "gdt.h"
#define GDT_ENTRIES 6

#define ALLOC_BASE 0x00600000

#define PAGEDIR_BASE 0x00200000
#define PAGEDIR_OFFSET (1024 * 4)
#define PAGEFRAME_BASE (PAGEDIR_BASE + PAGEDIR_OFFSET)
#define PAGEFRAME_OFFSET 0x8000
#define PAGETABLE_BASE (PAGEFRAME_BASE + PAGEFRAME_OFFSET)
#define PAGETABLE_OFFSET (1024 * 1024 * 4)
#define TASKLIST_BASE (PAGETABLE_BASE + PAGETABLE_OFFSET)
#define TASKLIST_OFFSET (65536 * sizeof(void *))
#define GDT_BASE (TASKLIST_BASE + TASKLIST_OFFSET)
#define GDT_OFFSET (GDT_ENTRIES * sizeof(struct gdt_entry_bits))
#define TSS_BASE (GDT_BASE + GDT_OFFSET)
#define TSS_OFFSET (sizeof(struct tss_entry))
#endif