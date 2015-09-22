#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  pid_t p = fork();
  if(p) {
    printf("Hello, World!\n");
  } else {
    printf("wtf\n");
  }
  return 0;
}
