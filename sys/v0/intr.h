#ifndef SYS_V0_INTR_H
#define SYS_V0_INTR_H

#include <stdint.h>

#define V0_IRQ_RING             UINT32_C(0x00000001)
#define V0_IRQ_RESERVED         UINT32_C(0x00000002)
#define V0_IRQ_FUNC_MASK        UINT32_C(0xfffffffc)

#define intrgetfunc(vm, i)                                              \
    ((void *)((uintptr_t)(vm).intrvec[(i)] & V0_IRQ_FUNC_MASK))
#define intrsetfunc(vm, i, func, ring)                                  \
    ((vm).intrvec[(i)] |= (void *)((uintptr_t)(func) | (ring)))

typedef void                   *v0intrfunc_t;
typedef int8_t                  v0intr_t;

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
#define V0_INTR_EVENT_MASK      UINT32_C(0x000000ff)    // 8-bit event code mask
#define V0_INTR_INFO_MASK       UINT32_C(0xffffff00)    // event info mask

/* system interrupts */
#define  V0_TMR_TRAP            0x00    // timer tick
#   define V0_TMR_INTR          0x00    // code for timer interrupt
#define V0_MACH_TRAP            0x01    // machine/hardware failure
#   define V0_MACH_NMI          0x00    // non-maskable interrupt
#define V0_UNDEF_TRAP           0x02    // undefined instruction/opcode
#   define V0_UNDEF_INST        0x00    // instruction ID reserved/undefined
#   define V0_INVAL_ARG         0x01    // invalid argument for operation
#define V0_MATH_TRAP            0x03    // e.g. division by 0
#   define V0_MATH_DIVZERO      0x00    // zero-divisor for DIV, REM, IRP
#   define V0_MATH_INVALID      0x01    // invalid argument for operation
#define V0_MEM_TRAP             0x04    // ALIGN, BUS, SEG, PAGE, ECC, BOUND
#   define V0_MEM_EXEC          0x00    // page-fault on execute
#   define V0_MEM_WRITE         0x01    // page-fault on write
#   define V0_MEM_READ          0x02    // page-fault on read
#   define V0_MEM_PROT          0x03    // page-access denied
#   define V0_MEM_BUS           0x04    // bus fault
#   define V0_MEM_SEG           0x05    // segment fault
#   define V0_MEM_FAIL          0x06    // memory (ECC or parity) failed
#   define V0_MEM_CACHE         0x07    // cache-memory [ECC or parity] failed
#define V0_IO_TRAP              0x05    // I/O
#   define V0_IO_SEEK           0x00    // I/O-seek failed or not supported
#   define V0_IO_WRITE          0x01    // I/O-write failed
#   define V0_IO_READ           0x02    // I/O-read failed
#   define V0_IO_PERM           0x04    // I/O-port permission denied
#   define V0_IO_DEV            0x05    // I/O-device failure
#   define V0_IO_BUS            0x06    // I/O-bus failure
#   define V0_IO_SYNC           0x07    // I/O-synchronization in progress
#   define V0_IO_URGENT         0x10    // I/O-data pending (e.g. NIC)
#   define V0_IO_DONE           0x11    // I/O-operation finished
#define V0_SYS_TRAP             0x06    // breakpoint, sleep, reboot, ...
#   define V0_SYS_PROC          0x00    // processor online
#   define V0_SYS_BUS           0x01    // system bus online
#   define V0_SYS_DEV           0x02    // system device online
#   define V0_SYS_RUN           0x03    // system online and running
#   define V0_SYS_STOP          0x04    // system sleep-mode activated (WFI/WFE)
#   define V0_SYS_CONT          0x05    // system ops continued/awaken (SEV)
#   define V0_SYS_BREAK         0x06    // debugger breakpoint
#   define V0_SYS_RESET         0x07    // system reset request
#   define V0_SYS_ABORT         0x08    // system-triggered abort
#   define V0_SYS_BOUND         0x09
#   define V0_SYS_RING          0x0a    // attempted system-op in user-mode
#define V0_SOFT_TRAP            0x07    // user-generated interrupt
#   define V0_DEV_MASK          0xf0    // mask for device/interrupt ID
#   define V0_SOFT_RESET        0x00    // initialize reset sequence
#   define V0_SOFT_BOOT         0x01    // initialize reboot/shutdown wequence
#   define V0_SOFT_STOP         0x02    // stop/enter energy-saving mode
#   define V0_SOFT_CONT         0x03    // leave energy-saving mode
#   define V0_SOFT_YIELD        0x04    // release processor [for other threads]
#   define V0_SOFT_THREAD       0x05    // launch new thread
#   define V0_SOFT_DETACH       0x06    // detach a thread (make it unjoinable)
#   define V0_SOFT_JOIN         0x07    // join a thread (attach to wait)
/* 0x08-0x0f are reserved */
#   define V0_SOFT_BITS         15
#   define V0_KBD_BITS          0x10
#   define V0_KBD_INIT          0x10    // initialize keybaord to READY-state
#   define V0_KBD_READ          0x11    // read keyboard input; poll on != READY
/* 0x22-0x2f are reserved */
#   define V0_HID_BITS          0x20
#   define V0_SND_INIT          0x30    // initialize sound device (incl. bufs)
#   define V0_SND_LOAD          0x31    // load sample/patch
#   define V0_SND_UNLOAD        0x32    // unload sample/batch
#   define V0_SND_START         0x33    // start audio-playback
#   define V0_SND_SAMPLE        0x34    // start audio-recording
#   define V0_SND_READ          0x35    // read data into buffer
/* 0x36-0x3f are reserved */
#   define V0_SND_BITS          0x30
#   define V0_GFX_INIT          0x40    // initialize graphics (fb, bpp, res)
#   define V0_GFX_SYNC          0x41    // synchronize screen from framebuf
#   define V0_GFX_BITS          0x40
/* 0x42-0x4f are reserved */
#   define V0_STOR_INIT         0x50    // set storage device attributes
#   define V0_STOR_START        0x51    // e.g. spin disk up
#   define V0_STOR_STOP         0x52    // e.g. spin disk down
#   define V0_STOR_SEEK         0x53    // seek where relevant
#   define V0_STOR_READ         0x54    // read from device
#   define V0_STOR_WRITE        0x55    // write to device
#   define V0_STOR_SYNC         0x56    // synchronize device-data; write bufs
#   define V0_STOR_BITS         0x50
/* 0x57-0x5f are reserved */
#   define V0_CARD_INIT         0x60    // initialize flash/punch-card device
#   define V0_CARD_READ         0x61    // read from card
#   define V0_CARD_WRITE        0x62    // flash or punch to card
#   define V0_CARD_BITS         0x60
/* 0x63-0x6f are reserved */
#   define V0_TAPE_INIT         0x70    // initialize tape-drive
#   define V0_TAPE_START        0x71    // start tape-drive
#   define V0_TAPE_STOP         0x72    // stop tape-drive
#   define V0_TAPE_SEEK         0x73    // seek tape-drive
#   define V0_TAPE_READ         0x74    // read from tape-drive
#   define V0_TAPE_WRITE        0x75    // write to tape-drive
#   define V0_TAPE_SYNC         0x76    // synchronize tape-drive; write bufs
#   define V0_TAPE_BITS         0x70
/* 0x77-0x7f are reserved */
#   define V0_FAST_INIT         0x80    // initialize device(s) for fast IRQ
#   define V0_FAST_BITS         0x80
/* 0x81-0x8f are reserved */
#define V0_SYSTEM_INTRS         8       // # of system-triggered interrupts

