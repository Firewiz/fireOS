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

void mt_test() {
  vga_write("MT test thread 1 initialized\n");
  int i;
  static volatile char mc = 33;
  while(1) {
    for(i = 0; i < 10000000; i++) ;
    vga_addch(mc++, vga_color(COLOR_LIGHT_BROWN, COLOR_BLACK), 40, 0);
    if(mc >= 127) mc = 33;
  }
}

void mt_2() {
  vga_write("MT test thread 2 initialized\n");
  int i;
  static volatile char mc = 33;
  while(1) {
    for(i = 0; i < 10000000; i++) ;
    vga_addch(mc++, vga_color(COLOR_LIGHT_GREEN, COLOR_BLACK), 41, 0);
    if(mc >= 127) mc = 33;
  }
}

void kernel_main() {
  vga_init();
  setup_paging();
  unsigned int i;
  vga_write("Identity paging to 0x400000\n");
  for(i = 0; i < 0x400; i++) {
    identity_page(i);
  }
  vga_write("Non-identity paging 0x1000000 to 0x2000000\n");
  for(i = 0x1000; i < 0x2000; i++) {
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
  init_mt();

  vga_write("Setting MT entry point.\n");
  taskid_t test_id = start_task(mt_test);
  taskid_t test2_id= start_task(mt_2);
  vga_write("Attempting to start MT.\nThis will probably fail.\n");
  asm volatile ("sti");
  
  /*
  struct fat_fs filesys;
  vga_write("Reading file system\n");
  read_fs(&filesys);
  printf("Read FAT16 filesystem, label %s\n", filesys.bpb.ebr.label);
  printf("Reading root directory...\n");
  struct fat_dirent *rdir = read_root_directory(&filesys);
  char name[9], ext[4];
  char line[80];
  int selection;
  printf("INDEX\tNAME\t TYPE\tSIZE\tLOCATION\n");
  for(i = 0; i < filesys.bpb.n_dirents; i++) {
    if(rdir[i].filename[0] == 0) break;
    if(rdir[i].filename[0] == (signed char) 0xE5) continue;
    if(rdir[i].attrs == 0x0F) continue;
    parse_filename(rdir[i].filename, name, ext);
    printf("%d\t%s\t %s\t%d\t%d (%d)\n", i, name, ext, rdir[i].size, rdir[i].cluster_low, rdir[i].cluster_low * filesys.bpb.spc + filesys.first_data - 7);
  }
  printf("Load module: ");
  getline(line);
  selection = atoi(line);
  parse_filename(rdir[selection].filename, name, ext);
  printf("Reading file %d (%s.%s)...\n", selection, name, ext);
  unsigned int n_file_sectors = rdir[selection].size;
  n_file_sectors |= 0x1FF;
  n_file_sectors++;
  n_file_sectors /= 512;
  unsigned char *file = malloc(n_file_sectors * 512);
  int base_sector = rdir[selection].cluster_low * filesys.bpb.spc + filesys.first_data - 7;
  for(i = 0; i < n_file_sectors; i++) {
    read_sector(file + (i * 512), 0, base_sector + i);
  }
  char *module = (char *)0x1000000;
  for(i = 0; i < n_file_sectors * 512; i++)
    module[i] = file[i];
  ((void (*)()) module)();
  free(rdir); */
}
