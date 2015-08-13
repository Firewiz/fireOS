#ifndef MT_H
#define MT_H

#ifndef DEFS
extern volatile int cur_ctx;
extern volatile struct task **tasks;
#undef DEFS
#endif

#include "irq.h"

#define FD_READABLE 0x1
#define FD_WRITEABLE 0x2
#define FD_TERMINAL 0x4
#define FD_HAS_NAME 0x8

struct fd_list {
  struct fd_list *next;
  unsigned int fd_no;
  unsigned int fd_flags;
  char *name;
};

struct task {
  int active;
  struct regs *state;
  void (*entry)();
  void *stack_base;
  char *name;
  char usermode;
  void *syscall_stack;
  struct fd_list *fds;
};

typedef unsigned short taskid_t;

void init_mt();
taskid_t start_task(void (*entry)(), int user);
void end_task(taskid_t id);
void next_ctx(int no, struct regs *r);
void yield();

#define TASK_STACK_SIZE 0x10000
#define SYSCALL_STACK_SIZE 0x10000

#endif
