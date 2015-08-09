#ifndef TIMER_H
#define TIMER_H
#include "idt.h"

void timer_phase(int);
void init_timer();
void timer_intr(int no, struct regs *r);
void delay(unsigned long);

#endif
