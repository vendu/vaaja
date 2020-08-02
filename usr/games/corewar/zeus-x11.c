#include <corewar/conf.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <zero/cdefs.h>
#include <mach/param.h>
#include <zero/trix.h>
#include <corewar/cw.h>
#include <corewar/rc.h>
#include <corewar/zeus.h>
#if defined(ZEUSWINX11)
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/keysymdef.h>
#include <X11/Xutil.h>
#include <corewar/x11.h>
#endif
#if defined(ZEUSIMLIB2)
#include <Imlib2.h>
#endif

extern struct cwmars    g_cwmars;
extern struct zeussel   g_zeussel;

struct zeusx11buttons   g_x11buttons;

void zeusdrawsimop(struct zeusx11 *x11, long pc);

void
zeusrun(C_UNUSED struct zeusx11 *x11, C_UNUSED XEvent *event)
{
    long pid = g_cwmars.curpid;

    g_cwmars.running = 1;
    while ((g_cwmars.running) && (g_cwmars.nturn[pid])) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
        g_cwmars.nturn[pid]--;
    }
    if (!g_cwmars.nturn[pid]) {
        fprintf(stderr, "TIE\n");
        sleep(5);

        exit(0);
    }

    return;
}

void
zeusstop(C_UNUSED struct zeusx11 *x11, C_UNUSED XEvent *event)
{
    g_cwmars.running = 0;

    return;
}

void
zeusstep(C_UNUSED struct zeusx11 *x11, C_UNUSED XEvent *event)
{
    long pid = g_cwmars.curpid;

    g_cwmars.running = 0;
    if (g_cwmars.nturn[pid]--) {
        cwexec(pid);
        pid++;
        pid &= 0x01;
        g_cwmars.curpid = pid;
    } else {
        fprintf(stderr, "TIE\n");
        sleep(5);
    }

    return;
}

void
zeustogglesel(struct zeusx11 *x11, XEvent *event)
{
    int  x = event->xbutton.x;
    int  y = event->xbutton.y;
    long pc;

    x /= 5;
    y /= 5;
//    pc = y * (x11->w / 5) + x;
    pc = y * (x11->simw / 5) + x;
    if (!g_zeussel.bmap) {
        g_zeussel.bmap = calloc(CWCORESIZE >> 3, sizeof(uint8_t));
    }
    if (!g_zeussel.bmap) {
        fprintf(stderr, "memory allocation failure\n");
    } else if (bitset(g_zeussel.bmap, pc)) {
        clrbit(g_zeussel.bmap, pc);
    } else {
        setbit(g_zeussel.bmap, pc);
    }
    g_zeussel.last = pc;
    zeusdrawsimop(x11, pc);

    return;
}

void
zeusaddsel(struct zeusx11 *x11, XEvent *event)
{
    int  x = event->xbutton.x;
    int  y = event->xbutton.y;
    long lim;
    long pc;

    x /= 5;
    y /= 5;
//    pc = y * (x11->w / 5) + x;
    pc = y * (x11->simw / 5) + x;
    if (!g_zeussel.bmap) {
        g_zeussel.bmap = calloc(CWCORESIZE >> 3, sizeof(uint8_t));
    }
    if (!g_zeussel.bmap) {
        fprintf(stderr, "memory allocation failure\n");
    } else if (g_zeussel.last >= 0) {
        if (pc < g_zeussel.last) {
            lim = g_zeussel.last + 1;
        } else {
            lim = pc;
            pc = g_zeussel.last + 1;
        }
        while (pc < lim) {
            setbit(g_zeussel.bmap, pc);
            zeusdrawsimop(x11, pc);
            pc++;
        }
        g_zeussel.last = pc;
    }

    return;
}

void
zeusclear(struct zeusx11 *x11, C_UNUSED XEvent *event)
{
    if (g_zeussel.bmap) {
        memset(g_zeussel.bmap, 0, CWCORESIZE >> 3);
    }
    g_zeussel.last = -1;
    zeusdrawsim(x11);
}

