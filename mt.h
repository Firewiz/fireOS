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

struct page_list {
  struct page_list *next;
  unsigned int vaddr;
  unsigned int paddr;
};

struct task {
  int active;
  struct regs *state;
  void (*entry)();
  void *stack_base;
  void *syscall_stack;
  struct fd_list *fds;
  struct page_list *pages;
};

typedef unsigned short taskid_t;

void init_mt();
taskid_t create_task();
void set_entry(void (*entry)(), taskid_t id);
void run_task(taskid_t id);
taskid_t start_task(void (*entry)());
void end_task(taskid_t id);
void next_ctx(int no, struct regs *r);
void page_task(unsigned int vaddr, taskid_t id, int user);
void yield();
taskid_t fork_task(taskid_t orig);

#define TASK_STACK_SIZE 0x10000
#define SYSCALL_STACK_SIZE 0x10000

#endif
