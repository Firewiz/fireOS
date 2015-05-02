#define EXTERN
#include "keyboard.h"
#undef EXTERN
#include "irq.h"
#include "asmintr.h"

unsigned char kb_buffer[32];
volatile int kb_write, kb_read, kb_state = 0;
char key_table[2][128] = {
  {
    0, '\e', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0x8F, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', KEY_LSHIFT, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', KEY_RSHIFT, '*',
    0x8F, ' ', KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLLOCK, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, KEY_F11,
    KEY_F12, 0, 0
  },
  {
    0, '\e', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0x8F, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', KEY_LSHIFT, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', KEY_RSHIFT, '*',
    0x8F, ' ', KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUMLOCK, KEY_SCROLLLOCK, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, KEY_F11,
    KEY_F12, 0, 0
  }
};

void keyboard_stuff_buffer() {
  kb_buffer[kb_write++] = inb(0x60);
  kb_write %= KB_BUFSIZE;
}

unsigned char getkey() {
  while(kb_write == kb_read) ;
  unsigned char r;
  r = kb_buffer[kb_read++];
  kb_read %= KB_BUFSIZE;
  return r;
}

unsigned char getc() {
  unsigned char key;
  unsigned char c;
  do {
    key = getkey();
    c = key_table[kb_state & STATE_SHIFT][key & 0x7F];
    if(c == KEY_LSHIFT || c == KEY_RSHIFT) {
      if(key & 0x80)
	kb_state &= ~STATE_SHIFT;
      else
	kb_state |= STATE_SHIFT;
    }
  } while(c & 0x80 || key & 0x80);
  return c;
}

void init_keyboard() {
  install_irq(keyboard_stuff_buffer, 1);
  kb_read = kb_write = 0;
}

