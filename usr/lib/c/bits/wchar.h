#ifndef __BITS_WCHAR_H__
#define __BITS_WCHAR_H__

#if !defined(WCHARSIZE)

#include <mach/param.h>

#if defined(__STDC_ISO_10646__) || defined(_ZERO_SOURCE)
/* full ISO10646 character */
#define WCHARSIZE      4
#elif (defined(_WIN32) || defined(_WIN64))
/* 16-bit Unicode character */
#define WCHARSIZE      2
#else
#error WCHARSIZE not present in <bits/wchar.h>
#endif

#endif /* !defined(WCHARSIZE) */

#endif /* __BITS_WCHAR_H__ */

