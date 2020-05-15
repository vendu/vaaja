#ifndef __MJOLNIR_VGA_H__
#define __MJOLNIR_VGA_H__

#include <mjolnir/conf.h>

#if (MJOLNIR_VGA_TEXT)

#include <kern/io/drv/pc/vga.h>
#include <zero/ev.h>

#define mjolscrmove(scr, x, y) ((scr)->x = (x), (scr)->y = (y))
struct mjolvgascreen {
    long      x;        // screen X-coordinate
    long      y;        // screen Y-coordinate
    uint16_t  bgcolor;  // current background color
    uint16_t  fgcolor;  // current foreground color
    uint16_t *textbuf;  // VGA text buffer
};

#endif /* MJOLNIR_VGA_TEXT */

#endif /* __MJOLNIR_VGA_H__ */

