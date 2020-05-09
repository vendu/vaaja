/* representation for C-language code */

#ifndef __CC_CCODE_H__
#define __CC_CCODE_H__

#define C_COMMENT_BEGIN_1           "/*"
#define C_COMMENT_BEGIN_2           "//"
#define C_COMMENT_ASM               ';'

#define C_CONTEXT_GLOBAL            0
#define C_CONTEXT_LOCAL             1
#define C_CONTEXT_COMMENT           2
#define C_CONTEXT_CHARACTER         3
#define C_CONTEXT_STRING            4
#define C_CONTEXT_DECLARATION       5
#define C_CONTEXT_CODE              6

#define C_QUALIFIER_CONST           0
#define C_QUALIFIER_STATIC          1
#define C_QUALIFIER_VOLATILE        2
#define C_QUALIFIER_REGISTER        3

#define C_TYPE_CUSTOM               0   // typedef
#define C_TYPE_BUILTIN              1   // stdc types
#define C_TYPE_ENUM                 2   // enumerated
#define C_TYPE_STRUCT               3   // structure
#define C_TYPE_UNION                4   // union

#define C_OBJECT_KEYWORD            0   // c-language keyword
#define C_OBJECT_VALUE              1   // value (variable, const macro)
#define C_OBJECT_TYPE               2   // type object
#define C_OBJECT_LABEL              3   // label
#define C_OBJECT_MACRO              4   // function-like macro
#define C_OBJECT_FUNCTION           5   // function
#define C_OBJECT_SYMBOL             6   // var, agr, lbl, func
#define C_OBJECT_MEMBER             7   // tag, fld
#define C_OBJECT_PRAGMA             8   // misc nonsense =)

#define IO_SYNC                     (1 << 0)
#define IO_MAPPED                   (1 << 1)
struct cobj {
    uint16_t    iobits;             // [file] I/O parameters
    uint16_t    fileid;             // file index
    uint16_t    ctxbits;            // context-flags
    uint16_t    typebits;           // type information
    uint32_t    atrbits;            // [compiler-specific] attributes
    uint32_t    qualbits;           // c-language qualifier bits
    void       *data;               // object data
    uint32_t    id;                 // numerical index
};

#endif /* __CC_CCODE_H__ */

