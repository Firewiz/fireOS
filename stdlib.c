#include "stdlib.h"

unsigned long strlen(const char *s) {
  unsigned long r = 0;
  while(s[r])r++;
  return r;
}
