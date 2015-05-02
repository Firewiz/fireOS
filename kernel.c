#include "version.h"
#include "vga.h"
#include "idt.h"
#include "inthandle.h"
#include "keyboard.h"
#include "irq.h"
#include "fat.h"
#include "printf.h"

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
  struct fat_fs filesys;
  vga_write("Reading file system\n");
  read_fs(&filesys);
  printf("Read FAT16 filesystem, label %s\n", filesys.bpb.ebr.label);
  printf("File system info:\n");
  printf("\tTotal sectors: %d\n", filesys.total_sectors);
  printf("\tFAT size: %d\n", filesys.fat_size);
  printf("\tRoot dir size: %d\n", filesys.root_size);
  printf("\tFirst data sector: %d\n", filesys.first_data);
  printf("\tFirst FAT sector: %d\n", filesys.first_fat);
  printf("\tTotal data size: %d\n", filesys.total_data);
  printf("\tTotal clusters: %d\n", filesys.total_clusters);
  printf("\tCluster size (bytes): %d\n", filesys.bpb.bps);
}
