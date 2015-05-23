#ifndef MALLOC_H
#define MALLOC_H

struct malloc_header {
  unsigned int length; // next header is at . + length + sizeof(struct malloc_header)
  unsigned char flags;
  unsigned int magic;
};

#define FLAG_INUSE 0x01
#define FLAG_ALLOCATED 0x02
#define MALLOC_MAGIC 0xCAFEBABE

void *malloc(unsigned int);
void free(void *);

#endif
