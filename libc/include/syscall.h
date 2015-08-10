#ifndef SYSCALL_H
#define SYSCALL_H

unsigned long sys_exit(int code);
unsigned long sys_write(unsigned int fd, const char *buf, unsigned long count);

#endif
