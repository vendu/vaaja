#ifndef __MJOLNIR_TTY_H__
#define __MJOLNIR_TTY_H__

#include <mjolnir/conf.h>

#if defined(MJOLNIR_TTY)

#if defined(MJOLNIR_VT)
#include <stdio.h>
#elif defined(MJOLNIR_CURSES)
#include <curses.h>
#endif

struct ttywins {
    void               *mainwin;
    void               *msgwin;
    void               *statwin;
    void               *gamewin;
};

#endif /* MJOLNIR_TTY */

#endif /* __MJOLNIR_TTY_H__ */

