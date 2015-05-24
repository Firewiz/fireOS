#ifndef MT_H
#define MT_H
#include "irq.h"

struct task {
  struct regs *state;
  void (*entry)();
  char name[80];
};

typedef unsigned short taskid_t;

void init_mt();
taskid_t start_task(void (*entry)());
void end_task(taskid_t id);
void next_ctx(int no, struct regs *r);

#endif
