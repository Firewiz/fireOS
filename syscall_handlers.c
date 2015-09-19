#include "syscall.h"
#include "malloc.h"

#define SYSCALL_ARG(n, t) (*((t *) stack_base + n + 1))

void *sys_allocbuf(void *stack_base) {
  return malloc_user(SYSCALL_ARG(0, int), 1);
}

void register_syscall_handlers() {
  install_syscall_handler(sys_allocbuf, 7);
}