/* device interrups */
#define V0_KBD_IRQ              0x08    // keyboard
#   define V0_KBD_READY         0x00    // KBD ready for I/O
#   define V0_KBD_BUSY          0x01    // KBD busy
/* 0x02-0x0f are reserved */
#define V0_HID_IRQ              0x09    // human interface device such as mouse
#   define V0_HID_READY         0x00    // HID ready for I/O
#   define V0_HID_BUSY          0x01    // HID busy
#   define V0_HID_MOVE          0x02    // pointer-device move
#   define V0_HID_CLICK         0x03    // button clicked
#   define V0_HID_REL           0x04    // button released
#   define V0_HID_STOP          0x04    // V0_HID_READY when reconnected
/* 0x05-0x0f are reserved */
#define V0_SND_IRQ              0x0a    // audio event
#   define V0_SND_PLAY          0x00    // sound device playback started
#   define V0_SND_RECORD        0x01    // sound device recording started
#   define V0_SND_STOP          0x02    // sound device playback stopped
#   define V0_SND_LOBUF         0x03    // buffer is getting empty (25 %)
#   define V0_SND_HIBUF         0x04    // buffer is 75 % full
/* 0x05-0x0f are reserved */
#define V0_GFX_IRQ              0x0b    // video event (draw, syncbuf, copybuf)
#define V0_STOR_IRQ             0x0c    // disk/disc/tape/ssd; fast I/O
#define V0_CARD_IRQ             0x0d    // punchcard interface :)
#define V0_TAPE_IRQ             0x0e    // tape interface
#define V0_FAST_IRQ             0x0f    // fast-interrupt; handler inside vector
#define V0_DEV_IRQS             8       // # of device generated interrupts
#define V0_TOTAL_INTRS          16

#endif /* SYS_V0_INTR_H */

