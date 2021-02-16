#ifndef __GFX_RGB_MSB_H__
#define __GFX_RGB_MSB_H__

/* big-endian RGB888-color support */

#define GFX_ALPHA_OFS            0
#define GFX_RED_OFS              8
#define GFX_GREEN_OFS           16
#define GFX_BLUE_OFS            24

#define GFX_RGB888_RED_OFS       0
#define GFX_RGB888_GREEN_OFS     1
#define GFX_RGB888_BLUE_OFS      2

#define gfxsetrgb888(pix, ptr)                                          \
    (((uint8_t *)(ptr))[0] = gfxgetred(pix),                            \
     ((uint8_t *)(ptr))[1] = gfxgetgreen(pix),                          \
     ((uint8_t *)(ptr))[2] = gfxgetblue(pix))

#define gfxsetrgb888_p(pix, ptr)                                        \
    (((uint8_t *)(ptr))[0] = gfxgetred_p(pix),                          \
     ((uint8_t *)(ptr))[1] = gfxgetgreen_p(pix),                        \
     ((uint8_t *)(ptr))[2] = gfxgetblue_p(pix))

struct gfxpix32 {
    uint8_t alpha;
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

#endif __GFX_RGB_MSB_H__

