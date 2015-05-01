#ifndef KEYBOARD_H
#define KEYBOARD_H
#ifndef EXTERN
#define EXTERN extern
#else
#define EXTERN
#endif

#define KB_BUFSIZE 32

#define STATE_SHIFT 0x01

#define KEY_LSHIFT 0x80
#define KEY_RSHIFT 0x81
#define KEY_CAPSLOCK 0x82
#define KEY_NUMLOCK 0x83
#define KEY_SCROLLLOCK 0x84
#define KEY_F1 0x85
#define KEY_F2 0x86
#define KEY_F3 0x87
#define KEY_F4 0x88
#define KEY_F5 0x89
#define KEY_F6 0x8A
#define KEY_F7 0x8B
#define KEY_F8 0x8C
#define KEY_F9 0x8D
#define KEY_F10 0x8E
#define KEY_F11 0x8F
#define KEY_F12 0x90

EXTERN unsigned char kb_buffer[KB_BUFSIZE];
EXTERN char key_table[2][128];
EXTERN volatile int kb_write, kb_read, kb_state;

void init_keyboard();
unsigned char getkey();
unsigned char getc();

#endif
