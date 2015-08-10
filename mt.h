#ifndef MT_H
#define MT_H

#ifndef DEFS
extern volatile int cur_ctx;
extern struct task *tasks[65536];
#undef DEFS
#endif

#include "irq.h"

struct task {
  int active;
  struct regs *state;
  void (*entry)();
  void *stack_base;
  char name[80];
  char usermode;
  void *syscall_stack;
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
