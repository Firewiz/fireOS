#include <stdlib.h>
#include <syscall.h>

void exit(int code) {
  sys_exit(code);
  while(1) ;
}

void *malloc(size_t sz) {
  return sys_allocbuf(sz);
}

void free(void *buf) {
  sys_freebuf(buf);
}
