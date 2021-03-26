#ifndef __KERN_UNIT_X86_HPET_H__
#define __KERN_UNIT_X86_HPET_H__

#include <stdint.h>
#include <env/cdefs.h>
#include <mach/param.h>
#include <kern/util.h>

/* base addresses */
#define HPET0BASE     0xfed00000
#define HPET1BASE     0xfed80000
#define HPETREGSIZE   1024
#define HPETREGSIZE64 65536
//#define HPETSIG       0x54455048U       // "HPET"
#define HPETSIG       0x54455048U

/* arguments for hpetprobe() */
#define HPET0         0
#define HPET1         1

/* configuration space register offsets */
#define hpettmrconfofs(id)                                              \
    (0x0100 + (id) * 0x20)
#define hpettmrcmpofs(id)                                               \
    (0x0108 + (id) * 0x20)
#define hpettmrofs(id)                                                  \
    (0x0110 + (id) * 0x20)
/* ID register contents */
/* revision */
#define hpetidrev(val)                                                  \
    ((val) & 0xff)
/* number of timers in block */
#define hpetidnumtmr(val)                                               \
    ((((val) >> 8) & 0x1f) + 1)
/* counter size; 0 -> 32-bit, 1 -> 64-bit */
#define hpetidcntsize(val)                                              \
    (((val) >> 13) & 0x01)
/* reserved bit (should be zero) */
#define hpetidres(val)                                                  \
    0x00004000
/* support for legacy interrupt route */
#define hpetidlegrt(val)                                                \
    (((val) >> 15) & 0x01)
/* vendor ID; must not be 0 */
#define hpetidvendor(val)                                               \
    (((val) >> 16) & 0xffff)
/*
 * counter increment period in femtoseconds (10^-15); must be <= 0x5f5e100
 * (== 100 nanoseconds)
 */
#define hpetidclkperiod(val)                                            \
    ((val) >> 32)
/* CONF register contents */
/*
 * route timer 0 to IRQ0 on non-APIC, IRQ2 in the I/O APIC if set
 * route timer 1 to IRQ8 on non-APIC; IRQ8 in the I/O APIC if set
 * route timers 2-n according to routing bits in configuration registers
 * if not set, honor routing bits for all timers
 */
/*
 * allow main counter to run and allow timer interrupts if enabled
 */
#define HPETIDREG       0x0000  // general capabilities and ID, read-only
#define HPETGENCAPLO    0x0000
#define HPETGENCAPHIGH  0x0004
#define HPETGENCAP64    0x0000
#define HPETCONFENABLE  0x0001  // enable clock source for the chip
#define HPETCONFLEGRT   0x0002  // per-timer settings
#define HPETREVIDMASK   0x00ff  // revision of implemented functions
#define HPETNTMRMASK    0x1f00  // # of the last timer
#define HPETCNTSIZEMASK 0x2000  // 0 = 32-bit, 1 = 64-bit
#define HPETLEGRTBIT    0x8000  // 1 = support legacy replacement route
#define HPETVENDORMASK  0xffff0000
#define HPETCNTRPERIOD  UINT64_C(0xffffffff00000000)
/* all except the ID register are read-write */
#define HPETCONFREG     0x0010
#define HPETINTSTATREG  0x0020
#define HPETMAINCNTREG  0x00f0
#define HPETTMR0CONFREG 0x0100
#define HPETTMR0CMPREG  0x0108
#define HPETTMR0FSBREG  0x0110
#define HPETTMR1CONFREG 0x0120
#define HPETTMR1CMPREG  0x0128
#define HPETTMR1FSBREG  0x0130
#define HPETTMR2CONFREG 0x0140
#define HPETTMR2CMPREG  0x0148
#define HPETTMR2FSBREG  0x0150

#if 0
/* tmrblkid */
#define hpettmrid0(hdp)                                                 \
    ((hdp->tmrblkid) >> 16)
#define hpettmrlegrt(hdp)                                               \
    ((hdp)->tmrblkid & 0x8000)
#define hpettmrcntsz(hdp)                                               \
    ((hdp)->tmrblkid & 0x2000)
#define hpettmrncmp(hdp)                                                \
    (((hdp)->tmrblkid >> 8) & 0x1f)
#define hpethwrev(hdp)                                                  \
    ((hdp)->tmrblkid & 0xff)
#define hpetseqnum(hdp)                                                 \
    ((hdp)->hpetnum)
/* protoematr */
#define hpetpgprot(hdp)                                                 \
    ((hdp)->protoematr & 0x0f)
#define HPETNOPROT  0x00
#define HPET4KPROT  0x01
#define HPET64KPROT 0x02
/* acpiadr */
#define hpetadrmem(hdp)                                                 \
    (!(hpet->adrtype & 0xff))
#define hpetregsz(hdp)                                                  \
    ((hdp)->regsz)
#define hpetbifofs(hdp)                                                 \
    ((hdp)->bitofs)
#define hpetadr(hdp)                                                    \
    ((hdp)->baseadr)
struct hpet {
    uint32_t sig;               // HPETSIG ("HPET")
    uint32_t len;               // length in bytes
    uint8_t  rev;               // revision
    uint8_t  chksum;            // table must sum to zero
    uint8_t  oemid[6];          // OEM id
    uint64_t oemtabid;          // manufacturer model ID
    uint32_t oemrev;            // OEM revision
    uint32_t creatid;           // creator ID
    uint32_t creatrev;          // creator revision
    uint32_t tmrblkid;          // event timer block ID
    uint8_t  adrtype;           // ACPI address type -> 0 = memory, 1 = I/O
    uint8_t  regsz;             // register width in bits
    uint8_t  bitofs;            // register bit offset
    uint8_t  res;               // reserved
    uint64_t baseadr;           // 64-bit address
    uint8_t  hpetnum;           // sequence number; 0 = 1st table, ...
    uint16_t mintick;           // main counter minimum tick in periodic mode
    uint8_t  protoematr;
};
#endif /* 0 */

#define hpetrevid(hpet)                                                 \
    ((hpet)->gencaplo & 0xff)
#define hpetntmr(hpet)                                                  \
    (((hpet)->gencaplo >> 8) & 0x1f)
#define hpetcntsize(hpet)                                               \
    ((hpet)->gencaplo & HPETCNTSIZEMASK)
#define hpetlegrt(hpet)                                                 \
    (((hpet)->gencaplo & HPETLEGRTBIT))
#define hpetvendor(hpet)                                                \
    (((hpet)->gencaplo) >> 16)
#define hpetclkperiod(hpet)                                             \
    ((hpet)->gencaphi)
struct hpetdrv {
    void *iobase0;              // base I/O address for primary HPET
    void *iobase1;              // base I/O address for secondary HPET
    long  ntmr0;                // # of available timers for primary HPET
    long  tmr0size;             // non-zero if 64-bit, zero if 32-bit
    long  ntmr1;                // # of available timers for secondary HPET
    long  tmr1size;             // non-zero if 64-bit, zero if 32-bit
};

struct hpet {
    uint32_t gencaplo;
    uint32_t gencaphi;
};

#endif /* __KERN_UNIT_X86_HPET_H__ */

