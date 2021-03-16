#ifndef SYS_V0_MP_H
#define SYS_V0_MP_H

#if defined(V0_MP_EXTENSION)

/* multiprocessor support operations */

/* atomic arithmetic-logical operations */
/* ATOMIC, *adr |= src; */
#define V0_AORR_OP              V0_ORR_OP
/* ATOMIC, *adr ^= src; */
#define V0_AXOR_OP              V0_XOR_OP
/* ATOMIC, *adr &= src; */
#define V0_AAND_OP              V0_AND_OP
/* ATOMIC, dest = *adr++; */
#define V0_AINC_OP              V0_INC_OP
/* ATOMIC, dest = *adr--; */
#define V0_ADEC_OP              V0_DEC_OP
/* ATOMIC, ret = *adr, *adr += src; */
#define V0_AADD_OP              V0_ADD_OP
/* ATOMIC, ret = *adr, *adr -= src; */
#define V0_ASUB_OP              V0_DEC_OP

/*
 * - V0_BTS_OP:   parm=V0_SET|V0_ATOMIC_BIT
 * - V0_BTC_OP:   parm=V0_CLR|V0_ATOMIC_BIT
 * - V0_BTF_OP:   parm=V0_FLP|V0_ATOMIC_BIT
 */
/* ATOMIC, dest = *adr & (1 << src); *adr |= 1 << src; */
#define V0_BTS_OP               V0_BIT_OP // test-and-set bit
/* ATOMIC, dest = *adr & (1 << src); *adr &= ~(1 << src); */
#define V0_BTC_OP               V0_BIT_OP // test-and-clear bit
/* ATOMIC, dest = *adr & (1 << src); *adr ^= 1 << src; */
#define V0_BTF_OP               V0_BIT_OP // test-and-flip bit
/* ATOMIC, ret = *adr; if (ret == arg) *adr = src; dest = ret - arg; */
#define V0_CAS_OP               0x2b        // compare-and-swap
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
#define V0_SIG_OP               0x2c
/* parm-field values for V0_SIG_OP */
#define V0_SIG_SEV              0x00
#define V0_SIG_WFI              0x01
#define V0_SIG_WFE              0x02
#define V0_SEV_OP               V0_SIG_OP // parm & 0x03 == V0_SIG_SEV
#define V0_WFI_OP               V0_SIG_OP // parm & 0x03 == V0_SIG_WFI
#define V0_WFE_OP               V0_SIG_OP // parm & 0x03 == V0_SIG_WFE

/*
 * memory-type range registers
 */

#define V0_MT0_REG              0x00
#define V0_MT1_REG              0x01
#define V0_MT2_REG              0x02
#define V0_MT3_REG              0x03
#define V0_MT4_REG              0x04
#define V0_MT5_REG              0x05
#define V0_MT6_REG              0x06
#define V0_MT7_REG              0x07
#define V0_MT8_REG              0x08
#define V0_MT9_REG              0x09
#define V0_MT10_REG             0x0a
#define V0_MT11_REG             0x0b
#define V0_MT12_REG             0x0c
#define V0_MT13_REG             0x0d
#define V0_MT14_REG             0x0e
#define V0_MT15_REG             0x0f

#endif /* defined(V0_MP_EXTENSION) */

#endif /* SYS_V0_MP_H */