C_PURE
int
zeusfindbutton(Window win)
{
    int    id;

    for (id = 0 ; id < ZEUSNBUTTON ; id++) {
        if (g_x11buttons.wins[id] == win) {

            break;
        }
    }

    return id;
}

char *
zeusbuttonstring(Window win, int *lenret)
{
    char  *str = NULL;
    int    id = zeusfindbutton(win);

    if (id < ZEUSNBUTTON) {
        str = g_x11buttons.strs[id];
        if (str) {
            *lenret = g_x11buttons.strlens[id];
        }
    }

    return str;
}

void
zeusenterx11button(struct zeusx11 *x11, XEvent *event)
{
    Window  win = event->xany.window;
    int     len;
    char   *str = zeusbuttonstring(win, &len);

    if (str) {
        XSetWindowBackgroundPixmap(x11->disp,
                                   win,
                                   g_x11buttons.pmaps.hover);
        XClearWindow(x11->disp, win);
        XDrawString(x11->disp,
                    win,
                    x11->buttongc,
                    (ZEUSBUTTONW - len * x11->fontw) >> 1,
                    (ZEUSBUTTONH + x11->fontasc) >> 1,
                    str,
                    len);
        XSync(x11->disp, False);
    }

    return;
}

void
zeusleavex11button(struct zeusx11 *x11, XEvent *event)
{
    Window  win = event->xany.window;
    int     len;
    char   *str = zeusbuttonstring(win, &len);

    if (str) {
        XSetWindowBackgroundPixmap(x11->disp,
                                   win,
                                   g_x11buttons.pmaps.norm);
        XClearWindow(x11->disp, win);
        XDrawString(x11->disp,
                    win,
                    x11->buttongc,
                    (ZEUSBUTTONW - len * x11->fontw) >> 1,
                    (ZEUSBUTTONH + x11->fontasc) >> 1,
                    str,
                    len);
        XSync(x11->disp, False);
    }

    return;
}

void
zeusclickx11button(struct zeusx11 *x11, XEvent *event)
{
    Window             win = event->xany.window;
    int                id = zeusfindbutton(win);
    int                len;
    char              *str = zeusbuttonstring(win, &len);
    zeusx11buttonfunc *func;

    if (str) {
        win = event->xany.window;
        func = g_x11buttons.functab[id];
        func(x11, event);
        XSetWindowBackgroundPixmap(x11->disp,
                                   win,
                                   g_x11buttons.pmaps.click);
        XClearWindow(x11->disp, win);
        XDrawString(x11->disp,
                    win,
                    x11->buttongc,
                    (ZEUSBUTTONW - len * x11->fontw) >> 1,
                    (ZEUSBUTTONH + x11->fontasc) >> 1,
                    str,
                    len);
        XSync(x11->disp, False);
    }

    return;
}

void
zeusreleasex11button(struct zeusx11 *x11, XEvent *event)
{
    Window          win = event->xany.window;
    int             len;
    char           *str = zeusbuttonstring(win, &len);

    if (str) {
        win = event->xany.window;
        XSetWindowBackgroundPixmap(x11->disp,
                                   win,
                                   g_x11buttons.pmaps.hover);
        XClearWindow(x11->disp, win);
        XDrawString(x11->disp,
                    win,
                    x11->buttongc,
                    (ZEUSBUTTONW - len * x11->fontw) >> 1,
                    (ZEUSBUTTONH + x11->fontasc) >> 1,
                    str,
                    len);
        XSync(x11->disp, False);
    }

    return;
}

void
zeusexposex11button(struct zeusx11 *x11, XEvent *event)
{
    Window  win = event->xany.window;
    int     len;
    int     id = zeusfindbutton(win);
    char   *str = zeusbuttonstring(win, &len);

    if (!event->xexpose.count) {
        win = event->xany.window;
        if (!g_x11buttons.exposed[id]) {
            XSetWindowBackgroundPixmap(x11->disp, win, g_x11buttons.pmaps.norm);
            g_x11buttons.exposed[id] = 1;
        }
        XClearWindow(x11->disp, win);
        XDrawString(x11->disp,
                    win,
                    x11->buttongc,
                    (ZEUSBUTTONW - len * x11->fontw) >> 1,
                    (ZEUSBUTTONH + x11->fontasc) >> 1,
                    str,
                    len);
        XSync(x11->disp, False);
    }

    return;
}

