#include <mach/param.h>

#define VPC_INVALID             0
#define VPC_DOUBLE              1
#define VPC_FLOAT               2
#define VPC_INT64               3
#define VPC_UINT64              4
#define VPC_INT32               5
#define VPC_UINT32              6
#define VPC_INT16               7
#define VPC_UINT16              8
#define VPC_INT8                9
#define VPC_UINT8               10

union vpcnum {
    double                      dbl;
    float                       flt;
    int64_t                     i64;
    uint64_t                    u64;
    int32_t                     i32;
    uint32_t                    u32;
    int16_t                     i16;
    uint16_t                    u16;
    int8_t                      i8;
    uint8_t                     u8;
};

struct vpcnum {
    union vpcnum                num;
    size_t                      len;
    const char                 *str;
    int                         type;
};

