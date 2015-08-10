#include "shell.h"
#include "printf.h"
#include "disk.h"
#include "fat.h"
#include "malloc.h"
#include "vga.h"
#include "stdlib.h"
#include "keyboard.h"
#include "elf.h"
#include "mt.h"

void shell_main() {
  struct fat_fs filesys;
  vga_write("Reading file system\n");
  read_fs(&filesys);
  printf("Read FAT16 filesystem, label %s\n", filesys.bpb.ebr.label);
  printf("Reading root directory...\n");
  struct fat_dirent *rdir = read_root_directory(&filesys);
  char name[9], ext[4];
  char line[80];
  unsigned int selection, i;
  printf("Index\tName\t Type\tSize\tLocation\n");
  for(i = 0; i < filesys.bpb.n_dirents; i++) {
    if(rdir[i].filename[0] == 0) break;
    if(rdir[i].filename[0] == (signed char) 0xE5) continue;
    if(rdir[i].attrs == 0x0F) continue;
    parse_filename(rdir[i].filename, name, ext);
    printf("%d\t%s\t %s\t%d\t%d (%d)\n", i, name, ext, rdir[i].size,
	   rdir[i].cluster_low, rdir[i].cluster_low * filesys.bpb.spc + filesys.first_data - 7);
  }
  printf("Load program: ");
  getline(line);
  selection = atoi(line);
  parse_filename(rdir[selection].filename, name, ext);
  printf("Reading file %d (%s.%s)...\n", selection, name, ext);
  unsigned int n_file_sectors = rdir[selection].size;
  n_file_sectors |= 0x1FF;
  n_file_sectors++;
  n_file_sectors /= 512;
  unsigned char *file = malloc(n_file_sectors * 512);
  unsigned int base_sector = rdir[selection].cluster_low * filesys.bpb.spc + filesys.first_data - 7;
  for(i = 0; i < n_file_sectors; i++) {
    read_sector(file + (i * 512), 0, base_sector + i);
  }
  printf("File read.\nLoading ELF file...\n");
  start_task(load_elf(file), 1);
  free(rdir);
  yield();
}

