#ifndef DECK_INTR_H
#define DECK_INTR_H

#include <stdint.h>

#define irqgetfunc(vm, i)                                                   \
    ((void *)((uintptr_t)(vm).irqvec[(i)] & DECK_IRQ_FUNC_MASK))
#define irqsetfunc(vm, i, func, ring)                                   \
    ((vm).irqvec[(i)] |= (void *)((uintptr_t)(func) | (ring)))
#define DECK_IRQ_RING           UINT32_C(0x00000001)
#define DECK_IRQ_RESERVED       UINT32_C(0x00000002)
#define DECK_IRQ_FUNC_MASK      UINT32_C(0xfffffffc)
typedef void                   *deckirqfunc_t;

/* system traps and device interrupt requests */

/*
 * the interface vector - table of handler function pointers + flags - is
 * usually a single page (but could be bigger) with 15 descriptors + the FAST-
 * interrupt handler embedded into the vector right after the other descriptors
 */

/*
 * event/exception/interrupt/fault/abort reason is stored in low 8-bit byte of
 * the interrupt reason-code
 */
#define DECK_EVENT_MASK         UINT32_C(0x000000ff)    // 8-bit event code mask
#define DECK_INFO_MASK          UINT32_C(0xffffff00)    // event info mask

/* system interrupts */
#define     DECK_TMR_TRAP       0x00    // timer tick
#   define DECK_TMR_INTR        0x00    // code for timer interrupt
#define DECK_MACH_TRAP          0x01    // machine/hardware failure
#   define DECK_MACH_NMI        0x00    // non-maskable interrupt
#define DECK_UNDEF_TRAP         0x02    // undefined instruction/opcode
#   define DECK_UNDEF_INST      0x00    // instruction ID reserved/undefined
#   define DECK_INVAL_ARG       0x01    // invalid argument for operation
#define DECK_MATH_TRAP          0x03    // e.g. division by 0
#   define DECK_MATH_DIVZERO    0x00    // zero-divisor for DIV, REM, IRP
#   define DECK_MATH_INVALID    0x01    // invalid argument for operation
#define DECK_MEM_TRAP           0x04    // ALIGN, BUS, SEG, PAGE, ECC, BOUND
#   define DECK_MEM_EXEC        0x00    // page-fault on execute
#   define DECK_MEM_WRITE       0x01    // page-fault on write
#   define DECK_MEM_READ        0x02    // page-fault on read
#   define DECK_MEM_PROT        0x03    // page-access denied
#   define DECK_MEM_BUS         0x04    // bus fault
#   define DECK_MEM_SEG         0x05    // segment fault
#   define DECK_MEM_FAIL        0x06    // memory (ECC or parity) failed
#   define DECK_MEM_CACHE       0x07    // cache-memory [ECC or parity] failed
#define DECK_IO_TRAP            0x05    // I/O
#   define DECK_IO_SEEK         0x00    // I/O-seek failed or not supported
#   define DECK_IO_WRITE        0x01    // I/O-write failed
#   define DECK_IO_READ         0x02    // I/O-read failed
#   define DECK_IO_PERM         0x04    // I/O-port permission denied
#   define DECK_IO_DEV          0x05    // I/O-device failure
#   define DECK_IO_BUS          0x06    // I/O-bus failure
#   define DECK_IO_SYNC         0x07    // I/O-synchronization in progress
#   define DECK_IO_URGENT       0x10    // I/O-data pending (e.g. NIC)
#   define DECK_IO_DONE         0x11    // I/O-operation finished
#define DECK_SYS_TRAP           0x06    // breakpoint, sleep, reboot, ...
#   define DECK_SYS_PROC        0x00    // processor online
#   define DECK_SYS_BUS         0x01    // system bus online
#   define DECK_SYS_DEV         0x02    // system device online
#   define DECK_SYS_RUN         0x03    // system online and running
#   define DECK_SYS_STOP        0x04    // system sleep-mode activated (WFI/WFE)
#   define DECK_SYS_CONT        0x05    // system ops continued/awaken (SEV)
#   define DECK_SYS_BREAK       0x06    // debugger breakpoint
#   define DECK_SYS_RESET       0x07    // system reset request
#   define DECK_SYS_ABORT       0x08    // system-triggered abort
#   define DECK_SYS_BOUND       0x09
#   define DECK_SYS_RING        0x0a    // attempted system-op in user-mode
#define DECK_SOFT_TRAP          0x07    // user-generated interrupt
#   define DECK_DEV_MASK        0xf0    // mask for device/interrupt ID
#   define DECK_SOFT_RESET      0x00    // initialize reset sequence
#   define DECK_SOFT_BOOT       0x01    // initialize reboot/shutdown wequence
#   define DECK_SOFT_STOP       0x02    // stop/enter energy-saving mode
#   define DECK_SOFT_CONT       0x03    // leave energy-saving mode
#   define DECK_SOFT_YIELD      0x04    // release processor [for other threads]
#   define DECK_SOFT_THREAD     0x05    // launch new thread
#   define DECK_SOFT_DETACH     0x06    // detach a thread (make it unjoinable)
#   define DECK_SOFT_JOIN       0x07    // join a thread (attach to wait)
/* 0x08-0x0f are reserved */
#   define DECK_SOFT_BITS       15
#   define DECK_KBD_BITS        0x10
#   define DECK_KBD_INIT        0x10    // initialize keybaord to READY-state
#   define DECK_KBD_READ        0x11    // read keyboard input; poll on != READY
/* 0x22-0x2f are reserved */
#   define DECK_HID_BITS        0x20
#   define DECK_SND_INIT        0x30    // initialize sound device (incl. bufs)
#   define DECK_SND_LOAD        0x31    // load sample/patch
#   define DECK_SND_UNLOAD      0x32    // unload sample/batch
#   define DECK_SND_START       0x33    // start audio-playback
#   define DECK_SND_SAMPLE      0x34    // start audio-recording
#   define DECK_SND_READ        0x35    // read data into buffer
/* 0x36-0x3f are reserved */
#   define DECK_SND_BITS        0x30
#   define DECK_GFX_INIT        0x40    // initialize graphics (fb, bpp, res)
#   define DECK_GFX_SYNC        0x41    // synchronize screen from framebuf
#   define DECK_GFX_BITS        0x40
/* 0x42-0x4f are reserved */
#   define DECK_STOR_INIT       0x50    // set storage device attributes
#   define DECK_STOR_START      0x51    // e.g. spin disk up
#   define DECK_STOR_STOP       0x52    // e.g. spin disk down
#   define DECK_STOR_SEEK       0x53    // seek where relevant
#   define DECK_STOR_READ       0x54    // read from device
#   define DECK_STOR_WRITE      0x55    // write to device
#   define DECK_STOR_SYNC       0x56    // synchronize device-data; write bufs
#   define DECK_STOR_BITS       0x50
/* 0x57-0x5f are reserved */
#   define DECK_CARD_INIT       0x60    // initialize flash/punch-card device
#   define DECK_CARD_READ       0x61    // read from card
#   define DECK_CARD_WRITE      0x62    // flash or punch to card
#   define DECK_CARD_BITS       0x60
/* 0x63-0x6f are reserved */
#   define DECK_TAPE_INIT       0x70    // initialize tape-drive
#   define DECK_TAPE_START      0x71    // start tape-drive
#   define DECK_TAPE_STOP       0x72    // stop tape-drive
#   define DECK_TAPE_SEEK       0x73    // seek tape-drive
#   define DECK_TAPE_READ       0x74    // read from tape-drive
#   define DECK_TAPE_WRITE      0x75    // write to tape-drive
#   define DECK_TAPE_SYNC       0x76    // synchronize tape-drive; write bufs
#   define DECK_TAPE_BITS       0x70
/* 0x77-0x7f are reserved */
#   define DECK_FAST_INIT       0x80    // initialize device(s) for fast IRQ
#   define DECK_FAST_BITS       0x80
/* 0x81-0x8f are reserved */
#define DECK_SYSTEM_INTRS       8       // # of system-triggered interrupts

