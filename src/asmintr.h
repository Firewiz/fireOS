#ifndef ASMINTR_H
#define ASMINTR_H

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);
void outw(unsigned short port, unsigned short val);
unsigned short inw(unsigned short port);

#endif
