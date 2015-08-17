#define DEFS
#include "mt.h"
#undef DEFS
#include "malloc.h"
#include "irq.h"
#include "stdlib.h"
#include "gdt.h"
#include "printf.h"
#include "static.h"
#include "config.h"
#include "paging.h"

#ifdef DEBUG_MT
#define printd(f, ...) printf(f, __VA_ARGS__)
#else
#define printd(f, ...)
#endif

volatile struct task **tasks = (volatile struct task **) TASKLIST_BASE;
volatile int cur_ctx;
volatile int ctx_lock;

void yield() {
  printd("Task %d ended\n", cur_ctx);
  end_task(cur_ctx);
  asm volatile ("sti");
  while(1) ;
}

void init_mt() {
  //tasks = malloc(sizeof(struct task *) * 65536);
  printd("Tasks at 0x%x\n", &tasks);
  cur_ctx = 0;
  bzero(tasks, 65536 * sizeof(struct task *));
  install_handler(yield, 0x81);
  ctx_lock = 0;
}

taskid_t start_task(void (*entry)(), int user) {
  taskid_t id = create_task(user);
  set_entry(entry, id);
  run_task(id);
  return id;
}

unsigned int user_memptr = 0x06000000;
unsigned int user_offset = 0x06000000;
void *ualloc(unsigned int size) {
  void *r = (void *) user_offset;
  allocate_pages(user_offset, size, 1, cur_ctx);
  user_offset += size;
  return r;
}

taskid_t create_task(int user) {
  int i;
  for(i = 1; i < 65535; i++) {
    if(tasks[i] == 0) break;
  }
  if(user)
    tasks[i] = ualloc(sizeof(struct task));
  else 
    tasks[i] = malloc(sizeof(struct task));
  bzero((void *) tasks[i], sizeof(struct task));
  if(user)
    tasks[i]->state = ualloc(sizeof(struct regs));
  else
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
  if(user) {
    tasks[i]->stack_base = ualloc(TASK_STACK_SIZE) + (TASK_STACK_SIZE - 16);
  } else {
    tasks[i]->stack_base = malloc(TASK_STACK_SIZE) + (TASK_STACK_SIZE - 16);
  }
  tasks[i]->state->esp = (unsigned int) tasks[i]->stack_base;
  tasks[i]->state->useresp = (unsigned int) tasks[i]->stack_base;
  tasks[i]->usermode = user;
  if(user) {
    tasks[i]->syscall_stack = ualloc(SYSCALL_STACK_SIZE) + (SYSCALL_STACK_SIZE - 16);
    tasks[i]->pages = 0;
    tasks[i]->fds = 0;
  } else {
    tasks[i]->syscall_stack = 0;
  }
  tasks[i]->active = 0;
  if(user)
    allocate_pages(user_memptr, user_offset - user_memptr, 1, i);
  printd("Started task id %d, desc %x, stack %x, entry %x\n", i, tasks[i], tasks[i]->stack_base, tasks[i]->entry);
  return i;
}

void set_entry(void (*entry)(), taskid_t id) {
  tasks[id]->state->eip = (unsigned int) entry;
  tasks[id]->entry = entry;
}

void run_task(taskid_t id) {
  printf("Starting task %d\n", id);
  tasks[id]->active = 1;
}

void page_task(unsigned int vaddr, taskid_t id, int user) {
  unsigned int paddr = nonidentity_page(vaddr, user);
  struct page_list *p = tasks[id]->pages;
  if(p == 0) {
    p = malloc_user(sizeof(struct page_list), user);
    bzero(p, sizeof(struct page_list));
    p->vaddr = vaddr;
    p->paddr = paddr;
    p->next = 0;
    tasks[id]->pages = p;
  } else {
    while(p->next) p = p->next;
    p->next = malloc_user(sizeof(struct page_list), user);
    p->next->vaddr = vaddr;
    p->next->paddr = paddr;
    p->next->next = 0;
  }
  if(vaddr > 0x8000)
    printf("Paged %x to %x for task %d (%d)\n", vaddr, paddr, id, user);
}

void free_fd_list(struct fd_list *l) {
  if(l == 0) return;
  free_fd_list(l->next);
  free(l);
}

void free_page_list(struct page_list *l) {
  if(l == 0) return;
  free_page_list(l->next);
  free(l);
}

void end_task(taskid_t id) {
  if(tasks[id] == 0) return;
  tasks[id]->active = 0;
  if(!tasks[id]->usermode) {
    free(tasks[id]->stack_base - (SYSCALL_STACK_SIZE - 16));
    free(tasks[id]->state);
  }
  free_fd_list(tasks[id]->fds);
  free_page_list(tasks[id]->pages);
  if(!tasks[id]->usermode)
    free((void *) tasks[id]);
  tasks[id] = 0;
}

void next_ctx(int no, struct regs *r) {
  if(ctx_lock) return;
  ctx_lock = 1;
  int new_ctx = cur_ctx;
  if(tasks[cur_ctx] != 0) {// task ended since last timeslice
    memcpy(tasks[cur_ctx]->state, r, sizeof(struct regs));
  }
  do {
    new_ctx++;
    if(new_ctx > 65535) new_ctx = 1;
  } while(tasks[new_ctx] == 0 || tasks[new_ctx]->active != 1);
  tasks[new_ctx]->state->useresp = tasks[new_ctx]->state->esp;
#ifdef DEBUG_MT_LOUD
  printd("%x %x Loaded task %d useresp %x eip %x\n", &tasks[new_ctx], tasks[new_ctx]->active,
  	 new_ctx, tasks[new_ctx]->state->useresp, tasks[new_ctx]->state->eip);
#endif
  tasks[new_ctx]->state->load_stack = 1;
  if(tasks[new_ctx]->usermode) {
    set_kernel_stack((unsigned int) tasks[new_ctx]->syscall_stack);
  }
  struct page_list *p = tasks[new_ctx]->pages;
  while(p) {
    if(is_present(p->vaddr))
      unmap_page(p->vaddr);
    mapped_page(p->vaddr, p->paddr, tasks[new_ctx]->usermode);
    p = p->next;
  }
  memcpy(r, tasks[new_ctx]->state, sizeof(struct regs));
  cur_ctx = new_ctx;
  ctx_lock = 0;
}
