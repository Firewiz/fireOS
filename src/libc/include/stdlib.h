#ifndef STDLIB_H
#define STDLIB_H

#ifndef _CROSS_INCLUDE

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0
#define RAND_MAX 32767
#define MB_CUR_MAX ((size_t) 4)

#include <stddef.h>
#include <limits.h>
#include <math.h>

struct _div {
  // TODO
};

struct _ldiv {
  // TODO
};

struct _lldiv {
  //TODO
};

typedef struct _div div_t;
typedef struct _ldiv ldiv_t;
typedef struct _lldiv lldiv_t;

#include <sys/wait.h>

void _Exit(int);
void abort(void);
int abs(int);
int atexit(void (*)(void));
double atof(const char *);
int atoi(const char *);
long atol(const char *);
long long atoll(const char *);
void *bsearch(const void *, const void *, size_t, size_t, int (*)(const void *, const void *));
void *calloc(size_t, size_t);
div_t div(int, int);
void exit(int);
void free(void *);
char *getenv(const char *);
int getsubopt(char **, char *const *, char **);
long labs(long);
ldiv_t ldiv(long, long);
long long llabs(long long);
lldiv_t lldiv(long long, long, long);
void *malloc(size_t);
int mblen(const char *, size_t);
size_t mbstowcs(wchar_t *, const char *, size_t);
int mbtowc(wchar_t *, const char *, size_t);
char *mkdtemp(char *);
int mkstemp(char *);
void qsort(void *, size_t, size_t, int (*)(const void *, const void *));
int rand(void);
int rand_r(unsigned *);
void *realloc(void *, size_t);
int setenv(const char *, const char *, int);
void srand(unsigned);
double strtod(const char *, const char **);
float strtof(const char *, const char **);
long strtol(const char *, const char **);
long double strtold(const char *, const char **);
long long strtoll(const char *, const char **);
unsigned long strtoul(const char *, const char **);
unsigned long long strtoull(const char *, const char **);
int system(const char *);
int unsetenv(const char *);
size_t wcstombs(char *, const wchar_t *, size_t);
int wctomb(char *, wchar_t);

#endif

#endif
