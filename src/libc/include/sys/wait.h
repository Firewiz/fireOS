#ifndef SYS_WAIT_H
#define SYS_WAIT_H

#ifndef _CROSS_INCLUDE

#define WCONTINUED 0
#define WNOHANG 1
#define WUNTRACED 2


#define NEED_STDLIB_CONSTANTS

#define WEXITED 0
#define WNOWAIT 1
#define WSTOPPED 2

typedef enum {
  P_ALL,
  P_PGID,
  P_PID
} idtype_t;

#define _CROSS_INCLUDE
#define NEED_ID_T
#define NEED_PID_T
#include <sys/types.h>
#undef NEED_ID_T
#undef NEED_PID_T
#undef _CROSS_INCLUDE

#include <signal.h>

pid_t wait(int *);
pid_t waitpid(pid_t, int *, int);
int waitid(idtype_t, id_t, siginfo_t *, int);

#endif

#ifdef NEED_STDLIB_CONSTANTS
#define WEXITSTATUS 0
#define WIFCONTINUED 1
#define WIFEXITED 2
#define WIFSIGNALED 3
#define WIFSTOPPED 4
#define WSTOPSIG 5
#define WTERMSIG 6
#endif

#endif
