#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  pid_t p = fork();
  printf("Hello, World!\n");
  return 0;
}
