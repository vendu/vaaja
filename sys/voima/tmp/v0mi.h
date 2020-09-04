#ifndef __VOIMA_V0MI_H__
#define __VOIMA_V0MI_H__

#define V0_ALU_UNIT 0x0
#define V0_SYS_UNIT 0x1
#define V0_FPU_UNIT 0x2
#define V0_DSP_UNIT 0x3
#define V0_GPU_UNIT 0x4
#define V0_FXP_UNIT 0x5
#define V0_BSP_UNIT 0x6
#define V0_AUX_UNIT 0x7

#define v0getunit(ins)          ((ins)->op >> V0_INS_BITS)
#define v0getop(ins)            ((ins)->op & V0_INS_MASK)

#endif /* __VOIMA_V0MI_H__ */

