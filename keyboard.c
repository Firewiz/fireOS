#define EXTERN
#include "keyboard.h"
#undef EXTERN
#include "irq.h"
#include "asmintr.h"

unsigned char kb_buffer[32];
int kb_write, kb_read, kb_state = 0;
char key_table[2][128] = {
  {
    0, '\e', '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0x81, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0x82, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0x84, '*',
    0x88, ' ', KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM_LOCK, KEY_SCROLL_LOCK, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, KEY_F11,
    KEY_F12, 0, 0
  },
  {
    0, '\e', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '_', '+', '\b', '\t',
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0x81, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', 0x82, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0X84, '*',
    0x88, ' ', KEY_CAPSLOCK, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5,
    KEY_F6, KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_NUM_LOCK, KEY_SCROLL_LOCK, '7',
    '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, KEY_F11,
    KEY_F12, 0, 0
  }
};

void keyboard_stuff_buffer(int in) {
  kb_buffer[kb_write++] = inb(0x60);
  kb_write %= KB_BUFSIZE;
  outb(0x20, 0x20);
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
    c = key_table[kb_state & STATE_SHIFT][key];
    if(c & 0x80) {
      if(key & MASK_SPECIAL) {
	; // to handle later
      } else {
	if((key & MASK_LSHIFT)
	   | (key & MASK_RSHIFT)) {
	  if(key & 0x80)
	    kb_state &= ~STATE_SHIFT;
	  else
	    kb_state |= STATE_SHIFT;
	}
      }
    }
  } while(c & 0x80);
  return c;
}

void init_keyboard() {
  install_irq(keyboard_stuff_buffer, 1);
  kb_read = kb_write = 0;
}

