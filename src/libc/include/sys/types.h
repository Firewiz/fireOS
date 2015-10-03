#ifndef SYS_TYPES_H
#define SYS_TYPES_H

#ifndef _CROSS_INCLUDE

typedef long blkcnt_t;
typedef long blksize_t;
#define NEED_CLOCK_T
#define NEED_CLOCKID_T
typedef int dev_t;
typedef long fsblkcnt_t;
typedef long fsfilcnt_t;
typedef int gid_t;
#define NEED_ID_T
typedef long ino_t;
typedef int key_t;
typedef int mode_t;
typedef long nlink_t;
#define NEED_OFF_T
#define NEED_PID_T
#define NEED_PTHREAD_ATTR_T
typedef int pthread_barrier_t;
typedef int pthread_barrierattr_t;
typedef int pthread_cond_t;
typedef int pthread_condattr_t;
typedef int pthread_key_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_once_t;
typedef int pthread_rwlock_t;
typedef int pthread_rwlockattr_t;
typedef int pthread_spinlock_t;
#define NEED_PTHREAD_T
#define NEED_SIZE_T
#define NEED_SSIZE_T
typedef long suseconds_t;
#define NEED_TIME_T
#define NEED_TIMER_T
typedef int trace_attr_t;
typedef int trace_event_id_t;
typedef int trace_event_set_t;
typedef int trace_id_t;
#define NEED_UID_T

#endif

#endif

#if defined NEED_ID_T && !(defined HAVE_ID_T)
#define HAVE_ID_T
typedef int id_t;
#endif
#if defined NEED_SIZE_T && !(defined HAVE_SIZE_T)
#define HAVE_SIZE_T
typedef unsigned long size_t;
#endif
#if defined NEED_OFF_T && !(defined HAVE_OFF_T)
#define HAVE_OFF_T
typedef long off_t;
#endif
#if defined NEED_SSIZE_T && !(defined HAVE_SSIZE_T)
#define HAVE_SSIZE_T
typedef long ssize_t;
#endif
#if defined NEED_UID_T && !(defined HAVE_UID_T)
#define HAVE_UID_T
typedef int uid_t;
#endif
#if defined NEED_PTHREAD_T && !(defined HAVE_PTHREAD_T)
#define HAVE_PTHREAD_T
typedef int pthread_t;
#endif
#if defined NEED_TIME_T && !(defined HAVE_TIME_T)
#define HAVE_TIME_T
typedef long time_t;
#endif
#if defined NEED_CLOCK_T && !(defined HAVE_CLOCK_T)
#define HAVE_CLOCK_T
typedef long clock_t;
#endif
#if defined NEED_CLOCKID_T && !(defined HAVE_CLOCKID_T)
#define HAVE_CLOCKID_T
typedef int clockid_t;
#endif
#if defined NEED_TIMER_T && !(defined HAVE_TIMER_T)
#define HAVE_TIMER_T
typedef int timer_t;
#endif
#if defined NEED_PID_T && !(defined HAVE_PID_T)
#define HAVE_PID_T
typedef unsigned short pid_t;
#endif
#if defined NEED_PTHREAD_ATTR_T && !(defined HAVE_PTHREAD_ATTR_T)
#define HAVE_PTHREAD_ATTR_T
typedef int pthread_attr_t;
#endif
