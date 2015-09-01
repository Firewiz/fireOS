#define DEFS
#include "process.h"
#undef DEFS

#include "malloc.h"
#include "paging.h"
#include "stdlib.h"
#include "static.h"

// plist is sorted in ascending pid order
process_list *plist;
volatile pid_t current_pid;

process *get_proc(pid_t id) {
  if(plist == 0) return 0;
  process_list *p;
  for(p = plist; p->next; p = p->next) {
    if(p->p->id == id) return p->p;
  }
  return 0;
}

void allocate_pages(unsigned int base, unsigned int offset, int user, pid_t owner) {
  unsigned int i;
  unsigned int phy;
  process *proc = get_proc(owner);
  proc_page_list *pl = proc->pages;
  for(i = 0; i < offset; i += 0x1000) {
    if(!is_present((i + base) / 0x1000)) {
      phy = nonidentity_page(i + base, user);
      // only alter the page list if these were user pages
      if(user) {
	if(proc->pages == 0) {
	  proc->pages = malloc(sizeof(proc_page_list));
	  pl = proc->pages;
	} else {
	  while(pl->next) pl = pl->next;
	  pl->next = malloc(sizeof(proc_page_list));
	  pl = pl->next;
	}
      }
      pl->physical = phy;
      pl->virtual = (i + base) / 0x1000;
      pl->next = 0;
    }
  }
}

void init_mt(void (*entry)()) {
  plist = malloc(sizeof(process_list));
  current_pid = 0;
  plist->next = plist->prev = 0;
  process *init = malloc(sizeof(process));
  plist->p = init;
  init->id=0;
  init->state = malloc(sizeof(struct regs));
  bzero(init->state, sizeof(struct regs));
  init->pages = 0;
  init->stack = (unsigned char *) USER_STACK + PROCESS_STACK_SIZE;
  allocate_pages((unsigned int) init->stack, PROCESS_STACK_SIZE, 1, 0);
  init->kernel_stack = (unsigned char *) KERNEL_STACK + KERNEL_STACK_SIZE;
  allocate_pages((unsigned int) init->stack, KERNEL_STACK_SIZE, 1, 0);
  init->state->gs = init->state->es = init->state->fs = init->state->ds = init->state->ss = 0x23;
  init->state->ebp = init->state->esp = init->state->useresp = (unsigned int) init->stack;
  init->state->cs = 0x18;
  init->state->eip = (unsigned int) entry;
  init->flags |= PF_ACTIVE;
}

pid_t fork(void) {
  pid_t new_pid;
  for(new_pid = 0; get_proc(new_pid) != 0; new_pid++) ;
  process_list *pl = plist;
  while(pl->next) {
    if(pl->p->id > new_pid) break;
    pl = pl->next;
  }
  // At this point, pl is the first process with a greater process ID
  // than newpid, if there is an open space, or the last process in
  // the list, if there isn't.
  process_list *new_pl_entry = malloc(sizeof(process_list));
  bzero(new_pl_entry, sizeof(process_list));
  if(pl->next) { // there was a hole. pl is the process ahead of us.
    pl = pl->prev; // we need to insert ourselves at pl->next
    new_pl_entry->next = pl->next;
    pl->next->prev = new_pl_entry;
    pl->prev->next = new_pl_entry;
    new_pl_entry->prev = pl;
  } else { // no hole. pl is the process behind us.
    pl->next = new_pl_entry;
    new_pl_entry->prev = pl;
  }
  // Regardless of what just happened, new_pl_entry is our process
  // list entry, inserted in the right spot.
  process *old_proc = get_proc(current_pid);
  process *new_proc = malloc(sizeof(process));
  memcpy(new_proc, old_proc, sizeof(process));
  // fork() must be called by system call. Therefore, we have an
  // up-to-date state, and interrupts are disabled.
  memcpy(new_proc->state, old_proc->state, sizeof(struct regs));
  // Now comes the hard part: remapping the page table. The problem is
  // that, at some point, we will have to remap the pages containing
  // kernel_stack. This will result in a loss of _all_ of our
  // stack-allocated variables, if we don't work carefully. What we'll
  // do here is page in FAKE_PAGE_BASE, copy our data to it, and
  // re-page so that the old page points to the physical location of
  // our new page, and then unpage FAKE_PAGE_BASE, being sure not to
  // modify the frameset.
  proc_page_list *old_pages, *new_pages;
  // First, go through and make sure everything's allocated. We'll
  // also set the new virtual addresses at this time.
  new_pages = malloc(sizeof(proc_page_list));
  new_proc->pages = new_pages;
  old_pages = old_proc->pages;
  new_pages->physical = 0;
  new_pages->virtual = old_pages->virtual;
  while(old_pages->next) {
    new_pages->next = malloc(sizeof(proc_page_list));
    new_pages->next->physical = 0;
    new_pages->next->virtual = old_pages->next->virtual;
    new_pages->next->next = 0;
    old_pages = old_pages->next;
  }
  // Now, we can actually remap the pages.
  new_pages = new_proc->pages;
  while(new_pages) {
    new_pages->physical = nonidentity_page(FAKE_PAGE_BASE, 1); // FAKE_PAGE_BASE is now paged in
    memcpy(FAKE_PAGE_BASE, new_pages->virtual, 0x1000);        // our data is copied
    mapped_page(new_pages->virtual, new_pages->physical, 1);   // and the new page is set.
  }
  // We're now using the page table of the child process. However,
  // we're stuck in a kind of weird limbo here as the parent process's
  // data is not being updated, which means anything we do here is
  // affecting the child process _only_. Unfortunately, this includes
  // important things such as returning from this function.

  // This is a problem.
}

void next_ctx(struct regs *r) {
  process_list *pl = plist;
  int new_pid = 0;
  // Assumptions: Process id 0 always exists. (it's init, so if it's
  // gone we have other problems). The process list is in sorted
  // order. (if it isn't, fork() is whacked)
  while(pl->next) {
    if(pl->p->id > current_pid && pl->p->proc_flags & PF_ACTIVE) new_pid = pl->p->id;
    pl = pl->next;
  }
  process *old = get_proc(current_pid);
  process *new = get_proc(new_pid);
  memcpy(old->state, r, sizeof(struct regs));
  memcpy(r, new->state, sizeof(struct regs));
  set_kernel_stack(new->kernel_stack);
}
