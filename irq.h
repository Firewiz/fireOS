#ifndef IRQ_H
#define IRQ_H
#include "idt.h"

#define install_irq(handler, irq) install_handler(handler, irq + 32)

void init_irq();

#endif
