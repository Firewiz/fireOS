#include "mt.h"
#include "malloc.h"
#include "irq.h"
#include "stdlib.h"

struct task * tasks[65536];
volatile int cur_ctx;

void yield() {
  end_task(cur_ctx);
  asm volatile ("int $32");
}

void init_mt() {
  cur_ctx = 0;
  bzero(tasks, 65536 * sizeof(struct task *));
  install_irq(next_ctx, 0);
  install_handler(yield, 0x81);
  tasks[0] = malloc(sizeof(struct task));
  tasks[0]->state = malloc(sizeof(struct regs));
}

void task_wrapper() {
  void (*entry)() = tasks[cur_ctx]->entry;
  asm volatile("sti");
  entry();
  asm volatile("int $0x81");
}

taskid_t start_task(void (*entry)()) {
  int i;
  for(i = 1; i < 65535; i++) {
    if(tasks[i] == 0) break;
  }
  tasks[i] = malloc(sizeof(struct task));
  bzero(tasks[i], sizeof(struct task));
  tasks[i]->state = malloc(sizeof(struct regs));
  bzero(tasks[i]->state, sizeof(struct regs));
  tasks[i]->state->gs = tasks[i]->state->fs
    = tasks[i]->state->es
    = tasks[i]->state->ds = 0x10;
  tasks[i]->state->cs = 0x08;
  tasks[i]->state->eip = (unsigned int) task_wrapper;
  tasks[i]->entry = entry;
  return i;
}

void end_task(taskid_t id) {
  if(tasks[id] == 0) return;
  free(tasks[id]->state);
  free(tasks[id]);
  tasks[id] = 0;
}

void next_ctx(int no, struct regs *r) {
  if(tasks[cur_ctx] != 0) // task ended since last timeslice
    memcpy(tasks[cur_ctx]->state, r, sizeof(struct regs));
  do {
    cur_ctx++;
    if(cur_ctx > 65535) cur_ctx = 0;
  } while(tasks[cur_ctx] == 0);
  memcpy(r, tasks[cur_ctx]->state, sizeof(struct regs));
}
