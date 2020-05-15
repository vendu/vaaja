#include <mjolnir/conf.h>

#if (MJOLNIR_X11)

struct mjolscrx11 {
    Display *disp;
    Window  *gamewin;
    Window  *msgwin;
    Window  *statwin;
};

int
mjolgetch(void)
{
    XEvent ev;
    int    retval;

    XNextEvent(mjolscrx11.display, &ev);
    if (XFilterEvent(&ev, None)) {
        XUngrabKeyboard(mjolsrc11.display, CurrentTime);
    } else {
        /* FIXME: do the stuff */
    }

    return retval;
}

#endif

