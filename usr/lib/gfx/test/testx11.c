 #include <stdlib.h>
 #include <stdio.h>

 #include <gfx/types.h>
 #include <gfx/alpha.h>
 #include <gfx/fade.h>
 #include <gfx/xfade.h>

 #include <mmintrin.h>
 #include <xmmintrin.h>

 float   fadeftab[256];
 float   alphaftab[256];
 uint8_t fadetab[256][256];
 const char *imgtab[] = {
     "background.jpg",
     "cavefire.jpg",
     "wasteland.jpg",
     "wasteland2.jpg",
     "Resurrection.jpg",
     "sun.jpg",
     "fractalchip.png",
     "sword.jpg"
 };
 #define TEST_IMAGES 7

 #if 0
 struct argb32 {
     uint8_t bval;
     uint8_t gval;
     uint8_t rval;
     uint8_t aval;
 };
 #endif

 #define mkpix(a, r, g, b)                                               \
     (((a) << ALPHAOFS) | ((r) << REDOFS) | ((g) << GREENOFS) | ((b) << BLUEOFS))

 #define setpix_p(p, a, r, g, b)                                         \
     (((struct argb32 *)(p))->aval = (a),                                \
      ((struct argb32 *)(p))->rval = (r),                                \
      ((struct argb32 *)(p))->gval = (g),                                \
      ((struct argb32 *)(p))->bval = (b))
 #if 0
 #define alphaval_p(p) ((struct argb32 *)(p)->aval)
 #define gfxredval_p(p)   (((struct argb32 *)(p))->rval)
 #define gfxgreenval_p(p) (((struct argb32 *)(p))->gval)
 #define gfxblueval_p(p)  (((struct argb32 *)(p))->bval)
 #endif

 #define fadein_test(src, dest, fval)                                    \
 do {                                                                    \
     argb32_t _rval;                                                     \
     argb32_t _gval;                                                     \
     argb32_t _bval;                                                     \
     float    _fmul;                                                     \
                                                                         \
     _fmul = fadeftab[fval];                                             \
     _rval = (argb32_t)(_fmul * redval(*(src)));                         \
     _gval = (argb32_t)(_fmul * greenval(*(src)));                       \
     _bval = (argb32_t)(_fmul * blueval(*(src)));                        \
     mkpix(*(dest), 0, _rval, _gval, _bval);                             \
 } while (FALSE)
 #define fadein_test2(src, dest, val)                                    \
     do {                                                                \
     argb32_t _rval;                                                     \
     argb32_t _gval;                                                     \
     argb32_t _bval;                                                     \
                                                                         \
     _rval = fadetab[val][gfxredval(*(src))];                            \
     _gval = fadetab[val][gfxgreenval(*(src))];                          \
     _bval = fadetab[val][gfxblueval(*(src))];                           \
     gfxmkpix_p(dest, 0, _rval, _gval, _bval);                         \
 } while (FALSE)

 struct mmx16 {
     uint16_t wv[4];
 };

 struct mmx32 {
     uint32_t lw[2];
 };

 /* macro version */
 #define alphablendpix(c0, c1, a)                                        \
     ((dest) = ((((((((c0) >> 8) & 0xff00ff) - (((c1) >> 8) & 0xff00ff)) * (a)) \
                  + ((c1) & 0xff00ff00)) & 0xff00ff00)                   \
                + (((((((c0) & 0xff00ff) - ((c1) & 0xff00ff)) * (a)) >> 8) \
                    + ((c1) & 0xff00ff)) & 0xff00ff)))

 #define alphablend_test_p(src, dest, a)                                 \
     do {                                                                \
         __m64 _mzero;                                                   \
         __m64 _msrc1;                                                   \
         __m64 _mdest1;                                                  \
         __m64 _malpha;                                                  \
         __m64 _msrc2;                                                   \
         __m64 _mdest2;                                                  \
         __m64 _malpha2;                                                 \
         argb32_t _a1 = (a);                                             \
                                                                         \
         _a1 |= _a1 << 8;                                                \
         _a1 |= _a1 << 16;                                               \
         _mzero = _mm_cvtsi32_si64(0);                                   \
         _msrc1 = _mm_cvtsi32_si64(src[0]);                              \
         _mdest1 = _mm_cvtsi32_si64(dest[0]);                            \
         _msrc2 = _mm_cvtsi32_si64(src[1]);                              \
         _mdest2 = _mm_cvtsi32_si64(dest[1]);                            \
         _malpha = _mm_cvtsi32_si64(_a1);                                \
         _msrc1 = _mm_unpacklo_pi8(_msrc1, _mzero);                      \
         _mdest1 = _mm_unpacklo_pi8(_mdest1, _mzero);                    \
         _msrc2 = _mm_unpacklo_pi8(_msrc2, _mzero);                      \
         _mdest2 = _mm_unpacklo_pi8(_mdest2, _mzero);                    \
         _malpha = _mm_unpacklo_pi8(_malpha, _mzero);                    \
         _msrc1 = _mm_sub_pi16(_msrc1, _mdest1);                         \
         _msrc2 = _mm_sub_pi16(_msrc2, _mdest2);                         \
         _msrc1 = _mm_mullo_pi16(_msrc1, _malpha);                       \
         _msrc2 = _mm_mullo_pi16(_msrc2, _malpha);                       \
         _msrc1 = _mm_srli_pi16(_msrc1, 8);                              \
         _msrc2 = _mm_srli_pi16(_msrc2, 8);                              \
         _mdest1 = _mm_add_pi8(_msrc1, _mdest1);                         \
         _mdest2 = _mm_add_pi8(_msrc2, _mdest2);                         \
         _msrc1 = _mm_packs_pu16(_mdest1, _mzero);                       \
         _msrc2 = _mm_packs_pu16(_mdest2, _mzero);                       \
         (dest)[0] = _mm_cvtsi64_si32(_msrc1);                           \
         (dest)[1] = _mm_cvtsi64_si32(_msrc2);                           \
     } while (FALSE)

 #if 0
 #define div255(c)                                                       \
     ((((c) << 8) + (c) + 256) >> 16)
 #define div256(c) ((c) >> 8)
 #define redval(pix) gfxredval(pix)
 #define greenval(pix) gfxgreenval(pix)
 #define blueval(pix) gfxblueval(pix)
 #define alphaval(pix) _gfx_alpha_val(pix)
 #define setpix(a, r, g, b) setpix_p(dest, a, r, g, b)
 #define alphablendc(src, dest, a)                                       \
     ((dest) + div255(((src) - (dest)) * (a)))
 #define alphablendc2(src, dest, a)                                      \
     ((dest) + div256(((src) - (dest)) * (a)))
 #endif
 #define alphablendcf(src, dest, a)                                      \
     ((dest) + (((src) - (dest)) * (a)) / 255.0)
 #define alphablendcf2(src, dest, a)                                     \
     ((dest) + (((src) - (dest)) * (a)) * alphaftab[(a)])

 #if 0
 #define alphablendfast(src, dest, a)                                    \
     do {                                                                \
         argb32_t _rbmask = 0x00ff00ff;                                  \
         argb32_t _gamask = 0xff00ff00;                                  \
         argb32_t _destrb;                                               \
         argb32_t _srcrb;                                                \
         argb32_t _destag;                                               \
         argb32_t _val1;                                                 \
         argb32_t _val2;                                                 \
                                                                         \
         _srcrb = *(src) & _rbmask;                                      \
         _destrb = *(dest) & _rbmask;                                    \
         _destag = *(dest) & _gamask;                                    \
         _val1 = (*(src) >> 8) & _rbmask;                                \
         _val2 = _destag >> 8;                                           \
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
         *(dest) = _val1;                                                \
     } while (0)
 #endif

 #if 0
 #define alphablendfast(src, dest, aval)                                 \
     do {                                                                \
         uint64_t _rbmask = 0x00ff00ff00ff00ffULL;                       \
         argb32_t _gamask = 0xff00ff00ff00ff00ULL;                       \
         argb32_t _srcrb;                                                \
         argb32_t _destrb;                                               \
         argb32_t _destag;                                               \
         argb32_t _val1;                                                 \
         argb32_t _val2;                                                 \
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
         _srcrb *= (aval);                                               \
         _val1 *= (aval);                                                \
         _srcrb >>= 8;                                                   \
         _val1 += _destag;                                               \
         _srcrb += _destrb;                                              \
         _val1 &= _gamask;                                               \
         _srcrb &= _rbmask;                                              \
         _val1 += _srcrb;                                                \
         (dest) = _val1;                                                 \
     } while (0)

 /* TODO: test this, should be the same as the one above. */
 #define alphablendsse(src, dest, aval)                                  \
     do {                                                                \
         uint64_t _rbmask = 0x00ff00ff00ff00ffULL;                       \
         uint64_t _gamask = 0xff00ff00ff00ff00ULL;                       \
                                                                         \
         __asm__ ("movq %0, %%mm0\n" : : "m" (*(uint64_t *)(src)));      \
         __asm__ ("movq %0, %%mm1\n" : : "m" (*(uint64_t *)(dest)));     \
         __asm__ ("movd %0, %%mm2\n" : : "r" (aval));                    \
         __asm__ ("movq %mm0, %mm3\n"); /* srcrb */                      \
         __asm__ ("movq %mm1, %mm4\n"); /* destrb */                     \
         __asm__ ("movq %mm1, %mm5\n"); /* destag */                     \
         __asm__ ("movq %mm2, %mm6\n"); /* aval */                       \
         __asm__ ("psllq $32, %mm6\n"); /* aval << 32 */                 \
         __asm__ ("pxor %mm6, %mm2\n"); /* alpha */                      \
         __asm__ ("movq %0, %%mm6\n" : : "m" (_rbmask));                 \
         __asm__ ("movq %0, %%mm7\n" : : "m" (_gamask));                 \
         __asm__ ("pand %mm6, %mm3\n"); /* srcrb & rbmask */             \
         __asm__ ("pand %mm6, %mm4\n"); /* destrb & rbmask */            \
         __asm__ ("pand %mm5, %mm7\n"); /* destag & gamask */            \
         __asm__ ("psubd %mm4, %mm3\n"); /* srcrb - destrb */            \
         __asm__ ("movq %mm5, %mm1\n"); /* val2 */                       \
         __asm__ ("pslld $8, %mm0\n");  /* val1 >> 8 */                  \
         __asm__ ("pslld $8, %mm1\n");  /* val2 >> 8 */                  \
         __asm__ ("pand %mm6, %mm0\n"); /* val1 & rbmask */              \
         __asm__ ("psubd %mm1, %mm0\n"); /* val1 - val2 */               \
         __asm__ ("movq2dq %mm2, %xmm1\n");                             \
         __asm__ ("movq2dq %mm3, %xmm4\n");                                     \
         __asm__ ("pmuldq %xmm4, %xmm1\n"); /* srcrb * alpha */         \
         __asm__ ("movdq2q %xmm1, %mm1\n");                                 \
         __asm__ ("movq2dq %mm0, %xmm0\n");                             \
         __asm__ ("pmuldq %xmm4, %xmm0\n"); /* val1 * alpha */            \
         __asm__ ("movdq2q %xmm0, %mm0\n");                                     \
         __asm__ ("psrlq $8, %mm1\n");                                   \
         __asm__ ("paddd %mm0, %mm0\n"); /* val1 + destag */ \
         __asm__ ("paddd %mm4, %mm1\n"); /* srcrb + destrb */ \
         __asm__ ("pand %mm7, %mm0\n");  /* val1 & gamask */ \
         __asm__ ("pand %mm6, %mm1\n");  /* val2 & rbmask */ \
         __asm__ ("paddd %mm1, %mm0\n"); /* val1 + srcrb */ \
         __asm__ __volatile__ ("movq %%mm0, %0\n" : "=m" (*(uint64_t *)(dest))); \
         } while (0)

