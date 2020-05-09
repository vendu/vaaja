#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <zero/prof.h>
#include <byteswap.h>
//#include <desktop/types.h>
#include <gfx/types.h>
#include <gfx/rgb.h>
#include <gfx/alpha.h>
#include <gfx/fade.h>

//#include <machdep/i386/regs.h>

#define DATA_ROWS 720
#define DATA_COLS 480
#define DATA_MEGS ((double)(DATA_ROWS * DATA_COLS * sizeof(uint32_t)) \
                   / 1048576.0)

int
main(int argc,
     char *argv[])
{
    uint32_t      *u32tab1;
//    uint32_t *u32tab2;
    uint32_t      *u32tab3;
    uint16_t      *u16tab1;
#if 0
    uint64_t       u64;
    uint32_t       u32a;
    uint32_t       u32b;
    uint32_t       u32c;
    uint32_t       u32d;
    uint8_t        u8a;
    uint8_t        u8b;
#endif
    int            i;
    PROFDECLCLK(clk);
    PROFDECLTICK(tick);
    struct gfximg  img1;
    struct gfximg  img2;

#if 0
    _moveax(u32);
    _moveaxedx_p(&u64);
#endif

    fprintf(stderr, "oper\tusec\t\ttick\t\tinfo\n");
    fprintf(stderr, "----\t----\t\t----\t\t----\n");

    u32tab1 = malloc(DATA_ROWS * DATA_COLS * sizeof(uint32_t));

#if 0

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        _argb32_fade_out_8(u32tab1[i], 1);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "out\t%lu\t\t%llu\t",
            profclkdiff(clk), proftickdiff(tick));
    fprintf(stderr, "%f Mpix/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        _argb32_fade_in_8(u32tab1[i], 1);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "in\t%lu\t\t%llu\t\t",
            profclkdiff(clk), proftickdiff(tick));
    fprintf(stderr, "%f Mpix/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u8a = _masklo8(u32tab1[i]);
        u8b = _maskhi8(u32tab1[i]);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "get8\t%lu\t\t%llu\t\t\n",
            profclkdiff(clk), proftickdiff(tick));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = _bswap32(u32tab1[i]);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "bswap\t%lu\t\t%llu\t\t\n",
            profclkdiff(clk), proftickdiff(tick));

#endif /* 0 */

    u16tab1 = malloc(DATA_ROWS * DATA_COLS * sizeof(uint16_t));
    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }

    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u16tab1[i] = gfxtorgb555(u32tab1[i]);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "rgb555\t%lu\t\t%llu\t\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f Mpix/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

#if 0
    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u16tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    memset(u32tab1, 0, DATA_ROWS * DATA_COLS * sizeof(uint32_t));
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "memset\t%lu\t\t%llu\t\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f MB/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS
             * (double)sizeof(uint32_t) / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    u32tab2 = malloc(DATA_ROWS * DATA_COLS * sizeof(uint32_t));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
        u32tab2[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    memcpy(u32tab2, u32tab1, DATA_ROWS * DATA_COLS * sizeof(uint32_t));
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "memcpy\t%lu\t\t%llu\t\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f MB/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS * (double)sizeof(uint32_t)
             / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand << 16) | (rand() & 0xffff);
        u32tab2[i] = (rand << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i += 4) {
        u32tab1[i] = _alphablendfast(u32tab2[i], u32tab1[i]);
        u32tab1[i + 1] = _alphablendfast(u32tab2[i + 1], u32tab1[i + 1]);
        u32tab1[i + 2] = _alphablendfast(u32tab2[i + 2], u32tab1[i + 2]);
        u32tab1[i + 3] = _alphablendfast(u32tab2[i + 3], u32tab1[i + 3]);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "alpha\t%lu\t\t%llu\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f Mpair/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand << 16) | (rand() & 0xffff);
        u32tab2[i] = (rand << 16) | (rand() & 0xffff);
        u16tab1[i] = rand();
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i += 4) {
        u32a = _alphablendfast(u32tab2[i], u32tab1[i]);
        u32b = _alphablendfast(u32tab2[i + 1], u32tab1[i + 1]);
        u32c = _alphablendfast(u32tab2[i + 2], u32tab1[i + 2]);
        u32d = _alphablendfast(u32tab2[i + 3], u32tab1[i + 3]);
        u16tab1[i] = gfxtorgb565(u32a);
        u16tab1[i + 1] = gfxtorgb565(u32b);
        u16tab1[i + 2] = gfxtorgb565(u32c);
        u16tab1[i + 3] = gfxtorgb565(u32d);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "dith\t%lu\t\t%llu\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f Mpair/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand << 16) | (rand() & 0xffff);
        u32tab2[i] = (rand << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        _alphablendhq_p(u32tab2[i], &u32tab1[i]);
    }
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "alphahq\t%lu\t\t%llu\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f Mpix/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

#endif

    u32tab3 = malloc(4 * DATA_ROWS * DATA_COLS * sizeof(uint32_t));
    img1.data = u32tab1;
    img1.w = DATA_COLS;
    img1.h = DATA_ROWS;
    img2.data = u32tab3;
    img2.w = 2 * DATA_COLS;
    img2.h = 2 * DATA_ROWS;

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    for (i = 0 ; i < 4 * DATA_ROWS * DATA_COLS ; i++) {
        u32tab3[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    gfxscaleimg(&img1, &img2, 2 * DATA_COLS, 2 * DATA_ROWS);
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "scale\t%lu\t\t%llu\t\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f Mpix/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    srand(1);
    for (i = 0 ; i < DATA_ROWS * DATA_COLS ; i++) {
        u32tab1[i] = (rand() << 16) | (rand() & 0xffff);
    }
    for (i = 0 ; i < 4 * DATA_ROWS * DATA_COLS ; i++) {
        u32tab3[i] = (rand() << 16) | (rand() & 0xffff);
    }
    profstartclk(clk);
    profstarttick(tick);
    gfxscaleimg2x(&img1, &img2);
    profstoptick(tick);
    profstopclk(clk);
    fprintf(stderr, "scale2x\t%lu\t\t%llu\t\t",
            profclkdiff(clk), (long long)proftickdiff(tick));
    fprintf(stderr, "%f Mpix/s\n",
            ((double)DATA_ROWS * (double)DATA_COLS / 1048576.0)
            / ((double)profclkdiff(clk) / 1000000.0));

    exit(0);
}

