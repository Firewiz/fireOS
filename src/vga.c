#include "vga.h"
#include "stdlib.h"
#include "asmintr.h"
#include "static.h"
#include "config.h"

volatile unsigned char vga_row, vga_col;
unsigned char vga_color;
volatile unsigned short *vga_buf;
char curs_state;

void vga_init() {
  vga_row = 1;
  vga_col = 0;
  curs_state = 0;
  vga_color = vga_color(COLOR_LIGHT_GREY, COLOR_BLACK);
  vga_buf = (unsigned short *) 0xB8000;
  unsigned char x, y;
  for(y = 0; y < VGA_HEIGHT; y++) {
    for(x = 0; x < VGA_WIDTH; x++) {
      vga_buf[y * VGA_WIDTH + x] = vga_char(' ', vga_color);
    }
  }
  outb(0x3d4, 0xa);
  outb(0x3d5, 0x0d);
  serial_init();
}

#ifdef VGA_SERIAL
void serial_init() {
  outb(VGASERIAL_COM1 + 1, 0x00);
  outb(VGASERIAL_COM1 + 3, 0x80);
  outb(VGASERIAL_COM1 + 0, 0x03);
  outb(VGASERIAL_COM1 + 1, 0x00);
  outb(VGASERIAL_COM1 + 3, 0x03);
  outb(VGASERIAL_COM1 + 2, 0xC7);
  outb(VGASERIAL_COM1 + 4, 0x0B);
}

int poll_serial() {
  return !(inb(VGASERIAL_COM1 + 5) & 0x20);
}

void serial_putchar(char c) {
  while(poll_serial())
    ;
  outb(VGASERIAL_COM1, c);
}
#else
void serial_init() {
  return;
}
int poll_serial() {
  return 0;
}
void serial_putchar(char c) {
  return;
}
#endif

void vga_setcolor(unsigned char color) {
  vga_color = color;
}

void vga_addch(char c, unsigned char color, unsigned char x, unsigned char y) {
  vga_buf[y * VGA_WIDTH + x] = vga_char(c, color);
}

void vga_scroll() {
  int x, y;
  for(y = 2; y < VGA_HEIGHT; y++) {
    for(x = 0; x < VGA_WIDTH; x++) {
      vga_buf[(y - 1) * VGA_WIDTH + x] = vga_buf[y * VGA_WIDTH + x];
    }
  }
  for(x = 0; x < VGA_WIDTH; x++) {
    vga_buf[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = vga_char(' ', vga_color);
  }
}

void vga_putchar(char c) {
  serial_putchar(c);
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
    vga_addch(' ', vga_color, vga_col, vga_row);
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
  vga_setcurs(vga_col, vga_row);
}

void vga_write(char *s) {
  unsigned long len = strlen(s);
  unsigned long i;
  for(i = 0; i < len; i++)
    vga_putchar(s[i]);
}

void vga_setcurs(char x, char y) {
  unsigned short l = ((unsigned short) y * 80) + (unsigned short) x;
  outb(0x3d4, 0x0f);
  outb(0x3d5, (unsigned char) (l & 0xFF));
  outb(0x3d4, 0x0e);
  outb(0x3d5, (unsigned char) ((l & 0xFF00) >> 8));
}
