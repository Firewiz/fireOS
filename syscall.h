#ifndef SYSCALL_H
#define SYSCALL_H

void init_syscall(int errupt);
void install_syscall_handlers(void *handler, int id);
#endif
