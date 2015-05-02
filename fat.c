#include "fat.h"
#include "disk.h"

void read_fs(struct fat_fs *b) {
  read_sector((unsigned short *) &(b->bpb), 0, 0);
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

