#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  sys_write(0, "Hello, World!", 13);
  return 0;
}
