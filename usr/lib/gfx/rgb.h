#ifndef __GFX_RGB_H__
#define __GFX_RGB_H__

#include <stdint.h>
#include <endian.h>
#include <env/cdefs.h>
#include <mach/param.h>

/* supported pixel formats */
#define GFX_ARGB32              1
#define GFX_RGB888              2
#define GFX_RGB555              3
#define GFX_RGB565              4
#define GFX_RGB444              5

#if (__BYTE_ORDER == __LITTLE_ENDIAN)

#include <gfx/rgb-lsb.h>

#elif (__BYTE_ORDER == __BIG_ENDIAN)

#include <gfx/rgb-msb.h>

#endif /* __BYTE_ORDER */

typedef int32_t                 gfxargb32;
typedef int16_t                 gfxrgb555;
typedef int16_t                 gfxrgb565;
typedef int16_t                 gfxrgb444;

#define GFX_BLACK               0x00000000
#define GFX_WHITE               0xffffffff
//#define GFX_GREEN     0x0000bf00
#define GFX_NO_ALPHA            0

/* pix is 32-bit word */
#define gfxgetalpha(pix)        ((pix) >> GFX_ALPHA_OFS)                // alpha
#define gfxgetred(pix)          (((pix) >> GFX_RED_OFS) & 0xff)         // red
#define gfxgetgreen(pix)        (((pix) >> GFX_GREEN_OFS) & 0xff)       // green
#define gfxgetblue(pix)         (((pix) >> GFX_BLUE_OFS) & 0xff)        // blue

/* pointer version; faster byte-fetches from memory */
#define gfxgetalpha_p(ptr)      (((struct gfxpix32 *)(ptr))->alpha)
#define gfxgetred_p(ptr)        (((struct gfxpix32 *)(ptr))->red)
#define gfxgetgreen_p(ptr)      (((struct gfxpix32 *)(ptr))->green)
#define gfxgetblue_p(ptr)       (((struct gfxpix32 *)(ptr))->blue)

/* approximation for c / 0xff */
#define gfxdiv255(c)            ((((c) << 8) + (c) + 256) >> 16)
/* simple division by 256 by bitshift */
//#define gfxdiv256(c)            ((c) >> 8)
#define gfxalphablendc(src, dest, a)                                   \
    ((dest) + gfxdiv255(((src) - (dest)) * (a)))
#define gfxalphablendc2(src, dest, a)                                  \
    ((dest) + gfxdiv256(((src) - (dest)) * (a)))
#define gfxalphablendcf(src, dest, a)                                  \
    ((dest) + (((src) - (dest)) * (a)) / 255.0)

/* compose pixel value from components */
#define gfxmkpix(a, r, g, b)    (((a) << GFX_ALPHA_OFS)                \
                                 | ((r) << GFX_RED_OFS)                \
                                 | ((g) << GFX_GREEN_OFS)              \
                                 | ((b) << GFX_BLUE_OFS))
#if 0
#define gfxmkpix_p(dest, a, r, g, b)                                   \
    ((dest) = gfxmkpix(a, r, g, b))
#endif
#define gfxsetpix_p(p, a, r, g, b)                                     \
    (((struct gfxpix32 *)(ptr))->alpha = (a),                          \
     ((struct gfxpix32 *)(ptr))->red = (r),                            \
     ((struct gfxpix32 *)(ptr))->green = (g),                          \
     ((struct gfxpix32 *)(ptr))->blue = (b))

#define GFX_ARGB32_ALPHA_MASK    0xff000000
#define GFX_ARGB32_RED_MASK      0x00ff0000
#define GFX_ARGB32_GREEN_MASK    0x0000ff00
#define GFX_ARGB32_BLUE_MASK     0x000000ff

#define GFX_RGB888_RED_MASK      0x00ff0000
#define GFX_RGB888_GREEN_MASK    0x0000ff00
#define GFX_RGB888_BLUE_MASK     0x000000ff

