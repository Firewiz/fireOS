#ifndef MATH_H
#define MATH_H

#ifndef _CROSS_INCLUDE

#if FLT_EVAL_METHOD == 1
typedef double float_t;
typedef double double_t;
#elif FLT_EVAL_METHOD == 2
typedef long double float_t;
typedef long double double_t;
#else
typedef float float_t;
typedef double double_t;
#endif

#endif

#endif
