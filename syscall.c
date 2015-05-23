#include "idt.h"
#include "printf.h"
#include "syscall.h"

void handle_syscall(int no, struct regs *r) {
  if(no != syscall_interrupt) return;
  switch(r->eax) {
  case 0:
    printf((char *) r->ecx);
    break;
  }
}

void register_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
  syscall_interrupt = errupt;
}
