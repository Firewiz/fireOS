#ifndef PROCESS_H
#define PROCESS_H

typedef unsigned short pid_t;

#include "idt.h"

struct proc_page_list_t {
  struct proc_page_list_t *next;
  unsigned int physical, virtual;
};

typedef struct proc_page_list_t proc_page_list;

#define PROCESS_STACK_SIZE 0x1000

struct process_t {
  pid_t id;
  struct regs *state;
  proc_page_list *pages;
  unsigned char stack[PROCESS_STACK_SIZE];
};

typedef struct process_t process;

struct process_list_t {
  struct process_list_t *next, *prev;
  process *p;
};

typedef struct process_list_t process_list;

#ifndef DEFS
extern process_list *plist;
extern volatile pid_t current_pid;
#endif

process *get_proc(pid_t id);
void allocate_pages(unsigned int base, unsigned int offset, int user, pid_t owner);
void init_mt();
pid_t create_process();

#endif
