#include "paging.h"

unsigned int *pdir = (unsigned int *) 0x400000;
unsigned int *ptables = (unsigned int *) 0x401000;

unsigned int *frames;
unsigned int nframes = 0x80000;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

static void set_frame(unsigned int page_index) {
   unsigned int frame = page_index;
   unsigned int idx = INDEX_FROM_BIT(frame);
   unsigned int off = OFFSET_FROM_BIT(frame);
   frames[idx] |= (0x1 << off);
}

static void clear_frame(unsigned int page_index) {
   unsigned int frame = page_index;
   unsigned int idx = INDEX_FROM_BIT(frame);
   unsigned int off = OFFSET_FROM_BIT(frame);
   frames[idx] &= ~(0x1 << off);
}

static unsigned int test_frame(unsigned int page_index) {
   unsigned int frame = page_index;
   unsigned int idx = INDEX_FROM_BIT(frame);
   unsigned int off = OFFSET_FROM_BIT(frame);
   return frames[idx] & (0x1 << off);
}

static unsigned int first_frame() {
  unsigned int i, j;
   for(i = 0; i < INDEX_FROM_BIT(nframes); i++) {
     if(frames[i] != 0xFFFFFFFF) {
       for(j = 0; j < 32; j++) {
	 unsigned int testbit = 0x1 << j;
	 if(!(frames[i]&testbit)) {
	   return i*4*8+j;
	 }
       }
     }
   }
   return -1;
}

void setup_paging() {
  int i;
  for(i = 0; i < 0x1000; i++) {
    pdir[i] = ((int) ptables + i) | 0x03;
  }
}

void load_pagetable() {
  asm volatile("mov %0, %%cr3":: "r"(pdir));
  unsigned int cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000;
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

// identity_page: page this virtual memory to corresponding real memory
// nonidentity_page: page this virtual memory somewhere
// mapped_page: page this virtual memory here

void identity_page(unsigned int page_index) {
  ptables[page_index] = page_index * 0x1000 | 0x03;
  set_frame(page_index);
}

void nonidentity_page(unsigned int page_index) {
  unsigned int phy_addr = first_frame();
  mapped_page(page_index, phy_addr);
}

void mapped_page(unsigned int page_index, unsigned int phy_addr) {
  ptables[page_index] = phy_addr * 0x1000 | 0x03;
  set_frame(phy_addr);
}

void unmap_page(unsigned int page_index) {
  ptables[page_index] = 0;
  clear_frame(page_index);
}

int is_present(unsigned int page_index) {
  return ptables[page_index] & PRESENT;
}
