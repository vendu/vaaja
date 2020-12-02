#ifndef DECK_REGS_H
#define DECK_REGS_H

/*
 * general purpose registers R0-R7
 */
#define DECK_R0_REG             0x00        // generic register #0; argument 1
#define DECK_R1_REG             0x01        // generic register #1; argument 2
#define DECK_R2_REG             0x02        // generic register #2; argument 3
#define DECK_R3_REG             0x03        // generic register #3; argument 4
#define DECK_R4_REG             0x04        // generic register #4; scratch 1
#define DECK_R5_REG             0x05        // generic register #5; scratch 2
#define DECK_R6_REG             0x06        // generic register #6; scratch 3
#define DECK_R7_REG             0x07        // generic register #7; scratch 4
/*
 * special registers 8-15 */
#define DECK_CNT_REG            0x08        // loop counter for REP
#define DECK_NDX_REG            0x09        // index register
#define DECK_FP_REG             0x0a        // frame-pointer
#define DECK_SP_REG             0x0b        // stack-pointer
#define DECK_LR_REG             0x0c        // link-register
#define DECK_PC_REG             0x0d        // program-counter
#define DECK_MSW_REG            0x0e        // machine status word
#define DECK_MF_REG             0x0f        // machine feature-bits
#define DECK_REG_BITS           4           // number of register-ID bits
#define DECK_USER_REGS          16          // number of user-mode registers

/* register bits for STM, LDM */
#define DECK_R0_BIT             (1 << DECK_R0_REG)
#define DECK_R1_BIT             (1 << DECK_R1_REG)
#define DECK_R2_BIT             (1 << DECK_R2_REG)
#define DECK_R3_BIT             (1 << DECK_R3_REG)
#define DECK_R4_BIT             (1 << DECK_R4_REG)
#define DECK_R5_BIT             (1 << DECK_R5_REG)
#define DECK_R6_BIT             (1 << DECK_R6_REG)
#define DECK_R7_BIT             (1 << DECK_R7_REG)
#define DECK_CNT_BIT            (1 << DECK_CNT_REG)
#define DECK_NDX_BIT            (1 << DECK_NDX_REG)
#define DECK_FP_BIT             (1 << DECK_FP_REG)
#define DECK_SP_BIT             (1 << DECK_SP_REG)
#define DECK_LR_BIT             (1 << DECK_LR_REG)
#define DECK_PC_BIT             (1 << DECK_PC_REG)
#define DECK_MSW_BIT            (1 << DECK_MSW_REG)
#define DECK_MF_BIT             (1 << DECK_MF_REG)

#define DECK_MSW_ZF_BIT         (1 << 0)    // zero-bit
#define DECK_MSW_SF_BIT         (1 << 1)    // sign-bit
#define DECK_MSW_CF_BIT         (1 << 1)    // carry/borrow-bit
#define DECK_MSW_OF_BIT         (1 << 2)    // overflow-bit
#define DECK_MSW_RF_BIT         (1 << 3)    // round up (fraction >= 0.5)
#define DECK_MSW_IF_BIT         (1 << 4)    // interrupts enabled
#define DECK_MSW_AF_BIT         (1 << 5)    // auxiliary flag
#define DECK_MSW_LT_BIT         (1 << 6)    // less than
#define DECK_MSW_GT_BIT         (1 << 7)    // greater than

#endif /* DECK_REGS_H */

