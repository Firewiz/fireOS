#ifndef IDT_H
#define IDT_H

extern void setup_idt();
extern void idt_set_gate(void *handler, int gate);
void install_handler(void *handler, unsigned char gate);

struct idt_gate {
  unsigned short offset_1;
  unsigned short selector;
  unsigned char pad;
  unsigned char type;
  unsigned short offset_2;
} __attribute__((packed));

struct idt_desc {
  unsigned short size;
  unsigned int offset;
} __attribute__((packed));

struct regs {
  unsigned int gs, fs, es, ds;
  unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
  unsigned int int_no, err_code;
  unsigned int eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void int_handler(struct regs *r);

extern struct idt_gate idt[256];
extern struct idt_desc idtp;

#endif
