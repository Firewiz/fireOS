#include "printf.h"
#include "stdlib.h"
#include "vga.h"

void printf(char *fmt, ...) {
  __builtin_va_list vl;
  __builtin_va_start(vl, fmt);
  char c;
  char buf[32];
  while((c = *(fmt++)) != 0) {
    if(c == '%') {
      c = *(fmt++);
      switch(c) {
      case 's':
	vga_write(__builtin_va_arg(vl, char *));
	break;
      case 'd':
	itoa(__builtin_va_arg(vl, int), buf);
	vga_write(buf);
	break;
      }
    } else {
      vga_putchar(c);
    }
  }
  vga_updatepos();
  __builtin_va_end(vl);
}