#define alphablendc(src, dest, a)                                       \
    ((dest) + div255(((src) - (dest)) * (a)))

#define alphablendloq_p(src, dest, aval)                                \
    do {                                                                \
        argb32_t _a = (aval);                                           \
        argb32_t _sr = gfxredval_p(src);                                   \
        argb32_t _sg = gfxgreenval_p(src);                                 \
        argb32_t _sb = gfxblueval_p(src);                                  \
        argb32_t _dr = gfxredval_p(dest);                                  \
        argb32_t _dg = gfxgreenval_p(dest);                                \
        argb32_t _db = gfxblueval_p(dest);                                 \
                                                                        \
        _dr = alphablendc2(_sr, _dr, _a);                               \
        _dg = alphablendc2(_sg, _dg, _a);                               \
        _db = alphablendc2(_sb, _db, _a);                               \
        setpix_p(*(dest), 0, _dr, _dg, _db);                            \
    } while (FALSE)

#if 0

#define alphablendloq_mmx(src, dest, a)                                 \
    do {                                                                \
        __m64 _mzero;                                                   \
        __m64 _msrc;                                                    \
        __m64 _mdest;                                                   \
        __m64 _malpha;                                                  \
        __m64 _mtmp1;                                                   \
        __m64 _mtmp2;                                                   \
        __m64 _mtmp3;                                                   \
        argb32_t _a = (a);                                              \
                                                                        \
        _a |= _a << 8;                                                  \
        _a |= _a << 16;                                                 \
        _mzero = _mm_cvtsi32_si64(0);                                   \
        _msrc = _mm_cvtsi32_si64(src);                                  \
        _mdest = _mm_cvtsi32_si64(dest);                                \
        _malpha = _mm_cvtsi32_si64(_a);                                 \
        _mtmp1 = _mm_unpacklo_pi8(_msrc, _mzero);                       \
        _mtmp2 = _mm_unpacklo_pi8(_mdest, _mzero);                      \
        _mtmp3 = _mm_unpacklo_pi8(_malpha, _mzero);                     \
        _msrc = _mm_sub_pi16(_mtmp1, _mtmp2);                           \
        _mtmp1 = _mm_mullo_pi16(_msrc, _mtmp3);                         \
        _msrc = _mm_srli_pi16(_mtmp1, 8);                               \
        _mdest = _mm_add_pi8(_msrc, _mtmp2);                            \
        _mtmp1 = _mm_packs_pu16(_mdest, _mzero);                        \
        (dest) = _mm_cvtsi64_si32(_mtmp1);                              \
    } while (FALSE)
#endif

