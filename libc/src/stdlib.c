#include <stdlib.h>
#include <syscall.h>

void exit(int code) {
  sys_exit(code);
  while(1) ;
}
