#ifndef __UI_UI_H__
#define __UI_UI_H__

#include <ui/conf.h>
#include <ui/win.h>

#define UI_SYS_ZERO 0
#define UI_SYS_XORG 1

struct ui {
    long  type;
    void *env;
    void *api;
};

#if (UI_ENV_ZERO)
#include <ui/sys/zero.h>
#endif
#if (UI_ENV_XORG)
#include <ui/sys/xorg.h>
#endif

struct uifont {
    char *name;         // font name
    void *data;         // font information structure
    long  ascent;       // font ascent in pixels
    long  descent;      // font descent in pixels
    long  boxw;         // bounding box width in pixels
    long  boxh;         // bounding box height in pixels
};

void            uisetsys(struct ui *ui, long type);
void            uiinit(struct ui *ui, int argc, char *argv[]);
void          * uiinitcolors(struct ui *ui, int32_t *tab, size_t n);
struct uifont * uiinitfont(struct ui *ui, struct uifont *font, char *name);

typedef void            uiinit_t(struct ui *, int, char **);
typedef void          * uiinitcolors_t(struct ui *, int32_t *, size_t);
typedef struct uifont * uiinitfont_t(struct ui *, struct uifont *, char *);
typedef void            uifreefont_t(struct ui *, struct uifont *);
struct uiapi {
    uiinit_t       *init;
    uiinitcolors_t *initcolors;
    uiinitfont_t   *initfont;
    uifreefont_t   *freefont;
};

#endif /* __UI_UI_H__ */

