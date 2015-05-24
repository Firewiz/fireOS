#include "timer.h"
#include "asmintr.h"

void timer_phase(int hz) {
  int d = 1193180 / hz;
  outb(0x43, 0x36);
  outb(0x40, d & 0xFF);
  outb(0x40, d >> 8);
}
