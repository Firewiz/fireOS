#ifndef UNISTD_H
#define UNISTD_H

#ifndef _CROSS_INCLUDE

#define _CROSS_INCLUDE
#define NEED_PID_T
#include <sys/types.h>
#undef NEED_PID_T
#undef _CROSS_INCLUDE

pid_t fork(void);

#endif

#endif
