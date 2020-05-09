/*
 * macros.h - macros for Thunder Desktop Environment.
 * Copyright (C) 2004 Tuomo Venäläinen
 *
 * See the file COPYING for information about using this software.
 */

#ifndef __GFX_PIPE_H__
#define __GFX_PIPE_H__

#include <gfx/swap.h>

/*
 * Raster operations.
 *
 * NOTE: I have predefined the same operations the X Window System supports.
 */
#define GFX_COPY_OPERATION(s,d)          (s)
#define GFX_COPY_INVERTED_OPERATION(s,d) (~(s))
#define GFX_CLEAR_OPERATION(s,d)         (0)
#define GFX_SET_OPERATION(s,d)           (0xffffffff)
#define GFX_NO_OPERATION(s,d)            (d)
#define GFX_INVERT_OPERATION(s,d)        (~(d))
#define GFX_AND_OPERATION(s,d)           ((s) & (d))
#define GFX_AND_REVERSE_OPERATION(s,d)   ((s) & (~(d)))
#define GFX_AND_INVERTED_OPERATION(s,d)  ((~(s)) & (d))
#define GFX_OR_OPERATION(s,d)            ((s) | (d))
#define GFX_OR_INVERTED_OPERATION(s,d)   ((~(s)) | (d))
#define GFX_OR_REVERSE_OPERATION(s,d)    ((s) | (~(d)))
#define GFX_XOR_OPERATION(s,d)           ((s) ^ (d))
#define GFX_NOR_OPERATION(s,d)           ((~(s)) & (~(d)))
#define GFX_EQUIV_OPERATION(s,d)         ((~(s)) ^ (d))
#define GFX_NAND_OPERATION(s,d)          (~((s) & (d)))

/*
 * Check if real depth is 15 bpp.
 */
#define _GFX_RGB_DEPTH_15(v)                                            \
    (((v)->rmask | (v)->gmask | (v)->bmask) == _GFX_RGB_555_PIXEL_MASK)

/*
 * Graphics format detection.
 */
#define _GFX_SET_BYTE_ORDER()                                           \
    ((__BYTE_ORDER == __LITTLE_ENDIAN)                                  \
     ? _GFX_BYTE_ORDER_LSB                                              \
     : _GFX_BYTE_ORDER_MSB)
#define _GFX_IMAGE_FORMAT_NATIVE(i)                                     \
    (((__BYTE_ORDER == __LITTLE_ENDIAN)                                 \
      && ((i)->byteorder == _GFX_BYTE_ORDER_LSB))                       \
     || ((__BYTE_ORDER == __BIG_ENDIAN)                                 \
         && ((i)->byteorder == _GFX_BYTE_ORDER_MSB)))
#define _GFX_NATIVE_RGB_FORMAT()                                        \
    ((__BYTE_ORDER == __LITTLE_ENDIAN)                                  \
     ? THUNDER_RGB_LSB_FORMAT                                           \
     : THUNDER_RGB_MSB_FORMAT)

#define _GFX_SWAP_RGB_FORMAT()                                          \
    ((__BYTE_ORDER == __LITTLE_ENDIAN)                                  \
     ? THUNDER_RGB_MSB_FORMAT                                           \
     : THUNDER_RGB_LSB_FORMAT)

#define _GFX_RGB_FORMAT(i)                                              \
    ((_GFX_IMAGE_FORMAT_NATIVE(i)                                       \
      ? _GFX_NATIVE_RGB_FORMAT()                                        \
      : _GFX_SWAP_RGB_FORMAT()))

#define _GFX_RENDER_FORMAT_RGB(v)                                       \
    (((v)->red_mask == _GFX_RGB_RED_MASK)                               \
     && ((v)->green_mask == _GFX_RGB_GREEN_MASK)                        \
     && ((v)->blue_mask == _GFX_RGB_BLUE_MASK))
#define _GFX_RENDER_FORMAT_RGB_565(v)                                   \
    (((v)->red_mask == _GFX_RGB_565_RED_MASK)                           \
     && ((v)->green_mask == _GFX_RGB_565_GREEN_MASK)                    \
     && ((v)->blue_mask == _GFX_RGB_565_BLUE_MASK))
#define _GFX_RENDER_FORMAT_RGB_555(v)                                   \
    (((v)->red_mask == _GFX_RGB_555_RED_MASK)                           \
     && ((v)->green_mask == _GFX_RGB_555_GREEN_MASK)                    \
     && ((v)->blue_mask == _GFX_RGB_555_BLUE_MASK))
/*
 * FIXME: this macro needs to figure out whether to swap bytes.
 */
#define _GFX_RENDER_FORMAT(i,v)                                         \
    (_GFX_RENDER_FORMAT_RGB(v)                                          \
     ? _GFX_NATIVE_RGB_FORMAT()                                         \
     : (_GFX_RENDER_FORMAT_RGB_565(v)                                   \
        ? THUNDER_RGB_565_FORMAT                                        \
        : (_GFX_RENDER_FORMAT_RGB_555(v)                                \
           ? THUNDER_RGB_565_FORMAT                                     \
           : THUNDER_GENERIC_FORMAT)))
#define __X_RENDER_FORMAT(i) _GFX_RENDER_FORMAT(i,i)

/*
 * Filters.
 */
#define GFX_NO_FILTER(v,s,a) ((v) = (s))

#define GFX_FADE_IN  0U
#define GFX_FADE_OUT 1U

#define _GFX_FADE_VARS                                                  \
    Tuargb_t __redval;                                                  \
    Tuargb_t __greenval;                                                \
    Tuargb_t __blueval;

#define _GFX_FADE(v,s,a)                                                \
    do {                                                                \
        _GFX_FADE_VARS;                                                 \
        __redval = _GFX_ARGB_RED_VALUE((s));                            \
        __greenval = _GFX_ARGB_GREEN_VALUE((s));                        \
        __blueval = _GFX_ARGB_BLUE_VALUE((s));                          \
        __redval = __rgbfadetable->values[(a)][__redval];               \
        __greenval = __rgbfadetable->values[(a)][__greenval];           \
        __blueval = __rgbfadetable->values[(a)][__blueval];             \
        (v) = _GFX_ARGB_SET_VALUE(0, __redval, __greenval, __blueval);  \
    } while (FALSE)

/*
 * Blenders.
 *
 * NOTE: the lookup alpha-blender seems to be slower than the "regular" one on
 * my system...
 */
#define _GFX_ALPHA_BLEND_VARS                                           \
    Tuargb_t __fgval;                                                   \
    Tuargb_t __bgval;                                                   \
    Tuargb_t __alpha;                                                   \
    Tuargb_t __tmp;

#define _GFX_ALPHA_BLEND_CONST_VARS                                     \
    Tuargb_t __fgval;                                                   \
    Tuargb_t __bgval;                                                   \
    Tuargb_t __alpha;                                                   \
    Tuargb_t __tmp;

#define _GFX_ADD_BLEND_VARS                                             \
    Tuargb_t __tmp;

