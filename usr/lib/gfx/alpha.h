#ifndef __GFX_ALPHA_H__
#define __GFX_ALPHA_H__

#include <gfx/rgb.h>

#define __INTEL_MMX__ 1
#if (__INTEL_MMX__)
#include <mmintrin.h>
#endif

#define gfxdiv255a(c)                                                   \
    ((((c) << 8) + (c) + 256) >> 16)
#define gfxdiv255b(c)                                                   \
    (((c) * 257 + 257) >> 16)
#define gfxdiv256(c)                                                    \
    ((c) >> 8)
#define gfxalphablendcfast(src, dest, a)                                \
    ((dest) + gfxdiv256(((src) - (dest)) * (a)))
#define gfxalphablendchiq(src, dest, a)                                 \
    ((dest) + gfxdiv255(((src) - (dest)) * (a)))

#define gfxalphablendhiq(src, dest, aval)                               \
    do {                                                                \
        gfxargb32 _a = (aval);                                          \
        gfxargb32 _sr = gfxredval(src);                                 \
        gfxargb32 _sg = gfxgreenval(src);                               \
        gfxargb32 _sb = gfxblueval(src);                                \
        gfxargb32 _dr;                                                  \
        gfxargb32 _dg;                                                  \
        gfxargb32 _db;                                                  \
                                                                        \
        if ((aval) == 0xff) {                                           \
            _dr = _sr;                                                  \
            _dg = _sg;                                                  \
            _db = _sb;                                                  \
        } else if (aval) {                                              \
            _dr = gfxredval(dest);                                      \
            _dg = gfxgreenval(dest);                                    \
            _db = gfxblueval(dest);                                     \
            _dr = gfxalphablendchiq(_sr, _dr, _a);                      \
            _dg = gfxalphablendchiq(_sg, _dg, _a);                      \
            _db = gfxalphablendchiq(_sb, _db, _a);                      \
        }                                                               \
        (dest) = gfxmkpix(0, _dr, _dg, _db);                            \
    } while (0)

#define gfxalphablendhiq_p(src, dest, aval)                             \
    do {                                                                \
        gfxargb32 _a = (aval);                                          \
        gfxargb32 _sr = gfxredval_p(src);                               \
        gfxargb32 _sg = gfxgreenval_p(src);                             \
        gfxargb32 _sb = gfxblueval_p(src);                              \
        gfxargb32 _dr = gfxredval_p(dest);                              \
        gfxargb32 _dg = gfxgreenval_p(dest);                            \
        gfxargb32 _db = gfxblueval_p(dest);                             \
                                                                        \
        _dr = gfxalphablendchiq(_sr, _dr, _a);                          \
        _dg = gfxalphablendchiq(_sg, _dg, _a);                          \
        _db = gfxalphablendchiq(_sb, _db, _a);                          \
        gfxmkpix_p((dest), 0, _dr, _dg, _db);                           \
    } while (0)

#define gfxalphablendloq(src, dest, aval)                               \
    do {                                                                \
        gfxargb32 _a = (aval);                                          \
        gfxargb32 _sr = gfxredval(src);                                 \
        gfxargb32 _sg = gfxgreenval(src);                               \
        gfxargb32 _sb = gfxblueval(src);                                \
        gfxargb32 _dr = gfxgfxredval(dest);                             \
        gfxargb32 _dg = gfxgreenval(dest);                              \
        gfxargb32 _db = gfxblueval(dest);                               \
                                                                        \
        _dr = gfxalphablendfast(_sr, _dr, _a);                          \
        _dg = gfxalphablendfast(_sg, _dg, _a);                          \
        _db = gfxalphablendfast(_sb, _db, _a);                          \
        (dest) = gfxmkpix(0, _dr, _dg, _db);                            \
    } while (0)

#define gfxalphablendloq_p(src, dest, aval)                             \
    do {                                                                \
        gfxargb32 _a = (aval);                                          \
        gfxargb32 _sr = gfxredval_p(src);                               \
        gfxargb32 _sg = gfxgreenval_p(src);                             \
        gfxargb32 _sb = gfxblueval_p(src);                              \
        gfxargb32 _dr = gfxredval_p(dest);                              \
        gfxargb32 _dg = gfxgreenval_p(dest);                            \
        gfxargb32 _db = gfxblueval_p(dest);                             \
                                                                        \
        _dr = gfxalphablendfast(_sr, _dr, _a);                          \
        _dg = gfxalphablendfast(_sg, _dg, _a);                          \
        _db = gfxalphablendfast(_sb, _db, _a);                          \
        *(dest) = gfxmkpix(0, _dr, _dg, _db);                           \
    } while (0)

/* Jose's fast alphablend-algorithm */

