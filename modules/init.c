#include <stdio.h>
#include <stdlib.h>

int main(void) {
  printf("$ ");
  char buf[32];
  sys_read(stdin->fd, buf, 32);
}
