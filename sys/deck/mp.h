#ifndef DECK_MP_H
#define DECK_MP_H

#if defined(DECK_MP_EXTENSION)

/* multiprocessor support operations */

/* atomic arithmetic-logical operations */
/* ATOMIC, *adr |= src; */
#define DECK_AORR_OP            DECK_ORR_OP
/* ATOMIC, *adr ^= src; */
#define DECK_AXOR_OP            DECK_XOR_OP
/* ATOMIC, *adr &= src; */
#define DECK_AAND_OP            DECK_AND_OP
/* ATOMIC, dest = *adr++; */
#define DECK_AINC_OP            DECK_INC_OP
/* ATOMIC, dest = *adr--; */
#define DECK_ADEC_OP            DECK_DEC_OP
/* ATOMIC, ret = *adr, *adr += src; */
#define DECK_AADD_OP            DECK_ADD_OP
/* ATOMIC, ret = *adr, *adr -= src; */
#define DECK_ASUB_OP            DECK_DEC_OP

/*
 * - DECK_BTS_OP:   parm=DECK_SET|DECK_ATOMIC_BIT
 * - DECK_BTC_OP:   parm=DECK_CLR|DECK_ATOMIC_BIT
 * - DECK_BTF_OP:   parm=DECK_FLP|DECK_ATOMIC_BIT
 */
/* ATOMIC, dest = *adr & (1 << src); *adr |= 1 << src; */
#define DECK_BTS_OP             DECK_BIT_OP // test-and-set bit
/* ATOMIC, dest = *adr & (1 << src); *adr &= ~(1 << src); */
#define DECK_BTC_OP             DECK_BIT_OP // test-and-clear bit
/* ATOMIC, dest = *adr & (1 << src); *adr ^= 1 << src; */
#define DECK_BTF_OP             DECK_BIT_OP // test-and-flip bit
/* ATOMIC, ret = *adr; if (ret == arg) *adr = src; dest = ret - arg; */
#define DECK_CAS_OP             0x2b        // compare-and-swap
/*
 * WFE and WFI wait for
 * --------------------
 * - IRQ unless masked
 * - FIQ unless masked
 * - a debug request
 *
 * WFE also waits for
 * ------------------
 * - event signaled by another processor using SEV
 */
#define DECK_SIG_OP             0x2c
/* parm-field values for DECK_SIG_OP */
#define DECK_SIG_SEV            0x00
#define DECK_SIG_WFI            0x01
#define DECK_SIG_WFE            0x02
#define DECK_SEV_OP             DECK_SIG_OP // parm & 0x03 == DECK_SIG_SEV
#define DECK_WFI_OP             DECK_SIG_OP // parm & 0x03 == DECK_SIG_WFI
#define DECK_WFE_OP             DECK_SIG_OP // parm & 0x03 == DECK_SIG_WFE

/*
 * memory-type range registers
 */

#define DECK_MT0_REG            0x00
#define DECK_MT1_REG            0x01
#define DECK_MT2_REG            0x02
#define DECK_MT3_REG            0x03
#define DECK_MT4_REG            0x04
#define DECK_MT5_REG            0x05
#define DECK_MT6_REG            0x06
#define DECK_MT7_REG            0x07
#define DECK_MT8_REG            0x08
#define DECK_MT9_REG            0x09
#define DECK_MT10_REG           0x0a
#define DECK_MT11_REG           0x0b
#define DECK_MT12_REG           0x0c
#define DECK_MT13_REG           0x0d
#define DECK_MT14_REG           0x0e
#define DECK_MT15_REG           0x0f

#endif /* defined(DECK_MP_EXTENSION) */

#endif /* DECK_MP_H */

