#include "malloc.h"
#include "paging.h"

unsigned char *malloc_arena = (unsigned char *)0x1800000;

void *malloc(unsigned int size) {
  return malloc_ap(size, 0, 0);
}


#ifndef USE_OLD_MALLOC

#define PHY_TO_PAGE(addr) ((int) (addr) >> 12)

void *alloc_new_header(struct malloc_header *headptr, unsigned int size) {
  unsigned int i;
  for(i = 0; i < size + sizeof(struct malloc_header); i += 0x1000) {
    if(!is_present(PHY_TO_PAGE((unsigned int) headptr + i)))
      nonidentity_page(PHY_TO_PAGE((unsigned int) headptr + i));
  }
  // Now that the block's paged in, we can write our header.
  headptr->length = size;
  headptr->flags = FLAG_INUSE | FLAG_ALLOCATED;
  headptr->magic = MALLOC_MAGIC;
  return headptr + 1;
}

void *malloc_ap(unsigned int size, int align, void *phy) {
  // Set our head to the start of the arena and prepare to traverse
  // it.
  struct malloc_header *headptr = (struct malloc_header *) malloc_arena;

  while(1) {
    // First, check if we need to page the current header.
    if(!is_present(PHY_TO_PAGE(headptr))) {
      nonidentity_page(PHY_TO_PAGE(headptr));
    }
    // Now that we're sure this memory exists, check if there's a
    // header there.
    if(headptr->magic != MALLOC_MAGIC
       || !(headptr->flags & FLAG_ALLOCATED)) {
      // We've reached the end of the arena. (or possibly a corrupt
      // header, but that's SEP)
      return alloc_new_header(headptr, size);
    } else {
      // This header exists.  If it's in use, we'll just go on to the
      // next one.
      if(headptr->flags & FLAG_INUSE) {
	// Typecast BS to force addition to work properly.
	headptr = (struct malloc_header *) (((unsigned char *) headptr) + headptr->length + sizeof(struct malloc_header));
	continue;
      }
      // Header is not in use, so let's see if it would fit our block.
      if(headptr->length <= size + sizeof(struct malloc_header) * 2) {
	// Looks like it doesn't. Move on.
	
	// Typecast BS to force addition to work properly.
	headptr = (struct malloc_header *) (((unsigned char *) headptr) + headptr->length + sizeof(struct malloc_header));
	continue;
      }
      // Header is unused and fits our block, so let's allocate it! Not worrying about splitting blocks just yet.
      return headptr + 1;
    }
  }
}

#else

void *malloc_ap(unsigned int size, int align, void *phy) {
  printf("Allocating a unit, size %x\n", size);
  struct malloc_header *headptr = (struct malloc_header *) malloc_arena;
  while(1) {
    if(!is_present((int) headptr / 0x1000)) {
      nonidentity_page((int) headptr / 0x1000);
    }
    if(!(headptr->flags & FLAG_ALLOCATED) || headptr->magic != MALLOC_MAGIC) { /* end of arena */
      headptr->magic = MALLOC_MAGIC;
      headptr->flags = FLAG_ALLOCATED | FLAG_INUSE;
      headptr->length = size;
      unsigned int i;
      for(i = 0; i < size; i+=4096) {
	if(!is_present(i + ((int) headptr / 0x1000))) {
	  nonidentity_page(i + ((int) headptr / 0x1000));
	}
      }
      printf("Allocated (new), headptr %x size %x\n", headptr, size);
      return headptr + 1;
    } else if(!(headptr->flags & FLAG_INUSE)) { /* possible fit */
      if(headptr->length >= size) {	 /* definite fit */
	// check to see if we need to alloc another header
	if((headptr->length - 1) > size) { /* ok to alloc a new header */
	  struct malloc_header *newhead = headptr + (size + sizeof(struct malloc_header)) / sizeof(struct malloc_header);
	  newhead->length = headptr->length - size - sizeof(struct malloc_header);
	  newhead->flags = FLAG_ALLOCATED;
	  newhead->magic = MALLOC_MAGIC;
	  headptr->length = size;
	}
	headptr->flags |= FLAG_INUSE;
	unsigned int i;
	for(i = 0; i < size; i+=4096) {
	  if(!is_present(i + ((int) headptr / 0x1000))) {
	    nonidentity_page(i + ((int) headptr / 0x1000));
	  }
	}	
	printf("Allocated, headptr %x size %x\n", headptr, size);
	return headptr + 1;
      } else {
	headptr += headptr->length / sizeof(struct malloc_header);
	headptr += 1;
      }
    } else {			/* next headptr */
      headptr += headptr->length / sizeof(struct malloc_header);
      headptr += 1;
    }
  }
}
#endif

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
