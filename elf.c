#include "elf.h"
#include "malloc.h"
#include "stdlib.h"
#include "printf.h"
#include "paging.h"
#include "config.h"

#ifdef DEBUG_ELF
#define printd(f, ...) printf(f, ##__VA_ARGS__)
#else
#define printd(f, ...)
#endif

unsigned int load_elf(unsigned char *file, pid_t owner) {
  struct elf_header *header = malloc(sizeof(struct elf_header));
  struct program_header *phead = malloc(sizeof(struct program_header));
  memcpy(header, file, sizeof(struct elf_header));
  printd("Read ELF file header.\n");
  printd("Bits %d, version %d, type %d, i-set %d.\n", header->bits * 32, header->version, header->type, header->iset);
  printd("Program entry %x, header table position %x, header table entries %d (%d bytes each)\n", header->entry_pos, header->header_table_pos, header->phentries, header->phsize);
  int tid = 0;
  printd("Attempting to load program headers.\n");
  int i;
  for(i = 0; i < header->phentries; i++) {
    printd("Reading header %d/%d (%d bytes)...\n", i, header->phentries, header->phsize);
    memcpy(phead, file + header->header_table_pos + header->phsize * i, header->phsize);
    printd("Read header, section type %d, data offset %x, VM location %x, file size %d, mem size %d.\n", phead->stype, phead->p_offset, phead->p_vaddr, phead->p_filesz, phead->p_memsz);
    printd("Attempting to load section...\n");
    // map VM pages
    allocate_pages(phead->p_vaddr, phead->p_memsz, 1, owner);
    // load section
    memcpy((void *)phead->p_vaddr, file + phead->p_offset, phead->p_filesz);
    printd("Section loaded.\n");
  }
  printd("Program loaded.\n");
  free(header);
  free(phead);
  return header->entry_pos;
}
