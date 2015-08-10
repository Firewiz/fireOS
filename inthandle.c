#include "inthandle.h"
#include "idt.h"
#include "vga.h"
#include "printf.h"
#include "mt.h"

char *exceptions[] = {
  "Division by zero",
  "Debug",
  "NMI",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check"
};

#define BLUE_SCREEN

void handle_exception(int in, struct regs *r) {

#ifdef BLUE_SCREEN
  vga_setcolor(vga_color(COLOR_WHITE, COLOR_BROWN));
  vga_setcurs(0, 0);
  vga_write("Kernel panic: ");
  vga_write(exceptions[in]);
  printf("(thread %d)", cur_ctx);
  vga_write("\n--------\nDetails:\n");
  if(r->load_stack) printf("Stack loaded\n");
  printf("Error code: %x\n", r->err_code);
  printf("Segments: GS %x FS %x ES %x DS %x\n", r->gs, r->fs, r->es, r->ds);
  printf("Registers: EDI %x ESI %x EBP %x ESP %x\n           EDX %x ECX %x EBX %x EAX %x\n", r->edi, r->esi, r->ebp, r->esp, r->edx, r->ecx, r->ebx, r->eax);
  printf("Location: %x:%x\n", r->cs, r->eip);
  printf("Flags %x, stack %x:%x\n", r->eflags, r->ss, r->useresp);
  printf("\n\nWill now halt.");
#else
  vga_setcolor(vga_color(COLOR_BLACK, COLOR_LIGHT_RED));
  printf("Kernel Panic: %s at %x\n", exceptions[in], r->eip);
#endif
  for(;;);
}

void install_exc_handlers() {
  int i;
  for(i = 0; i < 18; i++) {
    install_handler(handle_exception, i);
  }
}
