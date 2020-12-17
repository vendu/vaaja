#ifndef DECK_MMU_H
#define DECK_MMU_H

#if defined(DECK_MM_EXTENSION)

#include <stdint.h>

/* memory management operations */
#define DECK_MTR_OP             0x25        // memory type range register access
#define DECK_CPF_OP             0x26        // cacheline-prefetch
#define DECK_CSP_OP             0x27        // set [default/global] cache-policy
#define DECK_FCL_OP             0x28        // flush cache up level (all)
#define DECK_IPG_OP             0x29        // invalidate TLB-entry for page

/* memory barriers */
#define DECK_BAR_OP             0x2a
/* parm-field values for DECK_BAR_OP */
#define DECK_BAR_DATA           0x00
#define DECK_BAR_READ           0x01
#define DECK_BAR_WRITE          0x02
#define DECK_BDT_OP             DECK_BAR_OP // parm & 0x03 == DECK_BAR_DATA
#define DECK_BRD_OP             DECK_BAR_OP // parm & 0x03 == DECK_BAR_READ
#define DECK_BWR_OP             DECK_BAR_OP // parm & 0x03 == DECK_BAR_WRITE

#define DECK_PAGE_SIZE          1024
#define DECK_PAGE_ADR_BITS      22
#define DECK_PAGE_OFS_BITS      10

/* flg-field */
#define DECK_MTR_NT             0x00
#define DECK_MTR_WP             0x01
#define DECK_MTR_WT             0x02
#define DECK_MTR_WC             0x03
#define DECK_MTR_WB             0x04
#define DECK_MTR_TYPE_BITS      3

#define DECK_PAGE_USER_EXEC     (1 << 0)
#define DECK_PAGE_USER_WRITE    (1 << 1)
#define DECK_PAGE_USER_READ     (1 << 2)
#define DECK_PAGE_GROUP_EXEC    (1 << 3)
#define DECK_PAGE_GROUP_WRITE   (1 << 4)
#define DECK_PAGE_GROUP_READ    (1 << 5)
#define DECK_PAGE_WORLD_EXEC    (1 << 6)
#define DECK_PAGE_WORLD_WRITE   (1 << 7)
#define DECK_PAGE_WORLD_READ    (1 << 8)
#define DECK_PAGE_SHARE_PERM    (1 << 9)
#define DECK_PAGE_WIRE_PERM     (1 << 10)
#define DECK_PAGE_PERM_BITS     10

/* the actual size of a region is npg + 1 pages */
struct deckmtr {
    /* header */
    unsigned                    base    : DECK_PAGE_ADR_BITS;   // range address
    unsigned                    type    : DECK_MTR_TYPE_BITS;   // cache policy
    unsigned                    flg     : 7;                    // op flags
    /* footer */
    unsigned                    npg     : DECK_PAGE_ADR_BITS;   // # of pages
    unsigned                    perm    : DECK_PAGE_PERM_BITS;  // permissions
};

#define DECK_PAGE_EXEC          (UINT32_C(1) << 0)  // page execution permission
#define DECK_PAGE_WRITE         (UINT32_C(1) << 1)  // page write permission
#define DECK_PAGE_READ          (UINT32_C(1) << 2)  // page read permission
#define DECK_PAGE_WIRE          (UINT32_C(1) << 3)  // permanen physical memory
#define DECK_PAGE_DIRTY         (UINT32_C(1) << 4)  // page has been written to
#define DECK_PAGE_PHYS          (UINT32_C(1) << 5)  // page in physical memory

typedef uint32_t                deckpagedesc_t;

#endif /* defined(DECK_MM_EXTENSION) */

#endif /* DECK_MMU_H */

