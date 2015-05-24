#ifndef STDLIB_H
#define STDLIB_H

#define isdigit(c) ((c) >= '0' && (c) <= '9')

unsigned long strlen(const char *);
void reverse(char s[]);
void itoa(int n, char s[]);
int atoi(char *);
void memcpy(void *dest, const void *src, unsigned int n);
void bzero(void *mem, unsigned int n);
#endif
