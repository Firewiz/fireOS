#include "printf.h"

void printf(char *fmt, ...) {
  __gnuc_va_list vl;
  __builtin_va_start(vl, fmt);
  char c;
  while(c = fmt++) {
    if(c == '%') {
      c = fmt++;
      switch(c) {
      case 's':
	vga_write(__builtin_va_arg(va, char *));
	break;
      case 'd':
	
    }    
  }
  __builtin_va_end(vl);
}
