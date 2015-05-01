#include "idt.h"

void (*int_handlers[256])(int no);

void install_handler(void *handler, unsigned char gate) {
  int_handlers[gate] = handler;
}

void int_handler(struct regs *r) {
  int_handlers[r->int_no](r->int_no);
}
