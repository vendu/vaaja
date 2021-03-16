#ifndef DECK_LIB_FPIRP_H
#define DECK_LIB_FPIRP_H

#define IRP_MAGIC_64        UINT64_C(0xbfcdd6a18f6a6f52)
#define IRP_MAGIC_32        UINT32_C(0xbe6eb3be)
#define FIRP32_FAST_MAGIC   UINT32_C(0x7ef127ea)

union irp64 {
    double              dbl;
    uint64_t            u64;
};

union irp32 {
    float               flt;
    uint32_t            u32;
};

#endif /* DECK_LIB_FPIRP_H */

