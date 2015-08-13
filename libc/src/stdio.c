#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/syscall.h"
#include <stdlib.h>

FILE *_new_file(int r, int w) {
  FILE *new = malloc(sizeof(FILE));
  new->wbufp = new->rbufp = 0;
  if(w) new->wbuffer = malloc(BUFSIZ);
  else new->wbuffer = 0;
  if(r) new->rbuffer = malloc(BUFSIZ);
  else new->rbuffer = 0;
  new->fd = sys_getfd();
  sys_clearfdflag(new->fd, FD_ALL);
  if(w)
    sys_setfdflag(new->fd, FD_WRITEABLE);
  if(r)
    sys_setfdflag(new->fd, FD_READABLE);
  return new;
}

void _setup_stdio(void) {
  _stdin = _new_file(1, 0);
  _stdout = _new_file(0, 1);
  _stderr = _new_file(0, 1);
  sys_setfdflag(_stdin->fd, FD_TERMINAL);
  sys_setfdflag(_stdout->fd, FD_TERMINAL);
  sys_setfdflag(_stderr->fd, FD_TERMINAL);
}

int _flush_writebuf(FILE *f) {
  int r = sys_write(f->fd, f->wbuffer, f->wbufp);
  f->wbufp = 0;
  return r;
}

int fputc(int c, FILE *f) {
  if(f->wbufp < BUFSIZ) {
    f->wbuffer[f->wbufp++] = c;
    if(c == '\n') _flush_writebuf(f);
    return 1;
  }
  else return 0;
}

int fputs(const char *str, FILE *f) {
  int len = strlen(str);
  int i, total = 0;
  for(i = 0; i < len; i++)
    total += fputc(str[i], f);
  total += fputc('\n', f);
  return total;
}

int puts(const char *str) {
  return fputs(str, _stdout);
}

int _putstr(const char *str) {
  return sys_write(1, str, strlen(str));
}

const char *hex = "0123456789ABCDEF";

void _reverse(char s[])
{
  int i, j;
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    if(i == j) continue;
    s[i] ^= s[j];
    s[j] ^= s[i];
    s[i] ^= s[j];
  }
}

void _itoa(int n, char s[]) {
  int i, sign;
  if ((sign = n) < 0)
    n = -n;
  i = 0;
  do {
    s[i++] = n % 10 + '0';
  } while ((n /= 10) > 0);
  if (sign < 0)
    s[i++] = '-';
  s[i] = '\0';
  _reverse(s); 
}


int printf(const char *fmt, ...) {
  __builtin_va_list vl;
  __builtin_va_start(vl, fmt);
  char c;
  char written = 0;
  char buf[32];
  unsigned int va, i;
  while((c = *(fmt++)) != 0) {
    if(c == '%') {
      c = *(fmt++);
      switch(c) {
      case 'c':
	written += putchar(__builtin_va_arg(vl, int));
	break;
      case 's':
	written += _putstr(__builtin_va_arg(vl, char *));
	break;
      case 'd':
	_itoa(__builtin_va_arg(vl, int), buf);
	written += _putstr(buf);
	break;
      case 'x':
	va = __builtin_va_arg(vl, unsigned int);
	for(i = 0; i < 8; i++) {
	  written += putchar(hex[(va >> ((7-i) * 4)) & 0xF]);
	}
      }
    } else {
      written += putchar(c);
    }
  }
  __builtin_va_end(vl);
  return written;
}
