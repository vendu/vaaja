#ifndef __V0_TRAP_H__
#define __V0_TRAP_H__

#include <v0/types.h>

#define v0trapirq(t)            ((t) & V0_IRQ_TRAP_BIT)
#define v0trapirqid(t)          ((t) & ~V0_IRQ_TRAP_BIT)

/* hardware exceptions */
#define V0_DZ_TRAP          0x00 // divizion by zero            - code is PC
#define V0_OP_TRAP          0x01 // invalid opcode              - code is PC
#define V0_PF_TRAP          0x02 // page-fault                  - ADR + flags
#define V0_DF_TRAP          0x03 // double-fault                - code is ADR
#define V0_TF_TRAP          0x04 // triple-fault                - code is ADR
#define V0_TS_TRAP          0x05 // invalid task state          - code is ADR
#define V0_NP_TRAP          0x06 // segment not present         - code is ADR

#define V0_SS_TRAP          0x07 // stack-segment full          - code is SP
#define V0_PE_TRAP          0x08 // protection error            - code is ADR
#define V0_FP_TRAP          0x09 // floating-point error        - code is PC
#define V0_OF_TRAP          0x0a // overflow                    - code is PC
#define V0_BR_TRAP          0x0b // bound-range exceeded        - code is ADR
#define V0_AC_TRAP          0x0c // alignment check             - code is ADR
#define V0_MF_TRAP          0x0d // memory failure (ECC)        - code is ADR
#define V0_MC_TRAP          0x0e // machine check               - code is ERROR
#define V0_IO_TRAP          0x0f // I/O error                   - ADR + flags
#define V0_DB_TRAP          0x10 // debug trap                  - code is PC
#define V0_BP_TRAP          0x11 // breakpoint                  - code is PC
#define V0_NMI_TRAP         0x12 // non-maskable interrupt
#define V0_LAST_SYS_TRAP    0x1f
#define V0_IRQ_TRAP_BIT     0x20
/*
 * peripheral devices
 * - human interface devices are _very_ high priority
 */
#define V0_IRQ_TRAP(i)      (V0_IRQ_TRAP_BIT | (i))
#define V0_IRQ_0x00_TRAP    V0_IRQ_TRAP(0x00)   // interrupt 0  - TMR
#define V0_IRQ_0x04_TRAP    V0_IRQ_TRAP(0x01)   // interrupt 1  - KBD
#define V0_IRQ_0x05_TRAP    V0_IRQ_TRAP(0x02)   // interrupt 2  - PTR
#define V0_IRQ_0x01_TRAP    V0_IRQ_TRAP(0x03)   // interrupt 3  - HID
#define V0_IRQ_0x02_TRAP    V0_IRQ_TRAP(0x04)   // interrupt 4  - AUD
#define V0_IRQ_0x03_TRAP    V0_IRQ_TRAP(0x05)   // interrupt 5  - VID
#define V0_IRQ_0x06_TRAP    V0_IRQ_TRAP(0x06)   // interrupt 6  - NET
#define V0_IRQ_0x07_TRAP    V0_IRQ_TRAP(0x07)   // interrupt 7  - DSK
#define V0_IRQ_0x08_TRAP    V0_IRQ_TRAP(0x08)   // interrupt 8  - OPT
#define V0_IRQ_0x09_TRAP    V0_IRQ_TRAP(0x09)   // interrupt 9  - USB
#define V0_IRQ_0x0a_TRAP    V0_IRQ_TRAP(0x0a)   // interrupt 10 - DEV
#define V0_IRQ_0x0b_TRAP    V0_IRQ_TRAP(0x0b)   // interrupt 11
#define V0_IRQ_0x0c_TRAP    V0_IRQ_TRAP(0x0c)   // interrupt 12
#define V0_IRQ_0x0d_TRAP    V0_IRQ_TRAP(0x0d)   // interrupt 13
#define V0_IRQ_0x0e_TRAP    V0_IRQ_TRAP(0x0e)   // interrupt 14
#define V0_IRQ_0x0f_TRAP    V0_IRQ_TRAP(0x0f)   // interrupt 15
#define V0_MAX_TRAPS        32                  // max # of traps for system

/* predefined traps */
#define V0_TMR_TRAP         V0_IRQ_0x00_TRAP    // timer interrupt
#define V0_KBD_TRAP         V0_IRQ_0x01_TRAP    // keyboard interrupt
#define V0_PTR_TRAP         V0_IRQ_0x02_TRAP    // pointer such as mouse
#define V0_HID_TRAP         V0_IRQ_0x03_TRAP    // human interface device
#define V0_AUD_TRAP         V0_IRQ_0x04_TRAP    // audio interrupt
#define V0_VID_TRAP         V0_IRQ_0x05_TRAP    // video interrupt
#define V0_NET_TRAP         V0_IRQ_0x06_TRAP    // network interrupt
#define V0_DSK_TRAP         V0_IRQ_0x07_TRAP    // disk interrupt
#define V0_OPT_TRAP         V0_IRQ_0x08_TRAP    // optical/disc interrupt
#define V0_USB_TRAP         V0_IRQ_0x09_TRAP    // USB interrupt
#define V0_DEV_TRAP         V0_IRQ_0x0a_TRAP    // miscellaneous devices

/* IV-register contents */

/* v0trapfunc */
#define V0_SYS_TRAP_BIT     (1 << 0)            // system-mode trap execution
#define V0_STK_TRAP_BIT     (1 << 1)            // traps on separate stack
#define V0_TRAP_ADR_MASK    0xfffffffc     	    // trap-handler function pointer

/* PF (page-fault) trap error code bits */
#define V0_PF_EXEC          (1 << 0)       	    // page-fault during execution
#define V0_PF_WRITE         (1 << 1)       	    // page-fault during write
#define V0_PF_READ          (1 << 2)       	    // page-fault during read
#define V0_PF_RING0         (1 << 3)       	    // page-fault with system page
#define V0_PF_STACK         (1 << 4)       	    // stack-fault
/* IO (input/output) trap error code bits */
#define V0_IO_EXEC          (1 << 0)       	    // execute error
#define V0_IO_WRITE         (1 << 1)       	    // write error
#define V0_IO_READ          (1 << 2)            // read error
#define V0_IO_PERM          (1 << 3)       	    // no I/O permission
#define V0_IO_SEEK          (1 << 4)       	    // invalid seek position
#define V0_IO_DEV_ERROR     (1 << 5)            // I/O error
#define V0_IO_CHK           (1 << 6)            // check if I/O allowed

/* trap stack-frame */
/*
 * top-to-bottom
 * -------------
 * - ret
 * - ufp
 * - usp
 * - msw
 * - code
 */
struct v0trapframe {
    m_uword_t   code;                           // error code or 0 if none
    m_uword_t   msw;                            // machine status-word
    m_uword_t   usp;                   	        // user-mode stack-pointer
    m_uword_t   ufp;                   	        // user-mode frame-pointer
    m_uword_t   retp;                           // return address
};

#endif /* __V0_TRAP_H__ */

