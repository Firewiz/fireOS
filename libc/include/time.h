#ifndef TIME_H
#define TIME_H

#ifndef _CROSS_INCLUDE
#define NEED_TIMESPEC
#endif

#ifdef NEED_TIMESPEC
#define _CROSS_INCLUDE
#define NEED_TIME_T
#include <sys/types.h>
#undef NEED_TIME_T
#undef _CROSS_INCLUDE

struct timespec {
  time_t tv_sec;
  long tv_nsec;
};
#endif

#ifndef _CROSS_INCLUDE

#define _CROSS_INCLUDE
#define NEED_CLOCK_T
#define NEED_SIZE_T
#define NEED_TIME_T
#define NEED_CLOCKID_T
#define NEED_TIMER_T
#define NEED_PID_T
#include <sys/types.h>
#undef NEED_PID_T
#undef NEED_TIMER_T
#undef NEED_CLOCKID_T
#undef NEED_TIME_T
#undef NEED_SIZE_T
#undef NEED_CLOCK_T
#undef _CROSS_INCLUDE

#define _CROSS_INCLUDE
#define NEED_LOCALE_T
#include <locale.h>
#undef NEED_LOCALE_T
#undef _CROSS_INCLUDE

struct tm {
  int tm_sec;
  int tm_min;
  int tm_hour;
  int tm_mday;
  int tm_mon;
  int tm_year;
  int tm_wday;
  int tm_yday;
  int tm_isdst;
};

struct itimerspec {
  struct timespec it_interval;
  struct timespec it_value;
};

#define _CROSS_INCLUDE
#define NEED_NULL
#include <stddef.h>
#undef NEED_NULL
#undef _CROSS_INCLUDE

#define CLOCKS_PER_SEC 100

#define CLOCK_MONOTONIC 0
#define CLOCK_PROCESS_CPUTIME_ID 1
#define CLOCK_REALTIME 2
#define CLOCK_THREAD_CPUTIME_ID 3

#define TIMER_ABSTIME 1

char *asctime(const struct tm *);
char *asctime_r(const struct tm *, char *);
clock_t clock(void);
int clock_getcpuclockid(pid_t, clockid_t *);
int clock_getres(clockid_t, struct timespec *);
int clock_gettime(clockid_t, struct timespec *);
int clock_nanosleep(clockid_t, int, const struct timespec *, struct timespec *);
int clock_settime(clockid_t, const struct timespec *);
char *ctime(const time_t *);
char *ctime_r(const time_t *, char *);
double difftime(time_t, time_t);
struct tm *gmtime(const time_t *);
struct tm *gmtime_r(const time_t *, struct tm *);
struct tm *localtime(const time_t *);
struct tm *localtime_r(const time_t *, struct tm *);
time_t mktime(struct tm *);
int nanosleep(const struct timespec *, struct timespec *);
size_t strftime(char *, size_t, const char *, const struct tm *);
size_t strftime_l(char *, size_t, const char *, const struct tm *, locale_t);
time_t time(time_t *);
int timer_delete(timer_t);
int timer_getoverrun(timer_t);
int timer_gettime(timer_t, struct itimerspec *);
int timer_settime(timer_t, int, const struct itimerspec *, struct itimerspec *);
void tzset(void);

extern char *tzname[];

#include <signal.h>
int timer_create(clockid_t, struct sigevent *, timer_t *);

#endif

#endif