#define alphablendloq_mmx(src, dest, aval)                              \
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
        _mdest = _mm_add_pi8(_msrc, _mdest);       /* D = D + T */       \
        _mdest = _mm_packs_pu16(_mdest, _mzero);   /* D:00000000??RRGGBB */ \
        (dest) = _mm_cvtsi64_si32(_mdest);         /* DEST = D */       \
    } while (FALSE)

#define alphablendloq_mmx2(src, dest, aval)                             \
    do {                                                                \
        __asm__ ("emms\n");                                             \
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
    } while (FALSE)
#endif /* 0 */

#define alphablendfast_c(src, dest, aval)                               \
    do {                                                                \
        argb32_t _rbmask = 0x00ff00ff;                                  \
        argb32_t _gamask = 0xff00ff00;                                  \
        argb32_t _srcrb;                                                \
        argb32_t _destrb;                                               \
        argb32_t _destag;                                               \
        argb32_t _val1;                                                 \
        argb32_t _val2;                                                 \
                                                                        \
        _srcrb = (src);                                                 \
        _val1 = _srcrb;                                                 \
        _destrb = (dest);                                               \
        _destag = (dest);                                               \
        _srcrb &= _rbmask;                                              \
        _destrb &= _rbmask;                                             \
        _destag &= _gamask;                                             \
        _val2 = _destag;                                                \
        _val1 >>= 8;                                                    \
        _val2 >>= 8;                                                    \
        _val1 &= _rbmask;                                               \
        _srcrb -= _destrb;                                              \
        _val1 -= _val2;                                                 \
        _srcrb *= (aval);                                               \
        _val1 *= (aval);                                                \
        _srcrb >>= 8;                                                   \
        _val1 += _destag;                                               \
        _srcrb += _destrb;                                              \
        _val1 &= _gamask;                                               \
        _srcrb &= _rbmask;                                              \
        _val1 += _srcrb;                                                \
        (dest) = _val1;                                                 \
    } while (0)

#define USE_XV        0
#define USE_COMPOSITE 0
#define USE_RENDER    0
#define USE_SHM 0
#define USE_MMX 0
#define SSE 0
#if (SSE)
#include <xmmintrin.h>
#include <stdint.h>
__m128 _atab128[256];
struct _v128 {
    uint16_t w1;
    uint16_t w2;
    uint16_t w3;
    uint16_t w4;
    uint16_t w5;
    uint16_t w6;
    uint16_t w7;
    uint16_t w8;
};
#endif

#define TEST_FADE_IN     1
#define TEST_ALPHA_BLEND 0

#define TEST_WIDTH       800
#define TEST_HEIGHT      600

#define FALSE 0

#if defined(linux)
#include <sys/param.h>
#else
#include <machine/param.h>
#endif
#include <sys/types.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#if (USE_SHM)
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/extensions/XShm.h>
#endif
#if (USE_COMPOSITE)
#include <X11/extensions/composite.h>
#endif
#if (USE_XV)
#include <X11/extensions/Xvlib.h>
#endif

#include <X11/Xlibint.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xmd.h>
#include <X11/Xlocale.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#if (USE_RENDER)
#include <X11/extensions/Xrender.h>
#include <X11/extensions/render.h>
#include <X11/extensions/renderproto.h>
#endif
#include <Imlib2.h>

#include <zero/prof.h>
#if 0
#define alphablendfast2(src, dest, a)                                   \
((dest) = ( (((((((src) >> 8) & 0xff00ff) - (((dest) >> 8) & 0xff00ff)) * (a)) \
              + ((dest) & 0xff00ff00)) & 0xff00ff00) +                    \
            (((((((src) & 0xff00ff) - ((dest) & 0xff00ff)) * (a)) >> 8)    \
              + ((dest) & 0xff00ff)) & 0xff00ff) ))

