#include "../include/stdio.h"
#include "../include/string.h"
#include "../include/syscall.h"

int putchar(int c) {
  sys_write(1, (char *) &c, 1);
}

int puts(const char *str) {
  sys_write(1, str, strlen(str));
  putchar('\n');
}

int _putstr(const char *str) {
  sys_write(1, str, strlen(str));
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
  char buf[32];
  unsigned int va, i;
  while((c = *(fmt++)) != 0) {
    if(c == '%') {
      c = *(fmt++);
      switch(c) {
      case 'c':
	putchar(__builtin_va_arg(vl, int));
	break;
      case 's':
	_putstr(__builtin_va_arg(vl, char *));
	break;
      case 'd':
	_itoa(__builtin_va_arg(vl, int), buf);
	_putstr(buf);
	break;
      case 'x':
	va = __builtin_va_arg(vl, unsigned int);
	for(i = 0; i < 8; i++) {
	  putchar(hex[(va >> ((7-i) * 4)) & 0xF]);
	}
      }
    } else {
      putchar(c);
    }
  }
  __builtin_va_end(vl);
}
