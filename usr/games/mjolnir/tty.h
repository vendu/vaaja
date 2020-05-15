#ifndef __MJOLNIR_TTY_H__
#define __MJOLNIR_TTY_H__

#include <mjolnir/conf.h>

#if (MJOLNIR_TTY)

#if (MJOLNIR_VT)
#include <stdio.h>
#else
#include <curses.h>
#endif

struct mjolwindatatty {
    void *mainwin;
    void *msgwin;
    void *statwin;
    void *gamewin;
};

#endif /* MJOLNIR_TTY */

#endif /* __MJOLNIR_TTY_H__ */

