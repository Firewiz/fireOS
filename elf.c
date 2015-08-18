#include "elf.h"
#include "malloc.h"
#include "stdlib.h"
#include "printf.h"
#include "paging.h"
#include "config.h"
#include "mt.h"

#ifdef DEBUG_ELF
#define printd(f, ...) printf(f, ##__VA_ARGS__)
#else
#define printd(f, ...)
#endif

taskid_t load_elf(unsigned char *file) {
  struct elf_header *header = malloc(sizeof(struct elf_header));
  struct program_header *phead = malloc(sizeof(struct program_header));
  memcpy(header, file, sizeof(struct elf_header));
  printd("Read ELF file header.\n");
  printd("Bits %d, version %d, type %d, i-set %d.\n", header->bits * 32, header->version, header->type, header->iset);
  printd("Program entry %x, header table position %x, header table entries %d (%d bytes each)\n", header->entry_pos, header->header_table_pos, header->phentries, header->phsize);
  printd("Creating task...\n");
  taskid_t tid = create_task();
  printd("Attempting to load program headers.\n");
  int i;
  for(i = 0; i < header->phentries; i++) {
    printd("Reading header %d/%d (%d bytes)...\n", i, header->phentries, header->phsize);
    memcpy(phead, file + header->header_table_pos + header->phsize * i, header->phsize);
    printd("Read header, section type %d, data offset %x, VM location %x, file size %d, mem size %d.\n", phead->stype, phead->p_offset, phead->p_vaddr, phead->p_filesz, phead->p_memsz);
    printd("Attempting to load section...\n");
    // map VM pages
    unsigned int page;
    for(page = 0; page < phead->p_memsz; page += 0x1000) {
      printd("Mapping page %x\n", (page + phead->p_vaddr) / 0x1000);
      page_task((page + phead->p_vaddr) / 0x1000, tid, 1);
    }
    // load section
    memcpy((void *)phead->p_vaddr, file + phead->p_offset, phead->p_filesz);
    printd("Section loaded.\n");
  }
  printd("Program loaded.\n");
  set_entry((void (*)(void)) header->entry_pos, tid);
  free(header);
  free(phead);
  return tid;
}