#define _GFX_ALPHA_BLEND(v,s,d,a)                                       \
    do {                                                                \
        _GFX_ALPHA_BLEND_VARS;                                          \
        __alpha = _GFX_ARGB_ALPHA_VALUE((s));                           \
        if (__alpha == 0) {                                             \
            (v) = (s);                                                  \
        } else if (__alpha == 0xff) {                                   \
            (v) = (d);                                                  \
        } else {                                                        \
            __fgval = _GFX_ARGB_RED_VALUE((s));                         \
            __bgval = _GFX_ARGB_RED_VALUE((d));                         \
            __tmp = __bgval + div255((__fgval - __bgval) * __alpha);    \
            (v) = _GFX_ARGB_SET_RED_VALUE(__tmp);                       \
                                                                        \
            __fgval = _GFX_ARGB_GREEN_VALUE((s));                       \
            __bgval = _GFX_ARGB_GREEN_VALUE((d));                       \
            __tmp = __bgval + div255((__fgval - __bgval) * __alpha);    \
            (v) |= _GFX_ARGB_SET_GREEN_VALUE(__tmp);                    \
                                                                        \
            __fgval = _GFX_ARGB_BLUE_VALUE((s));                        \
            __bgval = _GFX_ARGB_BLUE_VALUE((d));                        \
            __tmp = __bgval + div255((__fgval - __bgval) * __alpha);    \
            (v) |= _GFX_ARGB_SET_BLUE_VALUE(__tmp);                     \
        }                                                               \
    } while (FALSE)

#define _GFX_ALPHA_BLEND_CONST(v,s,d,a)                                 \
    do {                                                                \
        _GFX_ALPHA_BLEND_VARS;                                          \
        __alpha = (a);                                                  \
        if (__alpha == 0) {                                             \
            (v) = (s);                                                  \
        } else if (__alpha == 0xff) {                                   \
            (v) = (d);                                                  \
        } else {                                                        \
            __fgval = _GFX_ARGB_RED_VALUE((s));                         \
            __bgval = _GFX_ARGB_RED_VALUE((d));                         \
            __tmp = __bgval + div255((__fgval - __bgval) * __alpha);    \
            (v) = _GFX_ARGB_SET_RED_VALUE(__tmp);                       \
                                                                        \
            __fgval = _GFX_ARGB_GREEN_VALUE((s));                       \
            __bgval = _GFX_ARGB_GREEN_VALUE((d));                       \
            __tmp = __bgval + div255((__fgval - __bgval) * __alpha);    \
            (v) |= _GFX_ARGB_SET_GREEN_VALUE(__tmp);                    \
                                                                        \
            __fgval = _GFX_ARGB_BLUE_VALUE((s));                        \
            __bgval = _GFX_ARGB_BLUE_VALUE((d));                        \
            __tmp = __bgval + div255((__fgval - __bgval) * __alpha);    \
            (v) |= _GFX_ARGB_SET_BLUE_VALUE(__tmp);                     \
        }                                                               \
    } while (FALSE)

#define GFX_NO_BLEND(v,s,d,a) ((v) = (s))

#define GFX_ADD_BLEND(v,s,d,a)                                          \
    do {                                                                \
        _GFX_ADD_BLEND_VARS;                                            \
        (v) = ((s) & _GFX_ARGB_RED_MASK)                                \
              + ((d) & _GFX_ARGB_RED_MASK);                             \
        if ((v) & 0xff000000) {                                         \
            (v) = _GFX_ARGB_RED_MASK;                                   \
        }                                                               \
                                                                        \
        __tmp = ((s) & _GFX_ARGB_GREEN_MASK)                            \
                + ((d) & _GFX_ARGB_GREEN_MASK);                         \
        if (__tmp & 0xffff0000) {                                       \
            __tmp = _GFX_ARGB_GREEN_MASK;                               \
        }                                                               \
        (v) |= __tmp;                                                   \
                                                                        \
        __tmp = ((s) & _GFX_ARGB_BLUE_MASK)                             \
                + ((d) & _GFX_ARGB_BLUE_MASK);                          \
        if (__tmp & 0xffffff00) {                                       \
            __tmp = _GFX_ARGB_BLUE_MASK;                                \
        }                                                               \
        (v) |= __tmp;                                                   \
    } while (FALSE)

#define GFX_NO_RENDER(v,p,a)
#define _GFX_NO_RENDER_VARS(r)                                          \
    Tuargb_t *__rendptr = (r);
#define _GFX_NO_RENDER_POINTER(p,v)
#define _GFX_NO_RENDER_ADVANCE(p,i)

#define _GFX_RGB_555_RENDER(v,p,a)                                      \
    do {                                                                \
        Turgb555_t __rendval;                                           \
                                                                        \
        __rendval = _GFX_DITHER_RGB_555((v));                           \
        *(p) = __rendval;                                               \
    } while (FALSE)
#define _GFX_RGB_555_RENDER_VARS(r)                                     \
    Turgb_t *__rendptr = (r);
#define _GFX_RGB_555_RENDER_POINTER(p,v) Turgb555_t *p = (v)
#define _GFX_RGB_555_RENDER_ADVANCE(p,i) ((p) += (i))

#define GFX_RGB_565_RENDER(v,p,a)                                       \
    do {                                                                \
        Turgb565_t __rendval;                                           \
                                                                        \
        __rendval = _GFX_DITHER_RGB_565((v));                           \
        *(p) = __rendval;                                               \
    } while (FALSE)
#define _GFX_RGB_565_RENDER_VARS(r)                                     \
    Turgb565_t *__rendptr = (r);
#define _GFX_RGB_565_RENDER_POINTER(p,v) Turgb565_t *p = (v)
#define _GFX_RGB_565_RENDER_ADVANCE(p,i) ((p) += (i))

#define GFX_RGB_RENDER(v,p,a)                                           \
    do {                                                                \
        Turgb_t __rval;                                                 \
        Turgb_t __gval;                                                 \
        Turgb_t __bval;                                                 \
                                                                        \
        __rval = _GFX_ARGB_RED_VALUE(v);                                \
        __gval = _GFX_ARGB_GREEN_VALUE(v);                              \
        __bval = _GFX_ARGB_BLUE_VALUE(v);                               \
        _GFX_RGB_SET_VALUE((p), __rval, __gval, __bval);                \
    } while (FALSE)
#define _GFX_RGB_RENDER_VARS(r)                                         \
    Turgb_t *__rendptr = (r);
#define _GFX_RGB_RENDER_POINTER(p,v) Turgb_t *p = (v)
#define _GFX_RGB_RENDER_ADVANCE(p,i) (p += 4 * (i))

#define _GFX_RGB_RENDER_LSB(v,p,a)                                      \
    do {                                                                \
        Turgb_t __rval;                                                 \
        Turgb_t __gval;                                                 \
        Turgb_t __bval;                                                 \
                                                                        \
        __rval = _GFX_ARGB_RED_VALUE(v);                                \
        __gval = _GFX_ARGB_GREEN_VALUE(v);                              \
        __bval = _GFX_ARGB_BLUE_VALUE(v);                               \
        _GFX_RGB_SET_VALUE_LSB((p), __rval, __gval, __bval);            \
    } while (FALSE)
#define _GFX_RGB_RENDER_MSB(v,p,a)                                      \
    do {                                                                \
        uint8_t __rval;                                                 \
        uint8_t __gval;                                                 \
        uint8_t __bval;                                                 \
                                                                        \
        __rval = _GFX_ARGB_RED_VALUE(v);                                \
        __gval = _GFX_ARGB_GREEN_VALUE(v);                              \
        __bval = _GFX_ARGB_BLUE_VALUE(v);                               \
        _GFX_RGB_SET_VALUE_MSB((p), __rval, __gval, __bval);            \
    } while (FALSE)

