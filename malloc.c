#include "malloc.h"

unsigned char *malloc_arena = (unsigned char *)0x80000;

void *malloc(unsigned int size) {
  struct malloc_header *headptr = (struct malloc_header *) malloc_arena;
  while(1) {
    if(!(headptr->flags & FLAG_ALLOCATED)) { /* end of arena */
      headptr->flags = FLAG_ALLOCATED | FLAG_INUSE;
      headptr->length = size;
      return headptr + 1;
    } else if(!(headptr->flags & FLAG_INUSE)) { /* possible fit */
      if(headptr->length >= size) {	 /* definite fit */
	// check to see if we need to alloc another header
	if((headptr->length - 1) > size) { /* ok to alloc a new header */
	  struct malloc_header *newhead = headptr + (size + sizeof(struct malloc_header)) / sizeof(struct malloc_header);
	  newhead->length = headptr->length - size - sizeof(struct malloc_header);
	  newhead->flags = FLAG_ALLOCATED;
	  headptr->length = size;
	}
	headptr->flags |= FLAG_INUSE;
	return headptr + 1;
      } else {
	headptr += headptr->length;
	headptr += 1;
      }
    } else {			/* next headptr */
      headptr += (headptr->length + sizeof(struct malloc_header)) / sizeof(struct malloc_header);
    }
  }
}

void free(void *p) {
  struct malloc_header *headptr = p - sizeof(struct malloc_header);
  struct malloc_header *nexthead = p + headptr->length;
  if(!(nexthead->flags & FLAG_ALLOCATED)) { /* this block is at the end of the arena */
    headptr->flags = 0;
  } else if(!(nexthead->flags & FLAG_INUSE)) { /* join free blocks */
    headptr->length += nexthead->length + sizeof(struct malloc_header);
    nexthead->flags = 0;
    headptr->flags &= ~(FLAG_INUSE);
  } else {			/* just free it */
    headptr->flags &= ~(FLAG_INUSE);
  }
}
