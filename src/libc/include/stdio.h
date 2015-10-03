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

typedef long int fpos_t;

#define BUFSIZ 4096

struct _file_t {
  int fd;
  char *wbuffer, *rbuffer;
  int wbufp, rbufp;
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

#ifndef _DEFS
extern FILE *_stderr, *_stdin, *_stdout;
#endif

void _setup_stdio(void);

#define stderr (_stderr)
#define stdin (_stdin)
#define stdout (_stdout)

void *clearerr(FILE *);
char *ctermid(char *);
int dprintf(int, const char *, ...);
int fclose(FILE *);
FILE *fdopen(int, const char *);
int feof(FILE *);
int ferror(FILE *);
int fflush(FILE *);
int fgetc(FILE *);
int fgetpos(FILE *, fpos_t *);
char *fgets(char *, int, FILE *);
int fileno(FILE *);
void flockfile(FILE *);
FILE *fmemopen(void *, size_t, const char *);
FILE *fopen(const char *, const char *);
int fprintf(FILE *, const char *, ...);
int fputc(int, FILE *);
int fputs(const char *, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
FILE *freopen(const char *, const char *, FILE *);
int fscanf(FILE *, const char *, ...);
int fseek(FILE *, long, int);
int fseeko(FILE *, off_t, int);
int fsetpos(FILE *, const fpos_t *);
long ftell(FILE *);
off_t ftello(FILE *);
int ftrylockfile(FILE *);
void funlockfile(FILE *);
size_t fwrite(const void *, size_t, size_t, FILE *);
int getc(FILE *);
int getc_unlocked(FILE *);
int getchar_unlocked(void);
ssize_t getdelim(char **, size_t *, int, FILE *);
char *gets(char *);
FILE *open_memstream(char **, size_t *);
int pclose(FILE *);
void perror(const char *);
FILE *popen(const char *, const char *);
int printf(const char *, ...);
int putc(int, FILE *);
#define putchar(c) fputc((c), stdout)
int putc_unlocked(int, FILE *);
int putchar_unlocked(int);
int puts(const char *str);
int remove(const char *);
int rename(const char *, const char *);
void rewind(FILE *);
int scanf(const char *, ...);
void setbuf(FILE *, char *);
int setvbuf(FILE *, char *, int, size_t);
int snprintf(char *, size_t, const char *, ...);
int sprintf(char *, const char *, ...);
int sscanf(const char *, const char *, ...);
FILE *tmpfile(void);
int ungetc(int, FILE *);
int vdprintf(int, const char *, va_list);
int vfprintf(FILE *, const char *, va_list);
int vfscanf(FILE *, const char *, va_list);
int vprintf(const char *, va_list);
int vscanf(const char *, va_list);
int vsnprintf(char *, size_t, const char *, va_list);
int vsprintf(char *, const char *, va_list);
int vsscanf(const char *, const char *, va_list);

#endif