void
zeusinitx11font(struct zeusx11 *x11)
{
    XFontStruct *font;

    font = XLoadQueryFont(x11->disp, "fixed");
    if (font) {
        x11->font = font;
        x11->fontw = font->max_bounds.width;
        x11->fonth = font->ascent + font->descent;
        x11->fontasc = font->ascent;
    } else {
        fprintf(stderr, "failed to load font\n");

        exit(1);
    }

    return;
}

void
zeusinitx11win(struct zeusx11 *x11)
{
    XSetWindowAttributes atr = { 0 };
    Window               win;
    Window               parent = (x11->parentwin != None
                                   ? x11->parentwin
                                   : RootWindow(x11->disp, x11->screen));
    int                  winw = ZEUSSIMNCOL * 5 + ZEUSBUTTONW;
    int                  winh = ZEUSSIMNROW * 5 + ZEUSDBNROW * x11->fonth;
    int                  x = x11->x;
    int                  y = x11->y;

    atr.background_pixel = BlackPixel(x11->disp, x11->screen);
    win = XCreateWindow(x11->disp,
                        parent,
                        x, y,
                        winw, winh,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create main window\n");

        exit(1);
    }
    x11->mainwin = win;
    x11->w = winw;
    x11->h = winh;
    parent = win;

    x11->simw = ZEUSSIMNCOL * 5;
    x11->simh = ZEUSSIMNROW * 5;

#if defined(ZEUSIMLIB2)
    win = XCreateWindow(x11->disp,
                        x11->mainwin,
//                        x11->simw, x11->dbh,
                        x11->simw, y,
                        ZEUSBUTTONW, ZEUSNBUTTON * ZEUSBUTTONH,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create main window\n");

        exit(1);
    }
    x11->buttonwin = win;
#if defined(ZEUSDEBUG)
    XSelectInput(x11->buttonwin, ExporuseMask);
#endif
#endif

    winw = ZEUSSIMNCOL * 5;
    winh = ZEUSSIMNROW * 5;
    win = XCreateWindow(x11->disp,
                        x11->mainwin,
                        x, y,
                        winw, winh,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create simulation window\n");

        exit(1);
    }
    x11->simwin = win;
    y = x11->simh;
#if defined(ZEUSIMLIB2)
//    y += ZEUSBUTTONH;
//    x += winw;
    x = 0;
#endif

    winw = ZEUSDBNCOL * x11->fontw;
    winh = ZEUSDBNROW * x11->fonth;
    win = XCreateWindow(x11->disp,
                        x11->mainwin,
                        x, y,
                        winw, winh,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create debug window\n");

        exit(1);
    }
    x11->db1win = win;
    x11->dbw = winw;
    x11->dbh = winh;
    win = XCreateWindow(x11->disp,
                        x11->mainwin,
                        x + winw, y,
                        winw, winh,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create debug window\n");

        exit(1);
    }
    x11->db2win = win;

    return;
}

void
zeusinitx11title(struct zeusx11 *x11)
{
    XTextProperty  prop;
    const char    *str = "Zero MARS";

    XStringListToTextProperty((char **)&str, 1, &prop);
    XSetWMName(x11->disp, x11->mainwin, &prop);
    XFree(prop.value);

    return;
}

