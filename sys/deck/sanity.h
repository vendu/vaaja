#ifndef DECK_SANITY_H
#define DECK_SANITY_H

#include <stdio.h>

#if defined(DECK_SANITY_CHECK)
#   define DECK_PRINT_SANITY()  fprintf(stderr, "SANITY CHECKS PASSED\n")
#endif

/* make sure the instruction-structs in <deck/inst.h> occupy DECK_INST_BITS */

#if (DECK_OP_BITS + DECK_COND_BITS + 2 * DECK_GEN_REG_BITS              \
     + DECK_REG_BITS + DECK_ADR_BITS + DECK_FOLD_BITS + 8 != DECK_INST_BITS)
#   error sizeof(struct deckinst) != DECK_INST_BITS <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + DECK_GEN_REG_BITS + DECK_REG_BITS  \
     + 7 + 9 != DECK_INST_BITS)
#   error sizeof(struct deckcmpinst) != DECK_INST_BITS <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_OFS_BITS != DECK_INST_BITS)
#   error sizeof(struct deckjmpinst) != DECK_INST_BITS <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + DECK_BRA_BITS != DECK_INST_BITS)
#   error sizeof(struct deckbrainst) != DECK_INST_BITS <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + 7 + DECK_REG_BITS + 12 != DECK_INST_BITS)
#   error sizeof(struct deckstkhinst) != DECK_INST_BITS <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + 7 + DECK_INT_REGS != DECK_INST_BITS)
#   error sizeof(struct deckmapinst) != DECK_INST_BITS <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + 2 * DECK_GEN_REG_BITS + 1 \
     + DECK_PORT_BITS + 4 != DECK_INST_BITS)
#   error sizeof(struct deckioinst) != DECK_INST_BITS <deck/inst.h>
#endif

#endif /* DECK_SANITY_H */