#define alphablendfast3(src, dest, a)                                   \
    do {                                                                \
        argb32_t _rbmask = 0x00ff00ff;                                  \
        argb32_t _gamask = 0xff00ff00;                                  \
        argb32_t _destrb;                                               \
        argb32_t _srcrb;                                                \
        argb32_t _destag;                                               \
        argb32_t _val1;                                                 \
        argb32_t _val2;                                                 \
                                                                        \
        _destrb = (dest);                                               \
        _srcrb = (src);                                                 \
        _destag = (dest);                                               \
        _destrb &= _rbmask;                                             \
        _srcrb &= _rbmask;                                              \
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

#if 0
/* function-like macro */
#define alphablendfast2(src, dest, a)                                   \
    do {                                                                \
        argb32_t _mask1 = 0x00ff00ff;                                   \
        argb32_t _mask2 = 0xff00ff00;                                   \
        argb32_t tmp1 = ((src) >> 8) & _mask1;                          \
        argb32_t tmp2 = ((dest) >> 8) & _mask1;                         \
        argb32_t tmp3 = (dest) & _mask2;                                \
                                                                        \
        tmp1 -= tmp2;                                                   \
        tmp2 = (dest) & _mask1;                                         \
        tmp1 *= a;                                                      \
        tmp3 = (src) & _mask1;                                          \
        tmp1 += tmp3;                                                   \
        tmp1 &= _mask2;                                                 \
        tmp3 -= tmp2;                                                   \
        (dest) = tmp1;                                                  \
        tmp3 *= a;                                                      \
        tmp3 >>= 8;                                                     \
        tmp3 += tmp2;                                                   \
        tmp3 &= _mask1;                                                 \
                                                                        \
        (dest) += tmp3;                                                 \
    } while (FALSE)
#endif

#define alphablendhiq_p(src, dest, aval)                                \
    do {                                                                \
        argb32_t _a = (aval);                                           \
        argb32_t _sr = gfxredval_p(src);                                   \
        argb32_t _sg = gfxgreenval_p(src);                                 \
        argb32_t _sb = gfxblueval_p(src);                                  \
        argb32_t _dr = gfxredval_p(dest);                                  \
        argb32_t _dg = gfxgreenval_p(dest);                                \
        argb32_t _db = gfxblueval_p(dest);                                 \
                                                                        \
        _dr = alphablendc(_sr, _dr, _a);                                \
        _dg = alphablendc(_sg, _dg, _a);                                \
        _db = alphablendc(_sb, _db, _a);                                \
        setpix_p((dest), 0, _dr, _dg, _db);                             \
    } while (FALSE)

#endif /* 0 */

#define alphablend_f(src, dest, aval)                                   \
    do {                                                                \
        float _a = (aval);                                              \
        float _sr = gfxredval_p(src);                                      \
        float _sg = gfxgreenval_p(src);                                    \
        float _sb = gfxblueval_p(src);                                     \
        float _dr = gfxredval_p(dest);                                     \
        float _dg = gfxgreenval_p(dest);                                   \
        float _db = gfxblueval_p(dest);                                    \
                                                                        \
        _dr = alphablendcf(_sr, _dr, _a);                               \
        _dg = alphablendcf(_sg, _dg, _a);                               \
        _db = alphablendcf(_sb, _db, _a);                               \
        setpix_p((dest), 0, (uint8_t)_dr, (uint8_t)_dg, (uint8_t)_db);  \
    } while (FALSE)

#define alphablend_f2(src, dest, aval)                                  \
    do {                                                                \
        argb32_t _a = (aval);                                           \
        float    _sr = gfxredval_p(src);                                   \
        float    _sg = gfxgreenval_p(src);                                 \
        float    _sb = gfxblueval_p(src);                                  \
        float    _dr = gfxredval_p(dest);                                  \
        float    _dg = gfxgreenval_p(dest);                                \
        float    _db = gfxblueval_p(dest);                                 \
                                                                        \
        _dr = alphablendcf2(_sr, _dr, _a);                              \
        _dg = alphablendcf2(_sg, _dg, _a);                              \
        _db = alphablendcf2(_sb, _db, _a);                              \
        setpix_p((dest), 0, (uint8_t)_dr, (uint8_t)_dg, (uint8_t)_db);  \
    } while (FALSE)

#define xfadenew(src1, src2, dest, val)                                 \
    do {                                                                \
        alphablendfast(src1, dest, 0xff - (val));                       \
        alphablendfast(src2, dest, (val));                              \
    } while (FALSE)
#define xfademmx(src1, src2, dest, val)                                 \
    do {                                                                \
        alphablendfast_c(*src1, *dest, 0xff - (val));                   \
        alphablendfast_c(*src2, *dest, (val));                          \
    } while (FALSE)
#define xfademmx2(src1, src2, dest, val)                                \
    do {                                                                \
        gfxalphablendloq_asm_mmx(src1, dest, 0xff - (val));             \
        gfxalphablendloq_asm_mmx(src2, dest, (val));                    \
    } while (FALSE)

struct attr {
    Display           *disp;
    Visual            *vis;
#if (USE_RENDER)
    XRenderPictFormat *rendfmt;
    Picture            pict;
#endif
#if (USE_XV)
    XvPortID           vport;
#endif
    int                depth;
    GC                 gc;
    Window             win;
};

struct img {
    char            *fname;
    argb32_t        *data;
    argb32_t        *maskdata;
    unsigned long    width;
    unsigned long    height;
    Imlib_Image     *imlib2;
    XImage          *xim;
#if (USE_SHM)
    int              shmid;
    int              pmapshmid;
    XShmSegmentInfo  ximseg;
    XShmSegmentInfo  pmapseg;
#endif
    Pixmap           pmap;
    Pixmap           mask;
};

void
gfxscale(struct img *src, struct img *dest,
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
    sw = src->width;
    sh = src->height;
    dw = dest->width;
    dh = dest->height;
    xmul = (float)sw / w;
    ymul = (float)sh / h;
    dxmax = min(destx + w, dw);
    dymax = min(desty + h, dh);
    for (dy = desty ; dy < dymax ; dy++) {
        sy = dy * ymul;
        sptr1 = sptr + sy * sw;
        dptr1 = dptr + dy * dw;
        for (dx = destx ; dx < dxmax ; dx++) {
            sx = srcx + dx * xmul;
            sptr2 = sptr1 + sx;
            *dptr1++ = *sptr2;
        }
    }

    return;
}

/*
 * Here is a scaling algorithm that should eliminate aliasing artifacts; it's
 * a bit slower than the one above.
 */
void
scaleantialias(argb32_t *src, argb32_t *dest,
               unsigned long srcw, unsigned long srch,
               unsigned long destw, unsigned long desth)
{
    argb32_t *srcptr1;
    argb32_t *srcptr2;
    argb32_t *destptr;
    float xfactor;
    float yfactor;
    unsigned long xval;
    unsigned long yval;
    unsigned long srcx;
    unsigned long srcy;
    unsigned long destx;
    unsigned long desty;
    argb32_t destval;
    
    destptr = dest;
    xfactor = (float)srcw / destw;
    yfactor = (float)srch / desth;
    yval = 0;
    desty = 0;
    for (desty = 0 ; desty < desth ; desty++) {
        yval = (yval + srch) % destw;
        srcy = desty * yfactor;
        if (yval < srch) {
            xval = 0;
            srcptr1 = src + srcy * srcw;
            for (destx = 0 ; destx < destw ; destx++) {
                xval = (xval + srcw) % destw;
                srcx = destx * xfactor;
                if (xval < srcw) {
                    srcptr2 = srcptr1 + srcx;
                    destval = *srcptr2;
                    *destptr = destval;
                }
                destptr++;
            }
        } else {
            destptr += destw;
        }
    }

    return;
}

void
scalehalf(argb32_t *src, argb32_t *dest,
          unsigned long w, unsigned long h)
{
    argb32_t *srcptr1;
    argb32_t *srcptr2;
    argb32_t *destptr;
    unsigned long srcw;
    unsigned long srch;
    unsigned long srcx;
    unsigned long srcy;
    argb32_t srcval1;
    argb32_t srcval2;
    argb32_t srcval3;
    argb32_t srcval4;
    argb32_t destval;
    argb32_t redval;
    argb32_t greenval;
    argb32_t blueval;

    destptr = dest;
    srcw = w;
    srch = h;
    for (srcy = 0 ; srcy < srch ; srcy += 2) {
        srcptr1 = src + srcy * srcw;
        srcptr2 = srcptr1 + srcw;
        for (srcx = 0 ; srcx < srcw ; srcx += 2) {
            srcval1 = *srcptr1++;
            srcval2 = *srcptr1++;
            srcval3 = *srcptr2++;
            srcval4 = *srcptr2++;
            redval = gfxredval(srcval1);
            greenval = gfxgreenval(srcval1);
            blueval = gfxblueval(srcval1);
            redval += gfxredval(srcval2);
            greenval += gfxgreenval(srcval2);
            blueval += gfxblueval(srcval2);
            redval += gfxredval(srcval3);
            greenval += gfxgreenval(srcval3);
            blueval += gfxblueval(srcval3);
            redval += gfxredval(srcval4);
            greenval += gfxgreenval(srcval4);
            blueval += gfxblueval(srcval4);
            redval >>= 2;
            greenval >>= 2;
            blueval >>= 2;
            gfxmkpix_p(destval, 0, redval, greenval, blueval);
            *destptr = destval;
            destptr++;
        }
    }

    return;
}

/*
 * Here is a high-quality double-size scaler known as AdvMame2x. More
 * information can be found at http://scale2x.sourceforge.net.
 */
void
scale2x(argb32_t *src, argb32_t *dest, unsigned long srcw, unsigned long srch)
{
    argb32_t *srcptr;
    argb32_t *destptr;
    unsigned long destw;
    unsigned long desth;
    unsigned long srcx;
    unsigned long srcy;
    argb32_t val1;
    argb32_t val2;
    argb32_t val3;
    argb32_t val4;
    argb32_t val5;
    argb32_t val6;
    argb32_t val7;
    argb32_t val8;
    argb32_t val9;

    srcptr = src;
    destw = srcw << 1;
    desth = srch << 1;
    /* top row */
    val1 = 0;
    val2 = 0;
    val3 = 0;
    val4 = 0;
    val5 = srcptr[0];
    val6 = srcptr[1];
    val7 = 0;
    val8 = srcptr[srcw];
    val9 = srcptr[srcw + 1];
    srcptr++;
    destptr = dest + 2;
    for (srcx = 1 ; srcx < srcw ; srcx++) {
        destptr[0] = (val4 == val2 && val2 != val6 && val4 != val8)
            ? val4
            : val5;
        destptr[1] = (val2 == val6 && val2 != val4 && val6 != val8)
            ? val6
            : val5;
        destptr[destw] = (val4 == val8 && val4 != val2 && val8 != val6)
            ? val4
            : val5;
        destptr[destw + 1] = (val8 == val6 && val4 != val8 && val2 != val6)
            ? val6
            : val5;
        val4 = val5;
        val5 = srcptr[0];
        val6 = srcptr[1];
        val7 = val8;
        val8 = val9;
        val9 = srcptr[srcw + 1];
        srcptr++;
        destptr += 2;
    }
    /* middle rows */
    destptr += destw - 2;
    for (srcy = 1 ; srcy < srch - 1; srcy++) {
        val1 = 0;
        val2 = srcptr[-srcw];
        val3 = srcptr[-srcw + 1];
        val4 = 0;
        val5 = srcptr[0];
        val6 = srcptr[1];
        val7 = 0;
        val8 = srcptr[srcw];
        val9 = srcptr[srcw + 1];
        srcptr++;
        for (srcx = 1 ; srcx < srcw ; srcx++) {
            destptr[0] = (val4 == val2 && val2 != val6 && val4 != val8)
                ? val4
                : val5;
            destptr[1] = (val2 == val6 && val2 != val4 && val6 != val8)
                ? val6
                : val5;
            destptr[destw] = (val4 == val8 && val4 != val2 && val8 != val6)
                ? val4
                : val5;
            destptr[destw + 1] = (val8 == val6 && val4 != val8 && val2 != val6)
                ? val6
                : val5;
            val1 = val2;
            val2 = val3;
            val3 = srcptr[-srcw + 1];
            val4 = val5;
            val5 = srcptr[0];
            val6 = srcptr[1];
            val7 = val8;
            val8 = val9;
            val9 = srcptr[srcw + 1];
            srcptr++;
            destptr += 2;
        }
        destptr += destw + 2;
    }
    /* bottom row */
    val1 = 0;
    val2 = srcptr[-srcw];
    val3 = srcptr[-srcw + 1];
    val4 = 0;
    val5 = srcptr[0];
    val6 = srcptr[1];
    val7 = 0;
    val8 = 0;
    val9 = 0;
    srcptr++;
    for (srcx = 1 ; srcx < srcw ; srcx++) {
        destptr[0] = (val4 == val2 && val2 != val6 && val4 != val8)
            ? val4
            : val5;
        destptr[1] = (val2 == val6 && val2 != val4 && val6 != val8)
            ? val6
            : val5;
        destptr[destw] = (val4 == val8 && val4 != val2 && val8 != val6)
            ? val4
            : val5;
        destptr[destw + 1] = (val8 == val6 && val4 != val8 && val2 != val6)
            ? val6
            : val5;
        val1 = val2;
        val2 = val3;
        val3 = srcptr[-srcw + 1];
        val4 = val5;
        val5 = srcptr[0];
        val6 = srcptr[1];
        srcptr++;
        destptr += 2;
    }

    return;
}

void
xfade1(struct img *src1, struct img *src2, struct img *dest, argb32_t val)
{
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr;
    unsigned long ul;

    sptr1 = (argb32_t *)src1->xim->data;
    sptr2 = (argb32_t *)src2->xim->data;
    dptr = (argb32_t *)dest->xim->data;
    for (ul = 0 ; ul <= 0xff ; ul++) {
        alphaftab[ul] = 1.0 / (float)0xff;
    }

#if 0 && (USE_MMX)
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul += 2) {
        gfxxfade1_mmx(*sptr1, *sptr2, *dptr, val);
        sptr1 += 2;
        sptr2 += 2;
        dptr += 2;
    }
#else
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        gfxxfade2_jose(*sptr1, *sptr2, *dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }
#endif

#if 0
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        xfadenew(sptr1, sptr2, dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }
#endif

    return;
}

