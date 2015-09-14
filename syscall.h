#ifndef SYSCALL_H
#define SYSCALL_H

void init_syscall(int errupt);
void install_syscall_handler(void *handler, int id);
#endif
