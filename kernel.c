#include "version.h"

enum vga_color {
  COLOR_BLACK = 0,
  COLOR_BLUE = 1,
  COLOR_GREEN = 2,
  COLOR_CYAN = 3,
  COLOR_RED = 4,
  COLOR_MAGENTA = 5,
  COLOR_BROWN = 6,
  COLOR_LIGHT_GREY = 7,
  COLOR_DARK_GREY = 8,
  COLOR_LIGHT_BLUE = 9,
  COLOR_LIGHT_GREEN = 10,
  COLOR_LIGHT_CYAN = 11,
  COLOR_LIGHT_RED = 12,
  COLOR_LIGHT_MAGENTA = 13,
  COLOR_LIGHT_BROWN = 14,
  COLOR_WHITE = 15,
};

#define vga_color(fore, back) ((fore) | (back) << 4)
#define vga_char(c, color) (((unsigned short) (c)) | ((unsigned short) (color)) << 8)

unsigned long strlen(const char *s) {
  unsigned long r = 0;
  while(s[r])r++;
  return r;
}

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
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

void vga_putchar(char c) {
  vga_addch(c, vga_color, vga_col++, vga_row);
  if(vga_col >= VGA_WIDTH) {
    vga_col = 0;
    vga_row++;
    if(vga_row >= VGA_HEIGHT) {
      vga_row = 0;		/* TODO: scrolling */
    }
  }
}

void vga_write(char *s) {
  unsigned long len = strlen(s);
  unsigned long i;
  for(i = 0; i < len; i++)
    vga_putchar(s[i]);
}

void kernel_main() {
  vga_init();
  vga_setcolor(vga_color(COLOR_RED, COLOR_BLACK));
  vga_write("Fire");
  vga_setcolor(vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK));
  vga_write("OS");
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  vga_write(" version " VERSION " initialized\n");
}
