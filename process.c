#define DEFS
#include "process.h"
#undef DEFS

#include "malloc.h"
#include "paging.h"
#include "stdlib.h"
#include "static.h"
#include "printf.h"

// plist is sorted in ascending pid order
process_list *plist;
volatile pid_t current_pid = 0;
int mt_enabled = 0;

process *get_proc(pid_t id) {
  if(plist == 0) return 0;
  process_list *p;
  for(p = plist; p; p = p->next) {
    if(p->p->id == id) return p->p;
  }
  return 0;
}

static int ap_recurse_count = 0;

void allocate_pages(unsigned int base, unsigned int offset, int user, pid_t owner) {
  //  printf("AP (%d) %x %x %d %d\n", ap_recurse_count++, base, offset, user, owner);
  if(base < 0x8000 && user) for(;;);
  unsigned int i;
  unsigned int phy;
  process *proc = get_proc(owner);
  proc_page_list *pl = proc->pages;
  for(i = 0; i < offset; i += 0x1000) {
    if(!is_present((i + base) / 0x1000)) {
      //      printf("paging in %x\n", (i + base) / 0x1000);
      phy = nonidentity_page((i + base) / 0x1000, user);
      //      printf("paged\n");
      // only alter the page list if these were user pages
      if(proc != 0) {
	if(user) {
	  printf("Updating page list\n");
	  if(proc->pages == 0) {
	    proc->pages = malloc(sizeof(proc_page_list));
	    pl = proc->pages;
	  } else {
	    while(pl->next) pl = pl->next;
	    pl->next = malloc(sizeof(proc_page_list));
	    pl = pl->next;
	  }
	  asm volatile ("nop");
	  pl->physical = phy;
	  pl->virtual = (i + base) / 0x1000;
	  pl->next = 0;
	}
      }
    }
  }
  if(!is_present((i + base + offset) / 0x1000))
    phy = nonidentity_page((i + base + offset) / 0x1000, user);
  //  printf("AP (%d) done\n", --ap_recurse_count);
}

void int_83(int no, struct regs *r) {
  mt_enabled = 1;
  next_ctx(r, 0);
}

void init_mt() {
  printf("In init_mt().\n");
  install_handler(int_83, 0x83);
  plist = malloc(sizeof(process_list));
  printf("Allocated process list\n");
  current_pid = 0;
  plist->next = plist->prev = 0;
  process *init = malloc(sizeof(process));
  printf("Allocated init.\n");
  plist->p = init;
  init->id=0;
  init->state = malloc(sizeof(struct regs));
  bzero(init->state, sizeof(struct regs));
  printf("Allocated state buffer.\n");
  init->pages = 0;
  init->fds = 0;
  init->stack = (unsigned char *) USER_STACK + PROCESS_STACK_SIZE;
  printf("Allocating usermode stack...\n");
  allocate_pages((unsigned int) USER_STACK, PROCESS_STACK_SIZE, 1, 0);
  printf("Allocated usermode stack.\n");
  init->kernel_stack = (unsigned char *) KERNEL_STACK + KERNEL_STACK_SIZE;
  allocate_pages((unsigned int) KERNEL_STACK, KERNEL_STACK_SIZE, 0, 0);
  init->state->gs = init->state->es = init->state->fs = init->state->ds = init->state->ss = 0x23;
  init->state->ebp = init->state->esp = init->state->useresp = (unsigned int) init->stack;
  init->state->cs = 0x1b;
}

void run_init(void (*entry)()) {
  process *init = get_proc(0);
  init->state->eip = (unsigned int) entry;
  init->flags |= PF_ACTIVE;
  printf("INIT active!\n");
  asm volatile("int $0x83");
}

