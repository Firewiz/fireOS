#include "fat.h"
#include "disk.h"
#include "malloc.h"

void read_fs(struct fat_fs *b) {
  read_sector((unsigned char *) &(b->bpb), 0, 0);
  b->total_sectors = b->bpb.n_sectors;
  b->fat_size = b->bpb.spf;
  b->root_size = ((b->bpb.n_dirents * 32)
		  + (b->bpb.bps - 1)) / b->bpb.bps;
  b->first_data = b->bpb.n_hidden
    + (b->bpb.n_fats * b->fat_size)
    + b->root_size;
  b->first_fat = b->bpb.n_hidden;
  b->total_data = b->total_sectors
    - (b->bpb.n_hidden
       + (b->bpb.n_fats * b->fat_size)
       + b->root_size);
  b->total_clusters = b->total_data / b->bpb.spc;
}

struct fat_dirent *read_root_directory(struct fat_fs *b) {
  struct fat_dirent *r = malloc(b->root_size * 512);
  unsigned char *data = (unsigned char *) r;
  unsigned int sector = b->first_data - b->root_size + 1;
  unsigned int i;
  for(i = 0; i < b->root_size; i++)
    read_sector(data + (i * 512), 0, sector + i);
  return r;
}

void parse_filename(char fname[11], char name[9], char ext[4]) {
  int i;
  for(i = 0; i < 8; i++) {
    name[i] = fname[i];
    if(fname[i] == ' ') {
      name[i] = 0;
      break;
    }
  }
  for(i = 0; i < 3; i++)
    ext[i] = fname[i + 8];
  name[8] = ext[3] = 0;
}

unsigned int sector_from_fat(struct fat_fs *b, unsigned short fat_offset) {
  unsigned char *fat = malloc(b->fat_size * b->bpb.bps);
  unsigned short cluster;
  unsigned int r;
  cluster = fat[fat_offset];
  cluster |= fat[fat_offset + 1];
  if(cluster >= 0xFFF8) r = -1;
  else r = cluster * b->bpb.spc;
  free(fat);
  return r;
}
