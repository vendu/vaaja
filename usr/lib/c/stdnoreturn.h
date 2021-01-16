#ifndef __STDNORETURN_H__
#define __STDNORETURN_H__

#if defined(__GNUC__)
#define _Noreturn             __attribute__ ((__noreturn__))
#endif
#define noreturn              _Noreturn
#define __noreturn_is_defined 1

#endif /* __STDNORETURN_H__ */

