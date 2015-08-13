#ifndef SYSCALL_H
#define SYSCALL_H

#define FD_READABLE 0x1
#define FD_WRITEABLE 0x2
#define FD_TERMINAL 0x4
#define FD_HAS_NAME 0x8
#define FD_ALL 0xF

unsigned long sys_exit(int code);
unsigned long sys_write(unsigned int fd, const char *buf, unsigned long count);
unsigned int sys_getfd(void);
int sys_setfdflag(unsigned int fd, unsigned int flags);
int sys_clearfdflag(unsigned int fd, unsigned int flags);
unsigned int sys_getfdflags(unsigned int fd);
void *sys_allocbuf(unsigned int len);
int sys_freebuf(void *buf);
#endif
