#ifndef __ZERO_BITS_SHUNTC_H__
#define __ZERO_BITS_SHUNTC_H__

#include <stdint.h>

#define SHUNTCINTSIZE    64

/* token types */

/* C integer operations */
#define SHUNTCILL        0                      // illegal/unitialised operation
#define SHUNTCNOT        1                      // logical NOT
#define SHUNTCINC        2                      // increment by one
#define SHUNTCDEC        3                      // decrement by one
#define SHUNTCSHL        4                      // shift left
#define SHUNTCSHR        5                      // shift right
#define SHUNTCAND        6                      // logical AND
#define SHUNTCOR         7                      // logical OR
#define SHUNTCXOR        8                      // logical XOR
#define SHUNTCADD        9                      // integer addition
#define SHUNTCSUB        10                     // integer subtraction
#define SHUNTCMUL        11                     // integer multiplication
#define SHUNTCDIV        12                     // integer division
#define SHUNTCMOD        13                     // integer modulus
#define SHUNTCSAR        14                     // shift right arithmetic
/* assignment */
#define SHUNTCASSIGN     15                     // assignment
#define SHUNTCNINTOP     16                     // # of integer operations
#define SHUNTCNOP        SHUNTCNINTOP           // # of defined operations
/* special tokens for parsing */
#define SHUNTCSEP        (SHUNTCNOP + 1)        // separator token
#define SHUNTCLEFTPAREN  (SHUNTCNOP + 2)        // left parenthesis
#define SHUNTCRIGHTPAREN (SHUNTCNOP + 3)        // right parenthesis
/* type-tokens */
#define SHUNTCINT        (SHUNTCNOP + 4)        // integer value
#define SHUNTCUINT       (SHUNTCNOP + 5)        // unsigned integer value
#define SHUNTCFLOAT      (SHUNTCNOP + 6)        // float value
#define SHUNTCDOUBLE     (SHUNTCNOP + 7)        // double value
#define SHUNTCLDOUBLE    (SHUNTCNOP + 8)        // long double value
#define SHUNTCSTRING     (SHUNTCNOP + 9)        // string
#define SHUNTCPTR        (SHUNTCNOP + 10)       // pointer
#define SHUNTCFUNC       (SHUNTCNOP + 11)       // function token
/* variables and registers */
#define SHUNTCVAR        (SHUNTCNOP + 12)       // variable
#define SHUNTCREG        (SHUNTCNOP + 13)       // register
/* complex, vector, and matrix types */
#define SHUNTCCOMPLEX    (SHUNTCNOP + 14)       // complex number
#define SHUNTCVECTOR     (SHUNTCNOP + 15)       // vector
#define SHUNTCMATRIX     (SHUNTCNOP + 16)       // matrix
#define SHUNTCLAST       SHUNTCMATRIX           // # of last defined token type
#define SHUNTCNTAB       (SHUNTCLAST + 1)       // # of items in tables
#define SHUNTCRTOL       (1U << 7)              // left-to-right precedence

/* token sign */

#define SHUNTCUNSIGNED   0
#define SHUNTCSIGNED     1

/* token flags */

#define SHUNTCZERO       (1 << 0)
#define SHUNTCUNDERFLOW  (1 << 1)
#define SHUNTCOVERFLOW   (1 << 2)

#endif /* __ZERO_BITS_SHUNTC_H__ */

