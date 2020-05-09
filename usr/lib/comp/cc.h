#ifndef __VC_VC_H__
#define __VC_VC_H__

#define V0_ALU_OP               0x01
#define V0_FPU_OP               0x02
#define V0_SIMD_OP              0x03
#define V0_MEM_OP               0x04
#define V0_IO_OP                0x05
/* ALU operations */
#define VC_NOT_OP               0x01 // ~
#define VC_AND_OP               0x02 // &
#define VC_OR_OP                0x03 // |
#define VC_XOR_OP               0x04 // ^
#define VC_NEG_OP               0x05 // -X
#define VC_INC_OP               0x06 // ++
#define VC_DEC_OP               0x07 // --
#define VC_ADD_OP               0x08 // +
#define VC_SUB_OP               0x09 // -
#define VC_MUL_OP               0x0a // *
#define VC_DIV_OP               0x0b // /
#define VC_REM_OP               0x0c // %
#define VC_SHL_OP               0x0d // <<
#define VC_SHR_OP               0x0e // >>
#define VC_SAR_OP               0x0f // >>>
#define VC_CMP_OP               0x10 /// SUB + set flags
#define VC_LT_OP                0x11 // <
#define VC_LTE_OP               0x12 // <=
#define VC_EQ_OP                0x13 // ==
#define V0_ZF_OP                V0_EQ_OP
#define VC_NEQ_OP               0x14 // !
#define V0_NZ_OP                V0_NEQ_OP
#define VC_GT_OP                0x15 // >
#define VC_GTE_OP               0x16 // >=
#define VC_ZEX_OP               0x17 // 0>
#define VC_SEX_OP               0x18 // 1>
#define VC_CLZ_OP               0x19 // 0.
#define VC_HAM_OP               0x1a // #1
#define VC_PAR_OP               0x1b // odd parity
#define VC_CRC_OP               0x1d // compute checksum
#define VC_BCD_OP               0x1e // convert to binary-coded decimal
#define VC_DCD_OP               0x1f // convert from binary-coded decimal
#define VC_JMP_OP               0x20 // unconditional jump
#define VC_BRA_OP               0x21 // conditional branch
#define VC_CSF_OP               0x22 // construct stack frame
#define VC_CSR_OP               0x23 // function call
#define VC_BEG_OP               0x24 // function prologue
#define VC_FIN_OP               0x25 // function epilogue
#define VC_RET_OP               0x26 // return from function
#define VC_IRT_OP               0x27 // return from interrupt handler
#define VC_SYS_OP               0x28 // system call dispatch
#define VC_SRT_OP               0x29 // return from system call

/* FMISC */
#define VC_FMISC_BIT            0x40
#define VC_SRAND_OP             0x40
#define VC_FRAND_OP             0x41 // random number
#define VC_FLOOR_OP             0x42 // floating-point towards negative infinity
#define VC_FCEIL_OP             0x43 // floating-point towards positive infinity
#define VC_FTRUNC_OP            0x44 // floating-point towards zero
#define VC_ITOF_OP              0x45 // integer to floating-point conversion
/* FEXP */
#define VC_FEXP_MASK            0x50
#define VC_FEXP_OP              0x51 // exponential function
#define VC_FLOG_OP              0x52 // [floating-point] natural logarithm
#define VC_FPOW_OP              0x53
#define VC_FSQRT_OP             0x54 // [floating-point] square root
#define VC_FCBRT_OP             0x55 // floating-point cubic root
#define VF_FTRIG_BIT            0x60 // trigonometric instructions
#define VC_FSIN_OP              0x60 // floating-point sine
#define VC_FCOS_OP              0x61 // floating-poing cosine
#define VC_FTAN_OP              0x62 // floating-point tangent
#define VC_FASIN_OP             0x63 // floating-point arcus sine
#define VC_FACOS_OP             0x64 // floating-point arcus cosine
#define VC_FATAN_OP             0x65 // floating-point arcus tangent
#define VC_FSINH_OP             0x66 // floating-point hyperbolic sine
#define VC_FCOSH_OP             0x67 // floating-point hyperbolic cosine
#define VC_FTANH_OP             0x68 // floating-point hyperbolic tangent

/* FPU operations */

/* MEM operations */
#define V0_LEA_OP               0x71 // calculate/load effective address
#define V0_LDR_OP               0x72 // load from register or memory to register
#define V0_STR_OP               0x73 // store register or immediate to memory
#define V0_LDL_OP               0x74 // load-linked
#define V0_STC_OP               0x75 // store conditional
#define V0_CAS_OP               0x76 // compare and swap
#define V0_BRD_OP               0x72 // read-barrier
#define V0_BWR_OP               0x73 // write-barrier
#define V0_BAR_OP               0x74 // full barrier
#define V0_CPF_OP               0x75 // cache prefetch
#define V0_FLS_OP               0x76 // cache flush
#define V0_STN_OP               0x77 // non-temporal (cache-bypassing) store
#define V0_LDN_OP               0x78 // non-temporal load
#define V0_FPG_OP               0x77 // invalidate page entry
#define V0_SET_OP               0x78 // set bit
#define V0_CLR_OP               0x79 // clear bit
#define V0_BTS_OP               0x7a // bit test and set
#define V0_BTC_OP               0x7b // bit test and clear
#define V0_STEQ                 0x10 // store if equal
#define V0_STZF                 V0_STEQ // store if zero
#define V0_STNE                 0x11 // store if not equal
#define V0_STLT                 0x12 // store if less than
#define V0_STLE                 0x13 // store if less than or equal
#define V0_STGT                 0x14 // store if greater than
#define V0_LDEQ                 0x10 // load if equal
#define V0_LDZF                 V0_LDEQ // load if zero
#define V0_LDNE                 0x11 // load if not equal
#define V0_LDLT                 0x12 // load if less than
#define V0_LDLE                 0x13 // load if less than or equal
#define V0_LDGT                 0x14 // load if greater than
#definf V0_LDGE                 0x15

