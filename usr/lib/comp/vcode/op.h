#ifndef __SYS_VCODE_OP_H__
#define __SYS_VCODE_OP_H__

#define VCODE_OP_ARG_PTR        0x00
#define VCODE_OP_ARG_CHAR_PTR   0x01
#define VCODE_OP_ARG_UCHAR_PTR  0x02
#define VCODE_OP_ARG_IPTR       0x03
#define VCODE_OP_ARG_UPTR       0x04
#define VCODE_OP_ARG_PTRDIFF    0x05
#define VCODE_OP_ARG_LONGLONG   0x06
#define VCODE_OP_ARG_ULONGLONG  0x07
#define VCODE_OP_ARG_LONG       0x08
#define VCODE_OP_ARG_ULONG      0x09
#define VCODE_OP_ARG_DOUBLE     0x0a
#define VCODE_OP_ARG_FLOAT      0x0b
#define VCODE_OP_ARG_INT64      0x0c
#define VCODE_OP_ARG_UINT64     0x0d
#define VCODE_OP_ARG_INT32      0x0e
#define VCODE_OP_ARG_UINT32     0x0f
#define VCODE_OP_ARG_INT16      0x10
#define VCODE_OP_ARG_UINT16     0x11
#define VCODE_OP_ARG_INT8       0x12
#define VCODE_OP_ARG_UINT8      0x13
#define VCODE_OP_ARG_SCHAR      0x14
#define VCODE_OP_ARG_UCHAR      0x15
union varg {
    void               *ptr;
    char               *cptr;
    unsigned char      *str;
    intptr_t            iptr;
    uintptr_t           uptr;
    ptrdiff_t           pdiff;
    long long           ll;
    unsigned long long  ull;
    long                l;
    unsigned long       ul;
    double              d64;
    float               f32;
#if (V_WORDSIZE == 64)
    int64_t             i64;
    uint64_t            u64;
#endif
    int32_t             i32;
    uint32_t            u32;
    int16_t             i16;
    uint16_t            u16;
    int8_t              i8;
    uint8_t             u8;
    signed char         c;
    unsigned char       uc;
};

struct varg {
    unsigned long       type;
    union varg          data;
}

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
    size_t              size;   // instruction size
};

#endif /* __SYS_VCODE_OP_H__ */

