#include <gfx/types.h>

void
drawline(struct image *dest,
         coord_t x1,
         coord_t y1,
         coord_t x2,
         coord_t y2,
         argb_t pixel)
{
    argb_t  *destptr1;
    argb_t  *destptr2;
    coord_t  x;
    coord_t  y;
    coord_t  xmin;
    coord_t  xmax;
    coord_t  ybase;
    coord_t  ylim;
    coord_t  xpos;
    coord_t  destw;
    coord_t  dir;
    float    slope;

    destptr1 = dest->data;
    destw = dest->w;
    xmin = min2(x1, x2);
    xmax = max2(x1, x2);
    if (xmin == x1) {
        ybase = y1;
        ylim = y2;
    } else {
        ybase = y2;
        ylim = y1;
    }
    if (ylim < ybase) {
        dir = -1;
    } else {
        dir = 1;
    }
    xpos = 0;
    y = ybase;
    slope = ((float)ylim - (float)ybase) / ((float)xmax - (float)xmin);
    for (x = xmin ; x <= xmax ; x++) {
        ylim = ybase + (coord_t)((float)xpos * slope);
        destptr2 = destptr1 + y * destw + x;
        while (y != ylim) {
            *destptr2 = pixel;
            y += dir;
            destptr2 += destw;
        }
        xpos++;
    }

    return;
}

