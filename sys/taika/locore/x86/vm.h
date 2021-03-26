#ifndef __KERN_UNIT_X86_VM_H__
#define __KERN_UNIT_X86_VM_H__

#include <env/cdefs.h>

#define VM_USER_ZONE    0       // default pages
#define VM_ISADMA_ZONE  1       // pages below 16 megs for ISA DMA
#define VM_DMA32_ZONE   2       // pages below 4 gigs not available for ISA DMA
#define VM_KERN_ZONE    3
#define VM_DMA32_ADRLIM 16777216
#define VM_NFREE_ORDER  13
#define VM_NLVL_RESERVE 1       // superpage reservations: 1 level
#define VM_LVL0_ORDER   9       // level 0 reservations: 512 pages

#if ((defined(__i386__) || defined(__i486__)                            \
      || defined(__i586__) || defined(__i686__))                        \
     && (!defined(__x86_64__) && !defined(__amd64__)))
#include <kern/unit/ia32/vm.h>
#else
#include <kern/unit/x86-64/vm.h>
#endif

#endif /* __KERN_UNIT_X86_VM_H__ */

