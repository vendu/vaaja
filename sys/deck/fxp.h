#ifndef DECK_FXP_H
#define DECK_FXP_H

#if defined(DECK_FX_EXTENSION)

#define DECK_FXP_REG_BITS        64

typedef int64_t                 deckfxp_t;

#define DECK_FXP_QUOT_BITS      32
#define DECK_FXP_FRAC_BITS      32

#define DECK_FXON_OP            0x00    // enable fixed-point coprocessor
#define DECK_FXADD_OP           0x01
#define DECK_FXSUB_OP           0x02
#define DECK_FXMUL_OP           0x03
#define DECK_FXDIV_OP           0x04

#endif /* defined(DECK_FX_EXTENSION) */

#endif /* DECK_FXP_H */

