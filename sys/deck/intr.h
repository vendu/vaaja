#ifndef DECK_INTR_H
#define DECK_INTR_H

/* system traps and device interrupt requests */

/*
 * the interface vector - table of handler function pointers + flags - is
 * usually a single page (but could be bigger) with 15 descriptors + the FAST-
 * interrupt handler embedded into the vector right after the other descriptors
 */

/*
 *
 * errno                if a system call is interrupted, errno is set to EINTR
 *
 * system trap handlers return values in R1 and on user-stack as follows
 *
 * DECK_TMR_TRAP        - R1: system timestamp value at the time of interrupt
 * DECK_SYS_TRAP        - R1: system call return value; 0 on success
 * DECK_MACH_TRAP       - R1: machine error-code
 * DECK_UNDEF_TRAP      - R1: invalid instruction ID from the opcode
 * DECK_MATH_TRAP       - R1: error type (MATH_DIVZERO, ...)
 * DECK_MEM_TRAP        - R1: failure/fault type (MEM_ECC_FAIL, MEM_ALIGN_FAIL,
 *                            MEM_BUS_ERROR, MEM_SEG_ERROR, MEM_PAGE_FAULT)
 * DECK_PROT_TRAP       - R1: error type (PORT_PERM_FAIL, SYS_MEM_PERM,
 *                            SYS_IO_PERM, SYS_DEV_PERM)
 * DECK_STOP_TRAP       - R1: trap code (SYS_BREAKPOINT, SYS_SLEEP, SYS_WAIT,
 *                                       SYS_PAUSE, SYS_IOSEL)
 * device interrupt handlers return values in R1 and on event-queues as follows
 * DECK_KBD_TRAP        - R1: 21-bit unicode symbol + 11 modifier bits
 *                            (CAPS, SHIFT, NUM, CTRL, ALT, ALTGR,
 *                             COMPOSE, WIN, ...)
 * DECK_HID_TRAP        - R1: 16-bit (low) X + 16-bit (high) Y coordinates
 */

/* system traps */
#define DECK_TMR_TRAP           0x00    // timer tick
#define DECK_BRK_TRAP           0x01    // debugger breakpoint
#define DECK_MACH_TRAP          0x02    // machine/hardware failure
#define DECK_UNDEF_TRAP         0x03    // undefined instruction/opcode
#define DECK_MATH_TRAP          0x04    // e.g. division by 0
#define DECK_MEM_TRAP           0x05    // ALIGN, BUS, SEG, PAGE, ECC, BOUND
#define DECK_PROT_TRAP          0x06    // no I/O, code, or memory permission
#define DECK_NMI_TRAP           0x07    // breakpoint, sleep, reboot, ...
/* device interrups */
#define DECK_KBD_IRQ            0x08    // keyboard
#define DECK_HID_IRQ            0x09    // human interface device such as mouse
#define DECK_SND_IRQ            0x0a    // audio event (play, readbuf, ...)
#define DECK_GFX_IRQ            0x0b    // video event (draw, syncbuf, copybuf)
#define DECK_STOR_IRQ           0x0c    // disk/disc/tape/ssd; fast I/O
#define DECK_CARD_IRQ           0x0d    // punchcard interface :)
#define DECK_TAPE_IRQ           0x0e    // tape interface
#define DECK_FAST_IRQ           0x0f    // fast-interrupt; handler inside vector

#endif /* DECK_INTR_H */

