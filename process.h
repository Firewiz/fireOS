#ifndef PROCESS_H
#define PROCESS_H

typedef unsigned short pid_t;
#define PID_MAX 65535

#include "idt.h"

struct proc_page_list_t {
  struct proc_page_list_t *next;
  unsigned int physical, virtual;
};

typedef struct proc_page_list_t proc_page_list;

#define FD_FLAG_RD  0x01
#define FD_FLAG_WR  0x02
#define FD_FLAG_TTY 0x04

struct proc_fd_list_t {
  struct proc_fd_list_t *next;
  int fd;
  unsigned int fd_flags;
};

typedef struct proc_fd_list_t proc_fd_list;

#define PROCESS_STACK_SIZE 0x10000
#define KERNEL_STACK_SIZE 0x10000
#define PF_ACTIVE 0x01

struct process_t {
  pid_t id;
  struct regs *state;
  proc_page_list *pages;
  proc_fd_list *fds;
  unsigned char *stack;
  unsigned char *kernel_stack;
  unsigned int flags;
};

typedef struct process_t process;

struct process_list_t {
  struct process_list_t *next, *prev;
  process *p;
};

typedef struct process_list_t process_list;

#ifndef DEFS
extern process_list *plist;
extern volatile pid_t current_pid;
extern int mt_enabled;
#endif

process *get_proc(pid_t id);
void allocate_pages(unsigned int base, unsigned int offset, int user, pid_t owner);
void init_mt();
void run_init(void (*entry)());
pid_t fork(void);
void yield();
void next_ctx(struct regs *r, int save_state);
#endif
