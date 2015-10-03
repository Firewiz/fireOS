#include <wait.h>
#include <stdio.h>

pid_t wait(int *stat_loc) {
  printf("Stub function: wait()\n");
  while(1) ;
}

int waitid(idtype_t idtype, id_t id, siginfo_t *infop, int options) {
  printf("Stub function: waitid()\n");
  while(1) ;
}

pid_t waitpid(pid_t pid, int *stat_loc, int options) {
  printf("Stub function: waitpid()\n");
  while(1) ;
}
