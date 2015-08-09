#ifndef MT_H
#define MT_H

#ifndef DEFS
extern volatile int cur_ctx;
extern struct task *tasks[65536];
#undef DEFS
#endif

#include "irq.h"

struct task {
  struct regs *state;
  void (*entry)();
  void *stack_base;
  char name[80];
};

typedef unsigned short taskid_t;

void init_mt();
taskid_t start_task(void (*entry)());
void end_task(taskid_t id);
void next_ctx(int no, struct regs *r);

#define TASK_STACK_SIZE 0x1000

#endif
