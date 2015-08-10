#include "elf.h"
#include "malloc.h"
#include "stdlib.h"
#include "printf.h"
#include "paging.h"

void (*load_elf(unsigned char *file))(void) {
  struct elf_header *header = malloc(sizeof(struct elf_header));
  struct program_header *phead = malloc(sizeof(struct program_header));
  memcpy(header, file, sizeof(struct elf_header));
  
  printf("Read ELF file header.\n");
  printf("Bits %d, version %d, type %d, i-set %d.\n", header->bits * 32, header->version, header->type, header->iset);
  printf("Program entry %x, header table position %x, header table entries %d (%d bytes each)\n", header->entry_pos, header->header_table_pos, header->phentries, header->phsize);
  printf("Attempting to load program headers.\n");
  int i;
  for(i = 0; i < header->phentries; i++) {
    printf("Reading header %d...\n", i);
    memcpy(phead, file + header->header_table_pos + header->phsize * i, header->phsize);
    printf("Read header, section type %d, data offset %x, VM location %x, file size %d, mem size %d.\n", phead->stype, phead->p_offset, phead->p_vaddr, phead->p_filesz, phead->p_memsz);
    printf("Attempting to load section...\n");
    // map VM pages
    unsigned int page;
    for(page = 0; page < phead->p_memsz; page += 0x1000) {
      if(is_present((page + phead->p_vaddr) / 0x1000)) {
	printf("Something's funky - page %x is already mapped!\n", page / 0x1000);
	free(header);
	free(phead);
	return 0;
      }
      nonidentity_page(page / 0x1000);
    }
    // load section
    memcpy((void *)phead->p_vaddr, file + phead->p_offset, phead->p_filesz);
    printf("Section loaded.\n");
  }
  printf("Program loaded\n");
  void (*entry)(void) = (void (*)(void)) header->entry_pos;
  free(header);
  free(phead);
  return entry;
}
