#ifndef PAGING_H
#define PAGING_H

#define PRESENT 0x01
#define RW      0x02
#define USER    0x04
#define ACCESS  0x08
#define DIRTY   0x10

void setup_paging();
void load_pagetable();
void identity_page(unsigned int);
unsigned int nonidentity_page(unsigned int, int);
void mapped_page(unsigned int, unsigned int, int);
void unmap_page(unsigned int);
int is_present(unsigned int);
unsigned int get_mapping(unsigned int);
#endif
