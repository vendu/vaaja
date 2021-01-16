#ifndef __VALUES_H__
#define __VALUES_H__

#if (defined(_MSDOS_SOURCE)                                             \
     || defined(_BORLANDC_SOURCE) || defined(_TURBOC_SOURCE))

#if defined(__STDC__)
#include <limits.h>
#define BITSPERBYTE CHAR_BIT
#endif

/* integer parameters */
#define HIBITS        (1 << (BITSPERBYTE * sizeof(short) - 1))
#define HIBITI        (1 << (BITSPERBYTE * sizeof(int) - 1))
#define HIBITL        (1L << (BITSPERBYTE * sizeof(long) - 1))
#define MAXSHORT      ((1 << (BITSPERBYTE * sizeof(short) - 1)) - 1)
#define MAXINT        ((1 << (BITSPERBYTE * sizeof(int) - 1)) - 1)
#define MAXLONG       ((1L << (BITSPERBYTE * sizeof(long) - 1)) - 1)
/* floating-point parameters */
#define _LENBASE      1
#define _DEXPLEN      11
#define _FEXPLEN      8
#define _EXPBASE      2
#define _IEEE         1
#define HIDDENBIT     1
#define DMAXEXP       308
#define FMAXEXP       38
#define DMINEXP      -307
#define FMINEXP      -37
#define MAXDOUBLE     1.797693E+308
#define MAXFLOAT      3.37E+38
#define MINDOUBLE     2.225074E-308
#define MINFLOAT      8.43E-37
#define DSIGNIF       53
#define FSIGNIF       24
#define DMAXPOWTWO    0x3ff
#define FMAXPOWTWO    0x7f
#define LN_MAXDOUBLE  7.0978E+2
#define LN_MINDOUBLE -7.0840E+2

#endif

#endif /* __VALUES_H__ */

