#ifndef DISK_H
#define DISK_H

int read_sector(unsigned char *, unsigned char, unsigned int);
int write_sector(unsigned char *, unsigned char, unsigned int);
void reset_bus();

#endif
