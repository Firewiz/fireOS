#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  if(fork()) {
    printf("Hello, World!\n");
  } else {
    printf("Task 2!\n");
  }
}
