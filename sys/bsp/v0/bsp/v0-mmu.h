#ifndef V0_BSP_V0_MMU_H
#define V0_BSP_V0_MMU_H

#if defined(V0_MM_EXTENSION)

/* memory management operations */
#define V0_MTR_OP               0x25        // memory type range register access
#define V0_CPF_OP               0x26        // cacheline-prefetch
#define V0_CSP_OP               0x27        // set [default/global] cache-policy
#define V0_FCL_OP               0x28        // flush cache up level (all)
#define V0_IPG_OP               0x29        // invalidate TLB-entry for page

/* memory barriers */
#define V0_BAR_OP               0x2a
/* parm-field values for V0_BAR_OP */
#define V0_BAR_DATA             0x00
#define V0_BAR_READ             0x01
#define V0_BAR_WRITE            0x02
#define V0_BDT_OP               V0_BAR_OP // parm & 0x03 == V0_BAR_DATA
#define V0_BRD_OP               V0_BAR_OP // parm & 0x03 == V0_BAR_READ
#define V0_BWR_OP               V0_BAR_OP // parm & 0x03 == V0_BAR_WRITE

#define V0_PAGE_SIZE            4096
#define V0_PAGE_ADR_BITS        20
#define V0_PAGE_OFS_BITS        12

/* flg-field */
#define V0_MTR_NT               0x00
#define V0_MTR_WP               0x01
#define V0_MTR_WT               0x02
#define V0_MTR_WC               0x03
#define V0_MTR_WB               0x04
#define V0_MTR_TYPE_BITS        3

#define V0_PAGE_USER_EXEC       (1 << 0)
#define V0_PAGE_USER_WRITE      (1 << 1)
#define V0_PAGE_USER_READ       (1 << 2)
#define V0_PAGE_GROUP_EXEC      (1 << 3)
#define V0_PAGE_GROUP_WRITE     (1 << 4)
#define V0_PAGE_GROUP_READ      (1 << 5)
#define V0_PAGE_WORLD_EXEC      (1 << 6)
#define V0_PAGE_WORLD_WRITE     (1 << 7)
#define V0_PAGE_WORLD_READ      (1 << 8)
#define V0_PAGE_WIRE_PERM       (1 << 9)
#define V0_PAGE_SHARE_PERM      (1 << 10)
#define V0_PAGE_PERM_BITS       12

/* the actual size of a region is npg + 1 pages */
struct v0mtr {
    /* header */
    unsigned                    base    : V0_PAGE_ADR_BITS;   // base address
    unsigned                    type    : V0_MTR_TYPE_BITS;   // cache policy
    unsigned                    flg     : 9;                  // op flags
    /* footer */
    unsigned                    npg     : V0_PAGE_ADR_BITS;   // # pages - 1
    unsigned                    perm    : V0_PAGE_PERM_BITS;  // permissions
};
#define V0_MTR_SIZE1            (V0_PAGE_ADR_BITS                     \
                                 + V0_MTR_TYPE_BITS                   \
                                 + 9)
#define V0_MTR_SIZE2            (V0_PAGE_ADR_BITS                       \
                                 + V0_PAGE_PERM_BITS)
#define V0_MTR_SIZE             (V0_MTR_SIZE1 + V0_MTR_SIZE2)

typedef uint32_t                v0pagedesc_t;

#endif /* defined(V0_MM_EXTENSION) */

#endif /* V0_BSP_V0_MMU_H */