#define gfxalphablendpix(c0, c1, a)                                     \
    ((((((((c0) >> 8) & 0xff00ff) - (((c1) >> 8) & 0xff00ff)) * (aval)) \
       + ((c1) & 0xff00ff00)) & 0xff00ff00)                             \
     + (((((((c0) & 0xff00ff) - ((c1) & 0xff00ff)) * (aval)) >> 8)      \
         + ((c1) & 0xff00ff)) & 0xff00ff))

#if 0
#define INTERP_256(a, c0, c1)                                           \
    ( (((((((c0) >> 8) & 0xff00ff) - (((c1) >> 8) & 0xff00ff)) * (a))   \
        + ((c1) & 0xff00ff00)) & 0xff00ff00) +                          \
      (((((((c0) & 0xff00ff) - ((c1) & 0xff00ff)) * (a)) >> 8)          \
        + ((c1) & 0xff00ff)) & 0xff00ff))
#endif

#define gfxalphablend_nodiv(src, dest, a)                               \
    do {                                                                \
        gfxargb32 _m1 = 0x00ff00ff;                                     \
        gfxargb32 _m2 = 0xff00ff00;                                     \
        gfxargb32 _ss = (src) >> 8;                                     \
        gfxargb32 _ds = (dest) >> 8;                                    \
                                                                        \
        (dest) =  (((((((_ss) & _m1) - ((_ds) & _m1)) * (a))            \
                     + ((dest) & _m2)) & _m2)                           \
                   + (((((((src) & _m1)                                 \
                          - ((dest) & _m1)) * (a)) >> 8)                \
                       + ((dest) & _m1)) & _m1));                       \
    } while (0)

#define gfxalphablendloq_asm_mmx(src, dest, aval)                       \
    do {                                                                \
        __asm__ ("pxor %mm0, %mm0\n");                                  \
        __asm__ ("movd %0, %%mm1\n" : : "rm" (src));                    \
        __asm__ ("movd %0, %%mm2\n" : : "rm" (dest));                   \
        __asm__ ("movd %0, %%mm3\n" : : "rm" (aval));                   \
        __asm__ ("punpcklbw %mm0, %mm1\n");                             \
        __asm__ ("movq %mm3, %mm5\n");                                  \
        __asm__ ("punpcklbw %mm0, %mm2\n");                             \
        __asm__ ("psllq $16, %mm5\n");                                  \
        __asm__ ("pxor %mm5, %mm3\n");                                  \
        __asm__ ("movq %mm3, %mm5\n");                                  \
        __asm__ ("psllq $32, %mm5\n");                                  \
        __asm__ ("pxor %mm5, %mm3\n");                                  \
        __asm__ ("psubw %mm2, %mm1\n");                                 \
        __asm__ ("movq %mm1, %mm4\n");                                  \
        __asm__ ("pmullw %mm3, %mm4\n");                                \
        __asm__ ("psrlw $8, %mm4\n");                                   \
        __asm__ ("paddb %mm4, %mm2\n");                                 \
        __asm__ ("packuswb %mm0, %mm2\n");                              \
        __asm__ __volatile__ ("movd %%mm2, %0\n" : "=rm" (dest));       \
        __asm__ ("emms\n");                                             \
    } while (0)

#if (__INTEL_MMX__)
/* NOTE: leaves destination ALPHA undefined */
#define gfxalphablendloq_intel_mmx(src, dest, aval)                     \
    do {                                                                \
        __m64 _mzero;                                                   \
        __m64 _msrc;                                                    \
        __m64 _mdest;                                                   \
        __m64 _malpha;                                                  \
        __m64 _mtmp;                                                    \
                                                                        \
        _mzero = _mm_cvtsi32_si64(0);              /* 0000000000000000 */ \
        _malpha = _mm_cvtsi32_si64(aval);          /* 00000000000000AA */ \
        _mtmp = _mm_slli_si64(_malpha, 16);        /* 0000000000AA0000 */ \
        _malpha = _mm_or_si64(_mtmp, _malpha);     /* 0000000000AA00AA */ \
        _mtmp = _mm_slli_si64(_malpha, 32);        /* 00AA00AA00000000 */ \
        _malpha = _mm_or_si64(_malpha, _mtmp);     /* 00AA00AA00AA00AA */ \
        _msrc = _mm_cvtsi32_si64(src);             /* S:00000000AARRGGBB */ \
        _mdest = _mm_cvtsi32_si64(dest);           /* D:00000000AARRGGBB */ \
        _msrc = _mm_unpacklo_pi8(_msrc, _mzero);   /* S:00AA00RR00GG00BB */ \
        _mdest = _mm_unpacklo_pi8(_mdest, _mzero); /* D:00AA00RR00GG00BB */ \
        _msrc = _mm_sub_pi16(_msrc, _mdest);       /* S - D */          \
        _msrc = _mm_mullo_pi16(_msrc, _malpha);    /* T = (S - D) * A */ \
        _msrc = _mm_srli_pi16(_msrc, 8);           /* T >> 8 */         \
        _mdest = _mm_add_pi8(_msrc, _mdest);       /* D = D + T */      \
        _mdest = _mm_packs_pu16(_mdest, _mzero);   /* D:00000000??RRGGBB */ \
        (dest) = _mm_cvtsi64_si32(_mdest);         /* DEST = D */       \
        _mm_empty();                                                    \
    } while (0)