#define _GFX_FADE_COPY_LOOP(c,s,d,sx,sy,dx,dy,aw,ah,dir,t,a,sync)       \
    do {                                                                \
        long __brightness;                                              \
        long __lastbrightness;                                          \
        unsigned long __usedtime;                                       \
        float __flim;                                                   \
        float __ffactor;                                                \
        struct timeval __fstarttv;                                      \
        struct timeval __fstoptv;                                       \
        unsigned long __nframes;                                        \
                                                                        \
        __nframes = 0;                                                  \
        __brightness = 0;                                               \
        __lastbrightness = _GFX_NORMAL_BRIGHTNESS - 1;                  \
        if ((dir) == _GFX_FADE_OUT) {                                   \
            __brightness = _GFX_NORMAL_BRIGHTNESS - 1;                  \
            __lastbrightness = 0;                                       \
        }                                                               \
        __usedtime = 0;                                                 \
        gettimeofday(&__fstarttv, NULL);                                \
        while (__usedtime < (t)) {                                      \
            if (__brightness != __lastbrightness) {                     \
                __lastbrightness = __brightness;                        \
                __nframes++;                                            \
                if ((a)) {                                              \
                    _GFX_COPY_LOOP((s), (d),                            \
                                   _GFX_RGB_PIXEL_MASK,                 \
                                   (sx), (sy),                          \
                                   (dx), (dy),                          \
                                   (aw), (ah),                          \
                                   _GFX_FADE, __brightness,             \
                                   _GFX_ALPHA_BLEND_CONST, (a),         \
                                   _GFX_COMBINE_PIXELS,                 \
                                   _GFX_COPY_OPERATION);                \
                } else {                                                \
                    _GFX_COPY_LOOP((s), (d),                            \
                                   _GFX_RGB_PIXEL_MASK,                 \
                                   (sx), (sy),                          \
                                   (dx), (dy),                          \
                                   (aw), (ah),                          \
                                   _GFX_FADE, __brightness,             \
                                   _GFX_NO_BLEND, NULL,                 \
                                   _GFX_COMBINE_PIXELS,                 \
                                   _GFX_COPY_OPERATION);                \
                }                                                       \
                sync();                                                 \
            }                                                           \
            gettimeofday(&__fstoptv, NULL);                             \
            __usedtime = TIMEVAL_COMPARE(&__fstarttv, &__fstoptv);      \
            __flim = (float)(_GFX_NORMAL_BRIGHTNESS) - 1.0;             \
            __ffactor = (float)__usedtime / (float)(t);                 \
            __brightness = (long)(__flim * __ffactor + 0.5);            \
            if ((dir) == _GFX_FADE_OUT) {                               \
                __brightness = _GFX_NORMAL_BRIGHTNESS - 1 - __brightness; \
            }                                                           \
        }                                                               \
        fprintf(stderr, "%lu frames faded, %lf fps\n",                  \
                __nframes, 1000000.0 * (double)__nframes / (double)__usedtime); \
        if ((dir) == _GFX_FADE_IN) {                                    \
            if ((a)) {                                                  \
                _GFX_COPY_LOOP((s), (d),                                \
                               _GFX_RGB_PIXEL_MASK,                     \
                               (sx), (sy),                              \
                               (dx), (dy),                              \
                               (aw), (ah),                              \
                               _GFX_NO_FILTER, NULL,                    \
                               _GFX_ALPHA_BLEND_CONST, (a),             \
                               _GFX_COMBINE_PIXELS,                     \
                               _GFX_COPY_OPERATION);                    \
            } else {                                                    \
                _GFX_COPY_LOOP((s), (d),                                \
                               _GFX_RGB_PIXEL_MASK,                     \
                               (sx), (sy),                              \
                               (dx), (dy),                              \
                               (aw), (ah),                              \
                               _GFX_NO_FILTER, NULL,                    \
                               _GFX_NO_BLEND, NULL,                     \
                               _GFX_COMBINE_PIXELS,                     \
                               _GFX_COPY_OPERATION);                    \
            }                                                           \
        } else {                                                        \
            __brightness = 0;                                           \
            if ((a)) {                                                  \
                _GFX_COPY_LOOP((s), (d),                                \
                               _GFX_RGB_PIXEL_MASK,                     \
                               (sx), (sy),                              \
                               (dx), (dy),                              \
                               (aw), (ah),                              \
                               _GFX_FADE, __brightness,                 \
                               _GFX_ALPHA_BLEND_CONST, (a),             \
                               _GFX_COMBINE_PIXELS,                     \
                               _GFX_COPY_OPERATION);                    \
            } else {                                                    \
                _GFX_COPY_LOOP((s), (d),                                \
                               _GFX_RGB_PIXEL_MASK,                     \
                               (sx), (sy),                              \
                               (dx), (dy),                              \
                               (aw), (ah),                              \
                               _GFX_FADE, __brightness,                 \
                               _GFX_NO_BLEND, NULL,                     \
                               _GFX_COMBINE_PIXELS,                     \
                               _GFX_COPY_OPERATION);                    \
            }                                                           \
        }                                                               \
        sync();                                                         \
    } while (FALSE)

#define _GFX_CROSSFADE_COPY_LOOP(c,s,d,r,sx,sy,dx,dy,aw,ah,t,sync)      \
    do {                                                                \
        long __alpha;                                                   \
        long __lastalpha;                                               \
        unsigned long __usedtime;                                       \
        float __ffactor;                                                \
        struct timeval __mstarttv;                                      \
        struct timeval __mstoptv;                                       \
        unsigned long __nframes;                                        \
                                                                        \
        __nframes = 0;                                                  \
        __alpha = 0xff;                                                 \
        __lastalpha = 0;                                                \
        __usedtime = 0;                                                 \
        gettimeofday(&__mstarttv, NULL);                                \
        while (__usedtime < (t)) {                                      \
            if (__alpha != __lastalpha) {                               \
                __lastalpha = __alpha;                                  \
                __nframes++;                                            \
                _GFX_COPY_LOOP((s), (d),                                \
                               _GFX_RGB_PIXEL_MASK,                     \
                               (sx), (sy),                              \
                               (dx), (dy),                              \
                               (aw), (ah),                              \
                               _GFX_NO_FILTER, NULL,                    \
                               _GFX_ALPHA_BLEND_CONST, __alpha,         \
                               _GFX_NO_COMBINE,                         \
                               _GFX_NO_OPERATION,                       \
                               _GFX_RGB_RENDER, NULL);                  \
                sync();                                                 \
            }                                                           \
            gettimeofday(&__mstoptv, NULL);                             \
            __usedtime = TIMEVAL_COMPARE(&__mstarttv, &__mstoptv);      \
            __ffactor = (float)__usedtime / (float)(t);                 \
            __alpha = (long)((float)0xff * __ffactor + 0.5);            \
            fprintf(stderr, "alpha: %ld\n", __alpha);                   \
        }                                                               \
        fprintf(stderr, "%lu frames faded, %lf fps\n",                  \
                __nframes, 1000000.0 * (double)__nframes / (double)__usedtime); \
        _GFX_COPY_LOOP((s), (d),                                        \
                       _GFX_RGB_PIXEL_MASK,                             \
                       (sx), (sy),                                      \
                       (dx), (dy),                                      \
                       (aw), (ah),                                      \
                       _GFX_NO_FILTER, NULL,                            \
                       _GFX_NO_BLEND, NULL,                             \
                       _GFX_COMBINE_PIXELS,                             \
                       _GFX_COPY_OPERATION,                             \
                       _GFX_RGB_RENDER, NULL);                          \
        sync();                                                         \
    } while (FALSE)

/*
 * Copy ARGB-format data.
 */
