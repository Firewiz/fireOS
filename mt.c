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

taskid_t start_task(void (*entry)()) {
  taskid_t id = create_task();
  set_entry(entry, id);
  run_task(id);
  return id;
}

unsigned int user_memptr = 0x06000000;
unsigned int user_offset = 0x06000000;
void *ualloc(unsigned int size) {
  void *r = (void *) user_offset;
  unsigned int i;
  for(i = 0; i < size; i += 0x1000)
    nonidentity_page((i + user_offset) / 0x1000, 0);
  if(!is_present((user_offset + size) / 0x1000))
    nonidentity_page((user_offset + size) / 0x1000, 0);
  user_offset += size;
  return r;
}

taskid_t create_task() {
  int i;
  for(i = 1; i < 65535; i++) {
    if(tasks[i] == 0) break;
  }
  tasks[i] = malloc(sizeof(struct task));
  bzero((void *) tasks[i], sizeof(struct task));
  tasks[i]->state = malloc(sizeof(struct regs));
  bzero(tasks[i]->state, sizeof(struct regs));
  tasks[i]->syscall_state = malloc(sizeof(struct regs));
  bzero(tasks[i]->syscall_state, sizeof(struct regs));
  tasks[i]->state->gs = tasks[i]->state->fs
    = tasks[i]->state->es
    = tasks[i]->state->ds
    = tasks[i]->state->ss = 0x23;
  tasks[i]->state->cs = 0x1b;
  tasks[i]->stack_base = ualloc(TASK_STACK_SIZE) + (TASK_STACK_SIZE - 16);
  tasks[i]->state->esp = (unsigned int) tasks[i]->stack_base;
  tasks[i]->state->useresp = (unsigned int) tasks[i]->stack_base;
  tasks[i]->syscall_stack = ualloc(SYSCALL_STACK_SIZE) + (SYSCALL_STACK_SIZE - 16);
  tasks[i]->pages = 0;
  tasks[i]->fds = 0;
  tasks[i]->active = 0;
  tasks[i]->usermode = 1;
  int umsize = user_offset - user_memptr;
  void *mem = malloc(umsize);
  memcpy(mem, (void *) user_memptr, umsize);
  int j;
  for(j = 0; j < umsize; j += 0x1000) {
    unmap_page((j + user_memptr) / 0x1000);
  }
  allocate_pages((void *) user_memptr, umsize, 1, i);
  memcpy((void *) user_memptr, mem, umsize);
  free(mem);
  printd("Created task id %d, desc %x, stack %x, entry %x\n", i, tasks[i], tasks[i]->stack_base, tasks[i]->entry);
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

void load_task_vm(taskid_t id) {
  if(tasks[id] == 0) return;
  struct page_list *p = tasks[id]->pages;
  while(p) {
    mapped_page(p->vaddr, p->paddr, 1);
    p = p->next;
  }
}

void page_task(unsigned int vaddr, taskid_t id, int user) {
  unsigned int paddr = nonidentity_page(vaddr, user);
  struct page_list *p = tasks[id]->pages;
  printd("Paging in %x for %d\n", vaddr, id);
  load_task_vm(id); // to avoid malloc confusion later
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
    printd("Paged %x to %x for task %d (%d)\n", vaddr, paddr, id, user);
  load_task_vm(cur_ctx);
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

struct fd_list *dup_fd_list(struct fd_list *l) {
  if(l == 0) return 0;
  struct fd_list *r = malloc_user(sizeof(struct fd_list), 1);
  memcpy(r, l, sizeof(struct fd_list));
  r->next = dup_fd_list(l->next);
  return r;
}

struct page_list *dup_page_list(struct page_list *l) {
  if(l == 0) return 0;
  struct page_list *r = malloc(sizeof(struct page_list));
  memcpy(r, l, sizeof(struct fd_list));
  r->next = dup_page_list(l->next);
  return r;
}


void end_task(taskid_t id) {
  if(tasks[id] == 0) return;
  tasks[id]->active = 0;
  free_fd_list(tasks[id]->fds);
  free_page_list(tasks[id]->pages);
  tasks[id] = 0;
}

void dump_regs(struct regs *r) {
  printf("PEX %x  LS %x\n", r->preserve_eax, r->load_stack);
  printf(" GS %x  FS %x  ES %x  DS %x\n", r->gs, r->fs, r->es, r->ds);
  printf("EDI %x ESI %x EBP %x ESP %x\n", r->edi, r->esi, r->ebp, r->esp);
  printf("EBX %x EDX %x ECX %x EAX %x\n", r->ebx, r->edx, r->ecx, r->eax);
  printf(" IN %x  EC %x\nEIP %x  CS %x FGS %x UEP %x  SS %x\n", r->int_no, r->err_code, r->eip, r->cs, r->eflags, r->useresp, r->ss);
}

void next_ctx(int no, struct regs *r) {
  if(ctx_lock) return;
  ctx_lock = 1;
  int new_ctx = cur_ctx;
  if(tasks[cur_ctx] != 0) {// task ended since last timeslice
    if(r->cs == 0x08) {
      memcpy(tasks[cur_ctx]->syscall_state, r, sizeof(struct regs));
      tasks[cur_ctx]->usermode = 0;
    } else {
      memcpy(tasks[cur_ctx]->state, r, sizeof(struct regs));
      tasks[cur_ctx]->usermode = 1;
    }
  }
  do {
    new_ctx++;
    if(new_ctx > 65535) new_ctx = 1;
  } while(tasks[new_ctx] == 0 || tasks[new_ctx]->active != 1);
  if(new_ctx == cur_ctx) return;

  if(tasks[new_ctx]->state_copy) {
    memcpy(tasks[new_ctx]->state, tasks[tasks[new_ctx]->state_copy]->state, sizeof(struct regs));
    memcpy(tasks[new_ctx]->syscall_state, tasks[tasks[new_ctx]->state_copy]->syscall_state, sizeof(struct regs));
    tasks[new_ctx]->state_copy = 0;
  }

  tasks[new_ctx]->swapped = 1;
  if(tasks[new_ctx]->state->cs != 0x08) {
    set_kernel_stack((unsigned int) tasks[new_ctx]->syscall_stack);
    tasks[new_ctx]->state->load_stack = 1;
  }
  else
    set_kernel_stack((unsigned int) tasks[new_ctx]->syscall_state->esp);
  struct page_list *p = tasks[new_ctx]->pages;
  while(p) {
    if(is_present(p->vaddr))
      unmap_page(p->vaddr);
    mapped_page(p->vaddr, p->paddr, 1);
    p = p->next;
  }
  if(tasks[new_ctx]->usermode)
    memcpy(r, tasks[new_ctx]->state, sizeof(struct regs));
  else
    memcpy(r, tasks[new_ctx]->syscall_state, sizeof(struct regs));
#ifdef DEBUG_MT_LOUD
  printd("Loaded task %d\n", new_ctx);
  dump_regs(r);
#endif
  cur_ctx = new_ctx;
  ctx_lock = 0;
}

taskid_t fork_task() {
  int i, orig_id = cur_ctx;
  for(i = 1; i < 65535; i++) {
    if(tasks[i] == 0) break;
  }
  printf("Forking task %d to child %d\n", cur_ctx, i);
  tasks[i] = malloc(sizeof(struct task));
  printf("Copying task data... ");
  memcpy((void *) tasks[i], (void *) tasks[cur_ctx], sizeof(struct task));
  printf("done\n");
  tasks[i]->active = 0;
  tasks[i]->state = malloc(sizeof(struct regs));
  tasks[i]->syscall_state = malloc(sizeof(struct regs));
  printf("Copying state data... ");
  memcpy(tasks[i]->state, tasks[cur_ctx]->state, sizeof(struct regs));
  memcpy(tasks[i]->syscall_state, tasks[cur_ctx]->syscall_state, sizeof(struct regs));
  printf("done\n");
  struct page_list *p, *orig = dup_page_list(tasks[cur_ctx]->pages);
  p = dup_page_list(tasks[cur_ctx]->pages);
  unsigned int tp_addr;
  printf("Reallocating pages... \n");
  while(p) {
    // Create a temporary page, remap the new page to point to the
    // physical memory of the temp page. Don't unmap the temp page
    // because of frameset issues.
    tp_addr = nonidentity_page(0xF0000, 0);
    memcpy((void *) 0xF0000000, (void *) p->vaddr, 4096);
    mapped_page(p->vaddr, tp_addr, 1);
    p->paddr = tp_addr;
    p = p->next;
  }
  printf("done\n");
  // At this point, we're using the memory of the child process.
  p = orig;
  printf("Restoring page map...\n");
  while(p) {
    mapped_page(p->vaddr, p->paddr, 1);
    p = p->next;
  }
  free_page_list(p);
  printf("done\n");
  tasks[i]->state_copy = cur_ctx;
  tasks[i]->swapped = 0;
  tasks[i]->active = 1;
  tasks[cur_ctx]->swapped = 0;
  // The task splits here.
  // Now, we need to busy-wait until the new task is loaded and the
  // state is copied properly.
  while(tasks[cur_ctx]->swapped == 0) ;
  printf("Forked (%d).\n", cur_ctx);
  //memcpy(tasks[i]->state, tasks[cur_ctx]->state, sizeof(struct regs));
  if(cur_ctx == i) return 0;
  return i;
}
