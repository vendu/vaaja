#ifndef __MJOLNIR_SCR_H__
#define __MJOLNIR_SCR_H__

#include <mjolnir/conf.h>
#include <mjolnir/mjol.h>

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
    void              (*drawchr)(struct mjolgame *, struct mjolchr *);
    int               (*printmsg)(const char *fmt, ...);
    int               (*refresh)(void);
    void              (*close)(void);
};

#endif /* __MJOLNIR_SCR_H__ */

