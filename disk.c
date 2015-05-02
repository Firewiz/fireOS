#include "disk.h"
#include "asmintr.h"

int read_sector(unsigned short *sector, unsigned char drive, unsigned int lba) {
  outb(0x1F6, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
  outb(0x1F1, 0);
  outb(0x1F2, 1);
  outb(0x1F3, lba & 0xFF);
  outb(0x1F4, (lba >> 8) & 0xFF);
  outb(0x1F5, (lba >> 16) & 0xFF);
  outb(0x1F7, 0x20);
  // poll status
  unsigned char status;
  while((status = inb(0x1F7))) {
    if(!(status & 0x80) && (status & 0x08))
      break;
  }
  int i;
  for(i = 0; i < 256; i++) {
    sector[i] = inw(0x1F0);
  }
  return 0;
}

int write_sector(unsigned short *sector, unsigned char drive, unsigned int lba) {
  outb(0x1F6, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
  outb(0x1F2, 1);
  outb(0x1F3, lba & 0xFF);
  outb(0x1F4, (lba >> 8) & 0xFF);
  outb(0x1F5, (lba >> 16) & 0xFF);
  outb(0x1F7, 0x30);
  // poll status
  unsigned char status;
  while((status = inb(0x1F7))) {
    if(!(status & 0x80) && (status & 0x08))
      break;
  }
  int i;
  for(i = 0; i < 256; i++) {
    outw(0x1F0, sector[i]);
  }
  outb(0x1F7, 0xE7);
  return 0;
}

void reset_bus() {
  outb(0x3F6, 0x02);
  int i;
  for(i = 0; i < 10000; i++) ;
  outb(0x3F6, 0x0);
}
