#ifndef __UNIT_X86_64_BOOT_H__
#define __UNIT_X86_64_BOOT_H__

#define CR0PE    0x00000001     // protected-mode enable
#define CR0MP    0x00000002     // monitor coprocessor
#define CR0NE    0x00000020     // numeric error
#define CR0WP    0x00010000     // write protect
#define CR0PG    0x80000000     // paging

#define CR4PSE   0x00000020     // page size extensions
#define CR4PAE   0x00000040     // physical-address extensions
#define CR4PGE   0x00000080     // page-global enable

#define EFERADR  0xc0000080     // EFER MSR address
#define EFERSCE  0x00000001     // system call exceptions
#define EFERLME  0x00000100     // long mode enable
#define EFERNXE  0x00000400     // no-execute enable
#define EFERSVME 0x00001000     // secure virtual machine enable
#define EFERTCE  0x00008000     // translation cache extension

#endif /* __UNIT_X86_64_BOOT_H__ */

