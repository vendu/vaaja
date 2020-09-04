#ifndef __VOIMA_VM_INS_H__
#define __VOIMA_VM_INS_H__

/* unit-member */
#define VOIMA_UNIT_SYS          0x00
#define VOIMA_UNIT_LOGIC        0x01
#define VOIMA_UNIT_SHIFT        0x02
#define VOIMA_UNIT_ARITH        0x03
#define VOIMA_UNIT_BIT          0x04
#define VOIMA_UNIT_FLOW         0x05
#define VOIMA_UNIT_MEM          0x06
#define VOIMA_UNIT_IO           0x07
#define VOIMA_UNIT_COPROC_BIT   0x08
#define VOIMA_UNIT_FPU_ID       0x01
#define VOIMA_UNIT_GPU_ID       0x02
#define VOIMA_UNIT_DSP_ID       0x03
#define VOIMA_UNIT_FXP_ID       0x04
#define VOIMA_UNIT_UNUM_ID      0x05
/* 0x06 and 0x07 are reserved */
#define VOIMA_FPU_UNIT          (VOIMA_UNIT_COPROC_BIT | VOIMA_UNIT_FPU_ID)
#define VOIMA_GPU_UNIT          (VOIMA_UNIT_COPROC_BIT | VOIMA_UNIT_GPU_ID)
#define VOIMA_DSP_UNIT          (VOIMA_UNIT_COPROC_BIT | VOIMA_UNIT_DSP_ID)
#define VOIMA_FXP_UNIT          (VOIMA_UNIT_COPROC_BIT | VOIMA_UNIT_FXP_ID)
#define VOIMA_UNUM_UNIT         (VOIMA_UNIT_COPROC_BIT | VOIMA_UNIT_UNUM_ID)
/* parm-member */
#define VOIMA_PARM_VAL_BIT      (1 << 0)
#define VOIMA_PARM_XTRA_BIT     (1 << 1)
#define VOIMA_PARM_CLR_BIT      (1 << 1)
#define VOIMA_PARM_LONG_BIT     (1 << 2)
#define VOIMA_PARM_IMM_BIT      (1 << 3)
/* for sys unit */
/* VOIMA_PARM_VAL_BIT */
/* VOIMA_PARM_CLR_BIT */
/* for shift unit */
#define VOIMA_PARM_DIR_BIT      (1 << 1)
#define VOIMA_PARM_ROT_BIT      (1 << 2)
/* for arithmetic unit */
#define VOIMA_PARM_UNS_BIT      (1 << 0)
/* VOIMA_PARM_XTRA_BIT */
#define VOIMA_PARM_FLG_BIT      (1 << 2)
/* for bit unit */
/* BST, BCL, BFL, BTS, BTC, BTF */
/* VOIMA_PARM_CLR_BIT */
#define VOIMA_PARM_FLIP_BIT     (1 << 2)
/* CTZ, CLZ, PAR, HAM */
#define VOIMA_PARM_LOW_BIT      (1 << 0)
#define VOIMA_PARM_ONE_BIT      (1 << 1)
#define VOIMA_PARM_MASK_BIT     (1 << 2)
/* HSH, HUN */
#define VOIMA_PARM_REV_BIT      (1 << 0)
/* for memory unit */
/* LDC, LNT, LDL, STC, SNT, STL */
#define VOIMA_PARM_MEM_BIT      (1 << 0)
/* VOIMA_PARM_XTRA_BIT */
#define VOIMA_PARM_COND_BIT     (1 << 2)
/* for io unit */
/* VOIMA_PARM_CLR_BIT */
#define VOIMA_PARM_ALL_BIT      (1 << 2)
/* val-member */
#define VOIMA_VAL_ADR_BIT       (1 << 15)
#define VOIMA_VAL_NDX_BIT       (1 << 14)
#define VOIMA_VAL_OFS_BIT       (1 << 13)
#define VOIMA_VAL_LK_BIT        (1 << 12)
/* for MTR */
#define VOIMA_VAL_WB_BIT        (1 << 15)
#define VOIMA_VAL_WP_BIT        (1 << 14)
#define VOIMA_VAL_WC_BIT        (1 << 13)
#define VOIMA_VAL_WT_BIT        (1 << 12)
/* for BRA, CLD, CST, i.e. branches + conditional load and store  */
#define V0_VAL_ZF_BIT           (1 << 11)
#define V0_VAL_LT_BIT           (1 << 10)
#define V0_VAL_CF_BIT           (1 << 9)
#define V0_VAL_OF_BIT           (1 << 8)
#define VOIMA_VAL_MEM_MASK      (VOIMA_VAL_ADR_BIT                      \
                                 | VOIMA_VAL_NDX_BIT                    \
                                 | VOIMA_VAL_OFS_BIT)
#define VOIMA_VAL_DATA_BITS     12
#define VOIMA_VAL_DATA_MASK     0x0fff
#define VOIMA_VAL_FLG_SHIFT     VOIMA_VAL_DATA_BITS
#define VOIMA_VAL_FLG_MASK      0x0f
struct voimains {
    uint8_t     unit    : 4;
    uint8_t     src     : 4;
    uint8_t     dst     : 4;
    uint8_t     parm    : 4;
    uint16_t    val;
};

#endif /* __VOIMA_VM_INS_H__ */

