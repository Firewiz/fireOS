#ifndef ELF_H
#define ELF_H

#define ELF_BITS_32 0
#define ELF_BITS_64 1
#define ELF_ENDIANNESS_LITTLE 1
#define ELF_ENDIANNESS_BIG 2
#define ELF_TYPE_RELOCATABLE 1
#define ELF_TYPE_EXECUTABLE 2
#define ELF_TYPE_SHARED 3
#define ELF_TYPE_CORE 4


struct elf_header {
  unsigned char magic[4];
  unsigned char bits;
  unsigned char endian;
  unsigned char version;
  unsigned char abi;
  unsigned char padding[8];
  unsigned short type;
  unsigned short iset;
  unsigned int version2;
  unsigned int entry_pos;
  unsigned int header_table_pos;
  unsigned int section_table_pos;
  unsigned int flags;
  unsigned short hsize;
  unsigned short phsize;
  unsigned short phentries;
  unsigned short shsize;
  unsigned short shentries;
  unsigned short shindex;
} __attribute__ ((packed));

#define ELF_PTYPE_NULL 0
#define ELF_PTYPE_LOAD 1
#define ELF_PTYPE_DYN 2
#define ELF_PTYPE_INTERP 3
#define ELF_PTYPE_NOTE 4
#define ELF_PFLAGS_EXE 1
#define ELF_PFLAGS_READ 2
#define ELF_PFLAGS_WRITE 3

struct program_header {
  unsigned int stype;
  unsigned int p_offset;
  unsigned int p_vaddr;
  unsigned int padding;
  unsigned int p_filesz;
  unsigned int p_memsz;
  unsigned int flags;
  unsigned int align;
} __attribute__ ((packed));

unsigned int load_elf(unsigned char *file, pid_t owner);

#endif