void yield() {
  process_list *pl = plist;
  while(pl->next) {
    if(pl->p->id == current_pid) break;
    pl = pl->next;
  }
  if(pl->next) pl->next->prev = pl->prev;
  if(pl->prev) pl->prev->next = pl->next;
  pl->p->flags &= ~PF_ACTIVE;
  proc_fd_list *fdl = pl->p->fds, *fdt;
  while(fdl) {
    fdt = fdl->next;
    free(fdl);
    fdl = fdt;
  }
  proc_page_list *pgl = pl->p->pages, *pgt;
  while(pgl) {
    pgt = pgl->next;
    unmap_page(pgl->virtual);
    free(pgl);
    pgl = pgt;
  }
  free(pl->p->state);
  free(pl->p);
  printf("Process %d ended\n", current_pid);
}

pid_t fork(void) {
  printf("Fork!\n");
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
  new_pl_entry->p = new_proc;
  // Copy over the file descriptor list
  proc_fd_list *ofd = old_proc->fds;
  if(ofd) {
    proc_fd_list *nfd = malloc(sizeof(proc_fd_list));
    new_proc->fds = nfd;
    while(ofd) {
      nfd->fd = ofd->fd;
      nfd->fd_flags = ofd->fd_flags;
      if(ofd->next) nfd->next = malloc(sizeof(proc_fd_list));
      else nfd->next = 0;
      ofd = ofd->next;
      nfd = nfd->next;
    }
  } else {
    new_proc->fds = 0;
  }
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
    new_pages = new_pages->next;
  }
  // Now, we can actually remap the pages.
  new_pages = new_proc->pages;
  while(new_pages) {
    new_pages->physical = nonidentity_page(FAKE_PAGE_BASE / 0x1000, 1); // FAKE_PAGE_BASE is now paged in
    memcpy((void *) FAKE_PAGE_BASE, (void *) (new_pages->virtual * 0x1000), 0x1000); // our data is copied
    mapped_page(new_pages->virtual, new_pages->physical, 1);   // and the new page is set.
    //    printf("Paged in VM %x, new_proc = %x\n", new_pages->virtual, new_proc);
    new_pages = new_pages->next;
  }
  // We're now using the page table of the child process. However,
  // we're stuck in a kind of weird limbo here as the parent process's
  // data is not being updated, which means anything we do here is
  // affecting the child
  // process _only_. Unfortunately, this includes
  // important things such as returning from this function.

  // This is not a problem, because fork() runs on a separate
  // stack. We just need to modify the new process's state to set our
  // return value.
  new_proc->state->eax = 0;
  old_proc->state->eax = new_pid;
  new_proc->id = new_pid;
  new_proc->flags |= PF_ACTIVE;
  printf("Split\n");
  return 0;
}

void next_ctx(struct regs *r, int save_state) {
  //printf("NCTX!\n");
  process_list *pl = plist;
  int new_pid = -1;
  // Assumptions: Process id 0 always exists. (it's init, so if it's
  // gone we have other problems). The process list is in sorted
  // order. (if it isn't, fork() is whacked)
  while(pl) {
    if(pl->p->id > current_pid && pl->p->flags & PF_ACTIVE) {
      new_pid = pl->p->id;
      break;
    }
    pl = pl->next;
  }
  if(pl == 0) {
    pl = plist;
    while(pl) {
      if(pl->p->flags & PF_ACTIVE) {
	new_pid = pl->p->id;
	break;
      }
      pl = pl->next;
    }
  }
  if(new_pid == -1) return; // There are no processes
  process *nproc = get_proc(new_pid);
  pl = plist;
  proc_page_list *pages = nproc->pages;
  while(pages) {
    mapped_page(pages->virtual, pages->physical, 1);
    pages = pages->next;
  }
  if(save_state) {
    process *old = get_proc(current_pid);
    memcpy(old->state, r, sizeof(struct regs));
  }
  memcpy(r, nproc->state, sizeof(struct regs));
  //  printf("Loaded process %d (%x)\n", new_pid, r->eip);
  set_kernel_stack((unsigned int) nproc->kernel_stack);
  current_pid = new_pid;
}
