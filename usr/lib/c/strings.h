#ifndef __STRINGS_H__
#define __STRINGS_H__

#include <string.h>

//#define bzero(ptr, sz)  memset(ptr, 0, sz)
/* TODO: POSIX.1-2008 removes index() and rindex() */
#define index(s, c)     strchr(s, c)
#define rindex(s, c)    strrchr(s, c)

#endif /* __STRINGS_H__ */

