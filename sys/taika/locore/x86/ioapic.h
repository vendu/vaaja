#ifndef __KERN_UNIT_X86_IOAPIC_H__
#define __KERN_UNIT_X86_IOAPIC_H__

#include <stdint.h>
#include <mach/param.h>
#include <zero/trix.h>

void ioapicinit(long unit);

#define IOAPICID  0x00
#define IOAPICVER 0x01
#define IOAPICTAB 0x10

#define IOAPICDISABLED 0x00010000
#define IOAPICLEVEL    0x00008000
#define IOAPICACTIVELO 0x00002000
#define IOAPICLOGICAL  0x00000800

struct ioapic {
    uint32_t reg;
    uint32_t _pad1[3];
    uint32_t data;
};

static __inline__ uint32_t
ioapicread(volatile struct ioapic *ioapic, uint32_t reg)
{
    ioapic->reg = reg;

    return ioapic->data;
}

static __inline__ void
ioapicwrite(volatile struct ioapic *ioapic, uint32_t val, uint32_t reg)
{
    ioapic->reg = reg;
    ioapic->data = val;
}

#endif /* __KERN_UNIT_X86_IOAPIC_H__ */

