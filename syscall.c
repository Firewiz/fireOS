#include "syscall.h"
#include "idt.h"
#include "printf.h"

int (*syscall_handlers[256])(struct regs *r, void *stack_base);

void install_syscall_handler(void *handler, int id) {
  syscall_handlers[id] = handler;
}

int handle_syscall(int n, struct regs *r) {
  if(syscall_handlers[r->eax]) {
    printf("syscall %d\n", r->eax);
    if(r->eax == 10) { // fork requires special handling for the
		       // return value
      syscall_handlers[r->eax](r, (void *) r->ecx);
      return 0;
    } else {
      return r->eax = syscall_handlers[r->eax](r, (void *) r->ecx);
    }
  } else {
    printf("Syscall stub -- id %x\n", r->eax);
  }
  return 0;
}

void init_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
}
