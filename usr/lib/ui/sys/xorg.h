#ifndef __UI_SYS_XORG_H__
#define __UI_SYS_XORG_H__

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysymdef.h>

void uiinitapi_xorg(struct ui *ui);

#define UIXORGNDEFEV    LASTEvent
#define UIXORGROOTWINID None

struct uienv_xorg {
    Display       *display;
    Visual        *visual;
    Colormap       colormap;
    int            screen;
    int            depth;
    struct uiwin **wintab;
    Atom           wmdelete;
};

struct uifont_xorg {
    XFontStruct *font;          // font information structure
    long         ascent;        // font ascent in pixels
    long         descent;       // font descent in pixels
    long         boxw;          // bounding box width in pixels
    long         boxh;          // bounding box height in pixels
};

#endif /* __UI_SYS_XORG_H__ */

