#include "version.h"
#include "vga.h"
#include "idt.h"
#include "inthandle.h"
#include "keyboard.h"
#include "irq.h"

void kernel_main() {
  setup_idt();
  install_exc_handlers();
  init_irq();
  vga_init();
  vga_setcolor(vga_color(COLOR_RED, COLOR_BLACK));
  vga_write("Fire");
  vga_setcolor(vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK));
  vga_write("OS");
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  vga_write(" version " VERSION " initialized\n");
  init_keyboard();
  vga_write("Keyboard initialized\n");
  while(1) {
    char c = getc();
    vga_putchar(c);
  }
}
