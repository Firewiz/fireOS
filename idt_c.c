#include "idt.h"
#include "printf.h"
#include "process.h"
#include "stdlib.h"

void (*int_handlers[256])(int no, struct regs *r);

void install_handler(void *handler, unsigned char gate) {
  int_handlers[gate] = handler;
}

void int_handler(struct regs *r) {
  if(int_handlers[r->int_no]) {
    int_handlers[r->int_no](r->int_no, r);
  } else {
    printf("Unknown interrupt (%x)\n", r->int_no);
    //vga_write("Unknown interrupt\n");
  }
}
