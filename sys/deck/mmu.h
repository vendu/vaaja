#ifndef DECK_MMU_H
#define DECK_MMU_H

#if defined(DECK_MM_EXTENSION)

/* flg-field */
#define DECK_MTR_NT             0x00
#define DECK_MTR_WP             0x01
#define DECK_MTR_WT             0x02
#define DECK_MTR_WC             0x03
#define DECK_MTR_WB             0x04
#define DECK_MTR_TYPE_BITS      3

struct deckmtr {
    unsigned                    base    : 20;   // page-aligned range address
    unsigned                    npg     : 9;    // up to 512 pages (npg + 1)
    unsigned                    type    : DECK_MTR_TYPE_BITS; // cache policy
};

#endif /* defined(DECK_MM_EXTENSION) */

#endif /* DECK_MMU_H */

