#ifndef __SYS_VCODE_OP_H__
#define __SYS_VCODE_OP_H__

union varg {
    void                *ptr;
    char                *cptr;
    uintptr_t            uptr;
    intptr_t             iptr;
    ptrdiff_t            pdiff;
    unsigned long long   ull;
    long long            ll;
    unsigned long        ul;
    long                 l;
    double               d64;
    float                f32;
#if (V_WORDSIZE == 64)
    int64_t              i64;
    uint64_t             u64;
#endif
    int32_t              i32;
    uint32_t             u32;
    int16_t              i16;
    uint16_t             u16;
    int8_t               i8;
    uint8_t              u8;
    unsigned char        uc;
    signed char          c;
};

#define V_LOW_REG       0x00
#define V_HIGH_REG      0x01
struct vregmap {
    vreg                map[2]; // high and low register IDs
};

struct vop {
    vinst               inst;
    struct varg         arg1;
    union {
        struct varg     val;
        struct vregmap  rmap;
    } arg2;
    unsigned long       flg;
    uint16_t            cflg;   // machine control/status flags
    uint8_t             iflg;   // instruction flags
    uint8_t             qflg;   // qualifier flags
    uint8_t             tflg;   // type flags
    size_t              size;
};

#endif /* __SYS_VCODE_OP_H__ */

