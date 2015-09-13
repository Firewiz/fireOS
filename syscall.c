#include "syscall.h"
#include "idt.h"

void (*syscall_handlers[256])(void *stack_base);

void install_syscall_handler(void *handler, int id) {
  syscall_handlers[id] = handler;
}

void handle_syscall(int n, struct regs *r) {
  if(syscall_handlers[r->eax]) {
    syscall_handlers[r->eax](r->ecx);
  } else {
    printf("Syscall stub -- id %x\n", r->eax);
  }
}

void init_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
}
