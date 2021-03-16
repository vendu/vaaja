#ifndef __VOIMA_FEATURES_H__
#define __VOIMA_FEATURES_H__

#define _REENTRANT                      1
#define THREAD_SAFE                     1

#if defined(__STDC__)
#define _ISOC90_SOURCE                  1
#if (__STDC_VERSION__ >= 199901L)
#define _ISOC99_SOURCE                  1
#elif (__STDC_VERSION__ >= 201112L)
#define _ISOC11_SOURCE                  1
#endif /* defined(__STDC__ */

#define _FILE_OFFSET_BITS               64
#define _LARGEFILE_SOURCE               1
#define _LARGEFILE64_SOURCE             1

#define _POSIX_SOURCE                   1
#define _POSIX_C_SOURCE                 200809L

#define _XOPEN_SOURCE                   700
#define _XOPEN_VERSION                  4
#if (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 700)                   \
     && !defined(_POSIX_C_SOURCE))
#define _POSIX_C_SOURCE                 200809L
#elif (defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 600)                 \
       && !defined(_POSIX_C_SOURCE))
#define _POSIX_C_SOURCE                 200112L
#elif ((defined(_XOPEN_SOURCE) && (_XOPEN_SOURCE >= 500)                \
        && !defined(_POSIX_C_SOURCE))
#define _POSIX_C_SOURCE                 199506L
#endif

#endif /* __VOIMA_FEATURES_H__ */

