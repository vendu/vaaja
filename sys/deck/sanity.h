#ifndef DECK_SANITY_H
#define DECK_SANITY_H

#include <stdio.h>

#if defined(DECK_SANITY_CHECK)
#   define DECK_PRINT_SANITY()  fprintf(stderr, "SANITY CHECKS PASSED\n")
#endif

/* check structure sizes in <deck/inst.h> */

#if (DECK_OP_BITS + DECK_COND_BITS + 2 * DECK_GEN_REG_BITS              \
     + DECK_REG_BITS + DECK_ADR_BITS + DECK_FOLD_BITS + 8               \
     != DECK_INST_BITS)
#   error sizeof(struct deckinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + DECK_GEN_REG_BITS + DECK_REG_BITS  \
     + 7 + 9                                                            \
     != DECK_INST_BITS)
#   error sizeof(struct deckcmpinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_OFS_BITS                                       \
     != DECK_INST_BITS)
#   error sizeof(struct deckjmpinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + DECK_BRA_BITS                      \
     != DECK_INST_BITS)
#   error sizeof(struct deckbrainst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + 7 + DECK_REG_BITS + 12             \
     != DECK_INST_BITS)
#   error sizeof(struct deckstkhinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + 7 + DECK_INT_REGS                  \
     != DECK_INST_BITS)
#   error sizeof(struct deckmapinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_OP_BITS + DECK_COND_BITS + 2 * DECK_GEN_REG_BITS + 1 \
     + DECK_PORT_BITS + 4                                      \
     != DECK_INST_BITS)
#   error sizeof(struct deckioinst) != DECK_INST_BITS in <deck/inst.h>
#endif

/* check structure sizes in <deck/mmu.h> */

#if (DECK_PAGE_ADR_BITS + DECK_MTR_TYPE_BITS + 7                        \
     != DECK_REG_SIZE)
#   errof struct deckmtr header != DECK_REG_SIZE in <deck/mmu.h>
#endif
#if (DECK_PAGE_ADR_BITS + DECK_PAGE_PERM_BITS                           \
     != DECK_REG_SIZE)
#   errof struct deckmtr footer != DECK_REG_SIZE in <deck/mmu.h>
#endif
#if (DECK_PAGE_ADR_BITS + DECK_MTR_TYPE_BITS + 7                        \
     + DECK_PAGE_ADR_BITS + DECK_PAGE_PERM_BITS                         \
     != 2 * DECK_REG_SIZE)
#   error sizeof(struct deckmtr) != DECK_REG_SIZE in in <deck/mmu.h>
#endif

/* check structure sizes in <deck/fpu.h> */

#if (DECK_FLOAT_MANT_BITS + DECK_FLOAT_EXP_BITS                         \
     != DECK_FLOAT_BITS)
#   errof sizeof(struct deckfloat) != DECK_FLOAT_BITS in <deck/fpu.h>
#endif

#if (DECK_DOUBLE_MANT_BITS + DECK_DOUBLE_EXP_BITS                       \
     != DECK_DOUBLE_BITS)
#   errof sizeof(struct deckdouble) != DECK_DOUBLE_BITS in <deck/fpu.h>
#endif

#endif /* DECK_SANITY_H */

