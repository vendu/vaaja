#ifndef __FEATURES_H__
#define __FEATURES_H__

#define _ZERO_SOURCE                    1

/* system feature macros. */
#if !defined(_ISOC9X_SOURCE)
#define _ISOC99_SOURCE                  1
#endif
#if !defined(_ISOC11_SOURCE)
#define _ISOC11_SOURCE                  1
#endif

#if !defined(_POSIX_SOURCE)
#define _POSIX_SOURCE                   1
#endif
#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE                 200809L
#define _ATFILE_SOURCE                  1
#define _USE_POSIX2                     1
#endif

#if !defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE                   700
#define _XOPEN_SOURCE_EXTENDED          1
#endif
#if (_XOPEN_SOURCE >= 500)
#define _USE_UNIX98                     1
#endif
#if (_XOPEN_SOURCE >= 600)
#define _USE_XOPEN2K                    1
#endif
#if (_XOPEN_SOURCE >= 700)
#define _USE_XOPEN2K8                   1
#endif

#if !defined(_DEFAULT_SOURCE)
#define _DEFAULT_SOURCE                 1
#endif
#if !defined(_SVID_SOURCE)
#define _SVID_SOURCE                    1
#endif
#if !defined(_BSD_SOURCE)
#define _BSD_SOURCE                     1
#endif
#define _FAVOR_BSD                      0

#if !defined(_LARGEFILE_SOURCE)
#define _LARGEFILE_SOURCE               1
#endif
#if !defined(_FILE_OFFSET_BITS)
#define _FILE_OFFSET_BITS               64
#endif
#if !defined(_LARGE_FILES)
#define _LARGE_FILES                    1
#endif
#if !defined(_LARGEFILE64_SOURCE)
#define _LARGEFILE64_SOURCE             1
#endif

#endif /* __FEATURES_H__ */