void
zeusinitx11gc(struct zeusx11 *x11)
{
    XGCValues       gcval;
    GC              gc;
    XColor          color;

    gcval.foreground = BlackPixel(x11->disp, DefaultScreen(x11->disp));
    gcval.graphics_exposures = False;
    gcval.font = x11->font->fid;
    gcval.line_width = 1;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->bggc = gc;
    gcval.foreground = WhitePixel(x11->disp, DefaultScreen(x11->disp));
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->selgc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     ZEUSBUTTONTEXTCOLOR,
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    if (!XAllocColor(x11->disp,
                     x11->colormap,
                     &color)) {
        fprintf(stderr, "failed to allocate color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->buttongc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     ZEUSDATCOLOR,
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    if (!XAllocColor(x11->disp,
                     x11->colormap,
                     &color)) {
        fprintf(stderr, "failed to allocate color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->datgc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     ZEUSPROG1COLOR,
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    if (!XAllocColor(x11->disp,
                     x11->colormap,
                     &color)) {
        fprintf(stderr, "failed to allocate color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->prog1gc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     ZEUSPROG1DATCOLOR,
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    if (!XAllocColor(x11->disp,
                     x11->colormap,
                     &color)) {
        fprintf(stderr, "failed to allocate color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->prog1datgc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     ZEUSPROG2COLOR,
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    if (!XAllocColor(x11->disp,
                     x11->colormap,
                     &color)) {
        fprintf(stderr, "failed to allocate color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->prog2gc = gc;
    if (!XParseColor(x11->disp,
                     x11->colormap,
                     ZEUSPROG2DATCOLOR,
                     &color)) {
        fprintf(stderr, "failed to parse color\n");

        exit(1);
    }
    if (!XAllocColor(x11->disp,
                     x11->colormap,
                     &color)) {
        fprintf(stderr, "failed to allocate color\n");

        exit(1);
    }
    gcval.foreground = color.pixel;
    gc = XCreateGC(x11->disp, x11->mainwin,
                   GCForeground | GCLineWidth | GCFont,
                   &gcval);
    if (!gc) {
        fprintf(stderr, "failed to create GC\n");

        exit(1);
    }
    x11->prog2datgc = gc;

    return;
}

#if defined(ZEUSIMLIB2)

void
zeusinitimlib2(struct zeusx11 *x11)
{
    imlib_context_set_display(x11->disp);
    imlib_context_set_drawable(x11->buttonwin);
    imlib_context_set_visual(x11->visual);
    imlib_context_set_colormap(x11->colormap);
    imlib_context_set_blend(0);
    imlib_context_set_mask(0);

    return;
}

void
zeusloadx11buttonimgs(C_UNUSED struct zeusx11 *x11)
{
    Imlib_Image *img;

    img = imlib_load_image("buttonnormal.png");
    if (!img) {
        fprintf(stderr, "failed to load button image NORMAL\n");

        exit(1);
    }
    imlib_context_set_image(img);
    imlib_render_pixmaps_for_whole_image_at_size(&g_x11buttons.pmaps.norm,
                                                 &g_x11buttons.pmaps.normmask,
                                                 ZEUSBUTTONW, ZEUSBUTTONH);
    img = imlib_load_image("buttonhilite.png");
    if (!img) {
        fprintf(stderr, "failed to load button image HIGHLIGHT\n");

        exit(1);
    }
    imlib_context_set_image(img);
    imlib_render_pixmaps_for_whole_image_at_size(&g_x11buttons.pmaps.hover,
                                                 &g_x11buttons.pmaps.hovermask,
                                                 ZEUSBUTTONW, ZEUSBUTTONH);

    img = imlib_load_image("buttonpress.png");
    if (!img) {
        fprintf(stderr, "failed to load button image PRESS\n");

        exit(1);
    }
    imlib_context_set_image(img);
    imlib_render_pixmaps_for_whole_image_at_size(&g_x11buttons.pmaps.click,
                                                 &g_x11buttons.pmaps.clickmask,
                                                 ZEUSBUTTONW, ZEUSBUTTONH);

    return;
}

#endif /* ZEUSIMLIB2 */

/* TODO: event selection */
void
zeusaddx11button(struct zeusx11 *x11, int id, const char *str,
                 zeusx11buttonfunc *func)
{
    Window               parent = x11->buttonwin;
    Window               win;
    XSetWindowAttributes atr = { 0 };

    if (!id) {
#if defined(ZEUSIMLIB2)
        zeusloadx11buttonimgs(x11);
#endif
        g_x11buttons.funcs.enter = zeusenterx11button;
        g_x11buttons.funcs.leave = zeusleavex11button;
        g_x11buttons.funcs.click = zeusclickx11button;
        g_x11buttons.funcs.release = zeusreleasex11button;
        g_x11buttons.funcs.expose = zeusexposex11button;
    }
    atr.background_pixel = BlackPixel(x11->disp, x11->screen);
    win = XCreateWindow(x11->disp,
                        parent,
//                        id * ZEUSBUTTONW, 0,
                        0, id * ZEUSBUTTONH,
                        ZEUSBUTTONW, ZEUSBUTTONH,
                        0,
                        x11->depth,
                        InputOutput,
                        x11->visual,
                        CWBackPixel,
                        &atr);
    if (!win) {
        fprintf(stderr, "failed to create button window\n");

        exit(1);
    }
    g_x11buttons.wins[id] = win;
    g_x11buttons.strs[id] = (char *)str;
    g_x11buttons.strlens[id] = strlen(str);
    g_x11buttons.functab[id] = func;
    XSelectInput(x11->disp,
                 win,
                 ExposureMask
                 | EnterWindowMask
                 | LeaveWindowMask
                 | ButtonPressMask
                 | ButtonReleaseMask);
    XMapRaised(x11->disp, win);
    XSync(x11->disp, False);

    return;
}

#if defined(ZEUSIMLIB2)
void
zeusinitx11buttons(struct zeusx11 *x11)
{
    zeusloadx11buttonimgs(x11);
}
#endif

void
zeusinitx11buf(struct zeusx11 *x11)
{
    Pixmap pmap = XCreatePixmap(x11->disp,
                                x11->mainwin,
                                x11->w,
                                x11->h,
                                x11->depth);

    if (!pmap) {
        fprintf(stderr, "failed to create buffer pixmap\n");

        exit(1);
    }
    XFillRectangle(x11->disp, pmap,
                   x11->bggc,
                   0, 0,
                   x11->w, x11->h);
    x11->pixbuf = pmap;

    return;
}

void
zeusinitx11(struct zeusx11 *info)
{
    Display *disp;

    XInitThreads();
    disp = XOpenDisplay(NULL);
    if (!disp) {
        fprintf(stderr, "failed to open display\n");

        exit(1);
    }
    info->disp = disp;
    info->screen = DefaultScreen(disp);
    info->colormap = DefaultColormap(disp, info->screen);
    info->depth = DefaultDepth(disp, info->screen);
    info->visual = DefaultVisual(disp, info->screen);
    zeusinitx11font(info);
    zeusinitx11win(info);
    zeusinitx11title(info);
    zeusinitx11gc(info);
#if defined(ZEUSIMLIB2)
    zeusinitimlib2(info);
    zeusinitx11buttons(info);
    XMapRaised(disp, info->mainwin);
    XMapRaised(disp, info->buttonwin);
    zeusinitx11buf(info);
    XSelectInput(disp, info->simwin,
                 KeyPressMask
                 | ButtonPressMask
                 | EnterWindowMask
                 | LeaveWindowMask
#if defined(ZEUSHOVERTOOLTIP)
                 | PointerMotionMask
#endif
                 | ExposureMask);
//        XMapRaised(disp, info->tipwin);
    XMapRaised(disp, info->simwin);
    XMapRaised(disp, info->db1win);
    XMapRaised(disp, info->db2win);
    zeusaddx11button(info, ZEUSRUNBUTTON, "run", zeusrun);
    zeusaddx11button(info, ZEUSBRKBUTTON, "brk", zeusrun);
    zeusaddx11button(info, ZEUSSTOPBUTTON, "stop", zeusstop);
    zeusaddx11button(info, ZEUSSTEPBUTTON, "step", zeusstep);
    //    zeusaddx11button(info, ZEUSSTEPIBUTTON, "stepi", zeusstep);
    zeusaddx11button(info, ZEUSLOADBUTTON, "load", zeusstep);
    zeusaddx11button(info, ZEUSEDITBUTTON, "edit", zeusstep);
    zeusaddx11button(info, ZEUSSAVEBUTTON, "save", zeusstep);
    g_zeussel.last = -1;
#endif

    return;
}

int
zeusprintop(struct zeusx11 *x11, long pc, int x, int y)
{
    struct cwinstr *op = &g_cwmars.optab[pc];
    long            pid = bitset(g_cwmars.pidmap, pc);
    Window          win = (pid) ? x11->db2win : x11->db1win;
    char           *str;
    int             len;

    str = zeusdisasm(pc, &len);
    if (str) {
        if (pid) {

        }
        if (op->op == CWOPDAT) {
            if (!*((uint64_t *)op)) {
                XDrawString(x11->disp, win, x11->datgc,
                            x, y,
                            str, len);
            } else if (op->pid) {
                XDrawString(x11->disp, win, x11->prog2datgc,
                            x, y,
                            str, len);
            } else {
                XDrawString(x11->disp, win, x11->prog1datgc,
                            x, y,
                            str, len);
            }
        } else if (op->pid) {
            XDrawString(x11->disp, win, x11->prog2gc,
                        x, y,
                        str, len);
        } else {
            XDrawString(x11->disp, win, x11->prog1gc,
                        x, y,
                        str, len);
        }
        free(str);
    }

    return len;
}

void
zeusprintdb(struct zeusx11 *x11, int simx, int simy)
{
    struct cwinstr *op;
    const char     *str = " <";
    int             slen = strlen(str);
    long            pid;
    Window          win;
    long            pc;
    int             len;
    int             i;

    simx /= 5;
    simy /= 5;
//    pc = simy * (x11->w / 5) + simx;
    pc = simy * (x11->simw / 5) + simx;
    pid = bitset(g_cwmars.pidmap, pc);
    if (pid) {
        win = x11->db2win;
    } else {
        win = x11->db1win;
    }
    XFillRectangle(x11->disp, win,
                   x11->bggc,
                   0, 0,
                   x11->dbw, x11->dbh);
    len = zeusprintop(x11, pc, 0, x11->fontasc);
    op = &g_cwmars.optab[pc];
    if (op->op == CWOPDAT) {
        if (!*((uint64_t *)op)) {
            XDrawString(x11->disp, win, x11->datgc,
                        len * x11->fontw, x11->fontasc,
                        str, slen);
        } else if (op->pid) {
            XDrawString(x11->disp, win, x11->prog2datgc,
                        len * x11->fontw, x11->fontasc,
                        str, slen);
        } else {
            XDrawString(x11->disp, win, x11->prog1datgc,
                        len * x11->fontw, x11->fontasc,
                        str, slen);
        }
    } else if (op->pid) {
        XDrawString(x11->disp, win, x11->prog2gc,
                    len * x11->fontw, x11->fontasc,
                    str, slen);
    } else {
        XDrawString(x11->disp, win, x11->prog1gc,
                    len * x11->fontw, x11->fontasc,
                    str, slen);
    }
    pc++;
    pc %= CWCORESIZE;
    for (i = 0 ; i < 15 ; i++) {
        zeusprintop(x11, pc, 0, (i + 2) * x11->fontasc);
        pc++;
        pc %= CWCORESIZE;
    }

    return;
}

void
zeusprocev(struct zeusx11 *x11)
{
    XEvent ev;
    Window win;
    int    id;

    XNextEvent(x11->disp, &ev);
    win = ev.xany.window;
#if defined(ZEUSIMLIB2)
    id = zeusfindbutton(win);
#endif
#if defined(ZEUSDEBUG)
    if (win == x11->buttonwin) {
        switch (ev.type) {
            case Expose:

                break;
            default:

                break;
        }
    } else
#endif
    if (win == x11->mainwin) {
        switch (ev.type) {
            case Expose:
                /* IGNORE */

                break;
            default:

                break;
            }
    } else if (win == x11->simwin) {
        switch (ev.type) {
#if defined(ZEUSHOVERTOOLTIP)
            case MotionNotify:
                zeusprintdb(x11, ev.xmotion.x, ev.xmotion.y);

                break;
            case EnterNotify:
                zeusprintdb(x11, ev.xcrossing.x, ev.xcrossing.y);

                break;
            case LeaveNotify:
                XUnmapWindow(x11->disp, x11->tipwin);

                break;
#endif
            case ButtonPress:
                if (ev.xbutton.button == Button1) {
                    if (ev.xbutton.state & ControlMask) {
                        zeustogglesel(x11, &ev);
                    } else if (ev.xbutton.state & ShiftMask) {
                        zeusaddsel(x11, &ev);
                    }
                    zeusprintdb(x11, ev.xbutton.x, ev.xbutton.y);
                }

                break;
            case Expose:
                if (!ev.xexpose.count) {
                    XCopyArea(x11->disp, x11->pixbuf, x11->simwin,
                              x11->bggc,
                              0, 0,
                              x11->simw, x11->simh,
                              0, 0);
                }

                break;
            case KeyPress:

                break;
            default:

                break;
        }
#if defined(ZEUSHOVERTOOLTIP)
    } else if (win == x11->tipwin) {
        switch (ev.type) {
            case Expose:
                if (!ev.xexpose.count) {
                    XDrawString(x11->disp, x11->tipwin, x11->textgc,
                                0, x11->fontasc,
                                x11->tipstr, x11->tiplen);
                }

                break;
            default:

                break;
        }
#endif
    } else if (win == x11->db1win || win == x11->db2win) {
        ;
#if defined(ZEUSIMLIB2)
    } else if (id < ZEUSNBUTTON) {
        switch (ev.type) {
            case Expose:
                g_x11buttons.funcs.expose(x11, &ev);

                break;
            case EnterNotify:
                g_x11buttons.funcs.enter(x11, &ev);

                break;
            case LeaveNotify:
                g_x11buttons.funcs.leave(x11, &ev);

                break;
            case ButtonPress:
                g_x11buttons.funcs.click(x11, &ev);

                break;
            case ButtonRelease:
                g_x11buttons.funcs.release(x11, &ev);

                break;
            default:

                break;
        }
#endif
    }

    return;
}

void
zeusdrawsimop(struct zeusx11 *x11, long pc)
{
    struct cwinstr *op;
    int             row = pc / ZEUSSIMNCOL;
    int             col = pc - row * ZEUSSIMNCOL;
    int             x = col * 5;
    int             y = row * 5;

    op = &g_cwmars.optab[pc];
    if ((g_zeussel.bmap) && bitset(g_zeussel.bmap, pc)) {
        XFillRectangle(x11->disp, x11->pixbuf,
                       x11->selgc,
                       x, y,
                       4, 4);
    } else if (op->op == CWOPDAT) {
        if (!*((uint64_t *)op)) {
            XFillRectangle(x11->disp, x11->pixbuf,
                           x11->datgc,
                           x, y,
                           4, 4);
        } else if (op->pid) {
            XFillRectangle(x11->disp, x11->pixbuf,
                           x11->prog2datgc,
                           x, y,
                           4, 4);
        } else {
            XFillRectangle(x11->disp, x11->pixbuf,
                           x11->prog1datgc,
                           x, y,
                           4, 4);
        }
    } else if (op->pid) {
        XFillRectangle(x11->disp, x11->pixbuf,
                       x11->prog2gc,
                       x, y,
                       4, 4);

    } else {
        XFillRectangle(x11->disp, x11->pixbuf,
                       x11->prog1gc,
                       x, y,
                       4, 4);
    }
    XCopyArea(x11->disp, x11->pixbuf, x11->simwin,
              x11->bggc,
              x, y,
              4, 4,
              x, y);

    return;
}

void
zeusdrawsim(struct zeusx11 *x11)
{
    long pc;

    for (pc = 0 ; pc < CWCORESIZE ; pc++) {
        zeusdrawsimop(x11, pc);
    }
    XCopyArea(x11->disp, x11->pixbuf, x11->simwin,
              x11->bggc,
              0, 0,
              x11->simw, x11->simh,
              0, 0);

    return;
}