#define GFX_RGB555_RED_MASK      0x00007c00
#define GFX_RGB555_GREEN_MASK    0x000003e0
#define GFX_RGB555_BLUE_MASK     0x0000001f
#define GFX_RGB555_MASK          0x00007fff

#define GFX_RGB555_RED_SHIFT     7
#define GFX_RGB555_GREEN_SHIFT   2
#define GFX_RGB555_BLUE_SHIFT   -3

#define GFX_RGB565_RED_MASK      0x0000f800
#define GFX_RGB565_GREEN_MASK    0x000007e0
#define GFX_RGB565_BLUE_MASK     0x0000001f
#define GFX_RGB565_MASK          0x0000ffff

#define GFX_RGB565_RED_SHIFT     8
#define GFX_RGB565_GREEN_SHIFT   3
#define GFX_RGB565_BLUE_SHIFT   -3

#define GFX_RGB444_RED_MASK      0x0f00
#define GFX_RGB444_GREEN_MASK    0x00f0
#define GFX_RGB444_BLUE_MASK     0x000f

#define GFX_RGB444_RED_SHIFT     4
#define GFX_RGB444_GREEN_SHIFT   4
#define GFX_RGB444_BLUE_SHIFT    4

#define gfxtoc(pix, m, s)        ((s) > 0                               \
                                  ? (((pix) >> (s)) & (m))              \
                                  : (((pix) << -(s)) & (m)))
#define gfxtopix(dst, pix)       (gfxto##dst(pix))
#define gfxtopix32(pix)          (pix)

#define gfxtorgb555(pix)         (gfxtoc(gfxgetred(pix),                \
                                         GFX_RGB555_RED_MASK,           \
                                         GFX_RGB555_RED_SHIFT)          \
                                  | gfxtoc(gfxgetgreen(pix),            \
                                           GFX_RGB555_GREEN_MASK,       \
                                           GFX_RGB555_GREEN_SHIFT)      \
                                  | gfxtoc(gfxgetblue(pix),             \
                                           GFX_RGB555_BLUE_MASK,        \
                                           GFX_RGB555_BLUE_SHIFT))

#define gfxtorgb565(pix)         (gfxtoc(gfxgetred(pix),                \
                                         GFX_RGB565_RED_MASK,           \
                                         GFX_RGB565_RED_SHIFT)          \
                                  | gfxtoc(gfxgetgreen(pix),            \
                                           GFX_RGB565_GREEN_MASK,       \
                                           GFX_RGB565_GREEN_SHIFT)      \
                                  | gfxtoc(gfxgetblue(pix),             \
                                           GFX_RGB565_BLUE_MASK,        \
                                           GFX_RGB565_BLUE_SHIFT))

#define gfxtorgb444(pix)         (gfxtoc(gfxgetred(pix),                \
                                         GFX_RGB444_RED_MASK,           \
                                         GFX_RGB444_RED_SHIFT)          \
                                  | gfxtoc(gfxgetgreen(pix),            \
                                           GFX_RGB444_GREEN_MASK,       \
                                           GFX_RGB444_GREEN_SHIFT)      \
                                  | gfxtoc(gfxgetblue(pix),             \
                                           GFX_RGB444_BLUE_MASK,        \
                                           GFX_RGB444_BLUE_SHIFT))

#define GFX_COPY_ALPHA          256
#define gfxsetpix32_p(pix, ptr, aval)                                   \
    do {                                                                \
        gfxpix32         __pix = (pix);                                 \
        struct gfxpix32 *__src = (struct gfxpix32 *)&__pix;             \
        struct gfxpix32 *__dest = (struct gfxpix32 *)(ptr);             \
                                                                        \
        __dest->red = __src->red;                                       \
        __dest->green = __src->green;                                   \
        __dest->blue = __src->blue;                                     \
        if ((aval) == GFX_COPY_ALPHA) {                                 \
            __dest->alpha = __src->alpha;                               \
        } else {                                                        \
            __dest->alpha = (aval);                                     \
        }                                                               \
    } while (0)

#endif /* __GFX_RGB_H__ */

