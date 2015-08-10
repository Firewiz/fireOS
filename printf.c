#include "printf.h"
#include "stdlib.h"
#include "vga.h"

const char *hex = "0123456789ABCDEF";

void printf(char *fmt, ...) {
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
	vga_putchar(__builtin_va_arg(vl, char));
	break;
      case 's':
	vga_write(__builtin_va_arg(vl, char *));
	break;
      case 'd':
	itoa(__builtin_va_arg(vl, int), buf);
	vga_write(buf);
	break;
      case 'x':
	va = __builtin_va_arg(vl, unsigned int);
	for(i = 0; i < 8; i++) {
	  vga_putchar(hex[(va >> ((7-i) * 4)) & 0xF]);
	}
      }
    } else {
      vga_putchar(c);
    }
  }
  vga_updatepos();
  __builtin_va_end(vl);
}
