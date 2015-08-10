#include "idt.h"
#include "printf.h"
#include "syscall.h"
#include "mt.h"

void handle_syscall(int no, struct regs *r) {
  if(no != syscall_interrupt) return;
  unsigned int fd, count;
  char *buf;
  switch(r->eax) {
  case 1: // sys_exit
    yield();
    break;
  case 4: // sys_write
    fd = *((unsigned int *) r->useresp + 1);
    buf = *((unsigned int *) r->useresp + 2);
    count = *((unsigned int *) r->useresp + 3);
    if(fd == 1) { // stdout
      int i;
      for(i = 0; i < count; i++) {
	vga_putchar(buf[i]);
      }
      vga_updatepos();
    }
    r->eax = count;
    break;
  }
}

void register_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
  syscall_interrupt = errupt;
}
