#include "version.h"
#include "vga.h"
#include "idt.h"
#include "inthandle.h"
#include "keyboard.h"
#include "irq.h"
#include "disk.h"

void kernel_main() {
  setup_idt();
  install_exc_handlers();
  init_irq();
  vga_init();
  vga_write("Welcome to ");
  vga_setcolor(vga_color(COLOR_RED, COLOR_BLACK));
  vga_write("Fire");
  vga_setcolor(vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK));
  vga_write("OS");
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  vga_write(" version " VERSION "\n");
  init_keyboard();
  vga_write("Keyboard initialized\n");
  asm volatile ("sti");
  vga_write("Attempting sector read, hold on to your hats...\n");
  unsigned char sector[512];
  read_sector((unsigned short *)sector, 0, 0);
  vga_write("Let's see what we got...\n");
  int i;
  for(i = 0; i < 512; i++) {
    vga_putchar(sector[i]);
  }
  sector[0] = 'A';
  sector[1] = 'B';
  sector[2] = 'C';
  vga_write("\n\nAttempting sector write...\n");
  write_sector((unsigned short *)sector, 0, 0);
  vga_write("Write complete.\n");
  reset_bus();
  vga_write("Attempting sector read, hold on to your hats...\n");
  read_sector((unsigned short *)sector, 0, 0);
  vga_write("Let's see what we got...\n");
  for(i = 0; i < 512; i++) {
    vga_putchar(sector[i]);
  }
  vga_write("\n\nTest complete.\n");
}
