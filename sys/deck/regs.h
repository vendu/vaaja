#ifndef DECK_REGS_H
#define DECK_REGS_H

#define DECK_REG_SIZE           32

/* register types */
typedef int32_t                 deckreg_t;
typedef uint32_t                deckureg_t;

/*
 * general purpose registers R0-R7
 */
#define DECK_R0_REG             0x00        // 0-register
#define DECK_R1_REG             0x01        // generic register #1; argument 2
#define DECK_R2_REG             0x02        // generic register #2; argument 3
#define DECK_R3_REG             0x03        // generic register #3; argument 4
#define DECK_R4_REG             0x04        // generic register #4; scratch 1
#define DECK_R5_REG             0x05        // generic register #5; scratch 2
#define DECK_R6_REG             0x06        // generic register #6; scratch 3
#define DECK_R7_REG             0x07        // generic register #7; scratch 4
#define DECK_GEN_REG_BITS       3           // number of ID-bits
#define DECK_GEN_REGS           8           // number of generic registers
/*
 * special registers 8-15
*/
#define DECK_MS_REG             0x08        // machine status word
#define DECK_CR_REG             0x09        // loop counter for REP
#define DECK_IR_REG             0x0a        // index register
#define DECK_FP_REG             0x0b        // frame-pointer
#define DECK_SP_REG             0x0c        // stack-pointer
#define DECK_LR_REG             0x0d        // link-register
#define DECK_PC_REG             0x0e        // program-counter
#define DECK_MF_REG             0x0f        // machine feature-bits
#define DECK_REG_BITS           4           // number of register-ID bits
#define DECK_SYS_REGS           8
/* total # of register */
#define DECK_INT_REGS           16          // number of user-mode registers

/* register bits for STM, LDM */
#define DECK_R0_BIT             (1 << DECK_R0_REG)
#define DECK_R1_BIT             (1 << DECK_R1_REG)
#define DECK_R2_BIT             (1 << DECK_R2_REG)
#define DECK_R3_BIT             (1 << DECK_R3_REG)
#define DECK_R4_BIT             (1 << DECK_R4_REG)
#define DECK_R5_BIT             (1 << DECK_R5_REG)
#define DECK_R6_BIT             (1 << DECK_R6_REG)
#define DECK_R7_BIT             (1 << DECK_R7_REG)
#define DECK_MS_BIT             (1 << DECK_MS_REG)
#define DECK_CR_BIT             (1 << DECK_CR_REG)
#define DECK_NDX_BIT            (1 << DECK_IR_REG)
#define DECK_FP_BIT             (1 << DECK_FP_REG)
#define DECK_SP_BIT             (1 << DECK_SP_REG)
#define DECK_LR_BIT             (1 << DECK_LR_REG)
#define DECK_PC_BIT             (1 << DECK_PC_REG)
#define DECK_MF_BIT             (1 << DECK_MF_REG)
/* register MF/15 is static, so not available for POP */

#define DECK_MS_ZF_BIT          (1 << 0)    // zero-bit
#define DECK_MS_SF_BIT          (1 << 1)    // sign-bit
#define DECK_MS_CF_BIT          (1 << 2)    // carry/borrow-bit
#define DECK_MS_OF_BIT          (1 << 3)    // overflow-bit
#define DECK_MS_RF_BIT          (1 << 4)    // round up (fraction >= 0.5)
#define DECK_MS_IF_BIT          (1 << 5)    // interrupts enabled
#define DECK_MS_AF_BIT          (1 << 6)    // auxiliary flag
#define DECK_MS_LT_BIT          (1 << 7)    // less than
#define DECK_MS_GT_BIT          (1 << 8)    // greater than
#define DECK_MS_MP_BIT          (1 << 9)    // multiprocessor extension active
#define DECK_MS_MM_BIT          (1 << 10)   // memory management unit active
#define DECK_MS_CC_BIT          (1 << 11)   // CORDIC-core active
#define DECK_MS_FP_BIT          (1 << 12)   // floating-point coprocessor active
#define DECK_MS_FX_BIT          (1 << 13)   // fixed-point coprocessor active
/* currently, bits (1 << 14) through (1 << 23), i.e. [24:14] are reserved */
#define DECK_MS_IF_MASK         0xff000000  // mask IRQs 0x08-0x0f [31:24]
#define DECK_MS_IRQ_BIT(i)      (1 << (16 + (i))) // i has to be 8-15 (KBD-FAST)

#endif /* DECK_REGS_H */

