#ifndef DECK_MMU_H
#define DECK_MMU_H

#if defined(DECK_MM_EXTENSION)

#include <stdint.h>

#define DECK_PAGE_SIZE          4096
#define DECK_PAGE_ADR_BITS      20
#define DECK_PAGE_OFS_BITS      12

/* flg-field */
#define DECK_MTR_NT             0x00
#define DECK_MTR_WP             0x01
#define DECK_MTR_WT             0x02
#define DECK_MTR_WC             0x03
#define DECK_MTR_WB             0x04
#define DECK_MTR_TYPE_BITS      3

#define DECK_MTR_NPG_BITS       9

/* the actual size of a region is npg + 1 pages; max is 512 pages = 2 MB */
struct deckmtr {
    unsigned                    base    : DECK_PAGE_ADR_BITS;   // range address
    unsigned                    npg     : DECK_MTR_NPG_BITS;    // max 512 pages
    unsigned                    type    : DECK_MTR_TYPE_BITS;   // cache policy
};

#define DECK_PAGE_EXEC          (UINT32_C(1) << 0)  // page execution permission
#define DECK_PAGE_WRITE         (UINT32_C(1) << 1)  // page write paremission
#define DECK_PAGE_READ          (UINT32_C(1) << 2)  // page read permission
#define DECK_PAGE_WIRE          (UINT32_C(1) << 3)  // permanen physical memory
#define DECK_PAGE_DIRTY         (UINT32_C(1) << 4)  // page has been written to
#define DECK_PAGE_PHYS          (UINT32_C(1) << 5)  // page in physical memory

typedef uint32_t                deckpagedesc_t;

#endif /* defined(DECK_MM_EXTENSION) */

#endif /* DECK_MMU_H */

