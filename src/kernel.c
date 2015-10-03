#include "version.h"
#include "vga.h"
#include "inthandle.h"
#include "keyboard.h"
#include "irq.h"
#include "printf.h"
#include "malloc.h"
#include "paging.h"
#include "timer.h"
#include "shell.h"
#include "gdt.h"
#include "process.h"
#include "syscall.h"
#include "syscall_handlers.h"

void kernel_main() {
  vga_init();
  printf("Initializing paging...\n");
  setup_paging();
  unsigned int i;
  vga_write("Identity paging to 0x01000000\n");
  for(i = 0; i < 0x1000; i++) {
    identity_page(i);
  }
  *((unsigned int *) 0x20) = 0x89ABCDEF;
  vga_write("Non-identity paging 0x01000000 to 0x01800000\n");
  for(i = 0x1000; i < 0x1800; i++) {
    nonidentity_page(i, 0);
  }
  printf("Setting up malloc...\n");
  init_malloc();
  printf("Loading page table...\n");
  load_pagetable();
  printf("Installing GDT...\n");
  setup_gdt();
  printf("Installing IDT...\n");
  setup_idt();
  printf("Installing exception handlers...\n");
  install_exc_handlers();
  printf("Setting up IRQs...\n");
  init_irq();
  vga_write("Welcome to ");
  vga_setcolor(vga_color(COLOR_RED, COLOR_BLACK));
  vga_write("Fire");
  vga_setcolor(vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK));
  vga_write("OS");
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  vga_write(" version " VERSION "\n");
  init_keyboard();
  vga_write("Keyboard initialized\n");
  init_syscall(0x80);
  register_syscall_handlers();
  printf("Initializing init...\n");
  init_mt();
  printf("Initializing timer...\n");
  timer_phase(100);
  init_timer();
  vga_setcurs(0, 24);
  shell_main();
}
