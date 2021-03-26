#ifndef __KERN_UNIT_X86_MTRR_H__
#define __KERN_UNIT_X86_MTRR_H__

#include <stdint.h>
#include <kern/asm.h>

#define MTRRWRTHRUBIT  0x01
#define MTRRNOCACHEBIT 0x02
#define MTRRPATBIT     0x04

#define MTRRPATWRBACK  (MTRRPATBIT)
#define MTRRPATWRTHRU  (MTRRPATBIT | MTRRWRTHRUBIT)
#define MTRRPATNCMINUS (MTRRPATBIT | MTRRNOCACHEBIT)
#define MTRRPATNOCACHE (MTRRPATBIT | MTRRWRTHRUBIT | MTRRNOCACHEBIT)

#define MTRRNOCACHE    0
#define MTRRWRCOMB     1
#define MTRRWRTHRU     4
#define MTRRWRPROT     5
#define MTRRWRBACK     6

struct mtrrdata {
    uint32_t smpbase;
    uint32_t smpsize;
    uint32_t smpreg;
    uint32_t smptype;
};

/* probe for a free MTRR register pair in the range 0x200..0x20f */
static __inline__ uint32_t
mtrrprobe(void)
{
    uint32_t adr = 0x1ff;
    uint64_t val;

    do {
        adr += 2;
        if (adr == 0x211) {

            return 0;
        }
        val = k_readmsr(adr);
        if (!(val & 0x08)) {
            adr--;

            return adr;
        }
    } while (!val);

    return 0;
}

static __inline__ uintptr_t
mtrrsetwrcomb(uint32_t adr, uintptr_t size)
{
    uint32_t base = mtrrprobe();
    uint32_t word = adr;
    uint64_t val = MTRRWRCOMB;
    uint64_t lim = (UINT64_C(1) << ADRBITS) - 1;

    word >>= 12;
    val <<= 32;
    if (base) {
        val |= word;
        k_writemsr(base, val);  // write phycical base address
        word = size;
        base++;         // point to physmask register
        word--;
        lim -= word;
        lim >>= 12;
        k_writemsr(base, lim);
        base = 0x2ff;
        word = 1U << 11;
        k_readmsr(base);
        k_writemsr(base, word);
    }

    return base;
}

#endif /* __KERN_UNIT_X86_MTRR_H__ */