void
xfade2(struct img *src1, struct img *src2, struct img *dest, argb32_t val)
{
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr;
    unsigned long ul;

    for (ul = 0 ; ul <= 0xff ; ul++) {
        alphaftab[ul] = 1.0 / (float)0xff;
    }
    sptr1 = (argb32_t *)src1->xim->data;
    sptr2 = (argb32_t *)src2->xim->data;
    dptr = (argb32_t *)dest->xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        gfxxfade1_mmx(*sptr1, *sptr2, *dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }

    return;
}

void
xfadejose(struct img *src1, struct img *src2, struct img *dest, argb32_t val)
{
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr;
    unsigned long ul;

    sptr1 = (argb32_t *)src1->xim->data;
    sptr2 = (argb32_t *)src2->xim->data;
    dptr = (argb32_t *)dest->xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        gfxxfade2_jose(*sptr1, *sptr2, *dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }

    return;
}

#if 0
void
xfade2(struct img *src1, struct img *src2, struct img *dest, argb32_t val)
{
    argb32_t *sptr1;
    argb32_t *sptr2;
    argb32_t *dptr;
    unsigned long ul;

    sptr1 = (argb32_t *)src1->xim->data;
    sptr2 = (argb32_t *)src2->xim->data;
    dptr = (argb32_t *)dest->xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        _xfade2(*sptr1, *sptr2, *dptr, val);
        sptr1++;
        sptr2++;
        dptr++;
    }

    return;
}
#endif

struct attr _attr;
struct img  _plasmaimg;
struct img  _demiimg;
struct img  _blackimg;
struct img  _winimg;
struct img  _srcimg;
struct img  _destimg;
struct img  _scaleimg;

void
init(void)
{
    unsigned long ul;
#if (SSE)
    struct _v128 *p128;
#endif

#if (SSE)
    ul = 256;
    while (ul--) {
        p128 = (struct _v128 *)(&_atab128[ul]);
        p128->w1 = (uint16_t)ul;
        p128->w2 = (uint16_t)ul;
        p128->w3 = (uint16_t)ul;
        p128->w4 = (uint16_t)ul;
        p128->w5 = (uint16_t)ul;
        p128->w6 = (uint16_t)ul;
        p128->w7 = (uint16_t)ul;
        p128->w8 = (uint16_t)ul;
    }
#endif
}

void
initx(void)
{
#if (USE_XV)
    XvAdaptorInfo *ai;
    int            n;

    XvImageFormatValues *fv;
#endif

    _attr.disp = XOpenDisplay(NULL);
    if (!_attr.disp) {
        fprintf(stderr, "failed to open display\n");
        
        exit(1);
    }
//    XSynchronize(_attr.disp, True);
    _attr.vis = DefaultVisual(_attr.disp, DefaultScreen(_attr.disp));
#if (USE_RENDER)
    _attr.rendfmt = XRenderVisualFormat(_attr.disp, _attr.vis);
#endif
    _attr.depth = DefaultDepth(_attr.disp, DefaultScreen(_attr.disp));
    _attr.win = XCreateWindow(_attr.disp, DefaultRootWindow(_attr.disp),
                              0, 0,
                              TEST_WIDTH, TEST_HEIGHT,
                              0,
                              DefaultDepth(_attr.disp, DefaultScreen(_attr.disp)),
                              InputOutput,
                              _attr.vis,
                              0,
                              NULL);
    if (!_attr.win) {
        fprintf(stderr, "failed to create window\n");
        
        exit(1);
    }
    _attr.gc = XCreateGC(_attr.disp, _attr.win, 0, NULL);
    imlib_context_set_display(_attr.disp);
    imlib_context_set_visual(_attr.vis);
    imlib_context_set_drawable(_attr.win);
#if (USE_XV)
    XvQueryAdaptors(_attr.disp, _attr.win, &n, &ai);
    _attr.vport = ai->base_id;
    XvFreeAdaptorInfo(ai);

    fv = XvListImageFormats(_attr.disp, _attr.vport, &n);
    while (n--) {
        fprintf(stderr, "%s: %x %x %x\n",
                fv->type == XvRGB ? "RGB" : "YUV",
                fv->red_mask, fv->green_mask, fv->blue_mask);
        if (fv->red_mask == _attr.vis->red_mask
            && fv->green_mask == _attr.vis->green_mask
            && fv->blue_mask == _attr.vis->blue_mask) {
            fprintf(stderr, "RGB: %x %x %x\n",
                    fv->red_mask, fv->green_mask, fv->blue_mask);
        }
    }
    XFree(fv);

    exit(0);
#endif
    
    return;
}

