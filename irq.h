#ifndef IRQ_H
#define IRQ_H
#include "idt.h"

void init_irq();
void irq_common_handler(int inum);
void install_irq(void *handler, int gate);
#endif
