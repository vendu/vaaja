#ifndef __COREWAR_X11_H__
#define __COREWAR_X11_H__

#include <corewar/conf.h>

#if defined(ZEUS) && defined(ZEUSX11)

#if defined(ZEUSIMLIB2)
#include <Imlib2.h>
#endif

#define ZEUSRUNBUTTON   0
#define ZEUSBRKBUTTON   1
#define ZEUSSTOPBUTTON  2
#define ZEUSSTEPBUTTON  3
#define ZEUSSTEPIBUTTON 4
#define ZEUSLOADBUTTON  5
#define ZEUSEDITBUTTON  6
#define ZEUSSAVEBUTTON  7
#define ZEUSNBUTTON     8

struct zeusx11win {
    Display *disp;
    Window   id;
};

#define ZEUSBUTTONNORMAL 0
#define ZEUSBUTTONHOVER  1
#define ZEUSBUTTONCLICK  2
#define ZEUSNBUTTONSTATE 3

struct zeusx11 {
    Display               *disp;
    Pixmap                 pixbuf;
    Window                 parentwin;
    int                    x;
    int                    y;
    int                    w;
    int                    h;
    Window                 mainwin;
    Window                 simwin;
    Window                 buttonwin;
    Window                 db1win;
    Window                 db2win;
#if defined(ZEUSHOVERTOOLTIP)
    Window                 tipwin;
#endif
    int                    simw;
    int                    simh;
    int                    textw;
    int                    texth;
    int                    dbw;
    int                    dbh;
    GC                     bggc;
    GC                     selgc;
    GC                     buttongc;
    GC                     datgc;
    GC                     textgc;
    GC                     prog1gc;
    GC                     prog1datgc;
    GC                     prog2gc;
    GC                     prog2datgc;
    XFontStruct           *font;
    int                    fontw;
    int                    fonth;
    int                    fontasc;
    int                    depth;
    Colormap               colormap;
    Visual                *visual;
    int                    screen;
    Window               **buttons;
#if defined(ZEUSHOVERTOOLTIP)
    char                  *tipstr;
    int                    tiplen;
#endif
};

typedef void zeusx11buttonfunc(struct zeusx11 *, XEvent *);

struct zeusx11buttonfuncs {
    zeusx11buttonfunc *enter;
    zeusx11buttonfunc *leave;
    zeusx11buttonfunc *click;
    zeusx11buttonfunc *release;
    zeusx11buttonfunc *expose;
};

struct zeusx11buttonpmaps {
    Pixmap norm;
    Pixmap normmask;
    Pixmap hover;
    Pixmap hovermask;
    Pixmap click;
    Pixmap clickmask;
};

struct zeusx11buttons {
    Window                     wins[ZEUSNBUTTON];
    char                      *strs[ZEUSNBUTTON];
    int                        strlens[ZEUSNBUTTON];
    long                       exposed[ZEUSNBUTTON];
    zeusx11buttonfunc         *functab[ZEUSNBUTTON];
    struct zeusx11buttonpmaps  pmaps;
    struct zeusx11buttonfuncs  funcs;
};

void   zeusinitx11(struct zeusx11 *info);
void   zeusprocev(struct zeusx11 *x11);
void   zeusdrawdb(struct zeusx11 *x11, long pc);
void   zeusdrawsim(struct zeusx11 *x11);

void   zeusenterx11button(struct zeusx11 *x11, XEvent *event);
void   zeusleavex11button(struct zeusx11 *x11, XEvent *event);
void   zeusclickx11button(struct zeusx11 *x11, XEvent *event);
void   zeusreleasex11button(struct zeusx11 *x11, XEvent *event);
void   zeusexposex11button(struct zeusx11 *x11, XEvent *event);

#endif /* defined(ZEUS) && defined(ZEUSX11) */

#endif /* __COREWAR_X11_H__ */

