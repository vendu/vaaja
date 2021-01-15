#ifndef __MJOLNIR_SCR_H__
#define __MJOLNIR_SCR_H__

#include <mjolnir/conf.h>
#include <mjolnir/mjolnir.h>

#if (MJOLNIR_VGA_TEXT)
#include <mjolnir/vga.h>
#endif
#if (MJOLNIR_TTY)
#include <mjolnir/tty.h>
#endif

struct mjolscr {
    void               *data;
    long                x;
    long                y;
    int               (*getch)(void);
    void              (*moveto)(int, int);
    void              (*drawchr)(struct game *, struct chr *);
    int               (*printmsg)(const char *fmt, ...);
    int               (*refresh)(void);
    void              (*close)(void);
};

#endif /* __MJOLNIR_SCR_H__ */

