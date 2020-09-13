#include <stdint.h>
#include <math.h>
#include <voima/num/fxp/fxp.h>

#if defined(TEST_FXP)
#include <stdio.h>
#include <stdlib.h>
#endif

typedef int64_t                     fxpbig_t;
typedef uint64_t                    ufxpbig_t;
#define FXP_TYPE                    fxpbig_t
#define UFXP_TYPE                   ufxpbig_t
#include <voima/num/fxp/util.h>
#include <voima/num/fxp/double2fxp.h>

struct fxpdivu32                    fxpdivcache[FXP_DIV_CACHE_MAX];

#if defined(TEST_FXP)
int
main(int argc, char *argv[])
{
    fxp_t                           fx1 = double2fxpbig(8.0);
    fxp_t                           fx2 = double2fxpbig(-4.0);

    printfxpbig(fx1, 0);
    fxpdivu32(16, 3);
    printfxpbig(fx1, 0);
    fprintf(stderr, "\t*\t");
    printfxpbig(fx2, 0);
    fprintf(stderr, "\t");
    printfxpbig(fxpmul(fx1, fx2), 0);
    fprintf(stderr, "\n");
    printfxpbig(fx1, 0);
    fprintf(stderr, "\t//\t");
    printfxpbig(fx2, 0);
    fprintf(stderr, "\t");
    printfxpbig(fxpidiv(fx1, fx2), 0);
    fprintf(stderr, "\n");
    printfxpbig(fx1, 0);
    fprintf(stderr, "\t/\t");
    printfxpbig(fx2, 0);
    fprintf(stderr, "\t");
    printfxpbig(fxpdiv(fx1, fx2), 0);
    fprintf(stderr, "\n");

    exit(0);
}
#endif

