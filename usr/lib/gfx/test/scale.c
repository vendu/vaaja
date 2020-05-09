#include <stdio.h>
#include <zero/trix.h>
#include <gfx/types.h>

void
gfxscaleimg(struct gfximg *src, struct gfximg *dest,
            unsigned long srcx, unsigned long srcy,
            unsigned long destx, unsigned long desty,
            unsigned long w, unsigned long h)
{
    argb32_t *sptr;
    argb32_t *dptr;
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr1;
    unsigned long sx;
    unsigned long sy;
    unsigned long sw;
    unsigned long sh;
    unsigned long dx;
    unsigned long dy;
    unsigned long dw;
    unsigned long dh;
    unsigned long dxmax;
    unsigned long dymax;
    float         xmul;
    float         ymul;

    sptr = src->data;
    dptr = dest->data;
    sw = src->w;
    sh = src->h;
    dw = dest->w;
    dh = dest->h;
    xmul = (float)sw / w;
    ymul = (float)sh / h;
    dymax = min(desty + h, dh);
    dxmax = min(destx + w, dw);
    for (dy = desty ; dy < dymax ; dy++) {
        sy = dy * ymul;
        sptr1 = sptr + sy * sw + srcx;
        dptr1 = dptr + dy * dw + destx;
        for (dx = destx ; dx < dxmax ; dx++) {
            sx = srcx + dx * xmul;
            sptr2 = sptr1 + sx;
            *dptr1++ = *sptr2;
        }
    }

    return;
}
