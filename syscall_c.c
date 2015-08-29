#include "idt.h"
#include "printf.h"
#include "syscall.h"
#include "mt.h"
#include "vga.h"
#include "malloc.h"
#include "stdlib.h"
#include "keyboard.h"

#define STACK_VAR(n) (*((unsigned int *) r->useresp + (n)))

void check_fds(taskid_t t) {
  if(!tasks[t]->fds) {
    tasks[t]->fds = malloc_user(sizeof(struct fd_list), 1);
    bzero(tasks[t]->fds, sizeof(struct fd_list));
  }
}

void handle_sysexit(unsigned int code) {
  yield();
}

int handle_syswrite(unsigned int fd, char *buf, unsigned int count) {
  // look up the file descriptor
  check_fds(cur_ctx);
  struct fd_list *fds = tasks[cur_ctx]->fds;
  int found = 0;
  while(fds != 0) {
    if(fds->fd_no == fd) {
      found = 1;
      break;
    }
    fds = fds->next;
  }
  if(!found)
    return 0;
  if(fds->fd_flags & FD_WRITEABLE) {
    if(fds->fd_flags & FD_TERMINAL) {
      unsigned int i;
      for(i = 0; i < count; i++) {
	vga_putchar(buf[i]);
      }
    } else {
      return 0; // not implemented yet
    }	
  }
  return count;
}

unsigned int handle_sysgetfd() {
  check_fds(cur_ctx);
  struct fd_list *fds = tasks[cur_ctx]->fds;
  int found = 0;
  if(tasks[cur_ctx]->fds == 0) {
    tasks[cur_ctx]->fds = malloc_user(sizeof(struct fd_list), 1);
    bzero(tasks[cur_ctx]->fds, sizeof(struct fd_list));
    tasks[cur_ctx]->fds->fd_no = 1;
    return 1;
  }
  fds = tasks[cur_ctx]->fds;
  while(fds->next != 0) fds = fds->next;
  // fds is now the last link in the chain.
  fds->next = malloc_user(sizeof(struct fd_list), 1);
  bzero(fds->next, sizeof(struct fd_list));
  fds->next->fd_no = fds->fd_no + 1;
  return fds->next->fd_no;
}

int handle_syssetfdflag(unsigned int fd, unsigned int flags) {
  check_fds(cur_ctx);
  struct fd_list *fds = tasks[cur_ctx]->fds;
  int found = 0;
  fds = tasks[cur_ctx]->fds;
  found = 0;
  while(fds != 0) {
    if(fds->fd_no == fd) {
      found = 1;
      break;
    }
    fds = fds->next;
  }
  if(!found)
    return 1;
  fds->fd_flags |= flags;
  return 0;
}

int handle_sysclearfdflag(unsigned int fd, unsigned int flags) {
  check_fds(cur_ctx);
  struct fd_list *fds = tasks[cur_ctx]->fds;
  int found = 0;
  fds = tasks[cur_ctx]->fds;
  found = 0;
  while(fds != 0) {
    if(fds->fd_no == fd) {
      found = 1;
      break;
    }
    fds = fds->next;
  }
  if(!found)
    return 1;
  fds->fd_flags &= ~flags;
  return 0;
}

unsigned int handle_sysgetfdflags(unsigned int fd) {
  check_fds(cur_ctx);
  struct fd_list *fds = tasks[cur_ctx]->fds;
  int found = 0;
  fds = tasks[cur_ctx]->fds;
  found = 0;
  while(fds != 0) {
    if(fds->fd_no == fd) {
      found = 1;
      break;
    }
    fds = fds->next;
  }
  if(!found)
    return -1;
  return fds->fd_flags;
}

unsigned int handle_sysallocbuf(unsigned int count) {
  void *v = malloc_user(count, 1);
  return (unsigned int) v;
}

void handle_sysfreebuf(void *p) {
  free(p);
}

int handle_sysread(unsigned int fd, char *buf, unsigned int count) {
  check_fds(cur_ctx);
  struct fd_list *fds = tasks[cur_ctx]->fds;
  int found = 0;
  int read = 0;
  while(fds != 0) {
    if(fds->fd_no == fd) {
      found = 1;
      break;
    }
    fds = fds->next;
  }
  if(!found)
    return 0;
  if(fds->fd_flags & FD_READABLE) {
    if(fds->fd_flags & FD_TERMINAL) {
      unsigned int i;
      char c;
      for(i = 0; i < count; i++) {
	c = getc();
	vga_putchar(c);
	buf[i] = c;
	read++;
	if(c == '\n') break;
      }
    } else {
      return 0; // not implemented yet
    }	
  }
  return read;
}

unsigned int handle_sysfork() {
  return fork_task();
}

unsigned int do_syscall(int syscall, ...) {
  unsigned int fd, count;
  unsigned int flags;
  char *buf;
  __builtin_va_list vl;
  __builtin_va_start(vl, syscall);
  int r = 0;
  switch(syscall) {
  case 1:
    handle_sysexit(__builtin_va_arg(vl, unsigned int));
    break;
  case 2:
    r = handle_syswrite(__builtin_va_arg(vl, unsigned int), (char *) __builtin_va_arg(vl, unsigned int), __builtin_va_arg(vl, unsigned int));
    break;
  case 3:
    r = handle_sysgetfd();
    break;
  case 4:
    r = handle_syssetfdflag(__builtin_va_arg(vl, unsigned int), __builtin_va_arg(vl, unsigned int));
    break;
  case 5:
    r = handle_sysclearfdflag(__builtin_va_arg(vl, unsigned int), __builtin_va_arg(vl, unsigned int));
    break;
  case 6:
    r = handle_sysgetfdflags(__builtin_va_arg(vl, unsigned int));
    break;
  case 7:
    r = handle_sysallocbuf(__builtin_va_arg(vl, unsigned int));
    break;
  case 8:
    handle_sysfreebuf(__builtin_va_arg(vl, unsigned int));
    break;
  case 9:
    r = handle_sysread(__builtin_va_arg(vl, unsigned int), (char *) __builtin_va_arg(vl, unsigned int), __builtin_va_arg(vl, unsigned int));
    break;
  case 10:
    r = handle_sysfork();
    break;
  }
  return r;
}

void register_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
  syscall_interrupt = errupt;
}