#endif /* __INTEL_MMX__ */

#define gfxalphablend_hiq_const(src, dest, a)                           \
    do {                                                                \
        gfxargb32 _a = (a);                                             \
        gfxargb32 _sr = gfxredval(src);                                 \
        gfxargb32 _sg = gfxgreenval(src);                               \
        gfxargb32 _sb = gfxblueval(src);                                \
        gfxargb32 _dr = gfxredval(dest);                                \
        gfxargb32 _dg = gfxgreenval(dest);                              \
        gfxargb32 _db = gfxblueval(dest);                               \
                                                                        \
        _dr = gfxalphablendc(_sr, _dr, _a);                             \
        _dg = gfxalphablendc(_sg, _dg, _a);                             \
        _db = gfxalphablendc(_sb, _db, _a);                             \
        gfxmkpix_p((dest), 0, _dr, _dg, _db);                           \
    } while (0)

#define gfxalphablend_hiq_const2(src, dest, a)                          \
    do {                                                                \
        gfxargb32 _a = (a);                                             \
        gfxargb32 _sr;                                                  \
        gfxargb32 _dr;                                                  \
        gfxargb32 _sg;                                                  \
        gfxargb32 _dg;                                                  \
        gfxargb32 _sb;                                                  \
        gfxargb32 _db;                                                  \
        gfxargb32 _mask = 0xff;                                         \
                                                                        \
        _sb = (src) & _mask;                                            \
        _db = (dest) & _mask;                                           \
        (src) >>= 8;                                                    \
        (dest) >>= 8;                                                   \
        _sg = (src) & _mask;                                            \
        _dg = (dest) & _mask;                                           \
        (src) >>= 8;                                                    \
        (dest) >>= 8;                                                   \
        _sr = (src) & _mask;                                            \
        _dr = (dest) & _mask;                                           \
        _db = gfxalphablendc(_sr, _dr, _a);                             \
        _dg = gfxalphablendc(_sg, _dg, _a);                             \
        _dr = gfxalphablendc(_sr, _dr, _a);                             \
        _dg <<= 8;                                                      \
        _dr <<= 16;                                                     \
        (dest) = _db | _dg | _dr;                                       \
    } while (0)

#define gfxalphablendfast_const(src, dest, a)                           \
    do {                                                                \
        gfxargb32 _a = (a);                                             \
        gfxargb32 _sr = gfxredval(src);                                 \
        gfxargb32 _sg = gfxgreenval(src);                               \
        gfxargb32 _sb = gfxblueval(src);                                \
        gfxargb32 _dr = gfxredval(dest);                                \
        gfxargb32 _dg = gfxgreenval(dest);                              \
        gfxargb32 _db = gfxblueval(dest);                               \
                                                                        \
        _dr = gfxalphablendc2(_sr, _dr, _a);                            \
        _dg = gfxalphablendc2(_sg, _dg, _a);                            \
        _db = gfxalphablendc2(_sb, _db, _a);                            \
        gfxmkpix_p((dest), 0, _dr, _dg, _db);                           \
    } while (0)

#define gfxalphablendfast_const2(src, dest, a)                          \
    do {                                                                \
        uint64_t  _rbmask = 0x00ff00ff00ff00ffULL;                      \
        uint64_t  _gamask = 0xff00ff00ff00ff00ULL;                      \
        gfxargb32 _srcrb;                                               \
        gfxargb32 _destrb;                                              \
        gfxargb32 _destag;                                              \
        gfxargb32 _val1;                                                \
        gfxargb32 _val2;                                                \
                                                                        \
        _srcrb = (src);                                                 \
        _destrb = (dest);                                               \
        _destag = (dest);                                               \
        _srcrb &= _rbmask;                                              \
        _destrb &= _rbmask;                                             \
        _destag &= _gamask;                                             \
        _val1 = (src);                                                  \
        _val2 = _destag;                                                \
        _val1 >>= 8;                                                    \
        _val2 >>= 8;                                                    \
        _val1 &= _rbmask;                                               \
        _srcrb -= _destrb;                                              \
        _val1 -= _val2;                                                 \
        _srcrb *= (a);                                                  \
        _val1 *= (a);                                                   \
        _srcrb >>= 8;                                                   \
        _val1 += _destag;                                               \
        _srcrb += _destrb;                                              \
        _val1 &= _gamask;                                               \
        _srcrb &= _rbmask;                                              \
        _val1 += _srcrb;                                                \
        (dest) = _val1;                                                 \
    } while (0)

#endif /* __GFX_ALPHA_H__ */

