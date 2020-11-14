#ifndef __SYS_ZEN_BSP_V0_TYPES_H__
#define __SYS_ZEN_BSP_V0_TYPES_H__

#include <stdint.h>

typedef int32_t             m_atomic_t;
typedef int32_t             m_atomic32_t;

/* other system types */
typedef uint32_t            m_size_t;
typedef int32_t             m_ssize_t;
#if defined (__voima__)
typedef int32_t             m_off_t;
#else
typedef int64_t             m_off_t;
#endif
#endif
typedef uint32_t            m_intr_t;
typedef uint32_t            m_page_t;

#endif /* __SYS_ZEN_BSP_V0_TYPES_H__ */

