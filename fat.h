#ifndef FAT32_H
#define FAT32_H

struct fat_fs {
  struct fat_bpb {
    unsigned char magic[3];
    char oem[8];
    unsigned short bps;
    unsigned char spc;
    unsigned short n_res;
    unsigned char n_fats;
    unsigned short n_dirents;
    unsigned short n_sectors;
    unsigned char mdt;
    unsigned short spf;
    unsigned short n_heads;
    unsigned int n_hidden;
    unsigned short l_sectors;
    struct fat_ebr {
      unsigned char dnum;
      unsigned char flags;
      unsigned char signature;
      unsigned int v_serial;
      char label[11];
      char sysid[8];
      unsigned char boot[448];
      unsigned short boot_magic;
    } __attribute__ ((packed)) ebr;
  } __attribute__ ((packed)) bpb;
  unsigned int total_sectors;
  unsigned int fat_size;
  unsigned int root_size;
  unsigned int first_data;
  unsigned int first_fat;
  unsigned int total_data;
  unsigned int total_clusters;
};


#endif