#if (USE_SHM)
int
initshm(size_t size, XShmSegmentInfo *seg)
{
    int shmid;

    shmid = shmget(IPC_PRIVATE, size, 0777 | IPC_CREAT);
    if (shmid == -1) {
        perror("shmget");

        return -1;
    }
    seg->shmaddr = shmat(shmid, NULL, 0);
    if (seg->shmaddr == ((void *)-1)) {
        perror("shmat");

        return -1;
    }
    seg->shmid = shmid;
    seg->readOnly = False;

    return shmid;
}

void
initimg(struct attr *attr, struct img *img, int pmap)
{
    img->xim = XShmCreateImage(attr->disp,
                               attr->vis,
                               attr->depth,
                               ZPixmap,
                               NULL,
                               &img->ximseg,
                               img->width,
                               img->height);
    if (!img->xim) {
        fprintf(stderr, "cannot allocate image structure\n");

        exit(1);
    }
    img->shmid = initshm(img->width * img->height * 4, &img->ximseg);
    img->xim->data = img->ximseg.shmaddr;
    if (img->shmid < 0) {

        exit(1);
    }
    XShmAttach(attr->disp,
               &img->ximseg);
    if (pmap) {
        img->pmapshmid = initshm(img->width * img->height * 4, &img->pmapseg);
        if (img->pmapshmid < 0) {
            
            exit(1);
        }
        XShmAttach(attr->disp,
                   &img->pmapseg);
        img->pmap = XShmCreatePixmap(attr->disp,
                                     attr->win,
                                     img->pmapseg.shmaddr,
                                     &img->pmapseg,
                                     img->width,
                                     img->height,
                                     attr->depth);
        if (!img->pmap) {
            fprintf(stderr, "cannot create pixmap\n");

            exit(1);
        }
    }

    return;
}
#else
void
initimg(struct attr *attr, struct img *img, int pmap)
{
    img->data = malloc(img->width * img->height * 4);
    if (!img->data) {
        fprintf(stderr, "cannot allocate image\n");

        exit(1);
    }
    img->xim = XCreateImage(attr->disp,
                            attr->vis,
                            attr->depth,
                            ZPixmap,
                            0,
                            (char *)img->data,
                            img->width,
                            img->height,
                            32,
                            0);
    if (pmap) {
        img->pmap = XCreatePixmap(attr->disp,
                                  attr->win,
                                  img->width,
                                  img->height,
                                  attr->depth);
    }

    return;
}
#endif

int
loadimg(struct attr *attr, struct img *img, char *filename)
{
    if (img->imlib2) {
        imlib_context_set_image(img->imlib2);
        imlib_free_image();
        img->imlib2 = NULL;
    }
    fprintf(stderr, "LOAD: %s\n", filename);
    img->imlib2 = imlib_load_image(filename);
    if (!img->imlib2) {

        return -1;
    }
    img->fname = filename;
    imlib_context_set_image(img->imlib2);
#if 0
    img->width = imlib_image_get_width();
    img->height = imlib_image_get_height();
#endif
    fprintf(stderr, "%d x %d\n", img->width, img->height);
    imlib_render_pixmaps_for_whole_image_at_size(&img->pmap, &img->mask,
                                                 img->width, img->height);

    return 0;
}

#if (USE_SHM)
#define getimg(d, i) XShmGetImage(_attr.disp, (d), (i)->xim, 0, 0, ~0UL)
#define putimg(i, d) XShmPutImage(_attr.disp, (d), _attr.gc, (i)->xim, 0, 0, 0, 0, (i)->width, (i)->height, False); XSync(_attr.disp, False)
#else
#define getimg(d, i) (i)->xim = XGetImage(_attr.disp, (d), 0, 0, (i)->width, (i)->height, ~0UL, ZPixmap)
#define putimg(i, d) XPutImage(_attr.disp, (d), _attr.gc, (i)->xim, 0, 0, 0, 0, (i)->width, (i)->height); XSync(_attr.disp, False)
#endif

void
initimgs(void)
{
#if 0
    loadimg(&_attr, &_plasmaimg, "test.jpg");
    initimg(&_attr, &_plasmaimg, 0);
    getimg(_plasmaimg.pmap, &_plasmaimg);
    imlib_free_pixmap_and_mask(_plasmaimg.pmap);
    _plasmaimg.pmap = _plasmaimg.mask = 0;
    loadimg(&_attr, &_demiimg, "demi.jpg");
    initimg(&_attr, &_demiimg, 0);
    getimg(_demiimg.pmap, &_demiimg);
    imlib_free_pixmap_and_mask(_demiimg.pmap);
    _demiimg.pmap = _demiimg.mask = 0;
    _blackimg.width = _winimg.width = TEST_WIDTH;
    _blackimg.height = _winimg.height = TEST_HEIGHT;
    initimg(&_attr, &_blackimg, 1);
#endif
    XSetForeground(_attr.disp, _attr.gc, BlackPixel(_attr.disp, DefaultScreen(_attr.disp)));
//    XFillRectangle(_attr.disp, _blackimg.pmap, _attr.gc, 0, 0, _blackimg.width, _blackimg.height);
//    XFillRectangle(_attr.disp, _winimg.pmap, _attr.gc, 0, 0, _winimg.width, _winimg.height);
#if 0
    loadimg(&_attr, &_srcimg, imgtab[0]);
    initimg(&_attr, &_srcimg, 0);
    loadimg(&_attr, &_destimg, imgtab[1]);
    initimg(&_attr, &_destimg, 0);
#endif
    _srcimg.width = TEST_WIDTH;
    _srcimg.height = TEST_HEIGHT;
    _destimg.width = TEST_WIDTH;
    _destimg.height = TEST_HEIGHT;
    _winimg.width = TEST_WIDTH;
    _winimg.height = TEST_HEIGHT;
    loadimg(&_attr, &_winimg, imgtab[0]);
    initimg(&_attr, &_winimg, 0);
    getimg(_winimg.pmap, &_winimg);
#if 0
    initimg(&_attr, &_srcimg, 0);
    initimg(&_attr, &_destimg, 0);
#endif
    
    return;
}

