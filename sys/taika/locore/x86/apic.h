#ifndef __KERN_UNIT_X86_APIC_H__
#define __KERN_UNIT_X86_APIC_H__

#include <kern/conf.h>

#if (APIC) || (SMP)

#if !defined(__ASSEMBLER__)

#include <stdint.h>
#include <mach/param.h>
#include <env/cdefs.h>
#include <kern/unit/x86/pit.h>
#include <kern/unit/x86/link.h>
//#include <kern/unit/x86/mp.h>

#define apiceoi()   apicwrite(0, APICEOI)

extern void       (*mpspurint)(void);
extern void         irqerror(void);
extern void         irqspurious(void);
void                apicstart(uint8_t id, uint32_t *base);
void                kusleep(unsigned long nusec);

#endif /* !defined(__ASSEMBLER__) */

#define APICMSR        0x1b
#define APICENABLE     0x0800

/* registers */
#define APICID         0x0020
#define APICVER        0x0030
#define APICTASKPRIO   0x0080
#define APICEOI        0x00b0
#define APICLDR        0x00d0
#define APICDFR        0x00e0
#define APICSPURIOUS   0x00f0
#define APICERRSTAT    0x0280
#define APICINTRLO     0x0300
#define APICINTRHI     0x0310
#define APICTMR        0x0320         // timer
#define APICTHERMAL    0x0330         // thermal sensor interrupt
#define APICPERFINTR   0x0340         // performance counter overflow
#define APICINTR0      0x0350
#define APICINTR1      0x0360         // NMI
#define APICERROR      0x0370
#define APICTMRINITCNT 0x0380
#define APICTMRCURCNT  0x0390
#define APICTMRDIVCONF 0x03e0
#define APICLAST       0x038f
//#define APICENABLE   0x0100
#define APICCPUFOCUS   0x0200
#define APICNMI        (4 << 8)
/* timer configuration */
//#define APICDISABLE      0x00010000
//#define APICDIV1     0x00000008
#define APICBASEDIV    0x0000000b
#define APICPERIODIC   0x00020000
//#define APICMODEPERIODIC 0x02

/* flags */
#define APICSWENABLE   0x00000100
#define APICINIT       0x00000500
#define APICSTART      0x00000600
#define APICDELIVS     0x00001000
#define APICASSERT     0x00004000
#define APICDEASSERT   0x00000000
#define APICLEVEL      0x00008000
#define APICBCAST      0x00080000
#define APICPENDING    0x00001000
#define APICFIXED      0x00000000
#define APICSMI        0x00000200
#define APICEXTINTR    0x00000700
#define APICMASKED     0x00010000
#define APICSELF       0x00040000
#define APICBUTSELF    0x00080000
#define APICALL        0x000c0000

#if !defined(__ASSEMBLER__)

static __inline__ uint32_t
apicread(volatile uint32_t *base, uint32_t reg)
{
    uint32_t ret = base[reg >> 2];

    return ret;
}

/* wait for write to finish by reading ID */
static __inline__ void
apicwrite(volatile uint32_t *base, uint32_t val, uint32_t reg)
{
    base[(reg) >> 2] = val;
    apicread(base, APICID);
}

static __inline__ void
apicsendirq(volatile uint32_t *base, uint32_t hi, uint32_t lo, long nusec)
{
    __asm__ __volatile__ ("pushfl\n");
    __asm__ __volatile__ ("cli\n");
    apicwrite(base, hi, APICINTRHI);
    kusleep(nusec);
    apicwrite(base, lo, APICINTRLO);
    kusleep(nusec);
    __asm__ __volatile__ ("sti\n");
    __asm__ __volatile__ ("popfl\n");
}

