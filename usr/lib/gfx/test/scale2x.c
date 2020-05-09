#include <stdint.h>
#include <gfx/types.h>

/*
 * Here is a highly-tuned version of the AdvMame2x scaler.
 *
 * REFERENCE: http://scale2x.sourceforge.net/
 */
void
gfxscaleimg2x(struct gfximg *src,
              struct gfximg *dest)
{
    argb32_t      *srcptr;
    argb32_t      *destptr;
    argb32_t       val1;
    argb32_t       val2;
    argb32_t       val3;
    argb32_t       val4;
    argb32_t       val5;
    argb32_t       val6;
    argb32_t       val7;
    argb32_t       val8;
    argb32_t       val9;
    long        ndx1;
    long        ndx2;
    long        lim;
    long        srcx;
    long        srcy;
    long        srcw;
    long        srch;
    long        destw;
    long        desth;
    uint_fast8_t   bool;
    uint_fast8_t   zero;
    uint_fast8_t   cond1;
    uint_fast8_t   cond2;
    uint_fast8_t   cond3;
    uint_fast8_t   cond4;

    srcptr = src->data;
    destptr = dest->data + 2;
    srcw = src->w;
    srch = src->h;
    zero = 0;
    ndx1 = zero;
    ndx2 = srcw;
    srcx = zero;
    srcy = zero;
    srcw = src->w;
    srch = src->h;
    destw = srcw << 1;
    desth = srch << 1;
    val1 = val2 = val3 = val4 = val7 = zero;
    val5 = srcptr[ndx1++];
    val8 = srcptr[ndx2++];
    val6 = srcptr[ndx1];
    val9 = srcptr[ndx2];
    srcptr++;
    srcx = 1;
    /* top row */
    while (srcx < srcw) {
        /* setup. */
        ndx1 = zero;
        ndx2 = destw;
        cond1 = (val2 == val4);
        cond2 = (val2 == val6);
        cond3 = (val4 == val8);
        cond4 = (val6 == val8);
        bool = ((cond1) && !(cond2) && !(cond3));
        destptr[ndx1++] = (bool) ? val4 : val5;
        bool = ((cond3) && !(cond1) && !(cond4));
        destptr[ndx2++] = (bool) ? val4 : val5;
        bool = ((cond2) && !(cond1) && !(cond4));
        destptr[ndx1] = (bool) ? val4 : val5;
        bool = ((cond4) && !(cond3) && !(cond2));
        destptr[ndx2] = (bool) ? val4 : val5;
        val4 = val5;
        val7 = val8;
        val8 = val9;
        ndx1 = zero;
        ndx2 = srcw;
        val5 = srcptr[ndx1++];
        val9 = srcptr[ndx2];
        val6 = srcptr[ndx1];
        srcx++;
        ndx1 = 2;
        srcptr++;
        destptr += ndx1;
    }
    /* middle rows */
    destptr += destw - 2;
    lim = srch - 1;
    srcy = 1;
    while (srcy < lim) {
        ndx1 = 0;
        ndx2 = -srcw;
        val1 = val4 = val7 = zero;
        val5 = srcptr[ndx1++];
        val3 = srcptr[ndx2++];
        val6 = srcptr[ndx1];
        val9 = srcptr[ndx2];
        srcx = 1;
        while (srcx < srcw) {
            ndx1 = zero;
            ndx2 = destw;
            cond1 = (val2 == val4);
            cond2 = (val2 == val6);
            cond3 = (val4 == val8);
            cond4 = (val6 == val8);
            bool = ((cond1) && !(cond2) && !(cond3));
            destptr[ndx1++] = (bool) ? val4 : val5;
            bool = ((cond3) && !(cond1) && !(cond4));
            destptr[ndx2++] = (bool) ? val4 : val5;
            bool = ((cond2) && !(cond1) && !(cond4));
            destptr[ndx1] = (bool) ? val4 : val5;
            bool = ((cond4) && !(cond3) && !(cond2));
            destptr[ndx2] = (bool) ? val4 : val5;
            ndx1 = zero;
            ndx2 = -srcw + 1;
            val1 = val2;
            val2 = val3;
            val4 = val5;
            val3 = srcptr[ndx2];
            val5 = srcptr[ndx1++];
            ndx2 = srcw + 1;
            val6 = srcptr[ndx1];
            val7 = val8;
            val8 = val9;
            ndx1 = 2;
            val9 = srcptr[ndx2];
            srcx++;
            srcptr++;
            destptr += ndx1;
        }
        ndx1 = destw - 2;
        srcy++;
        destptr += ndx1;
    }
    /* bottom row. */
    ndx1 = 0;
    ndx2 = -srcw;
    val1 = val4 = val7 = val8 = val9 = zero;
    val2 = srcptr[ndx2++];
    val5 = srcptr[ndx1++];
    val3 = srcptr[ndx2];
    val6 = srcptr[ndx1];
    srcptr++;
    srcx = 1;
    while (srcx < srcw) {
        ndx1 = zero;
        ndx2 = destw;
        cond1 = (val2 == val4);
        cond2 = (val2 == val6);
        cond3 = (val4 == val8);
        cond4 = (val6 == val8);
        /* set destination pixels. */
        bool = ((cond1) && !(cond2) && !(cond3));
        destptr[ndx1++] = (bool) ? val4 : val5;
        bool = ((cond3) && !(cond1) && !(cond4));
        destptr[ndx2++] = (bool) ? val4 : val5;
        bool = ((cond2) && !(cond1) && !(cond4));
        destptr[ndx1] = (bool) ? val4 : val5;
        bool = ((cond4) && !(cond3) && !(cond2));
        destptr[ndx2] = (bool) ? val4 : val5;
        ndx1 = 2;
        srcx++;
        srcptr++;
        destptr += ndx1;
    }

    return;
}

