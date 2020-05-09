#ifndef __WCHAR_H__
#define __WCHAR_H__

#include <stddef.h>
#include <bits/wchar.h>

#if !defined(__wint_t_defined)
typedef int                     wint_t;
#define __wint_t_defined        1
#endif

#define WCHAR_MIN    	0
#if (WCHARSIZE == 4)
#define WEOF            ((wint_t)0xfffffff) /* -1 */
#define WCHAR_MAX    	0x0010ffff
#define WCHAR_MIN       (-0x7fffffff - 1)
#define WCHARBITS       21
#elif (WCHARSIZE == 2)
#define WEOF            ((wint_t)0xffff) /* invalid character value (Unicode) */
#define WCHAR_MAX    	0xffff
#define WCHAR_MIN    	0
#define WCHARBITS       16
#elif (WCHARSIZE == 1)
#define WEOF         	0x00
#define WCHAR_MAX    	0xff
#define WCHAR_MIN    	0
#define WCHARBITS       8
#endif /* WCHARSIZE */
#define WCHARMASK       ((1 << WCHARBITS) - 1)

#if !defined(__wchar_t_defined)
#if (WCHARSIZE == 4)
typedef int32_t                 wchar_t;
#define __wchar_t_defined       1
#elif (WCHARSIZE == 2)
typedef uint16_t                wchar_t;
#define __wchar_t_defined       1
#elif (WCHARSIZE == 1)
typedef uint8_t                 wchar_t;
#define __wchar_t_defined       1
#endif
#endif

#endif /* __WCHAR_H__ */