#define _GFX_COPY_ARGB_DATA(s,d,n)                                      \
    memcpy((d), (s), (n) * sizeof(Tuargb_t))
#define _GFX_COMBINE_PLANES(s,d,m,rop)                                  \
    (((rop((s), (d))) & (m)) | ((d) & ~(m)))

/*
 * Extract ARGB-pixel components.
 */
#define _GFX_ARGB_PIXEL_VALUE(u)                                        \
    ((u) & _GFX_ARGB_PIXEL_MASK)
#define _GFX_ARGB_ALPHA_VALUE(u)                                        \
    (((u) & _GFX_ARGB_ALPHA_MASK) >> _GFX_ARGB_ALPHA_SHIFT)
#define _GFX_ARGB_RED_VALUE(u)                                          \
    (((u) & _GFX_ARGB_RED_MASK) >> _GFX_ARGB_RED_SHIFT)
#define _GFX_ARGB_GREEN_VALUE(u)                                        \
    (((u) & _GFX_ARGB_GREEN_MASK) >> _GFX_ARGB_GREEN_SHIFT)
#define _GFX_ARGB_BLUE_VALUE(u)                                         \
    (((u) & _GFX_ARGB_BLUE_MASK) >> _GFX_ARGB_BLUE_SHIFT)

/*
 * Set ARGB-pixel components.
 */
#define _GFX_ARGB_SET_ALPHA_VALUE(u) ((u) << _GFX_ARGB_ALPHA_SHIFT)
#define _GFX_ARGB_SET_RED_VALUE(u)   ((u) << _GFX_ARGB_RED_SHIFT)
#define _GFX_ARGB_SET_GREEN_VALUE(u) ((u) << _GFX_ARGB_GREEN_SHIFT)
#define _GFX_ARGB_SET_BLUE_VALUE(u)  ((u) << _GFX_ARGB_BLUE_SHIFT)

#define _GFX_ARGB_SET_VALUE(a,r,g,b)                                    \
    (_GFX_ARGB_SET_ALPHA_VALUE((a))                                     \
     | _GFX_ARGB_SET_RED_VALUE((r))                                     \
     | _GFX_ARGB_SET_GREEN_VALUE((g))                                   \
     | _GFX_ARGB_SET_BLUE_VALUE((b)))

#define _GFX_RGB_ALPHA_BYTE_LSB(p) ((uint8_t *)(p))[3]
#define _GFX_RGB_RED_BYTE_LSB(p)   ((uint8_t *)(p))[2]
#define _GFX_RGB_GREEN_BYTE_LSB(p) ((uint8_t *)(p))[1]
#define _GFX_RGB_BLUE_BYTE_LSB(p)  ((uint8_t *)(p))[0]

#define _GFX_RGB_ALPHA_BYTE_MSB(p) ((uint8_t *)(p))[0]
#define _GFX_RGB_RED_BYTE_MSB(p)   ((uint8_t *)(p))[1]
#define _GFX_RGB_GREEN_BYTE_MSB(p) ((uint8_t *)(p))[2]
#define _GFX_RGB_BLUE_BYTE_MSB(p)  ((uint8_t *)(p))[3]

#define _GFX_RGB_SET_VALUE_LSB(p,r,g,b)                                 \
    do {                                                                \
        _GFX_RGB_BLUE_BYTE_LSB(p) = (b);                                \
        _GFX_RGB_GREEN_BYTE_LSB(p) = (g);                               \
        _GFX_RGB_RED_BYTE_LSB(p) = (r);                                 \
    } while (FALSE)

#define _GFX_RGB_SET_VALUE_MSB(p,r,g,b)                                 \
    do {                                                                \
        _GFX_RGB_RED_BYTE_MSB(p) = (r);                                 \
        _GFX_RGB_GREEN_BYTE_MSB(p) = (g);                               \
        _GFX_RGB_BLUE_BYTE_MSB(p) = (b);                                \
    } while (FALSE)

#define _GFX_RGB_ITER_VALUE_LSB(p,r,g,b)                                \
    do {                                                                \
        *((p)++) = (b);                                                 \
        *((p)++) = (g);                                                 \
        *((p)++) = (r);                                                 \
    } while (FALSE)

#define _GFX_RGB_ITER_VALUE_MSB(p,r,g,b)                                \
    do {                                                                \
        *((p)++) = (r);                                                 \
        *((p)++) = (g);                                                 \
        *((p)++) = (b);                                                 \
    } while (FALSE)

#if (__BYTE_ORDER == __LITTLE_ENDIAN)
#define _GFX_RGB_SET_VALUE(p,r,g,b) _GFX_RGB_SET_VALUE_LSB(p,r,g,b)
#define _GFX_RGB_ITER_VALUE(p,r,g,b) _GFX_RGB_ITER_VALUE_LSB(p,r,g,b)
#else
#define _GFX_RGB_SET_VALUE(p,r,g,b) _GFX_RGB_SET_VALUE_MSB(p,r,g,b)
#define _GFX_RGB_ITER_VALUE(p,r,g,b) _GFX_RGB_ITER_VALUE_MSB(p,r,g,b)
#endif

#define _GFX_NO_DITHER(u) (u)

#define _GFX_DITHER_RGB_COMPONENT(u,m,s)                                \
    (((s) >= 0)                                                         \
     ? (((u) & (m)) << (s))                                             \
     : (((u) & (m)) >> -(s)))

#define _GFX_DITHER_RGB_555(u)                                          \
    (_GFX_DITHER_RGB_COMPONENT(_GFX_ARGB_RED_VALUE(u),                  \
                               _GFX_RGB_555_DITHER_MASK_RED,            \
                               _GFX_RGB_555_DITHER_SHIFT_RED)           \
     | _GFX_DITHER_RGB_COMPONENT(_GFX_ARGB_GREEN_VALUE(u),              \
                                 _GFX_RGB_555_DITHER_MASK_GREEN,        \
                                 _GFX_RGB_555_DITHER_SHIFT_GREEN)       \
     | _GFX_DITHER_RGB_COMPONENT(_GFX_ARGB_BLUE_VALUE(u),               \
                                 _GFX_RGB_555_DITHER_MASK_BLUE,         \
                                 _GFX_RGB_555_DITHER_SHIFT_BLUE))

#define _GFX_DITHER_RGB_565_RED(r)                                      \
    (_GFX_DITHER_RGB_COMPONENT((r),                                     \
                               _GFX_RGB_565_DITHER_MASK_RED,            \
                               _GFX_RGB_565_DITHER_SHIFT_RED))
#define _GFX_DITHER_RGB_565_GREEN(g)                                    \
    (_GFX_DITHER_RGB_COMPONENT((g),                                     \
                               _GFX_RGB_565_DITHER_MASK_GREEN,          \
                               _GFX_RGB_565_DITHER_SHIFT_GREEN))
#define _GFX_DITHER_RGB_565_BLUE(b)                                     \
    (_GFX_DITHER_RGB_COMPONENT((b),                                     \
                               _GFX_RGB_565_DITHER_MASK_BLUE,           \
                               _GFX_RGB_565_DITHER_SHIFT_BLUE))

