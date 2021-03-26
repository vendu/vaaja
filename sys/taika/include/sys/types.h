#ifndef TAIKA_SYS_TYPES_H
#define TAIKA_SYS_TYPES_H

#include <internal/mach/types.h>

#if defined(_FILE_OFFSET_BITS) && (_FILE_OFFSET_BITS == 64)
typedef __OFF_T                 off_t;
typedef __SSIZE_T               ssize_t;
#else
typedef __OFF64_T               off64_t;
typedef __OFF_T                 off_t;
typedef __SSIZE_T               ssize_t;
#endif
typedef __SIZE_T                size_t;
typedef __TIME_T                time_t;
typedef __PID_T                 pid_t;
typedef __UID_T                 uid_t;
typedef __GID_T                 gid_t;
typedef __FD_T                  int;
typedef __BLKCNT_T              blkcnt_t;
typedef __BLKSIZE_T             blksize_t;
typedef __CLOCK_T               clock_t;
typedef __CLOCKID_T             clockid_t;
typedef __DEV_T                 dev_t;
typedef __FSBLKCNT_T            fsblkcnt_t;
typedef __FSFILCNT_T            fsfilcnt_t;
typedef __ID_T                  id_t;
typedef __INO_T                 ino_t;
typedef __MODE_T                mode_t;
typedef __NLINK_T               nlink_t;
typedef __USECONDS_T            useconds_t;
typedef __SUSECONDS_T           suseconds_t;
typedef __KEY_T                 key_t;

#endif /* TAIKA_SYS_TYPES_H */

