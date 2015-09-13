#include "inthandle.h"
#include "idt.h"
#include "vga.h"
#include "printf.h"
#include "process.h"

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
  vga_setcolor(vga_color(COLOR_LIGHT_RED, COLOR_BLACK));
  vga_write("Exception: ");
  vga_write(exceptions[in]);
  printf(" (PID #%d)\n", current_pid);
  printf("Error code: %x\n", r->err_code);
  printf("Segments: GS %x FS %x ES %x DS %x\n", r->gs, r->fs, r->es, r->ds);
  printf("Registers: EDI %x ESI %x EBP %x ESP %x\n           EDX %x ECX %x EBX %x EAX %x\n", r->edi, r->esi, r->ebp, r->esp, r->edx, r->ecx, r->ebx, r->eax);
  printf("Location: %x:%x ", r->cs, r->eip);
  printf("Flags %x Stack %x:%x ", r->eflags, r->ss, r->useresp);
  unsigned int cr2;
  asm volatile("mov %%cr2, %0": "=r"(cr2));
  printf("CR2: %x\n", cr2);
#else
  vga_setcolor(vga_color(COLOR_BLACK, COLOR_LIGHT_RED));
  printf("%s at %x\n", exceptions[in], r->eip);
#endif
  //  end_task(cur_ctx);
  vga_setcolor(vga_color(COLOR_LIGHT_GREY, COLOR_BLACK));
  asm volatile ("hlt");
  
  for(;;);
}

void install_exc_handlers() {
  int i;
  for(i = 0; i < 18; i++) {
    install_handler(handle_exception, i);
  }
}
