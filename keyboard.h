#ifndef KEYBOARD_H
#define KEYBOARD_H
#ifndef EXTERN
#define EXTERN extern
#else
#define EXTERN
#endif

#define KB_BUFSIZE 32

#define MASK_LCTRL 0x01
#define MASK_LSHIFT 0x02
#define MASK_RSHIFT 0x04
#define MASK_RCTRL 0x08

#define STATE_SHIFT 0x01

#define MASK_SPECIAL 0xC0
#define KEY_CAPSLOCK MASK_SPECIAL | 0
#define KEY_F1 MASK_SPECIAL | 1
#define KEY_F2 MASK_SPECIAL | 2
#define KEY_F3 MASK_SPECIAL | 3
#define KEY_F4 MASK_SPECIAL | 4
#define KEY_F5 MASK_SPECIAL | 5
#define KEY_F6 MASK_SPECIAL | 6
#define KEY_F7 MASK_SPECIAL | 7
#define KEY_F8 MASK_SPECIAL | 8
#define KEY_F9 MASK_SPECIAL | 9
#define KEY_F10 MASK_SPECIAL | 10
#define KEY_F11 MASK_SPECIAL | 11
#define KEY_F12 MASK_SPECIAL | 12
#define KEY_NUM_LOCK MASK_SPECIAL |13
#define KEY_SCROLL_LOCK MASK_SPECIAL | 14

EXTERN unsigned char kb_buffer[KB_BUFSIZE];
EXTERN char key_table[2][128];
EXTERN int kb_write, kb_read, kb_state;

void init_keyboard();
unsigned char getkey();
unsigned char getc();

#endif