#define _GFX_DITHER_RGB_565(u)                                          \
    (_GFX_DITHER_RGB_COMPONENT(_GFX_ARGB_RED_VALUE(u),                  \
                               _GFX_RGB_565_DITHER_MASK_RED,            \
                               _GFX_RGB_565_DITHER_SHIFT_RED)           \
     | _GFX_DITHER_RGB_COMPONENT(_GFX_ARGB_GREEN_VALUE(u),              \
                                 _GFX_RGB_565_DITHER_MASK_GREEN,        \
                                 _GFX_RGB_565_DITHER_SHIFT_GREEN)       \
     | _GFX_DITHER_RGB_COMPONENT(_GFX_ARGB_BLUE_VALUE(u),               \
                                 _GFX_RGB_565_DITHER_MASK_BLUE,         \
                                 _GFX_RGB_565_DITHER_SHIFT_BLUE))

#define _GFX_NO_COMBINE(s,d,p,r) ((d) = (s))
#define _GFX_COMBINE_PIXELS(s,d,p,r)                                    \
    do {                                                                \
        (d) = _GFX_COMBINE_PLANES((s), (d), (p), r);                    \
    } while (FALSE)

/*
 * s -    source              (Tid_t)
 * d -    destination         (Tid_t)
 * m -    mask                (Tid_t)
 * p -    planemask (bitmask) (Tuargb_t)
 * sa -   source area         (struct T2darea *)
 * da -   destination area    (struct T2darea *)
 * ma -   mask area           (struct T2darea *)
 * w  -   width               (Tdim_t)
 * h  -   height              (Tdim_t)
 * fop -  filter operation    (macro or function pointer)
 * farg - filter argument     (any)
 * bop  - blend operation     (macro or function pointer)
 * barg - blend argument      (any)
 * cop  - combine operation   (macro or function pointer)
 * rop  - raster operation    (macro or function pointer)
 */
/*
 * NOTES
 * - either loop unrolling or cache-prewarming didn't increase the performance
 *   of the non-masking copy (lower) part of this practically at all. It seems
 *   the memory accesses play no big role in the performance (which should be
 *   OK). :)
 */
