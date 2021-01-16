#include <ui/ui.h>

void *
uiinitcolors_zero(struct ui *ui, int32_t *tab, size_t n)
{
    return tab;
}

void
uiinit_zero(struct ui *ui, int argc, char *argv[])
{
    /* TODO: allocate draw buffer, set background, map framebuffer, ... */

    return;
}

void
uiinitapi_zero(struct ui *ui)
{
    struct uiapi *api = ui->api;

    api->init = uiinit_zero;
    api->initcolors = uiinitcolors_zero;

    return;
}

