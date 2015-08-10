#define DEFS
#include "mt.h"
#undef DEFS
#include "malloc.h"
#include "irq.h"
#include "stdlib.h"
#include "gdt.h"
#include "printf.h"

static volatile struct task *tasks[65536];
volatile int cur_ctx;

void yield() {
  printf("Task %d ended\n", cur_ctx);
  end_task(cur_ctx);
  asm volatile ("sti");
  while(1) ;
}

void init_mt() {
  //  tasks = malloc(sizeof(struct task *) * 65536);
  printf("Tasks at 0x%x\n", &tasks);
  cur_ctx = 0;
  bzero(tasks, 65536 * sizeof(struct task *));
  install_handler(yield, 0x81);
}

void task_wrapper() {
  void (*entry)() = tasks[cur_ctx]->entry;
  printf("Starting task %d\n", cur_ctx);
  //  asm volatile("sti");
  entry();
  yield();
}

taskid_t start_task(void (*entry)(), int user) {
  int i;
  for(i = 1; i < 65535; i++) {
    if(tasks[i] == 0) break;
  }
  tasks[i] = malloc(sizeof(struct task));
  bzero(tasks[i], sizeof(struct task));
  tasks[i]->state = malloc(sizeof(struct regs));
  bzero(tasks[i]->state, sizeof(struct regs));
  if(user) {
    tasks[i]->state->gs = tasks[i]->state->fs
      = tasks[i]->state->es
      = tasks[i]->state->ds
      = tasks[i]->state->ss = 0x23;
    tasks[i]->state->cs = 0x1b;
  } else {
    tasks[i]->state->gs = tasks[i]->state->fs
      = tasks[i]->state->es
      = tasks[i]->state->ds = 0x10;
    tasks[i]->state->cs = 0x08;
  }
  tasks[i]->state->eip = (unsigned int) task_wrapper;
  tasks[i]->entry = entry;
  tasks[i]->stack_base = malloc(TASK_STACK_SIZE) + (TASK_STACK_SIZE - 16);
  tasks[i]->state->esp = (unsigned int) tasks[i]->stack_base;
  tasks[i]->state->useresp = (unsigned int) tasks[i]->stack_base;
  tasks[i]->usermode = user;
  if(user) {
    tasks[i]->syscall_stack = malloc(SYSCALL_STACK_SIZE) + (SYSCALL_STACK_SIZE - 16);
  } else {
    tasks[i]->syscall_stack = 0;
  }
  tasks[i]->active = 1;
  printf("Started task id %d, desc %x, stack %x, entry %x\n", i, tasks[i], tasks[i]->stack_base, tasks[i]->entry);
  return i;
}

void end_task(taskid_t id) {
  if(tasks[id] == 0) return;
  tasks[id]->active = 0;
  free(tasks[id]->syscall_stack);
  free(tasks[id]->stack_base);
  free(tasks[id]->state);
  free(tasks[id]);
  tasks[id] = 0;
}

void next_ctx(int no, struct regs *r) {
  int new_ctx = cur_ctx;
  if(tasks[cur_ctx] != 0) {// task ended since last timeslice
    memcpy(tasks[cur_ctx]->state, r, sizeof(struct regs));
  }
  do {
    new_ctx++;
    if(new_ctx > 65535) new_ctx = 1;
  } while(tasks[new_ctx] == 0 || tasks[new_ctx]->active != 1);
  tasks[new_ctx]->state->useresp = tasks[new_ctx]->state->esp;
  /*  printf("%x %x Loaded task %d useresp %x eip %x\n", &tasks[new_ctx], tasks[new_ctx]->active,
      new_ctx, tasks[new_ctx]->state->useresp, tasks[new_ctx]->state->eip); */
  tasks[new_ctx]->state->load_stack = 1;
  if(tasks[new_ctx]->usermode) {
    set_kernel_stack((unsigned int) tasks[new_ctx]->syscall_stack);
  }
  memcpy(r, tasks[new_ctx]->state, sizeof(struct regs));
  cur_ctx = new_ctx;
}
