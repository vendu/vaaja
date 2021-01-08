#ifndef DECK_SANITY_H
#define DECK_SANITY_H

#include <deck/deck.h>
#include <stdio.h>

#if defined(DECK_SANITY_CHECK)
#   define DECK_PRINT_SANITY()  fprintf(stderr, "SANITY CHECKS PASSED\n")
#endif

/* check structure sizes in <deck/inst.h> */

#if (DECK_STDINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_CMPINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckcmpinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_JMPINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckjmpinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_BRAINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckbrainst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_STKINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckstkhinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_MAPINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckmapinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if (DECK_IOINST_SIZE != DECK_INST_BITS)
#   error sizeof(struct deckioinst) != DECK_INST_BITS in <deck/inst.h>
#endif

#if defined(DECK_MM_EXTENSION)

/* check structure sizes in <deck/mmu.h> */

#if (DECK_MTR_SIZE1 != DECK_REG_SIZE)
#   error struct deckmtr header != DECK_REG_SIZE in <deck/mmu.h>
#endif
#if (DECK_MTR_SIZE2 != DECK_REG_SIZE)
#   error struct deckmtr footer != DECK_REG_SIZE in <deck/mmu.h>
#endif
#if (DECK_MTR_SIZE != 2 * DECK_REG_SIZE)
#   error sizeof(struct deckmtr) != 2 * DECK_REG_SIZE in in <deck/mmu.h>
#endif

#endif /* defined(DECK_MM_EXTENSION) */

#if defined(DECK_FP_EXTENSIONS)

#include <deck/fpu.h>

/* check structure sizes in <deck/fpu.h> */

#if (DECK_FLOAT_MANT_BITS + DECK_FLOAT_EXP_BITS                         \
     != DECK_FLOAT_BITS)
#   error sizeof(struct deckfloat) != DECK_FLOAT_BITS in <deck/fpu.h>
#endif

#if (DECK_DOUBLE_MANT_BITS + DECK_DOUBLE_EXP_BITS                       \
     != DECK_DOUBLE_BITS)
#   error sizeof(struct deckdouble) != DECK_DOUBLE_BITS in <deck/fpu.h>
#endif

#endif /* defined(DECK_FP_EXTENSIONS) */

#endif /* DECK_SANITY_H */