void
initwin(void)
{
    unsigned long exposed = 0;
#if (USE_RENDER)
    XRenderPictureAttributes pattr;
#endif

#if (USE_COMPOSITE)
    XCompositeRedirectWindow(_attr.disp, _attr.win);
#endif
#if (USE_RENDER)
    pattr.subwindow_mode = IncludeInferiors;
    _attr.pict = XRenderCreatePicture(_attr.disp, _attr.win, _attr.rendfmt,
                                      CPSubwindowMode, &pattr);
#endif
    XSelectInput(_attr.disp, _attr.win, ExposureMask);
    XMapWindow(_attr.disp, _attr.win);
    while (!exposed) {
        XEvent event;

        XNextEvent(_attr.disp, &event);
        switch (event.type) {
            case Expose:
#if 0
                if (event.xexpose.count == 0) {
                    XClearWindow(_attr.disp, _attr.win);
                }
#endif
                exposed = 1;

                break;
            default:

                break;
        }
    }

    return;
}

#define fadeinft(src, dest, val, ftab)                                  \
    do {                                                                \
        argb32_t _rval;                                                 \
        argb32_t _gval;                                                 \
        argb32_t _bval;                                                 \
        float    _ftor;                                                 \
                                                                        \
        _ftor = ftab[(val)];                                            \
        _rval = (argb32_t)(_ftor * gfxredval_p(src));                      \
        _gval = (argb32_t)(_ftor * gfxgreenval_p(src));                    \
        _bval = (argb32_t)(_ftor * gfxblueval_p(src));                     \
        setpix_p(dest, 0, _rval, _gval, _bval);                          \
    } while (FALSE)

#define fadeoutft(src, dest, val, ftab)                                 \
    do {                                                                \
        argb32_t _rval;                                                 \
        argb32_t _gval;                                                 \
        argb32_t _bval;                                                 \
        float    _ftor;                                                 \
                                                                        \
        _ftor = ftab[(val)];                                            \
        _rval = (argb32_t)(_ftor * gfxredval_p(src));                      \
        _gval = (argb32_t)(_ftor * gfxgreenval_p(src));                    \
        _bval = (argb32_t)(_ftor * gfxblueval_p(src));                     \
        setpix_p(dest, 0, _rval, _gval, _bval);                          \
    } while (FALSE)

void
testfadein(void)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       pix;
    unsigned long  val;
    unsigned long  ul;
    float          f;
    PROFDECLCLK(clk);

    for (val = 0 ; val <= 0xff ; val++) {
        f = (float)val / 0xff;
        for (ul = 0 ; ul <= 0xff ; ul++) {
            f *= ul;
            fadetab[val][ul] = (uint8_t)f;
        }
    }

    for (val = 0 ; val < 256 ; val += 4) {
        if (val > 0xfb) {
            val = 0xff;
        }
        profstartclk(clk);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
//            fadein_test(*ptr2, *ptr1, val);
            ptr2++;
            ptr1++;
        }
        profstopclk(clk);
        fprintf(stderr,
                "fade: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH,
                TEST_HEIGHT,
                256, profclkdiff(clk));
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}

void
testfadeout(void)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       pix;
    unsigned long  val;
    unsigned long  ul;
    PROFDECLCLK(clk);

    for (val = 0xff ; val > 0 ; val -= 4) {
        if (val < 4) {
            val = 0;
        }
        profstartclk(clk);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
            fadeoutft(*ptr2, *ptr1, val, fadeftab);
            ptr2++;
            ptr1++;
        }
        profstopclk(clk);
        fprintf(stderr,
                "fade: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH,
                TEST_HEIGHT,
                256, profclkdiff(clk));
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
        if (!val) {

            break;
        }
    }
}

void
testfadeint(unsigned long usecs)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       tmp;
    argb32_t       val;
    argb32_t       lastval;
    long           diff;
    unsigned long  total;
    unsigned long  stop;
    unsigned long  ul;
    struct timeval start;
    struct timeval cur;
    struct timeval end;
    float          fmul;
    PROFDECLCLK(clk);
    PROFDECLCLK(fade);

    gettimeofday(&start, NULL);
    end = start;
    tvaddconst(&end, usecs);
    gettimeofday(&cur, NULL);
    val = 0xff;
    lastval = 0;
    profstartclk(clk);
    stop = 0;
    while (!stop) {
        profstartclk(fade);
        getimg(_blackimg.pmap, &_winimg);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        if (tvgt(&cur, &end)) {
            fprintf(stderr, "stopping fade\n");
            val = 0xff;
            lastval = 0;
            stop = 1;
        } else {
            diff = usecs - tvcmp(&cur, &end);
            diff = min(diff, usecs);
            fmul = (float)diff / (float)usecs;
            val = (argb32_t)(255.0 * fmul);
        }
        if (val != lastval) {
            for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
                gfxalphablendhiq(*ptr2, *ptr1, val);
                ptr2++;
                ptr1++;
            }
            putimg(&_winimg, _attr.win);
            XFlush(_attr.disp);
            lastval = val;
        }
        profstopclk(fade);
        gettimeofday(&cur, NULL);
    }
    profstopclk(clk);
    fprintf(stderr,
            "fade: %dx%d, %d steps: %lu microsecs\n",
            TEST_WIDTH, TEST_HEIGHT,
            256, profclkdiff(clk));
}

void
testfadeoutt(unsigned long usecs)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       tmp;
    argb32_t       val;
    argb32_t       lastval;
    long           diff;
    unsigned long  total;
    unsigned long  stop;
    unsigned long  ul;
    struct timeval start;
    struct timeval cur;
    struct timeval end;
    float          fmul;
    PROFDECLCLK(clk);
    PROFDECLCLK(fade);

    gettimeofday(&start, NULL);
    end = start;
    tvaddconst(&end, usecs);
    gettimeofday(&cur, NULL);
    val = 0;
    lastval = 0xff;
    profstartclk(clk);
    stop = 0;
    while (!stop) {
        profstartclk(fade);
        getimg(_blackimg.pmap, &_winimg);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        if (tvgt(&cur, &end)) {
            val = 0;
            lastval = 0xff;
            stop = 1;
        } else {
            diff = usecs - tvcmp(&cur, &end);
            diff = min(diff, usecs);
            fmul = (float)diff / usecs;
            val = 0xff - (argb32_t)(0xff * fmul);
        }
        if (val != lastval) {
            for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
                gfxalphablendhiq(*ptr2, *ptr1, val);
                ptr2++;
                ptr1++;
            }
            putimg(&_winimg, _attr.win);
            XFlush(_attr.disp);
            lastval = val;
        }
        profstopclk(fade);
        gettimeofday(&cur, NULL);
        fprintf(stderr,
                "step: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH, TEST_HEIGHT,
                256, profclkdiff(fade));
    }
    profstopclk(clk);
    fprintf(stderr,
            "fade: %dx%d, %d steps: %lu microsecs\n",
            TEST_WIDTH, TEST_HEIGHT,
            256, profclkdiff(clk));
}

