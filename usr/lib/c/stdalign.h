#ifndef __STDALIGN_H__
#define __STDALIGN_H__

#if (__STDC_VERSION__ < 201112L)

#define _Alignas(s) ALIGNED(s)
#define _Alignof(s) ALIGNOF(s)

#if !defined(alignas_is_defined) || !(alignas_is_defined)
#define alignas _Alignas
#define alignas_is_defined 1
#endif
#if !defined(alignof_is_defined) || !(alignof_is_defined)
#define alignof _Alignof
#define alignof_is_defined 1
#endif

#endif

#endif /* __STDALIGN_H__ */

