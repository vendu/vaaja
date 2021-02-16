#ifndef SYS_V0_REGS_H
#define SYS_V0_REGS_H

#define V0_REG_SIZE             32

/* register types */
typedef int32_t                 v0reg_t;
typedef uint32_t                v0ureg_t;

/*
 * general purpose registers R0-R7
 */
#define V0_R0_REG               0x00        // 0-register
#define V0_R1_REG               0x01        // generic register #1; argument 2
#define V0_R2_REG               0x02        // generic register #2; argument 3
#define V0_R3_REG               0x03        // generic register #3; argument 4
#define V0_R4_REG               0x04        // generic register #4; scratch 1
#define V0_R5_REG               0x05        // generic register #5; scratch 2
#define V0_R6_REG               0x06        // generic register #6; scratch 3
#define V0_R7_REG               0x07        // generic register #7; scratch 4
#define V0_GEN_REG_BITS         3           // number of ID-bits
#define V0_GEN_REGS             8           // number of generic registers
/*
 * special registers 8-15
*/
#define V0_MS_REG               0x08        // machine status word
#define V0_CR_REG               0x09        // control register
#define V0_IR_REG               0x0a        // index register/loop count
#define V0_FP_REG               0x0b        // frame-pointer
#define V0_SP_REG               0x0c        // stack-pointer
#define V0_LR_REG               0x0d        // link-register
#define V0_PC_REG               0x0e        // program-counter
#define V0_MF_REG               0x0f        // machine feature-bits
#define V0_REG_BITS             4           // number of register-ID bits
#define V0_SYS_REGS             8
/* total # of register */
#define V0_INT_REGS             16          // number of user-mode (GEN and SYS) registers

/* register bits for STM, LDM */
#define V0_R0_BIT               (1 << V0_R0_REG)
#define V0_R1_BIT               (1 << V0_R1_REG)
#define V0_R2_BIT               (1 << V0_R2_REG)
#define V0_R3_BIT               (1 << V0_R3_REG)
#define V0_R4_BIT               (1 << V0_R4_REG)
#define V0_R5_BIT               (1 << V0_R5_REG)
#define V0_R6_BIT               (1 << V0_R6_REG)
#define V0_R7_BIT               (1 << V0_R7_REG)
#define V0_MS_BIT               (1 << V0_MS_REG)
#define V0_CR_BIT               (1 << V0_CR_REG)
#define V0_IR_BIT               (1 << V0_IR_REG)
#define V0_FP_BIT               (1 << V0_FP_REG)
#define V0_SP_BIT               (1 << V0_SP_REG)
#define V0_LR_BIT               (1 << V0_LR_REG)
#define V0_PC_BIT               (1 << V0_PC_REG)
#define V0_MF_BIT               (1 << V0_MF_REG)
/* register MF/15 is static, so not available for POP */

#define V0_MS_ZF_BIT            (1 << 0)    // zero-bit
#define V0_MS_SF_BIT            (1 << 1)    // sign-bit
#define V0_MS_CF_BIT            (1 << 2)    // carry/borrow-bit
#define V0_MS_OF_BIT            (1 << 3)    // overflow-bit
#define V0_MS_RF_BIT            (1 << 4)    // round up (fraction >= 0.5)
#define V0_MS_IF_BIT            (1 << 5)    // interrupts enabled
#define V0_MS_AF_BIT            (1 << 6)    // auxiliary flag
#define V0_MS_LT_BIT            (1 << 7)    // less than
#define V0_MS_GT_BIT            (1 << 8)    // greater than
#define V0_MS_MP_BIT            (1 << 9)    // multiprocessor extension active
#define V0_MS_MM_BIT            (1 << 10)   // memory management unit active
#define V0_MS_CC_BIT            (1 << 11)   // CORDIC-core active
#define V0_MS_FP_BIT            (1 << 12)   // floating-point coprocessor active
#define V0_MS_FX_BIT            (1 << 13)   // fixed-point coprocessor active
/* currently, bits   (1 << 14) through (1 << 23), i.e. [24:14] are reserved */
#define V0_MS_IF_MASK           0xff000000  // mask IRQs 0x08-0x0f [31:24]
#define V0_MS_IRQ_BIT(i)        (1 << (16 + (i))) // i has to be 8-15 (KBD-FAST)

#endif /* SYS_V0_REGS_H */

