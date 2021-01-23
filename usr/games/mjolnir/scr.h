#ifndef __MJOLNIR_SCR_H__
#define __MJOLNIR_SCR_H__

#include <mjolnir/conf.h>
#include <mjolnir/mjolnir.h>

#if defined(MJOLNIR_VGA_TEXT)
#include <mjolnir/vga.h>
#endif
#if defined(MJOLNIR_TTY)
#include <mjolnir/tty.h>
#endif

struct gamescr {
    void               *lvldata;
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

