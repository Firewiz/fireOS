#ifndef CONFIG_H
#define CONFIG_H

#define DEBUG

#ifdef DEBUG

#define DEBUG_ELF
#undef DEBUG_MALLOC
#undef DEBUG_MT
#undef DEBUG_MT_LOUD

#endif

#endif