struct apic {
    uint32_t       res1;
    uint32_t       res2;
    unsigned       res3   : 24;
    unsigned       physid : 4;
    unsigned       res4   : 4;
    const unsigned version : 8;
    const unsigned res6    : 8;
    const unsigned maxlvt  : 8;
    unsigned       res7    : 8;
    uint8_t        res8[12];
    uint8_t        res9[16];
    struct {
        unsigned   prio : 8; // task priority
        unsigned   res1 : 24;
        uint32_t   res2[3];
    } task;
    struct {
        unsigned   prio : 8; // arbitration priority
        unsigned   res1 : 24;
        uint32_t   res2[3];
    } arb;
    struct {
        unsigned   lvl  : 8;
        unsigned   res1 : 24;
        uint32_t   res2[3];
    } proc;
    struct {
        uint32_t       val;
        uint32_t       res1[3];
    } eoi;
    const uint32_t res10[4];
    /* logical destination */
    struct {
        unsigned   res1 : 24;
        unsigned   val  : 8;
        uint32_t   res2[3];
    } logdest;
    /* destination format register */
    struct {
        unsigned   res20 : 28;
        unsigned   model : 4;
        uint32_t   res1[3];
    } destfmt;
    struct {
        unsigned   vec    : 8;
        unsigned   on     : 1;
        unsigned   focus  : 1; /* focus CPU, not supported on Xeon */
        unsigned   res1   : 2;
        unsigned   eoibcs : 1; /* EOI broadcast suppression */
        unsigned   res2  : 19;
        uint32_t   res3[3];
    } spurint;
    /* interrupt status register */
    const struct {
        uint32_t   bitmap;
        uint32_t   res[3];
    } intrstat;
    /* trigger mode register */
    const struct {
        uint32_t   bitmap;
        uint32_t   res[3];
    } trigmode;
    /* interrupt request register */
    const struct {
        uint32_t   bitmap;
        uint32_t   res[3];
    } intrreq;
    /* error status register */
    const struct {
        uint32_t   code;
        uint32_t   res[3];
    } error;
    uint32_t       res11[28];
    struct {
        unsigned   vec       : 8;
        unsigned   delivmode : 3;
        unsigned   destmode  : 1;
        unsigned   delivstat : 1;
        unsigned   res1      : 1;
        unsigned   lvl       : 1;
        unsigned   trig      : 1;
        unsigned   res2      : 2;
        unsigned   shorthand : 1;
        uint32_t   res3      : 13;
        uint32_t   res4[12];
        uint32_t   res5[12];
    } intrcmd1;
    struct {
        unsigned   res1      : 24;
        unsigned   phys      : 4;
        unsigned   res2      : 4;
        unsigned   res3      : 24;
        unsigned   log       : 8;
    } intrcmd2;
    struct {
        unsigned   vec       : 8;
        unsigned   res1      : 4;
        unsigned   delivstat : 1;
        unsigned   res2      : 3;
        unsigned   mask      : 1;
        unsigned   tmrmode   : 1;
        unsigned   res3      : 14;
    } lvttmr;
    struct {
        unsigned   vec       : 8;
        unsigned   delivmode : 3;
        unsigned   res1      : 1;
        unsigned   delivstat : 1;
        unsigned   res2      : 3;
        unsigned   mask      : 1;
        unsigned   res3      : 15;
    } lvttherm;
    //    https://git.sphere.ly/santhosh/kernel_cyanogen_msm8916/blob/78b020d035074fc3aa4d017353bb2c32e2aff56f/arch/x86/include/asm/apicdef.h
    uint32_t lvtpc;
    uint32_t lvtint0;
    uint32_t lvtint1;
    uint32_t lvterror;
    /* initial count register for timer */
    struct {
        uint32_t   cnt;
        uint32_t   res[3];
    } tmrinit;
    /* current count register for timer */
    const struct {
        uint32_t   cur;
        uint32_t   res[3];
    } tmrcnt;
    uint32_t       res12[16];
    /* divide configuration register for timer */
    struct {
        unsigned   div   : 4;
        unsigned   res44 : 28;
        uint32_t   res45[3];
    } divconf;
    uint32_t       res13[4];
};

#endif /* !defined(__ASSEMBLER__) */

#endif /* APIC */

#endif /* __KERN_UNIT_X86_APIC_H__ */

