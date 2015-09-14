#include "syscall.h"
#include "malloc.h"

void *sys_allocbuf(void *stack_base) {
  return malloc_user(*((int *) stack_base), 1);
}

void register_syscall_handlers() {
  install_syscall_handler(sys_allocbuf, 7);
}