/* device interrups */
#define DECK_KBD_IRQ            0x08    // keyboard
#   define DECK_KBD_READY       0x00    // KBD ready for I/O
#   define DECK_KBD_BUSY        0x01    // KBD busy
/* 0x02-0x0f are reserved */
#define DECK_HID_IRQ            0x09    // human interface device such as mouse
#   define DECK_HID_READY       0x00    // HID ready for I/O
#   define DECK_HID_BUSY        0x01    // HID busy
#   define DECK_HID_MOVE        0x02    // pointer-device move
#   define DECK_HID_CLICK       0x03    // button clicked
#   define DECK_HID_REL         0x04    // button released
#   define DECK_HID_STOP        0x04    // DECK_HID_READY when reconnected
/* 0x05-0x0f are reserved */
#define DECK_SND_IRQ            0x0a    // audio event
#   define DECK_SND_PLAY        0x00    // sound device playback started
#   define DECK_SND_RECORD      0x01    // sound device recording started
#   define DECK_SND_STOP        0x02    // sound device playback stopped
#   define DECK_SND_LOBUF       0x03    // buffer is getting empty (25 %)
#   define DECK_SND_HIBUF       0x04    // buffer is 75 % full
/* 0x05-0x0f are reserved */
#define DECK_GFX_IRQ            0x0b    // video event (draw, syncbuf, copybuf)
#define DECK_STOR_IRQ           0x0c    // disk/disc/tape/ssd; fast I/O
#define DECK_CARD_IRQ           0x0d    // punchcard interface :)
#define DECK_TAPE_IRQ           0x0e    // tape interface
#define DECK_FAST_IRQ           0x0f    // fast-interrupt; handler inside vector
#define DECK_DEV_IRQS           8       // # of device generated interrupts
#define DECK_TOTAL_INTRS        16

#endif /* DECK_INTR_H */

