#ifndef STDARG_H
#define STDARG_H

#ifndef _CROSS_INCLUDE

#define NEED_VA_LIST

#define va_start(ap, argN) __builtin_va_start(ap, argN)
#define va_copy(dest, src) __builtin_va_copy(dest, src)
#define va_arg(ap, type) __builtin_va_arg(ap, type)
#define va_end(ap) __builtin_va_end(ap)

#endif

#ifdef NEED_VA_LIST
typedef __builtin_va_list va_list;
#endif

#endif
