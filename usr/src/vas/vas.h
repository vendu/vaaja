#ifndef __VAS_VAS_H__
#define __VAS_VAS_H__

#include <vas/conf.h>

#define VASHASHSLOTS 1024

#include <stddef.h>
#include <stdint.h>
#include <mach/types.h>

#define VASUNRESOLVED   (~0U)

#define VASVALUE        1
#define VASLABEL        2
#define VASINST         3
#define VASREG          4
#define VASDEF          5
#define VASSYM          6
#define VASCHAR         7
#define VASIMMED        8
#define VASADR    	    9
#define VASINDIR        10
#define VASINDEX  		11
#define VASDATA   		12
#define VASGLOBL  		13
#define VASSPACE  		14
#define VASORG    		15
#define VASALIGN  		16
#define VASASCIZ  		17
#define VASSTRING 		18
#define VASPAREN  		19
#define VASMACRO        20
#define VASINST         21
#define VASTOKENS       32
#define VASMAXTOKEN     255

#define VAS_MNEMO_LEN   8
#define VAS_DEF_SHIFT   2

struct vastoken {
    uint8_t             type;   	// token type such as VASVALUE
    uint8_t             flg;    	// token subtype (e.g. IMMED)
    uint8_t             scope;  	// token scope
    uint8_t             size;   	// token data size in bytes
    uint32_t            line;   	// token line
    const char         *file;   	// source file name
    void               *data;   	// token data
    struct vastoken    *next;
};

/* value flags; type-qualifiers, storage classes, and attributes */
#define VAS_UNSIGNED    (1 << 0)        // unsigned value
#define VAS_POINTER     (1 << 1)        // pointer value
#define VAS_CONST       (1 << 2)        // C qualifier
#define VAS_VOLATILE    (1 << 3)        // C qualifier
#define VAS_RESTRICT    (1 << 4)        // C pointer qualifier
#define VAS_AUTO        (1 << 5)        // C storage class
#define VAS_EXTERN      (1 << 6)        // C storage class
#define VAS_STATIC      (1 << 7)        // C storage class
#define VAS_REGISTER    (1 << 8)        // C storage class
#define VAS_GLOBAL      (1 << 9)        // global storage
/* value size */
#define VAS_INT8        1
#define VAS_INT16       2
#define VAS_INT32       4
#define VAS_INT64       8

struct vasvalue {
    uint8_t             type;
    union {
        uintptr_t       uptr;   // unsigned pointer value
        intptr_t        iptr;   // signed pointer value, e.g. pointer difference
        int64_t         i64;    // signed 64-bit value
        uint64_t        u64;    // unsigned 64-bit value
        int32_t         i32;    // signed 32-bit value
        uint32_t        u32;    // unsigned 32-bit value
        int16_t         i16;    // signed 16-bit value
        uint16_t        u16;    // unsigned 16-bit value
        int8_t          i8;     // signed 8-bit value
        uint8_t         u8;     // unsigned 8-bit value
    };
    uint16_t            flg;    // value flags (see above)
    uint8_t             size;   // value size (in bytes)
    uint8_t             pad;
#if (VAS_WORD_SIZE == 8)
    uint32_t            _pad;
#endif
};

struct vasdef {
    uint8_t     type;
    char       *name;
    m_word_t    word;
};

struct vasobj {
    uint8_t     type;
    char       *name;
    void       *data;
};

struct vasinst {
    uint8_t     type;
    uint8_t     size;       // instruction size
    uint8_t     narg;       // number of arguments for instruction
    void       *inst;    	// instruction in storage
    void       *arg;        // immediate argument in storage
};

#endif /* __VAS_VAS_H__ */

