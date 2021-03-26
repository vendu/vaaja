#ifndef __UNIT_ARM_VM_H__
#define __UNIT_ARM_VM_H__

#include <zero/param.h>

#define VIRTBASE        0xc0000000U

#define PAGEBITS        0x00000013U
#define PAGEFRAMEMSK    0xfffff000U
#define pagetabid(adr)  ((adr) >> (PAGESIZELOG2 + NL1PAGETABLOG2))
#define pageid(adr)     ((adr) >> (PAGESIZELOG2 + NL2PAGETABLOG2))
#define pagegetadr(pte) ((pte) & PAGEFRAMEMSK)
#define pagesetflt(pte) ((pte) &= ~0x03)

//#define PAGETABSIZE    (1024 * sizeof(uint32_t))
//#define PAGESIZE       4096
#define NL1PAGETAB     4096
#define NL2PAGETAB     1024
#define NL1PAGETABLOG2 12
#define NL2PAGETABLOG2 10

#endif /* __UNIT_ARM_VM_H__ */

