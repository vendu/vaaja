#include <stdlib.h>
#include <stdio.h>
#include <ui/conf.h>
#include <ui/ui.h>
#include <ui/sys/xorg.h>
#include <ui/sys/zero.h>

void
uisetsys(struct ui *ui, long type)
{
#if (UI_ENV_ZERO)
    struct uienv_zero *zenv;
#endif
#if (UI_ENV_XORG)
    struct uienv_xorg *xenv;
#endif
    struct uiapi      *api;

    api = calloc(1, sizeof(struct uiapi));
    if (!api) {
        fprintf(stderr, "UI: failed to allocate Xorg API\n");
        
        exit(1);
    }
    ui->api = api;
    switch (type) {
#if (UI_ENV_ZERO)
        case UI_SYS_ZERO:
            zenv = calloc(1, sizeof(struct uienv_zero));
            if (!zenv) {
                fprintf(stderr, "UI: failed to allocate Zero environment\n");
                
                exit(1);
            }
            ui->env = zenv;
            uiinitapi_zero(ui);
            
            break;
#endif
#if (UI_ENV_XORG)
        case UI_SYS_XORG:
            xenv = calloc(1, sizeof(struct uienv_xorg));
            if (!xenv) {
                fprintf(stderr, "UI: failed to allocate Xorg environment\n");

                exit(1);
            }
            ui->env = xenv;
            uiinitapi_xorg(ui);

            break;
#endif /* UI_ENV_XORG */
        default:
            fprintf(stderr, "UI: unknown system type %ld\n", type);

            exit(1);
    }
    ui->type = type;

    return;
}

void
uiinit(struct ui *ui, int argc, char *argv[])
{
    struct uiapi *api = ui->api;
    uiinit_t     *func = api->init;

    if (func) {
        func(ui, argc, argv);
    }

    return;
}

void *
uiinitcolors(struct ui *ui, int32_t *tab, size_t n)
{
    struct uiapi   *api = ui->api;
    uiinitcolors_t *func = api->initcolors;
    void           *map = NULL;

    if (func) {
        map = func(ui, tab, n);
    }

    return map;
}

struct uifont *
uiinitfont(struct ui *ui, struct uifont *font, char *name)
{
    struct uiapi   *api = ui->api;
    uiinitfont_t   *func = api->initfont;
    struct uifont *retfont = NULL;

    if (func) {
        retfont = func(ui, font, name);
    }

    return retfont;
}

