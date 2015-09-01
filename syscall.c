#include "syscall.h"
#include "idt.h"

void handle_syscall(struct regs *r) {
  printf("Syscall stub!\n");
}

void init_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
}
