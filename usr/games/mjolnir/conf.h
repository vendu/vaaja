#ifndef __MJOLNIR_CONF_H__
#define __MJOLNIR_CONF_H__

#define MJOLNIR_ENGLISH_LANGUAGE  1
#define MJOLNIR_VGA_TEXT          0
#define MJOLNIR_TTY               1
#define MJOLNIR_X11               0
#define MJOLNIR_USE_COLORS        1
#define MJOLNIR_USE_ZERO_RANDMT32 0
#if (MJOLNIR_TTY)
#define MJOLNIR_VT                1
#define MJOLNIR_CURSES            0
#endif

/* PLEASE DO NOT EDIT BELOW */

#define MJOLNIR_ROOM_IDS          0
#define MJOLNIR_HASH              0
#if (MJOLNIR_USE_ZERO_RANDMT32)
#define mjolsrand(seed) srandmt32(seed)
#define mjolrand()      (randmt32() & 0x7fffffff)
#else
#define mjolsrand(seed) srand(seed)
#define mjolrand()      (rand() & 0x7fffffff)
#endif

#endif /* __MJOLNIR_CONF_H__ */