void
testxfade1t(unsigned long usecs)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t      *ptr3;
    argb32_t       val;
    argb32_t       lastval;
    long           diff;
    unsigned long  total;
    unsigned long  first;
    unsigned long  stop;
    unsigned long  last;
    unsigned long  ul;
    struct timeval start;
    struct timeval cur;
    struct timeval end;
    float          fmul;
    PROFDECLCLK(clk);
    PROFDECLCLK(fade);

    val = 0;
    lastval = 0xff;
    profstartclk(clk);
    first = 1;
    stop = 0;
    getimg(_blackimg.pmap, &_winimg);
    while (!stop) {
        gettimeofday(&start, NULL);
        end = start;
        last = (tvcmp(&end, &cur) > usecs);
        if (last) {
            stop = 1;
        }
        tvaddconst(&end, usecs);
        profstartclk(fade);
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        ptr3 = (argb32_t *)_demiimg.xim->data;
        if (first) {
            val = 0;
            first = 0;
        } else if (tvcmp(&cur, &end) > usecs) {
            val = 0xff;
            lastval = 0xff;
            stop = 1;
        } else {
            diff = usecs - tvcmp(&cur, &end);
            diff = min(diff, usecs);
            fmul = (float)diff / usecs;
            val = (argb32_t)(0xff * fmul);
        }
        if (!stop) {
            fprintf(stderr, "val: %x\n", val);
            for (ul = 0 ; ul < (TEST_WIDTH * TEST_HEIGHT) >> 1 ; ul++) {
//#if (USE_MMX)
                xfademmx2(ptr2, ptr3, ptr1, val);
//#else
//                _xfade1_jose(*ptr2, *ptr3, *ptr1, val);
//                ptr2++;
//                ptr3++;
//                ptr1++;
//#endif
                ptr2 += 2;
                ptr2 += 2;
                ptr1 += 2;
            }
            putimg(&_winimg, _attr.win);
            XFlush(_attr.disp);
            lastval = val;
        }
        profstopclk(fade);
        last = profclkdiff(fade);
        gettimeofday(&cur, NULL);
        fprintf(stderr,
                "step: %dx%d, %d steps: %lu microsecs\n",
                TEST_WIDTH, TEST_HEIGHT,
                256, last);
        gettimeofday(&cur, NULL);
    }
    ptr1 = (argb32_t *)_winimg.xim->data;
    ptr2 = (argb32_t *)_plasmaimg.xim->data;
    for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
        *ptr1++ = *ptr2++;
    }
    profstopclk(clk);
    fprintf(stderr,
            "fade: %dx%d, %d steps: %lu microsecs\n",
            TEST_WIDTH, TEST_HEIGHT,
            256, profclkdiff(clk));
}

#if 0

void
testfadein_mmx(void)
{
    argb32_t      *ptr1;
    argb32_t      *ptr2;
    argb32_t       tmp;
    unsigned long  val;
    unsigned long  ul;

    for (val = 0 ; val < 256 ; val++) {
        ptr1 = (argb32_t *)_winimg.xim->data;
        ptr2 = (argb32_t *)_plasmaimg.xim->data;
        for (ul = 0 ; ul < TEST_WIDTH * TEST_HEIGHT ; ul++) {
//            gfx_alphablend_fast_const_mmx(ptr2, ptr1, val);
//            *ptr1 = val;
            ptr2++;
            ptr1++;
        }
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}

void
testscale(void)
{
    PROFDECLCLK(clk);

    loadimg(&_attr, &_srcimg, "testhalf.jpg");
    initimg(&_attr, &_srcimg, 0);
    getimg(_srcimg.pmap, &_srcimg);
    _scaleimg.width = TEST_WIDTH;
    _scaleimg.height = TEST_HEIGHT;
    initimg(&_attr, &_scaleimg, 0);
    profstartclk(clk);
    gfxscale(&_srcimg, &_scaleimg, 0, 0, 0, 0, TEST_WIDTH, TEST_HEIGHT);
    initimg(&_attr, &_plasmaimg, 0);
#if 0
    scaleantialias(_srcimg.data, _scaleimg.data,
                   TEST_WIDTH / 2, TEST_HEIGHT / 2,
                   TEST_WIDTH, TEST_HEIGHT);
#endif
#if 0
    scale2x(_srcimg.data, _scaleimg.data,
            TEST_WIDTH / 2, TEST_HEIGHT / 2);
#endif
    putimg(&_scaleimg, _attr.win);
    XFlush(_attr.disp);
    profstopclk(clk);
    fprintf(stderr, "scale %lux%lu -> %lux%lu: %lu microsecs\n",
            _plasmaimg.width, _plasmaimg.height, _winimg.width, _winimg.height, profclkdiff(clk));
}

void
testxfade1(void)
{
    argb32_t val;
    PROFDECLCLK(clk);

    profstartclk(clk);
    for (val = 0 ; val <= 0xff ; val += 5) {
        fprintf(stderr, "%d\n", val);
        xfade1(&_srcimg, &_destimg, &winimg, val);
        putimg(&_destimg, _attr.win);
        XFlush(_attr.disp);
    }
    profstopclk(clk);
    fprintf(stderr,
            "xfade: %lu microsecs\n",
            profclkdiff(clk));
}

#endif /* 0 */

void
testxfade2(void)
{
    argb32_t val;

    for (val = 0 ; val < 256 ; val += 5) {
        fprintf(stderr, "%d\n", val);
        xfade1(&_srcimg, &_destimg, &_winimg, val);
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}

void
testxfadejose(void)
{
    argb32_t val;

    for (val = 0 ; val < 256 ; val += 5) {
        fprintf(stderr, "%d\n", val);
        xfadejose(&_plasmaimg, &_demiimg, &_winimg, val);
        putimg(&_winimg, _attr.win);
        XFlush(_attr.disp);
    }
}

#if (USE_SHM)
void
dtimg(struct attr *attr, struct img *img, int pmap)
{
    XShmDetach(attr->disp, &img->ximseg);
    shmdt(img->ximseg.shmaddr);
    shmctl(img->ximseg.shmid, IPC_RMID, NULL);
    if (pmap) {
        XShmDetach(attr->disp, &img->pmapseg);
        shmdt(img->pmapseg.shmaddr);
        shmctl(img->pmapseg.shmid, IPC_RMID, NULL);
    }

    return;
}
#endif

int
main(int argc,
     char *argv[])
{
    argb32_t val;
    unsigned long ul;
    int i;
    PROFDECLCLK(clk);

    init();
    initx();
    initimgs();
    initwin();

    for (i = 0 ; i < TEST_IMAGES - 1 ; i++) {
        loadimg(&_attr, &_srcimg, imgtab[i]);
        loadimg(&_attr, &_destimg, imgtab[i + 1]);
        getimg(_srcimg.pmap, &_srcimg);
        getimg(_destimg.pmap, &_destimg);
//        initimg(&_attr, &_srcimg, 0);
//        initimg(&_attr, &_destimg, 0);
        profstartclk(clk);
        for (val = 0 ; val <= 0xff ; val += 5) {
            xfade2(&_srcimg, &_destimg, &_winimg, val);
            __asm__ __volatile__ ("emms\n");
            putimg(&_winimg, _attr.win);
#if 0
#if (USE_SHM)
            dtimg(&_attr, &_winimg, 0);
#endif
#endif
        }
#if (USE_SHM)
        dtimg(&_attr, &_srcimg, 0);
        dtimg(&_attr, &_destimg, 0);
#endif
        profstopclk(clk);
        fprintf(stderr, "xfade time %lu\n", profclkdiff(clk));
        sleep(2);
    }

//    testscale();
#if 0
    testxfade1t(10000000);
    testfadeint(1000000);
    testfadeoutt(1000000);
#endif
//    testfadein_mmx();

#if (USE_SHM)
    dtimg(&_attr, &_plasmaimg, 0);
    dtimg(&_attr, &_demiimg, 0);
    dtimg(&_attr, &_blackimg, 1);
    dtimg(&_attr, &_winimg, 0);
#endif

    sleep(4);

    exit(0);
}

