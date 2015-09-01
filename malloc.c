#include "malloc.h"
#include "paging.h"
#include "printf.h"
#include "stdlib.h"
#include "config.h"
#include "static.h"
#include "process.h"

#ifdef DEBUG_MALLOC
#define printd(f, ...) printf(f, __VA_ARGS__)
#else
#define printd(f, ...)
#endif

unsigned char *malloc_arena_kernel = (unsigned char *)0x2000000;
unsigned char *malloc_arena_user =   (unsigned char *)0x80000000;

#define VADDR_TO_PAGE(a) (((unsigned int) (a)) / 0x1000)

void init_malloc(void) {
  nonidentity_page(VADDR_TO_PAGE(malloc_arena_kernel), 0);
  nonidentity_page(VADDR_TO_PAGE(malloc_arena_user), 1);
}

void *malloc(unsigned int size) {
  return malloc_ap(size, 0, 0, 0);
}

void *malloc_user(unsigned int size, int user) {
  return malloc_ap(size, 0, 0, user);
}

struct malloc_header *next_block(struct malloc_header *block) {
  // if this block is invalid, return 0
  if(block->magic != MALLOC_MAGIC) return 0;
  // else return next
  return block->next;
}

int get_ptr_distance(struct malloc_header *a, struct malloc_header *b) {
  return (int) (((void *) a) - ((void *) b));
}

void *alloc_old_block(struct malloc_header *block, unsigned int size) {
  block->length = size;
  block->btype = TYPE_USED;
  return block + 1;
}

#define PTR_ADD(p, q) ((int) ((void *) (p)) + ((void *) (q)))

void *alloc_new_block(struct malloc_header *arena_head, unsigned int size, int user) {
  struct malloc_header *p, *q = 0;
  p = arena_head;
  while((p = next_block(p)) > 0) {
    q = p;
  }
  // at this point, p = 0 and q = the last block, or 0 if there aren't
  // any blocks.
  if(q == 0 && arena_head->magic != MALLOC_MAGIC) { // will be the first block; allocate first header
    p = arena_head;
  } else {
    if(q == 0) // second block, q not set properly
      q = arena_head;
    p = PTR_ADD(PTR_ADD(q, sizeof(struct malloc_header)), q->length);
    q->next = p;
  }
  // at this point, p is the start of *our* header. if necessary, the
  // previous blocks' next pointer has been set.
  allocate_pages((unsigned int) p, size + sizeof(struct malloc_header), user, current_pid);
  p->magic = MALLOC_MAGIC;
  p->arena_head = arena_head;
  p->btype = TYPE_USED;
  if(user) p->buser = TYPE_USER;
  else p->buser = TYPE_KRNL;
  p->length = size;
  p->next = 0;
  return p + 1;
}
  

void *malloc_ap(unsigned int size, int align, void *phy, int user) {
  struct malloc_header *arena_head;
  if(user) arena_head = (struct malloc_header *) malloc_arena_user;
  else arena_head = (struct malloc_header *) malloc_arena_kernel;
  struct malloc_header *p = arena_head;
  while((p = next_block(p)) > 1) {
    if(p->btype == TYPE_FREE) {
      if(get_ptr_distance(p->next, p) > size) {
	return alloc_old_block(p, size);
      }
    }
  }
  return alloc_new_block(arena_head, size, user);
}

void free(void *p) {
  struct malloc_header *head = ((struct malloc_header *) p) - 1;
  if(!p) return;
  if(head->magic != MALLOC_MAGIC) {
    printf("Double free or corruption of %x\n", p);
    return;
  }
  // join with next block if possible
  if(head->next && head->next->btype == TYPE_FREE)
    head->next = head->next->next;
  head->btype = TYPE_FREE;
  head->length = 0;
}
