#ifndef STDDEF_H
#define STDDEF_H

#ifndef _CROSS_INCLUDE

#define NEED_NULL
#define offsetof(type, des) __builtin_offsetof(type, des)

typedef signed long ptrdiff_t;
#define NEED_WCHAR_T

#endif

#ifdef NEED_NULL
#define NULL ((void *) 0)
#endif

#ifdef NEED_WCHAR_T
typedef unsigned long wchar_t;
#endif

#define _CROSS_INCLUDE
#define NEED_SIZE_T
#include <sys/types.h>
#undef NEED_SIZE_T
#undef _CROSS_INCLUDE

#endif
