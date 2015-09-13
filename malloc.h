#ifndef MALLOC_H
#define MALLOC_H

struct malloc_header {
  unsigned int magic;
  struct malloc_header *arena_head;
  unsigned short btype, buser;
  unsigned int length;
  struct malloc_header *next;
} __attribute__ ((packed));

#define TYPE_FREE 0xF4EE
#define TYPE_USED 0x25ED
#define TYPE_KRNL 0x5157
#define TYPE_USER 0x25E4

#define MALLOC_MAGIC 0xA110C8ED
  
void init_malloc(void);
void *malloc(unsigned int);
void *malloc_user(unsigned int, int);
void *malloc_ap(unsigned int, int, void *, int);
void free(void *);
#endif
