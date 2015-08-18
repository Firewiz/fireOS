#ifndef STDLIB_H
#define STDLIB_H

#define isdigit(c) ((c) >= '0' && (c) <= '9')

unsigned long strlen(const char *);
void reverse(char s[]);
void itoa(int n, char s[]);
int atoi(char *);
void memcpy(void *dest, const void *src, unsigned int n);
void bzero(void *mem, unsigned int n);
int streq(const char *s1, const char *s2);
#endif
