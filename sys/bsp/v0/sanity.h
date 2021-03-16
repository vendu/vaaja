#ifndef SYS_V0_SANITY_H
#define SYS_V0_SANITY_H

#include <v0/v0.h>
#include <stdio.h>

#if defined(V0_SANITY_CHECK)
#   define V0_PRINT_SANITY()    fprintf(stderr, "SANITY CHECKS PASSED\n")
#endif

/* check structure sizes in <v0/inst.h> */

#if (V0_STDINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0inst) != V0_INST_BITS in <v0/inst.h>
#endif

#if (V0_CMPINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0cmpinst) != V0_INST_BITS in <v0/inst.h>
#endif

#if (V0_JMPINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0jmpinst) != V0_INST_BITS in <v0/inst.h>
#endif

#if (V0_BRAINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0brainst) != V0_INST_BITS in <v0/inst.h>
#endif

#if (V0_STKINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0stkhinst) != V0_INST_BITS in <v0/inst.h>
#endif

#if (V0_MAPINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0mapinst) != V0_INST_BITS in <v0/inst.h>
#endif

#if (V0_IOINST_SIZE != V0_INST_BITS)
#   error sizeof(struct v0ioinst) != V0_INST_BITS in <v0/inst.h>
#endif

#if defined(V0_MM_EXTENSION)

/* check structure sizes in <v0/mmu.h> */

#if (V0_MTR_SIZE1 != V0_REG_SIZE)
#   error struct v0mtr header != V0_REG_SIZE in <v0/mmu.h>
#endif
#if (V0_MTR_SIZE2 != V0_REG_SIZE)
#   error struct v0mtr footer != V0_REG_SIZE in <v0/mmu.h>
#endif
#if (V0_MTR_SIZE != 2 * V0_REG_SIZE)
#   error sizeof(struct v0mtr) != 2 * V0_REG_SIZE in in <v0/mmu.h>
#endif

#endif /* defined(V0_MM_EXTENSION) */

#if defined(V0_FP_EXTENSIONS)

#include <v0/fpu.h>

/* check structure sizes in <v0/fpu.h> */

#if (V0_FLOAT_MANT_BITS + V0_FLOAT_EXP_BITS                         \
     != V0_FLOAT_BITS)
#   error sizeof(struct v0float) != V0_FLOAT_BITS in <v0/fpu.h>
#endif

#if (V0_DOUBLE_MANT_BITS + V0_DOUBLE_EXP_BITS                       \
     != V0_DOUBLE_BITS)
#   error sizeof(struct v0double) != V0_DOUBLE_BITS in <v0/fpu.h>
#endif

#endif /* defined(V0_FP_EXTENSIONS) */

#endif /* SYS_V0_SANITY_H */

