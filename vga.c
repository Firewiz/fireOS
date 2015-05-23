#include "vga.h"
#include "stdlib.h"
#include "asmintr.h"

unsigned char vga_row, vga_col;
unsigned char vga_color;
unsigned short *vga_buf;

void vga_init() {
  vga_row = vga_col = 0;
  vga_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  vga_buf = (unsigned short *) 0xB8000;
  unsigned char x, y;
  for(y = 0; y < VGA_HEIGHT; y++) {
    for(x = 0; x < VGA_WIDTH; x++) {
      vga_buf[y * VGA_WIDTH + x] = vga_char(' ', vga_color);
    }
  }
}

void vga_setcolor(unsigned char color) {
  vga_color = color;
}

void vga_addch(char c, unsigned char color, unsigned char x, unsigned char y) {
  vga_buf[y * VGA_WIDTH + x] = vga_char(c, color);
}

void vga_scroll() {
  int x, y;
  for(y = 0; y < VGA_HEIGHT; y++) {
    for(x = 0; x < VGA_WIDTH; x++) {
      vga_buf[(y - 1) * VGA_WIDTH + x] = vga_buf[y * VGA_WIDTH + x];
    }
  }
  for(x = 0; x < VGA_WIDTH; x++) {
    vga_buf[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = 0;
  }
  vga_updatepos();
}

void vga_putchar(char c) {
  switch(c) {
  case '\t':
    vga_col = vga_col + 8 - (vga_col % 8);
    break;
  case '\n':
    vga_row++;
    vga_col = 0;
    break;
  case '\b':
    vga_col = (vga_col == 0) ? 0 : vga_col - 1;
    vga_putchar(' ');
    vga_col = (vga_col == 0) ? 0 : vga_col - 1;
    break;
  case 0:
    break;
  default:
    vga_addch(c, vga_color, vga_col++, vga_row);
    break;
  }
  if(vga_col >= VGA_WIDTH) {
    vga_col = 0;
    vga_row++;
  }
  if(vga_row >= VGA_HEIGHT) {
    vga_row--;
    vga_scroll();
  }
}

void vga_setcurs(char x, char y) {
  unsigned short pos = (y * 80) + x;
  outb(0x3D4, 0x0F);
  outb(0x3D5, (unsigned char) pos & 0xFF);
  outb(0x3D4, 0x0E);
  outb(0x3D5, (unsigned char) (pos >> 8) & 0xFF);
}

void vga_updatepos() {
  vga_setcurs(vga_col, vga_row);
}

void vga_write(char *s) {
  unsigned long len = strlen(s);
  unsigned long i;
  for(i = 0; i < len; i++)
    vga_putchar(s[i]);
  vga_setcurs(vga_col, vga_row);
}
