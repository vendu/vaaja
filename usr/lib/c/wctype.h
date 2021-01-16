#ifndef __WCTYPE_H__
#define __WCTYPE_H__

/* http://en.cppreference.com/w/c/string/wide */

#include <stddef.h>
#include <stdio.h>
#include <wchar.h>

typedef int32_t  wctype_t;
typedef int32_t  wctrans_t;

#if !defined(__KERNEL__)

extern wint_t    towctrans(wint_t, wctrans_t);
extern wint_t    towlower(wint_t);
extern wint_t    towupper(wint_t);
extern wctrans_t wctrans(const char *);
extern wctype_t  wctype(const char *);

#endif /* !__KERNEL__ */

#endif /* __WCTYPE_H__ */

