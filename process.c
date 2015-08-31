#define DEFS
#include "process.h"
#endif

#include "malloc.h"
#include "paging.h"

// plist is sorted in ascending pid order
process_list *plist;
volatile pid_t current_pid;

process *get_proc(pid_t id) {
  if(plist == 0) return 0;
  process_list p;
  for(p = plist; p->next; p = p->next) {
    if(p->p->id == id) return p->p;
  }
  return 0;
}

void allocate_pages(unsigned int base, unsigned int offset, int user, pid_t owner) {
  int i;
  unsigned int phy;
  process proc = get_proc(owner);
  proc_page_list *pl = proc->pages;
  for(i = 0; i < offset; i += 0x1000) {
    if(!is_present((i + base) / 0x1000)) {
      phy = nonidentity_page(i + base, user);
      if(proc->pages == 0) {
	proc->pages = malloc(sizeof(proc_page_list));
	pl = proc->pages;
      } else {
	while(pl->next) pl = pl->next;
	pl->next = malloc(sizeof(proc_page_list));
	pl = pl->next;
      }
      pl->physical = phy;
      pl->virtual = (i + base) / 0x1000;
      pl->next = 0;
    }
  }
}

void init_mt() {
  plist = 0;
  current_pid = 0;
}

pid_t create_process() {
  pid_t newpid = 0;
  while(get_proc(newpid)) newpid++;
  process *new_proc = malloc(sizeof(process));
  new_proc->state = malloc(sizeof(struct regs));
  new_proc->pages = 0;
  new_proc->stack = USER_STACK;
}
