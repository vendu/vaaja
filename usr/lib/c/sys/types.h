#ifndef __SYS_TYPES_H__
#define __SYS_TYPES_H__

#include <features.h>
#include <stdint.h>

#if !defined(__blkcnt_t_defined)
typedef uint32_t                        blkcnt_t;
#define __blkcnt_t_defined              1
#endif
#if !defined(__blksize_t_defined)
typedef uint32_t                        blksize_t;
#define __blksize_t_defined             1
#endif
#if !defined(__clock_t_defined)
typedef int32_t                         clock_t;
#define __clock_t_defined               1
#endif
#if !defined(__clockid_t_defined)
typedef int32_t                         clockid_t;
#define __clockid_t_defined             1
#endif
#if !defined(__dev_t_defined)
typedef uint32_t                        dev_t;
#define __dev_t_defined                 1
#endif
#if !defined(__id_t_defined)
typedef uint32_t                        id_t;
#define __id_t_defined                  1
#endif
#if !defined(__ino_t_defined)
typedef uint32_t                        ino_t;
#define __ino_t_defined                 1
#endif
#if !defined(__pid_t_defined)
typedef int32_t                         pid_t;
#define __pid_t_defined                 1
#endif
#if !defined(__uid_t_defined)
typedef uint32_t                        uid_t;
#define __uid_t_defined                 1
#endif
#if !defined(__gid_t_defined)
typedef uint32_t                        gid_t;
#define __gid_t_defined                 1
#endif
#if !defined(__mode_t_defined)
typedef uint32_t                        mode_t;
#define __mode_t_defined                1
#endif
#if !defined(__nlink_t_defined)
typedef uint32_t                        nlink_t;
#define __nlink_t_defined               1
#endif
#if !defined(__off_t_defined)
#if (_FILE_OFFSET_BITS == 32)
typedef int32_t                         off_t;
#elif (_FILE_OFFSET_BITS == 64)
typedef int64_t                         off_t;
#endif
#define __off_t_defined                 1
#endif
#if defined(_LARGEFILE_SOURCE) && (_FILE_OFFSET_BITS == 64) && !defined(__off64_t_defined)
typedef int64_t                         off64_t;
#define __off64_t_defined               1
#endif
#if !defined(__size_t_defined)
typedef uintptr_t                       size_t;
#define __size_t_defined                1
#endif
#if !defined(__time_t_defined)
typedef int64_t                         time_t;
#define __time_t_defined                1
#endif
#if !defined(__useconds_t_defined)
typedef int32_t                         useconds_t;
#define __useconds_t_defined            1
#endif

#endif /* __SYS_TYPES_H__ */

