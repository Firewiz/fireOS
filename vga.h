#ifndef VGA_H
#define VGA_H

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
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

void vga_init();
void vga_setcolor(unsigned char);
void vga_addch(char, unsigned char, unsigned char, unsigned char);
void vga_scroll();
void vga_putchar(char);
void vga_write(char *);
void vga_updatepos();
void vga_setcurs(char, char);

#endif