#define _GFX_COPY_LOOP(s,d,m,p,sa,da,ma,fop,farg,bop,barg,cop,rop)      \
    do {                                                                \
        struct Tobject *__srcobj;                                       \
        struct Tobject *__destobj;                                      \
        struct Tobject *__maskobj;                                      \
        struct Timage *__srcimg;                                        \
        struct Timage *__destimg;                                       \
        struct Tbitmap *__maskbitmap;                                   \
        Tbitmap_t *__mask;                                              \
        Tbitmap_t *__maskptr;                                           \
        Tuargb_t *__srcptr;                                             \
        Tuargb_t *__destptr;                                            \
        Tuargb_t __srcval;                                              \
        Tuargb_t __destval;                                             \
        Tuargb_t __filtval;                                             \
        Tuargb_t __val;                                                 \
        Tuargb_t __planemask;                                           \
        Tcoord_t __srcx;                                                \
        Tcoord_t __srcy;                                                \
        Tcoord_t __destx;                                               \
        Tcoord_t __desty;                                               \
        Tcoord_t __maskx;                                               \
        Tcoord_t __masky;                                               \
        Tdim_t __srcw;                                                  \
        Tdim_t __srch;                                                  \
        Tdim_t __destw;                                                 \
        Tdim_t __desth;                                                 \
        Tdim_t __maskw;                                                 \
        Tdim_t __maskh;                                                 \
        Tcoord_t __xlim;                                                \
        Tcoord_t __ylim;                                                \
        Tcoord_t __xdelta;                                              \
        Tcoord_t __ydelta;                                              \
        Tcoord_t __maskitem;                                            \
        Tcoord_t __maskdelta;                                           \
        Tbitmap_t __maskval;                                            \
        Tbitmap_t __masktmp;                                            \
        Tbitmap_t __maskflag;                                           \
        unsigned long __srcpitch;                                       \
        unsigned long __destpitch;                                      \
        unsigned long __nloops;                                         \
        unsigned long __maskbit;                                        \
        unsigned long __nleft;                                          \
        unsigned long __count;                                          \
                                                                        \
        __srcobj = Tfindobject(s);                                      \
        __destobj = Tfindobject(d);                                     \
        __srcimg = __srcobj->data;                                      \
        __destimg = __destobj->data;                                    \
        __planemask = (p);                                              \
        __srcx = (sa)->x;                                               \
        __srcy = (sa)->y;                                               \
        __destx = (da)->x;                                              \
        __desty = (da)->y;                                              \
        if (m) {                                                        \
            __maskobj = Tfindobject(m);                                 \
            __maskbitmap = __maskobj->data;                             \
            __maskx = (ma)->x;                                          \
            __masky = (ma)->y;                                          \
            __maskw = (ma)->w;                                          \
            __maskh = (ma)->h;                                          \
            __destx = MAX(__destx, __maskx);                            \
            __desty = MAX(__desty, __masky);                            \
            __xdelta = MAX(__destx - (da)->x, __destx - __srcx);        \
            __ydelta = MAX(__desty - (da)->x, __desty - __srcy);        \
            __srcx += __xdelta;                                         \
            __srcy += __ydelta;                                         \
        }                                                               \
        __srcw = __srcimg->w;                                           \
        __srch = __srcimg->h;                                           \
        __destw = __destimg->w;                                         \
        __desth = __destimg->h;                                         \
        if (m) {                                                        \
            __xlim = MIN(__destx + __srcw, __destx + __maskw - __maskx); \
            __xlim = MIN(__xlim, __destx + __srcw - __srcx);            \
            __ylim = MIN(__desty + __srch, __desty + __maskh - __masky); \
            __ylim = MIN(__ylim, __desty + __srch - __srcy);            \
        } else {                                                        \
            __xlim = MIN(__destx + __srcw, __destx + __srcw - __srcx);  \
            __ylim = MIN(__desty + __srch, __desty + __srch - __srcy);  \
        }                                                               \
        __srcptr = __srcimg->data + __srcy * __srcw + __srcx;           \
        __destptr = __destimg->data + __desty * __destw + __destx;      \
        __srcpitch = __srcw - __xlim + __destx;                         \
        __destpitch = __destw - __xlim + __destx;                       \
        if (__destx < __xlim) {                                         \
            if (m) {                                                    \
                while (__desty < __ylim) {                              \
                    __nleft = __xlim - __destx;                         \
                    __maskitem = __masky * __maskw + __maskx;           \
                    __maskdelta = __maskitem / (8 * sizeof(Tbitmap_t)); \
                    __maskbit = __maskitem & (8 * sizeof(Tbitmap_t) - 1); \
                    __maskptr = __mask + __maskdelta;                   \
                    if (__maskbit) {                                    \
                        __maskdelta = 8 * sizeof(Tbitmap_t) - __maskbit; \
                        __nloops = MIN(__nleft, __maskdelta);           \
                        __nleft -= __nloops;                            \
                        __maskval = *__maskptr;                         \
                        __maskval >>= __maskbit;                        \
                        __masktmp = 0;                                  \
                        __masktmp = ~__masktmp;                         \
                        __masktmp <<= __maskdelta;                      \
                        __masktmp &= __maskval;                         \
                        if (__masktmp) {                                \
                            while (__nloops--) {                        \
                                __srcval = *__srcptr;                   \
                                __destval = *__destptr;                 \
                                fop(__filtval, __srcval, (farg));       \
                                bop(__val, __filtval, __destval, (barg)); \
                                cop(__val, __destval, __planemask, rop); \
                                *__destptr = __destval;                 \
                                __srcptr++;                             \
                                __destptr++;                            \
                            }                                           \
                        } else if ((~__masktmp) == 0) {                 \
                            __srcptr += __nloops;                       \
                            __destptr += __nloops;                      \
                        } else {                                        \
                            while (__nloops--) {                        \
                                __maskflag = __maskval & 0x01U;         \
                                if (__maskflag) {                       \
                                    __srcval = *__srcptr;               \
                                    __destval = *__destptr;             \
                                    fop(__filtval, __srcval, (farg));   \
                                    bop(__val, __filtval, __destval, (barg)); \
                                    cop(__val, __destval, __planemask, rop); \
                                    *__destptr = __destval;             \
                                }                                       \
                                __maskval >>= 1;                        \
                                __srcptr++;                             \
                                __destptr++;                            \
                            }                                           \
                        }                                               \
                    }                                                   \
                    __nloops = __nleft / (8 * sizeof(Tbitmap_t));       \
                    __nleft -= __nloops * 8 * sizeof(Tbitmap_t);        \
                    while (__nloops--) {                                \
                        __count = 8 * sizeof(Tbitmap_t);                \
                        __maskval = *__maskptr;                         \
                        if ((~__maskval) == 0) {                        \
                            while (__count--) {                         \
                                __srcval = *__srcptr;                   \
                                __destval = *__destptr;                 \
                                fop(__filtval, __srcval, (farg));       \
                                bop(__val, __filtval, __destval, (barg)); \
                                cop(__val, __destval, __planemask, rop); \
                                *__destptr = __destval;                 \
                                __srcptr++;                             \
                                __destptr++;                            \
                            }                                           \
                        } else if (__maskval == 0) {                    \
                            __srcptr += __count;                        \
                            __destptr += __count;                       \
                        } else {                                        \
                            while (__count--) {                         \
                                __maskflag = __maskval & 0x01U;         \
                                if (__maskflag) {                       \
                                    __srcval = *__srcptr;               \
                                    __destval = *__destptr;             \
                                    fop(__filtval, __srcval, (farg));   \
                                    bop(__val, __filtval, __destval, (barg)); \
                                    cop(__val, __destval, __planemask, rop); \
                                    *__destptr = __destval;             \
                                }                                       \
                                __maskval >>= 1;                        \
                                __srcptr++;                             \
                                __destptr++;                            \
                            }                                           \
                        }                                               \
                        __maskptr++;                                    \
                    }                                                   \
                }                                                       \
                __nloops = __nleft;                                     \
                if (__nloops) {                                         \
                    __maskval = *__maskptr;                             \
                    __masktmp = 0;                                      \
                    __masktmp = ~__masktmp;                             \
                    __masktmp &= __maskval;                             \
                    if (__masktmp) {                                    \
                        while (__nloops--) {                            \
                            __srcval = *__srcptr;                       \
                            __destval = *__destptr;                     \
                            fop(__filtval, __srcval, (farg));           \
                            bop(__val, __filtval, __destval, (barg));   \
                            cop(__val, __destval, __planemask, rop);    \
                            *__destptr = __destval;                     \
                            __srcptr++;                                 \
                            __destptr++;                                \
                        }                                               \
                    } else if ((~__masktmp) == 0) {                     \
                        __srcptr += __nloops;                           \
                        __destptr += __nloops;                          \
                    } else {                                            \
                        while (__nloops--) {                            \
                            __maskflag = __maskval & 0x01U;             \
                            if (__maskflag) {                           \
                                __srcval = *__srcptr;                   \
                                __destval = *__destptr;                 \
                                fop(__filtval, __srcval, (farg));       \
                                bop(__val, __filtval, __destval, (barg)); \
                                cop(__val, __destval, __planemask, rop); \
                                *__destptr = __destval;                 \
                            }                                           \
                            __maskval >>= 1;                            \
                            __srcptr++;                                 \
                            __destptr++;                                \
                        }                                               \
                    }                                                   \
                }                                                       \
                __desty++;                                              \
                __masky++;                                              \
                __srcptr += __srcpitch;                                 \
                __destptr += __destpitch;                               \
            } else {                                                    \
                __nloops = __ylim - __desty;                            \
                __xdelta = __xlim - __destx;                            \
                while (__nloops--) {                                    \
                    __nleft = __xdelta;                                 \
                    while (__nleft--) {                                 \
                        __srcval = *__srcptr;                           \
                        __destval = *__destptr;                         \
                        fop(__filtval, __srcval, (farg));               \
                        bop(__val, __filtval, __destval, (barg));       \
                        cop(__val, __destval, __planemask, rop);        \
                        *__destptr = __destval;                         \
                        __srcptr++;                                     \
                        __destptr++;                                    \
                    }                                                   \
                    __desty++;                                          \
                    __srcptr += __srcpitch;                             \
                    __destptr += __destpitch;                           \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (FALSE)

/*
 * NOTES
 * - in the non-masking copy part at the bottom, processing two scanlines in
 *   parallel resulted in a speedup of about 50 % on my Pentium 3 box. :)
 */
#define _GFX_COPY_LOOP_NEW(s,d,m,p,sa,da,ma,fop,farg,bop,barg,cop,rop)  \
    do {                                                                \
        struct Tobject *__srcobj;                                       \
        struct Tobject *__destobj;                                      \
        struct Tobject *__maskobj;                                      \
        struct Timage *__srcimg;                                        \
        struct Timage *__destimg;                                       \
        struct Tbitmap *__maskbitmap;                                   \
        Tbitmap_t *__mask;                                              \
        Tbitmap_t *__maskptr;                                           \
        Tuargb_t *__srcptr1;                                            \
        Tuargb_t *__srcptr2;                                            \
        Tuargb_t *__destptr1;                                           \
        Tuargb_t *__destptr2;                                           \
        Tuargb_t __srcval1;                                             \
        Tuargb_t __srcval2;                                             \
        Tuargb_t __destval1;                                            \
        Tuargb_t __destval2;                                            \
        Tuargb_t __filtval1;                                            \
        Tuargb_t __filtval2;                                            \
        Tuargb_t __val1;                                                \
        Tuargb_t __val2;                                                \
        Tuargb_t __planemask;                                           \
        Tcoord_t __srcx;                                                \
        Tcoord_t __srcy;                                                \
        Tcoord_t __destx;                                               \
        Tcoord_t __desty;                                               \
        Tcoord_t __maskx;                                               \
        Tcoord_t __masky;                                               \
        Tdim_t __srcw;                                                  \
        Tdim_t __srch;                                                  \
        Tdim_t __destw;                                                 \
        Tdim_t __desth;                                                 \
        Tdim_t __maskw;                                                 \
        Tdim_t __maskh;                                                 \
        Tcoord_t __xlim;                                                \
        Tcoord_t __ylim;                                                \
        Tcoord_t __xdelta;                                              \
        Tcoord_t __ydelta;                                              \
        Tcoord_t __maskitem;                                            \
        Tcoord_t __maskdelta;                                           \
        Tbitmap_t __maskval;                                            \
        Tbitmap_t __masktmp;                                            \
        Tbitmap_t __maskflag;                                           \
        unsigned long __srcpitch;                                       \
        unsigned long __destpitch;                                      \
        unsigned long __nloops;                                         \
        unsigned long __maskbit;                                        \
        unsigned long __nleft;                                          \
        unsigned long __count;                                          \
                                                                        \
        __srcobj = Tfindobject(s);                                      \
        __destobj = Tfindobject(d);                                     \
        __srcimg = __srcobj->data;                                      \
        __destimg = __destobj->data;                                    \
        __planemask = (p);                                              \
        __srcx = (sa)->x;                                               \
        __srcy = (sa)->y;                                               \
        __destx = (da)->x;                                              \
        __desty = (da)->y;                                              \
        if (m) {                                                        \
            __maskobj = Tfindobject(m);                                 \
            __maskbitmap = __maskobj->data;                             \
            __maskx = (ma)->x;                                          \
            __masky = (ma)->y;                                          \
            __maskw = (ma)->w;                                          \
            __maskh = (ma)->h;                                          \
            __destx = MAX(__destx, __maskx);                            \
            __desty = MAX(__desty, __masky);                            \
            __xdelta = MAX(__destx - (da)->x, __destx - __srcx);        \
            __ydelta = MAX(__desty - (da)->x, __desty - __srcy);        \
            __srcx += __xdelta;                                         \
            __srcy += __ydelta;                                         \
        }                                                               \
        __srcw = __srcimg->w;                                           \
        __srch = __srcimg->h;                                           \
        __destw = __destimg->w;                                         \
        __desth = __destimg->h;                                         \
        if (m) {                                                        \
            __xlim = MIN(__destx + __srcw, __destx + __maskw - __maskx); \
            __xlim = MIN(__xlim, __destx + __srcw - __srcx);            \
            __ylim = MIN(__desty + __srch, __desty + __maskh - __masky); \
            __ylim = MIN(__ylim, __desty + __srch - __srcy);            \
        } else {                                                        \
            __xlim = MIN(__destx + __srcw, __destx + __srcw - __srcx);  \
            __ylim = MIN(__desty + __srch, __desty + __srch - __srcy);  \
        }                                                               \
        __srcptr1 = __srcimg->data + __srcy * __srcw + __srcx;          \
        __destptr1 = __destimg->data + __desty * __destw + __destx;     \
        __srcpitch = __srcw - __xlim + __destx;                         \
        __destpitch = __destw - __xlim + __destx;                       \
        if (__destx < __xlim) {                                         \
            if (m) {                                                    \
                while (__desty < __ylim) {                              \
                    __nleft = __xlim - __destx;                         \
                    __maskitem = __masky * __maskw + __maskx;           \
                    __maskdelta = __maskitem / (8 * sizeof(Tbitmap_t)); \
                    __maskbit = __maskitem & (8 * sizeof(Tbitmap_t) - 1); \
                    __maskptr = __mask + __maskdelta;                   \
                    if (__maskbit) {                                    \
                        __maskdelta = 8 * sizeof(Tbitmap_t) - __maskbit; \
                        __nloops = MIN(__nleft, __maskdelta);           \
                        __nleft -= __nloops;                            \
                        __maskval = *__maskptr;                         \
                        __maskval >>= __maskbit;                        \
                        __masktmp = 0;                                  \
                        __masktmp = ~__masktmp;                         \
                        __masktmp <<= __maskdelta;                      \
                        __masktmp &= __maskval;                         \
                        if (__masktmp) {                                \
                            while (__nloops--) {                        \
                                __srcval1 = *__srcptr1;                 \
                                __destval1 = *__destptr1;               \
                                fop(__filtval1, __srcval1, (farg));     \
                                bop(__val1, __filtval1, __destval1, (barg)); \
                                cop(__val1, __destval1, __planemask, rop); \
                                *__destptr1 = __destval1;               \
                                __srcptr1++;                            \
                                __destptr1++;                           \
                            }                                           \
                        } else if ((~__masktmp) == 0) {                 \
                            __srcptr1 += __nloops;                      \
                            __destptr1 += __nloops;                     \
                        } else {                                        \
                            while (__nloops--) {                        \
                                __maskflag = __maskval & 0x01U;         \
                                if (__maskflag) {                       \
                                    __srcval1 = *__srcptr1;             \
                                    __destval1 = *__destptr1;           \
                                    fop(__filtval1, __srcval1, (farg)); \
                                    bop(__val1, __filtval1, __destval1, (barg)); \
                                    cop(__val1, __destval1, __planemask, rop); \
                                    *__destptr1 = __destval1;           \
                                }                                       \
                                __maskval >>= 1;                        \
                                __srcptr1++;                            \
                                __destptr1++;                           \
                            }                                           \
                        }                                               \
                    }                                                   \
                    __nloops = __nleft / (8 * sizeof(Tbitmap_t));       \
                    __nleft -= __nloops * 8 * sizeof(Tbitmap_t);        \
                    while (__nloops--) {                                \
                        __count = 8 * sizeof(Tbitmap_t);                \
                        __maskval = *__maskptr;                         \
                        if ((~__maskval) == 0) {                        \
                            while (__count--) {                         \
                                __srcval1 = *__srcptr1;                 \
                                __destval1 = *__destptr1;               \
                                fop(__filtval1, __srcval1, (farg));     \
                                bop(__val1, __filtval1, __destval1, (barg)); \
                                cop(__val1, __destval1, __planemask, rop); \
                                *__destptr1 = __destval1;               \
                                __srcptr1++;                            \
                                __destptr1++;                           \
                            }                                           \
                        } else if (__maskval == 0) {                    \
                            __srcptr1 += __count;                       \
                            __destptr1 += __count;                      \
                        } else {                                        \
                            while (__count--) {                         \
                                __maskflag = __maskval & 0x01U;         \
                                if (__maskflag) {                       \
                                    __srcval1 = *__srcptr1;             \
                                    __destval1 = *__destptr1;           \
                                    fop(__filtval1, __srcval1, (farg)); \
                                    bop(__val1, __filtval1, __destval1, (barg)); \
                                    cop(__val1, __destval1, __planemask, rop); \
                                    *__destptr1 = __destval1;           \
                                }                                       \
                                __maskval >>= 1;                        \
                                __srcptr1++;                            \
                                __destptr1++;                           \
                            }                                           \
                        }                                               \
                        __maskptr++;                                    \
                    }                                                   \
                }                                                       \
                __nloops = __nleft;                                     \
                if (__nloops) {                                         \
                    __maskval = *__maskptr;                             \
                    __masktmp = 0;                                      \
                    __masktmp = ~__masktmp;                             \
                    __masktmp &= __maskval;                             \
                    if (__masktmp) {                                    \
                        while (__nloops--) {                            \
                            __srcval1 = *__srcptr1;                     \
                            __destval1 = *__destptr1;                   \
                            fop(__filtval1, __srcval1, (farg));         \
                            bop(__val1, __filtval1, __destval1, (barg)); \
                            cop(__val1, __destval1, __planemask, rop);  \
                            *__destptr1 = __destval1;                   \
                            __srcptr1++;                                \
                            __destptr1++;                               \
                        }                                               \
                    } else if ((~__masktmp) == 0) {                     \
                        __srcptr1 += __nloops;                          \
                        __destptr1 += __nloops;                         \
                    } else {                                            \
                        while (__nloops--) {                            \
                            __maskflag = __maskval & 0x01U;             \
                            if (__maskflag) {                           \
                                __srcval1 = *__srcptr1;                 \
                                __destval1 = *__destptr1;               \
                                fop(__filtval1, __srcval1, (farg));     \
                                bop(__val1, __filtval1, __destval1, (barg)); \
                                cop(__val1, __destval1, __planemask, rop); \
                                *__destptr1 = __destval1;               \
                            }                                           \
                            __maskval >>= 1;                            \
                            __srcptr1++;                                \
                            __destptr1++;                               \
                        }                                               \
                    }                                                   \
                }                                                       \
                __desty++;                                              \
                __masky++;                                              \
                __srcptr1 += __srcpitch;                                \
                __destptr1 += __destpitch;                              \
            } else {                                                    \
                __ydelta = __ylim - __desty;                            \
                __xdelta = __xlim - __destx;                            \
                __nloops = __ydelta >> 1;                               \
                __nleft = __ydelta & 0x01;                              \
                __srcptr2 = __srcptr1 + __xdelta + __srcpitch;          \
                __destptr2 = __destptr1 + __xdelta + __destpitch;       \
                while (__nloops--) {                                    \
                    __count = __xdelta;                                 \
                    while (__count--) {                                 \
                        __srcval1 = *__srcptr1;                         \
                        __srcval2 = *__srcptr2;                         \
                        __destval1 = *__destptr1;                       \
                        __destval2 = *__destptr2;                       \
                        fop(__filtval1, __srcval1, (farg));             \
                        fop(__filtval2, __srcval2, (farg));             \
                        bop(__val1, __filtval1, __destval1, (barg));    \
                        bop(__val2, __filtval2, __destval2, (barg));    \
                        cop(__val1, __destval1, __planemask, rop);      \
                        cop(__val2, __destval2, __planemask, rop);      \
                        *__destptr1 = __destval1;                       \
                        *__destptr2 = __destval2;                       \
                        __srcptr1++;                                    \
                        __srcptr2++;                                    \
                        __destptr1++;                                   \
                        __destptr2++;                                   \
                    }                                                   \
                    __desty += 2;                                       \
                    __srcptr1 += __srcpitch << 1;                       \
                    __srcptr2 += __srcpitch << 1;                       \
                    __destptr1 += __destpitch << 1;                     \
                    __destptr2 += __destpitch << 1;                     \
                }                                                       \
                if (__nleft) {                                          \
                    __srcval1 = *__srcptr1;                             \
                    __destval1 = *__destptr1;                           \
                    fop(__filtval1, __srcval1, (farg));                 \
                    bop(__val1, __filtval1, __destval1, (barg));        \
                    cop(__val1, __destval1, __planemask, rop);          \
                    *__destptr1 = __destval1;                           \
                    __srcptr1++;                                        \
                    __destptr1++;                                       \
                }                                                       \
            }                                                           \
        }                                                               \
    } while (FALSE)

#define _GFX_GENERATE_RGB_MAP_VALUE(v,u,b,c,g)                          \
    do {                                                                \
        double __dfactor;                                               \
        double __dval;                                                  \
                                                                        \
        __dfactor = (double)(u) / _GFX_NORMAL_COLOR_MODIFIER;           \
        __dval = (__dfactor - 0.5) * (c) + 0.5 + (b) - 1;               \
        if (__dval > 0.0) {                                             \
            __dval = pow(__dval, 1.0/(g)) * _GFX_NORMAL_COLOR_MODIFIER; \
            __dval = MAX(__dval, 255.0);                                \
        } else {                                                        \
            __dval = 0.0;                                               \
        }                                                               \
        (v) = (Turgb_t)__dval;                                          \
    } while (FALSE)

#define _GFX_GENERATE_RGB_MAP(m,b,c,g)                                  \
    do {                                                                \
        unsigned long __uw;                                             \
        double __brightness;                                            \
        double __contrast;                                              \
        double __gamma;                                                 \
        Turgb_t __byte;                                                 \
                                                                        \
        __brightness = (double)(b) / _GFX_NORMAL_BRIGHTNESS;            \
        __contrast = (double)(c) / _GFX_NORMAL_CONTRAST;                \
        __gamma = (double)(g) / _GFX_NORMAL_GAMMA;                      \
        __gamma = MAX(__gamma, 0.01);                                   \
        __byte = 0;                                                     \
        for (__uw = 0 ; __uw < 256 ; __uw++) {                          \
            _GFX_GENERATE_RGB_MAP_VALUE(__byte, __uw,                   \
                                        __brightness, __contrast, __gamma); \
            (m)->values[__uw].red = __byte;                             \
            (m)->values[__uw].green = __byte;                           \
            (m)->values[__uw].blue = __byte;                            \
        }                                                               \
    } while (FALSE)

#define _GFX_GENERATE_RGB_FADE_TABLE(t)                                 \
    do {                                                                \
        unsigned long __uw1;                                            \
        unsigned long __uw2;                                            \
        double __brightness;                                            \
        Tuargb_t __byte;                                                \
                                                                        \
        __byte = 0;                                                     \
        for (__uw1 = 0 ; __uw1 < _GFX_NORMAL_BRIGHTNESS ; __uw1++) {    \
            __brightness = (double)__uw1 / _GFX_NORMAL_BRIGHTNESS;      \
            for (__uw2 = 0 ; __uw2 < 256 ; __uw2++) {                   \
                __byte = (Tuargb_t)((double)__uw2 * __brightness);      \
                (t)->values[__uw1][__uw2] = __byte;                     \
            }                                                           \
        }                                                               \
    } while (FALSE)

/*
 * MMX.
 */

#define _GFX_ALPHA_BLEND_MMX_VARS                                       \
    __m64 __diff;                                                       \
    __m64 __alpha;                                                      \
    __m64 __tmp1;                                                       \
    __m64 __tmp2;

#define _GFX_ALPHA_BLEND_MMX_X2(v,s,d,a)                                \
    do {                                                                \
        _GFX_ALPHA_BLEND_MMX_VARS;                                      \
        __diff = _mm_subs_pu8((s), (d));                                \
                                                                        \
        (v) = _mm_and_si64((s), (__m64)UINT64_C(0x00000000ff000000));   \
        (v) = _mm_srl_si64((v), (__m64)UINT64_C(24));                   \
        __alpha = (__m64)__alphatable64[_mm_cvtsi64_si32((v))];         \
                                                                        \
        (v) = _mm_unpacklo_pi8(__diff, (__m64)UINT64_C(0));             \
        __tmp1 = _mm_mullo_pi16((v), __alpha);                          \
        (v) = _mm_unpacklo_pi8((d), (__m64)UINT64_C(0));                \
                                                                        \
        __tmp1 = _mm_srl_pi16(__tmp1, (__m64)UINT64_C(8));              \
        __tmp1 = _mm_add_pi16(__tmp1, (v));                             \
                                                                        \
        (v) = _mm_and_si64((s), (__m64)UINT64_C(0xff00000000000000));   \
        (v) = _mm_srl_si64((v), (__m64)UINT64_C(56));                   \
        __alpha = (__m64)__alphatable64[_mm_cvtsi64_si32((v))];         \
                                                                        \
        (v) = _mm_unpackhi_pi8(__diff, (__m64)UINT64_C(0));             \
        __tmp2 = _mm_mullo_pi16((v), __alpha);                          \
        (v) = _mm_unpackhi_pi8((d), (__m64)UINT64_C(0));                \
                                                                        \
        __tmp2 = _mm_srl_pi16(__tmp2, (__m64)UINT64_C(8));              \
        __tmp2 = _mm_add_pi16(__tmp2, (v));                             \
                                                                        \
        (v) = _mm_packs_pu16(__tmp1, __tmp2);                           \
        (v) = _mm_and_si64((v), (__m64)_GFX_ARGB_PIXEL_MASK_64);        \
    } while (FALSE)

#define _GFX_CLIP_LUT(i) ((__yuvtables.__yuvlutcliptbl + 384)[(i)])
#define _GFX_CLIP_YUV(i) (((i) & 256) ? (~(i >> 10)) : (i))

#endif /* __GFX_PIPE_H__ */

