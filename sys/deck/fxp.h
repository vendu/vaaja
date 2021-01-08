#ifndef DECK_FXP_H
#define DECK_FXP_H

#if defined(V0_FX_EXTENSION)

#define V0_FXP_REG_BITS         64

typedef int64_t                 deckfxp_t;

#define V0_FXP_QUOT_BITS        32
#define V0_FXP_FRAC_BITS        32

#define V0_FXON_OP              0x00    // enable fixed-point coprocessor
#define V0_FXADD_OP             0x01    // fixed-point addition
#define V0_FXSUB_OP             0x02    // fixed-point subtraction
#define V0_FXMUL_OP             0x03    // fixed-point multiplication
#define V0_FXDIV_OP             0x04    // fixed-point division

#endif /* defined(DECK_FX_EXTENSION) */

#endif /* DECK_FXP_H */

