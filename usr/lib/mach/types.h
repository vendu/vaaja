#ifndef  __MACH_TYPES_H__
#define  __MACH_TYPES_H__

#include <stdint.h>
#include <zero/cdefs.h>
#include <mach/param.h>

#if 0
/* machine types */
typedef int8_t     		        m_byte_t;
typedef int16_t    		        m_dualbyte_t;
//typedef uintptr_t               m_adr_t;
typedef void *                  m_ptr_t;

#if (LONGLONGSIZE > LONGSIZE)
typedef long long               m_word_t;
typedef unsigned long long      m_uword_t;
#else
typedef long                    m_word_t;
typedef unsigned long           m_uword_t;
#endif
#endif /* 0 */

#if defined(__v0__)
#include <mach/v0/types.h>
#elif defined(_WIN64)
#include <zero/msc/win64.h>
#elif defined(_WIN32)
#include <zero/msc/win32.h>
#elif defined(_x86_64__) || defined(__amd64__)
#include <mach/x86/types.h>
#include <mach/x86-64/types.h>
#elif (defined(__i386__) || defined(__i486__)                           \
       || defined(__i586__) || defined(__i686__))
#include <mach/x86/types.h>
#include <mach/ia32/types.h>
#elif defined(__arm__)
#include <mach/arm/types.h>
#elif defined(__ppc__)
#include <mach/ppc/types.h>
#endif

#endif /* __MACH_TYPES_H__ */

