#include <stdio.h>
#include <syscall.h>

int main(void) {
  char buf[32];
  int count = sys_read(stdin->fd, buf, 32);
  sys_write(stdout->fd, buf, count);
}
