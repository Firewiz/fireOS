#ifndef MALLOC_H
#define MALLOC_H

struct malloc_header {
  unsigned int length; // next header is at . + length + sizeof(struct malloc_header)
  unsigned char flags;
};

#define FLAG_INUSE 0x01
#define FLAG_ALLOCATED 0x02
void *malloc(unsigned int);
void free(void *);

#endif
