#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define UI_ENV_XORG 1
#include <ui/ui.h>

char *
uigetdisp_xorg(int argc, char *argv[])
{
    char *dispname = NULL;
    int   ndx;

    for (ndx = 1 ; ndx < argc ; ndx++) {
        if (!strncmp(argv[ndx], "-D", 2)
            || !strncmp(argv[ndx], "--display", 9)) {
            ndx++;
            if (ndx < argc) {
                dispname = argv[ndx];
            }
        }
    }

    return dispname;
}

void
uiinit_xorg(struct ui *ui, int argc, char *argv[])
{
    struct uienv_xorg *env = ui->env;
    Display           *disp;
    char              *dispname = uigetdisp_xorg(argc, argv);
    int                i;

    fprintf(stderr, "UI: opening display %s\n", dispname);
    XInitThreads();
    disp = XOpenDisplay(dispname);
    if (!disp) {
        fprintf(stderr, "failed to open display %s\n", dispname);

        exit(1);
    }
    env->display = disp;
    i = DefaultScreen(disp);
    env->screen = i;
    env->depth = DefaultDepth(disp, i);
    env->visual = DefaultVisual(disp, i);
    env->colormap = DefaultColormap(disp, i);

    return;
}

void *
uiinitcolors_xorg(struct ui *ui, int32_t *tab, size_t n)
{
    struct uienv_xorg *env = ui->env;
    unsigned long     *data;
    unsigned long      pixel;
    long               ndx;
    XColor             color;

    data = malloc(n * sizeof(unsigned long));
    if (!data) {

        return NULL;
    }
    color.flags = DoRed | DoGreen | DoBlue;
    for (ndx = 0 ; ndx < 16 ; ndx++) {
        pixel = tab[ndx];
        color.red = pixel >> 16;
        color.green = (pixel >> 8) & 0xff;
        color.blue = pixel & 0xff;
        if (!XAllocColor(env->display,
                         env->colormap,
                         &color)) {
            free(data);
            
            return NULL;
        }
        data[ndx] = color.pixel;
    }

    return data;
}

struct uifont *
uiinitfont_xorg(struct ui *ui, struct uifont *font, char *name)
{
    struct uienv_xorg *env = ui->env;
    XFontStruct       *fontinfo;
    long               newfont = (font) ? 0 : 1;
    long               asc;
    long               desc;

    if (!font) {
        font = malloc(sizeof(struct uifont));
        if (!font) {
            fprintf(stderr, "UI: failed to allocate font\n");

            exit(1);
        }
    }
    if (name) {
        fprintf(stderr, "FONT: loading %s\n", name);
        fontinfo = XLoadQueryFont(env->display, name);
        if (!fontinfo) {
            if (newfont) {
                fprintf(stderr, "UI: failed to load font %s\n", name);
                free(font);

                return NULL;
            }
        }
        font->name = name;
        font->data = fontinfo;
        asc = fontinfo->ascent;
        desc = fontinfo->descent;
        font->ascent = asc;
        font->descent = desc;
        font->boxw = fontinfo->max_bounds.width;
        font->boxh = asc + desc;
        fprintf(stderr, "UI: loaded font %s, ascent == %ld, descent == %ld, boxw == %ld, boxh == %ld\n",
                name, asc, desc, font->boxw, font->boxh);
    }

    return font;
}

void
uifreefont_xorg(struct ui *ui, struct uifont *font)
{
    struct uienv_xorg *env = ui->env;

    if ((font) && (font->data)) {
        XFreeFont(env->display, font->data);
    }

    return;
}

void
uiinitapi_xorg(struct ui *ui)
{
    struct uiapi *api = ui->api;

    api->init = uiinit_xorg;
    api->initcolors = uiinitcolors_xorg;
    api->initfont = uiinitfont_xorg;
    api->freefont = uifreefont_xorg;

    return;
}

struct uiwin *
uimkwin_xorg(struct ui *ui,
             Window parent,
             int x, int y,
             int w, int h)
{
    Window                 id;
    struct uienv_xorg     *env = ui->env;
    struct uiwin          *win = malloc(sizeof(struct uiwin));
    void                 **evtab;
    XSetWindowAttributes   atr = { 0 };

    if (win) {
        if (!parent) {
            parent = RootWindow(env->display, env->screen);
        }
        id = XCreateWindow(env->display,
                           parent,
                           x, y,
                           w, h,
                           0,
                           env->depth,
                           InputOutput,
                           env->visual,
                           0,
                           &atr);
        if (!id) {
            free(win);
        }
        win->id = id;
        evtab = calloc(UIXORGNDEFEV, sizeof(void *));
        if (!evtab) {
            fprintf(stderr, "UI: failed to allocate function table\n");
            free(win);

            exit(1);
        }
        win->evtab = evtab;
    }

    return win;
}

#if defined(UITESTXORG) && (UITESTXORG)

#include <stdio.h>

int
main(int argc, char *argv[])
{
    struct ui     ui;
    struct uiwin *win;

    uiinit_xorg(&ui, argc, argv);
    win = uimkwin_xorg(&ui, UIXORGROOTWINID,
                      0, 0,
                      16, 16);
    fprintf(stderr, "%p\n", win);

    exit(0);
}

#endif /* UITESTXORG */

