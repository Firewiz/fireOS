#ifndef STDIO_H
#define STDIO_H

#define _CROSS_INCLUDE
#define NEED_OFF_T
#define NEED_SSIZE_T
#include <sys/types.h>
#undef NEED_OFF_T
#undef NEED_SSIZE_T
#undef _CROSS_INCLUDE

#include <stddef.h>

#define _CROSS_INCLUDE
#define NEED_VA_LIST
#include <stdarg.h>
#undef NEED_SIZE_T
#undef _CROSS_INCLUDE

#define _CROSS_INCLUDE
#define NEED_MALLOC
#include <stdlib.h>
#undef NEED_MALLOC
#undef _CROSS_INCLUDE

typedef long int fpos_t;

#define BUFSIZ 4096

struct _file_t {
  int fd;
};

typedef struct _file_t FILE;


#define L_ctermid 32
#define L_tmpnam 32

#define _IOFBF 0
#define _IOLBF 1
#define _IONBF 2

#define SEEK_CUR 0
#define SEEK_END 1
#define SEEK_SET 2

#define FILENAME_MAX 256
#define FOPEN_MAX 8
#define TMP_MAX 25

#define EOF -1

#define P_tmpdir "/tmp/"

FILE *_stderr, *_stdin, *_stdout;

void _setup_stdio(void) {
  
}

// FIXME
#define stderr (_stderr)
#define stdin (_stdin)
#define stdout (_stdout)

int printf(const char *fmt, ...);
int puts(const char *str);
int putchar(int c);

#endif
