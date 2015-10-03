#ifndef GDT_H
#define GDT_H

struct gdt_entry_bits
{
  unsigned int limit_low:16;
  unsigned int base_low : 24;
  unsigned int accessed :1;
  unsigned int read_write :1;
  unsigned int conforming :1;
  unsigned int code :1;
  unsigned int always_1 :1;
  unsigned int DPL :2;
  unsigned int present :1;
  unsigned int limit_high :4;
  unsigned int available :1;
  unsigned int always_0 :1;
  unsigned int big :1;
  unsigned int gran :1;
  unsigned int base_high :8;
} __attribute__ ((packed)); 

struct tss_entry
{
   unsigned int prev_tss;   // The previous TSS - if we used hardware task switching this would form a linked list.
   unsigned int esp0;       // The stack pointer to load when we change to kernel mode.
   unsigned int ss0;        // The stack segment to load when we change to kernel mode.
   unsigned int esp1;       // everything below here is unusued now.. 
   unsigned int ss1;
   unsigned int esp2;
   unsigned int ss2;
   unsigned int cr3;
   unsigned int eip;
   unsigned int eflags;
   unsigned int eax;
   unsigned int ecx;
   unsigned int edx;
   unsigned int ebx;
   unsigned int esp;
   unsigned int ebp;
   unsigned int esi;
   unsigned int edi;
   unsigned int es;         
   unsigned int cs;        
   unsigned int ss;        
   unsigned int ds;        
   unsigned int fs;       
   unsigned int gs;         
   unsigned int ldt;      
   unsigned short trap;
   unsigned short iomap_base;
} __attribute__ ((packed)); 


struct gdt_desc {
  unsigned short size;
  unsigned int offset;
} __attribute__((packed));

void setup_gdt();
extern void install_gdt(struct gdt_entry_bits *gdt, unsigned long size);
extern void tss_flush();
extern void enter_usermode(void (*entry)());
void set_kernel_stack(unsigned int esp);
#endif
