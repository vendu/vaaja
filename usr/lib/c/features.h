#ifndef __FEATURES_H__
#define __FEATURES_H__

/* system feature macros. */
#if !defined(_ISOC9X_SOURCE)
#define _ISOC9X_SOURCE      1
#endif
#if !defined(_ISOC11_SOURCE)
#define _ISOC11_SOURCE      1
#endif

#if !defined(_POSIX_SOURCE)
#define _POSIX_SOURCE       1
#endif
#if !defined(_POSIX_C_SOURCE)
#define _POSIX_C_SOURCE     199506L
#endif

#if !defined(_BSD_SOURCE)
#define _BSD_SOURCE         1
#endif

#if !defined(_LARGEFILE_SOURCE)
#define _LARGEFILE_SOURCE   1
#endif
#if !defined(_FILE_OFFSET_BITS)
#define _FILE_OFFSET_BITS   64
#endif
#if !defined(_LARGE_FILES)
#define _LARGE_FILES        1
#endif
#if !defined(_LARGEFILE64_SOURCE)
#define _LARGEFILE64_SOURCE 1
#endif

#endif /* __FEATURES_H__ */

