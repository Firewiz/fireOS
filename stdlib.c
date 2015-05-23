#include "stdlib.h"

unsigned long strlen(const char *s) {
  unsigned long r = 0;
  while(s[r])r++;
  return r;
}

void reverse(char s[])
{
  int i, j;
  for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
    if(i == j) continue;
    s[i] ^= s[j];
    s[j] ^= s[i];
    s[i] ^= s[j];
  }
}

void itoa(int n, char s[]) {
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
  reverse(s); 
}

int atoi(char *str) {
    int r = 0;
    int i;
    for (i = 0; str[i] != '\0'; i++) {
      if(!isdigit(str[i])) continue;
      r *= 10;
      r += str[i] - '0';
    }
    return r;
}
