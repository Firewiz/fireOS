#include "../include/stdlib.h"
#include "../include/syscall.h"

void exit(int code) {
  sys_exit(code);
  while(1) ;
}
