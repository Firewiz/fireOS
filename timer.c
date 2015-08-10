#include "timer.h"
#include "asmintr.h"
#include "idt.h"
#include "irq.h"
#include "mt.h"
#include "printf.h"

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
  next_ctx(no, r);
}

void delay(unsigned long amount) {
  unsigned long end = ticks + amount;
  unsigned long remain = 0;
  int i;
  while(ticks < end) {
    remain = end - ticks;
  }
}
