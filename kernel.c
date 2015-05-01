#include "version.h"
#include "vga.h"

void kernel_main() {
  vga_init();
  vga_setcolor(vga_color(COLOR_RED, COLOR_BLACK));
  vga_write("Fire");
  vga_setcolor(vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK));
  vga_write("OS");
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  vga_write(" version " VERSION " initialized\n");
}
