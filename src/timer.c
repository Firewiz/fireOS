#include "timer.h"
#include "asmintr.h"
#include "idt.h"
#include "irq.h"
#include "mt.h"
#include "printf.h"
#include "process.h"

static volatile unsigned long ticks = 0;

void timer_phase(int hz) {
  int d = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, d & 0xFF);
  outb(0x40, (d >> 8) & 0xFF);
}

void init_timer() {
  install_irq(timer_intr, 0);
}

void timer_intr(int no, struct regs *r) {
  ticks++;
  if(ticks % 2 == 0) {
    next_ctx(r, 1);
  }
}

void delay(unsigned long amount) {
  unsigned long end = ticks + amount;
  int i;
  while(ticks < end) {
    ;
  }
}
