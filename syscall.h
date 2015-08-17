#ifndef SYSCALL_H
#define SYSCALL_H

void register_syscall(int errupt);
unsigned int do_syscall(int sysno, ...);
extern void handle_syscall();

int syscall_interrupt;

#endif
