#ifndef SIGNAL_H
#define SIGNAL_H

#ifndef _CROSS_INCLUDE

#define SIG_DFL ((void (*)(int)) -1)
#define SIG_ERR ((void (*)(int)) -2)
#define SIG_HOLD ((void (*)(int)) -3)
#define SIG_IGN ((void (*)(int)) -4)

#define _CROSS_INCLUDE
#define NEED_PTHREAD_T
#define NEED_SIZE_T
#define NEED_UID_T
#define NEED_PID_T
#define NEED_PTHREAD_ATTR_T
#include <sys/types.h>
#undef NEED_PTHREAD_ATTR_T
#undef NEED_PID_T
#undef NEED_UID_T
#undef NEED_SIZE_T
#undef NEED_PTHREAD_T
#undef _CROSS_INCLUDE

#include <time.h>

typedef volatile int sig_atomic_t;
typedef int sigset_t;

union sigval {
  int sival_int;
  void *sival_ptr;
};

struct _siginfo_t {
  int si_signo;
  int si_code;
  pid_t si_pid;
  uid_t si_uid;
  void *si_addr;
  int si_status;
  union sigval si_value;
};

typedef struct _siginfo_t siginfo_t;

struct sigevent {
  int sigev_notify;
  int sigev_signo;
  union sigval sigev_value;
  void (*sigev_notify_function)(union sigval);
  pthread_attr_t *sigev_notify_attributes;
};

#define SIGEV_NONE 0
#define SIGEV_SIGNAL 1
#define SIGEV_THREAD 2

#define SIGRTMIN 1
#define SIGRTMAX 32

#define RTSIG_MAX 31

#define SIGABRT 33
#define SIGFPE 34
#define SIGILL 35
#define SIGINT 36
#define SIGSEGV 37
#define SIGTERM 38

#define SIGALRM 39
#define SIGBUS 40
#define SIGCHLD 41
#define SIGCONT 42
#define SIGHUP 43
#define SIGKILL 44
#define SIGPIPE 45
#define SIGQUIT 46
#define SIGSTOP 47
#define SIGTSTP 48
#define SIGTTIN 49
#define SIGTTOU 50
#define SIGUSR1 51
#define SIGUSR2 52
#define SIGTRAP 53
#define SIGURG 54
#define SIGXCPU 55
#define SIGXFSZ 56

struct sigaction {
  void (*sa_handler)(int);
  sigset_t sa_mask;
  int sa_flags;
  void (*sa_sigaction)(int, siginfo_t *, void *);
};

#define SIG_BLOCK 0
#define SIG_UNBLOCK 1
#define SIG_SETMASK 2

#define SIG_NOCLDSTOP 0
#define SA_ONSTACK 1
#define SA_RESETHAND 2
#define SA_RESTART 3
#define SA_SIGINFO 4
#define SA_NOCLDWAIT 5
#define SA_NODEFER 6
#define SS_ONSTACK 7
#define SS_DISABLE 8

#define MINSIGSTKSZ 1024
#define SIGSTKSZ 1024

typedef int mcontext_t;

struct _stack_t {
  void *ss_sp;
  size_t ss_size;
  int ss_flags;
};

typedef struct _stack_t stack_t;

struct _ucontext_t {
  struct _ucontext_t *uc_link;
  sigset_t uc_sigmask;
  stack_t uc_stack;
  mcontext_t uc_mcontext;
};

typedef struct _ucontext_t ucontext_t;

#define ILL_ILLOPC 0
#define ILL_ILLOPN 1
#define ILL_ILLADR 2
#define ILL_ILLTRP 3
#define ILL_PRVOPC 4
#define ILL_PRVREG 5
#define ILL_COPROC 6
#define ILL_BADSTK 7
#define FPE_INTDIV 0
#define FPE_INTOVF 1
#define FPE_FLTDIV 2
#define FPE_FLTOVF 3
#define FPE_FLTUND 4
#define FPE_FLTRES 5
#define FPE_FLTINV 6
#define FPE_FLTSUB 7
#define SEGV_MAPERR 0
#define SEGV_ACCERR 1
#define CLD_EXITED 0
#define CLD_KILLED 1
#define CLD_DUMPED 2
#define CLD_TRAPPED 3
#define CLD_STOPPED 4
#define CLD_CONTINUED 5
#define SI_USER 8
#define SI_QUEUE 9
#define SI_TIMER 10
#define SI_ASYNCIO 11
#define SI_MESGQ 12

int kill(pid_t, int);
void psiginfo(const siginfo_t *, const char *);
void psignal(int, const char *);
int pthread_kill(pthread_t, int);
int pthread_sigmask(int, const sigset_t *, sigset_t *);
int raise(int);
int sigaction(int, const struct sigaction *, struct sigaction *);
int sigaddset(sigset_t *, int);
int sigdelset(sigset_t *, int);
int sigemptyset(sigset_t *);
int sigfillset(sigset_t *);
int sigismember(const sigset_t *, int);
void (*signal(int, void (*)(int)))(int);
int sigpending(sigset_t *);
int sigprocmask(int, const sigset_t *, sigset_t *);
int sigqueue(pid_t, int, const union sigval);
int sigsuspend(const sigset_t *);
int sigtimedwait(const sigset_t *, siginfo_t *, const struct timespec *);
int sigwait(const sigset_t *, int *);
int sigwaitinfo(const sigset_t *, siginfo_t *);

#endif

#endif
