#include "version.h"
#include "vga.h"
#include "idt.h"
#include "inthandle.h"
#include "keyboard.h"
#include "irq.h"
#include "fat.h"
#include "printf.h"
#include "malloc.h"
#include "stdlib.h"
#include "disk.h"
#include "syscall.h"
#include "paging.h"
#include "mt.h"
#include "timer.h"
#include "shell.h"
#include "gdt.h"

void mt_test() {
  vga_write("MT test thread 1 initialized\n");
  int i;
  while(1) {
    vga_addch('T', vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK), 0, 0);
    delay(50);
    vga_addch('T', vga_color(COLOR_DARK_GREY, COLOR_BLACK), 0, 0);
    delay(150);
  }
}

void mt_test_2() {
  vga_write("MT test thread 2 initialized\n");
  int i;
  while(1) {
    delay(50);
    vga_addch('e', vga_color(COLOR_LIGHT_RED, COLOR_BLACK), 1, 0);
    delay(50);
    vga_addch('e', vga_color(COLOR_DARK_GREY, COLOR_BLACK), 1, 0);
    delay(100);
  }
}

void mt_test_3() {
  vga_write("MT test thread 3 initialized\n");
  int i;
  while(1) {
    delay(100);
    vga_addch('s', vga_color(COLOR_LIGHT_BLUE, COLOR_BLACK), 2, 0);
    delay(50);
    vga_addch('s', vga_color(COLOR_DARK_GREY, COLOR_BLACK), 2, 0);
    delay(50);
  }
}

void mt_test_4() {
  vga_write("MT test thread 4 initialized\n");
  int i;
  while(1) {
    delay(150);
    vga_addch('t', vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK), 3, 0);
    delay(50);
    vga_addch('t', vga_color(COLOR_DARK_GREY, COLOR_BLACK), 3, 0);
  }
}

void kernel_main() {
  setup_gdt();
  vga_init();
  setup_paging();
  unsigned int i;
  vga_write("Identity paging to 0x1000000\n");
  for(i = 0; i < 0x1000; i++) {
    identity_page(i);
  }
  vga_write("Non-identity paging 0x1000000 to 0x1800000\n");
  for(i = 0x1000; i < 0x1800; i++) {
    nonidentity_page(i);
  }
  load_pagetable();
  vga_write("Done!\n");
  setup_idt();
  install_exc_handlers();
  init_irq();
  register_syscall(0x80);
  vga_write("Welcome to ");
  vga_setcolor(vga_color(COLOR_RED, COLOR_BLACK));
  vga_write("Fire");
  vga_setcolor(vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK));
  vga_write("OS");
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  vga_write(" version " VERSION "\n");
  init_keyboard();
  vga_write("Keyboard initialized\n");
  timer_phase(100);
  init_timer();

  init_mt();
  vga_write("Setting MT entry point.\n");
  taskid_t test_id = start_task(mt_test, 0);
  taskid_t test2_id= start_task(mt_test_2, 0);
  taskid_t test3_id= start_task(mt_test_3, 0);
  taskid_t test4_id= start_task(mt_test_4, 0);
  taskid_t shell_id = start_task(shell_main, 0);
  vga_write("Starting MT.\n");
  asm volatile ("sti");
  
}
