#ifndef DECK_REGS_MT_H
#define DECK_REGS_MT_H

/*
 * memory-type range registers
 */

#define DECK_MT0_REG            0x00
#define DECK_MT1_REG            0x01
#define DECK_MT2_REG            0x02
#define DECK_MT3_REG            0x03
#define DECK_MT4_REG            0x04
#define DECK_MT5_REG            0x05
#define DECK_MT6_REG            0x06
#define DECK_MT7_REG            0x07
#define DECK_MT8_REG            0x08
#define DECK_MT9_REG            0x09
#define DECK_MT10_REG           0x0a
#define DECK_MT11_REG           0x0b
#define DECK_MT12_REG           0x0c
#define DECK_MT13_REG           0x0d
#define DECK_MT14_REG           0x0e
#define DECK_MT15_REG           0x0f

/* flg-field */
#define DECK_MTR_WP             0x00
#define DECK_MTR_NT             0x01
#define DECK_MTR_WT             0x02
#define DECK_MTR_WC             0x03
#define DECK_MTR_WB             0x04
#define DECK_MTR_TYPE_BITS      3

struct deckmtr {
    unsigned                    base    : 20;   // megabyte-aligned
    unsigned                    nmeg    : 9;    // up to 512M in size (nmeg + 1)
    unsigned                    type    : DECK_MTR_TYPE_BITS; // cache policy
};

#endif /* DECK_REGS_MT_H */

