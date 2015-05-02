#include "inthandle.h"
#include "idt.h"
#include "vga.h"

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


void handle_exception(int in) {
  vga_setcolor(vga_color(COLOR_BLACK, COLOR_LIGHT_RED));
  vga_write("Kernel panic: ");
  vga_write(exceptions[in]);
  vga_write("\n");
  for(;;);
}

void install_exc_handlers() {
  int i;
  for(i = 0; i < 18; i++) {
    install_handler(handle_exception, i);
  }
}
