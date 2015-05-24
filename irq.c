#include "irq.h"
#include "idt.h"
#include "asmintr.h"

void (*irq_handlers[16])(int irq, struct regs *r);

void irq_common_handler(int inum, struct regs *r) {
  if(irq_handlers[inum - 32]) {
    irq_handlers[inum - 32](inum - 32, r);
  }
  if(inum >= 40)
    outb(0xA0, 0x20);
  outb(0x20, 0x20);
}

void install_irq(void *handler, int gate) {
  irq_handlers[gate] = handler;
}

void init_irq() {
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  install_handler(irq_common_handler, 32);
  install_handler(irq_common_handler, 33);
  install_handler(irq_common_handler, 34);
  install_handler(irq_common_handler, 35);
  install_handler(irq_common_handler, 36);
  install_handler(irq_common_handler, 37);
  install_handler(irq_common_handler, 38);
  install_handler(irq_common_handler, 39);
  install_handler(irq_common_handler, 40);
  install_handler(irq_common_handler, 41);
  install_handler(irq_common_handler, 42);
  install_handler(irq_common_handler, 43);
  install_handler(irq_common_handler, 44);
  install_handler(irq_common_handler, 45);
  install_handler(irq_common_handler, 46);
}

