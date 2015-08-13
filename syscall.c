#include "idt.h"
#include "printf.h"
#include "syscall.h"
#include "mt.h"
#include "vga.h"
#include "malloc.h"
#include "stdlib.h"

#define STACK_VAR(n) (*((unsigned int *) r->useresp + (n)))

void handle_sysexit(unsigned int code) {
  yield();
}

int handle_syswrite(unsigned int fd, char *buf, unsigned int count) {
  // look up the file descriptor
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
      vga_updatepos();
    } else {
      return 0; // not implemented yet
    }	
  }
  return count;
}

unsigned int handle_sysgetfd() {
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
  printf("Allocated FD %d\n", fds->next->fd_no);
  return fds->next->fd_no;
}

int handle_syssetfdflag(unsigned int fd, unsigned int flags) {
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
  return (unsigned int) malloc_user(count, 1);
}

void handle_sysfreebuf(void *p) {
  free(p);
}

unsigned int handle_syscall(int no, struct regs *r) {
  if(no != syscall_interrupt) return 0;
  unsigned int fd, count;
  unsigned int flags;
  char *buf;
  switch(r->eax) {
  case 1:
    handle_sysexit(STACK_VAR(1));
    break;
  case 2:
    return handle_syswrite(STACK_VAR(1), (char *) STACK_VAR(2), STACK_VAR(3));
  case 3:
    return handle_sysgetfd();
  case 4:
    return handle_syssetfdflag(STACK_VAR(1), STACK_VAR(2));
  case 5:
    return handle_sysclearfdflag(STACK_VAR(1), STACK_VAR(2));
  case 6:
    return handle_sysgetfdflags(STACK_VAR(1));
  case 7:
    return handle_sysallocbuf(STACK_VAR(1));
  case 8:
    handle_sysfreebuf(STACK_VAR(1));
    return 0;
  }
  return 0;
}

void register_syscall(int errupt) {
  install_handler(handle_syscall, errupt);
  syscall_interrupt = errupt;
}
