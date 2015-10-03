#include "gdt.h"
#include "stdlib.h"
#include "static.h"

static struct gdt_entry_bits *gdt = (struct gdt_entry_bits *) GDT_BASE;
static struct tss_entry *tss = (struct tss_entry *) TSS_BASE;

void install_tss(struct gdt_entry_bits *g) {
  unsigned int base = (unsigned int) tss;
  unsigned int limit = base + sizeof(struct tss_entry);

  g->base_low = base & 0xFFFFFF;
  g->accessed = 1;
  g->read_write = 0;
  g->conforming = 0;
  g->code = 1;
  g->always_1 = 0;
  g->DPL = 3;
  g->present = 1;
  g->limit_high = (limit & 0xF0000) >> 16;
  g->available = 0;
  g->always_0 = 0;
  g->big = 0;
  g->gran = 0;
  g->base_high = (base & 0xFF000000) >> 24;
  bzero(tss, sizeof(struct tss_entry));

  tss->ss0 = 0x10;
  tss->esp0 = 0;
}

void set_kernel_stack(unsigned int esp) {
  tss->esp0 = esp;
}
  

void setup_gdt() {
  bzero(gdt, GDT_OFFSET);
  struct gdt_entry_bits *kcode, *kdata, *ucode, *udata;
  kcode = (struct gdt_entry_bits *) gdt + 1;
  kdata = (struct gdt_entry_bits *) gdt + 2;
  ucode = (struct gdt_entry_bits *) gdt + 3;
  udata = (struct gdt_entry_bits *) gdt + 4;
  kcode->limit_low=0xFFFF;
  kcode->base_low=0;
  kcode->accessed=0;
  kcode->read_write=1;
  kcode->conforming=0;
  kcode->code=1;
  kcode->always_1=1;
  kcode->DPL=0;
  kcode->present=1;
  kcode->limit_high=0xF;
  kcode->available=1;
  kcode->always_0=0;
  kcode->big=1;
  kcode->gran=1;
  kcode->base_high=0;
  memcpy(kdata, kcode, sizeof(struct gdt_entry_bits));
  memcpy(ucode, kcode, sizeof(struct gdt_entry_bits));
  memcpy(udata, kcode, sizeof(struct gdt_entry_bits));
  kdata->code = udata->code = 0;
  udata->DPL = ucode->DPL = 3;

  install_tss(gdt + 5);
  
  install_gdt(gdt, GDT_OFFSET);

  tss_flush();
}