/* standard qualifier flags */
#define V0_REGISTER_BIT         (1 << 0)
#define VC_CONST_BIT            (1 << 1)
#define VC_STATIC_BIT           (1 << 2)
#define VC_EXTERN_BIT           (1 << 3)
#define VC_VOLATILE_BIT         (1 << 4)
#define VC_RESTRICT_BIT         (1 << 5)
/* compiler attribute flags */
#define V0_ATOMIC_BIT           (1 << 6)
#define VC_ALIGN_BIT            (1 << 7)
#define VC_PACK_BIT             (1 << 8)
/* some compiler optimisations */
#define VC_INLINE_BIT           (1 << 9)
#define VC_UNROLL_BIT           (1 << 10)
#define VC_HOTSPOT_BIT          (1 << 11)
/* function attributes */
#define VC_PURE_BIT             (1 << 12)
#define VC_NAKED_BIT            (1 << 13)
#define VC_SHARED_BIT           (1 << 14)
#define VC_ALIAS_BIT            (1 << 15)
#define VC_WEAK_BIT             (1 << 16)
/* call conventions */
#define VC_CDECL_BIT            (1 << 17)
#define VC_FASTCALL_BIT         (1 << 18)
#define VC_NORETURN_BIT         (1 << 19)
#define VC_INTR_BIT             (1 << 20)
#define VC_INTRFUNC_BIT         (1 << 21)
/* extra parameter flags */
#define VC_THROW_BIT            (1 << 22)
#define VC_TWICE_BIT            (1 << 23)
#define VC_STKPROT_BIT          (1 << 24)
#define VC_NULL_BIT             (1 << 25)
#define VC_USED_BIT             (1 << 26)
#define VC_UNUSED_BIT           (1 << 27)

#define VC_CHAR_TYPE            0x01
#define VC_UCHAR_TYPE           0x02
#define VC_SHORT_TYPE           0x03
#define VC_USHORT_TYPE          0x04
#define VC_INT_TYPE             0x05
#define VC_UINT_TYPE            0x06
#define VC_LONG_TYPE            0x07
#define VC_ULONG_TYPE           0x08
#define VC_LONGLONG_TYPE        0x09
#define VC_ULONGLONG_TYPE       0x0a
#define VC_FLOAT_TYPE           0x0b
#define VC_DOUBLE_TYPE          0x0c
#define VC_LONGDOUBLE_TYPE      0x0d
#define VC_POINTER_TYPE         0x0e
#define VC_PTRDIFF_TYPE         0x0f
#define VC_PTRVAL_TYPE          0x10
#define VC_SIMD64_TYPE          0x11
#define VC_SIMD128_TYPE         0x12
#define VC_SIMD256_TYPE         0x13

struct vcint {
    int8_t   i8;
    uint8_t  u8;
    int16_t  i16;
    uint16_t u16;
    int32_t  i32;
    uint32_t u32;
    int64_t  i64;
    uint64_t u64;
    v128_t   v128;
};

#define VC_FLOAT_TYPE           0x01
#define VC_DOUBLE_TYPE          0x02
#define VC_LONG_DOUBLE_TYPE     0x03
struct vcfloat {
    union {
        float           f;
        double          fd;
        long double     fld;
        uint8_t         u128[16];
    } data;
};

#define VC_PTR_DIFF_TYPE        0x01   // ptrdiff_t
#define VC_PTR_VAL_TYPE         0x02   // uintptr_t
#define VC_BYTE_PTR_TYPE        0x03   // int8_t *
#define VC_HALF_PTR_TYPE        0x04   // int16_t *
#define VC_WORD_PTR_TYPE        0x05   // int32_t *
#define VC_LONG_PTR_TYPE        0x06   // int64_t *
#define VC_PTR_TYPE             0x07   // void *
struct vcadr {
    union {
        intptr_t        diff;
        uintptr_t       val;
        int8_t         *byte;
        uint8_t        *ubyte;
        int16_t        *half;
        uint16_t       *uhalf;
        int32_t        *word;
        uint32_t       *uword;
        int64_t        *long;
        uint64_t       *ulong;
        void           *vptr;
    } ptr;
};

struct vctype {
    long                id;
    struct vcval        val;
};

#endif /* __VC_VC_H__ */

