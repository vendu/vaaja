#ifndef __UI_ZERO_H__
#define __UI_ZERO_H__

#include <stddef.h>
#include <stdint.h>

struct uienv_zero {
    long  consid;       // console ID
    void *bgadr;        // [optional] background image data
    void *bufadr;       // draw buffer
    void *fbadr;        // frame buffer
};

void   uiinitapi_zero(struct ui *ui);
void * uiinitcolors_zero(struct ui *, int32_t *tab, size_t n);

#endif /* __UI_ZERO_H__ */

