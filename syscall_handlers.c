#include "syscall.h"
#include "malloc.h"
#include "process.h"
#include "printf.h"

#define SYSCALL_ARG(n, t) ((t) *((int *) stack_base + n + 1))
#define SCH(r, n) r n(void *stack_base)

SCH(void, sys_exit) { // 1
  yield();
}

SCH(int, sys_write) { // 2
  int fd = SYSCALL_ARG(0, int);
  char *wbuf = SYSCALL_ARG(1, char *);
  int len = SYSCALL_ARG(2, int);
  proc_fd_list *fdl = get_proc(current_pid)->fds;
  while(fdl) {
    if(fdl->fd == fd) {
      if(!(fdl->fd_flags & FD_FLAG_WR)) return -1;
      if(fdl->fd_flags & FD_FLAG_TTY) {
	int i;
	for(i = 0; i < len; i++) {
	  printf("%c", wbuf[i]);
	}
      }
      return len;
    }
    fdl = fdl->next;
  }
  return -1;
}

SCH(int, sys_getfd) { // 3
  proc_fd_list *fdl = get_proc(current_pid)->fds;
  int fd = 0;
  if(!fdl) {
    fdl = malloc_user(sizeof(proc_fd_list), 1);
    fdl->fd = 0;
    fdl->fd_flags = 0;
    get_proc(current_pid)->fds = fdl;
    return 0;
  }
  while(fdl->next) { fd++; fdl = fdl->next; }
  fdl->next = malloc_user(sizeof(proc_fd_list), 1);
  fdl->next->fd = ++fd;
  fdl->next->fd_flags = 0;
  return fd;
}

SCH(void, sys_setfdflag) { // 4
  int fd = SYSCALL_ARG(0, int);
  unsigned int flags = SYSCALL_ARG(1, unsigned int);
  proc_fd_list *fdl = get_proc(current_pid)->fds;
  while(fdl) {
    if(fdl->fd == fd) {
      fdl->fd_flags |= flags;
      return;
    }
    fdl = fdl->next;
  }
  return;
}

SCH(void, sys_clearfdflag) { // 5
  int fd = SYSCALL_ARG(0, int);
  unsigned int flags = SYSCALL_ARG(1, unsigned int);
  proc_fd_list *fdl = get_proc(current_pid)->fds;
  while(fdl) {
    if(fdl->fd == fd) {
      fdl->fd_flags &= ~flags;
      return;
    }
    fdl = fdl->next;
  }
  return;
}

SCH(void *, sys_allocbuf) { // 7
  void *a = malloc_user(SYSCALL_ARG(0, int), 1);
  return a;
}

SCH(void, sys_fork) { // 10
  fork();
}

void register_syscall_handlers() {
  install_syscall_handler(sys_exit, 1);
  install_syscall_handler(sys_write, 2);
  install_syscall_handler(sys_getfd, 3);
  install_syscall_handler(sys_setfdflag, 4);
  install_syscall_handler(sys_clearfdflag, 5);
  install_syscall_handler(sys_allocbuf, 7);
  install_syscall_handler(sys_fork, 10);
}


