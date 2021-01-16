#ifndef __LOCKING_H__
#define __LOCKING_H__

#if (defined(_MSDOS_SOURCE)                                             \
     || defined(_BORLANDC_SOURCE) || defined(_TURBOC_SOURCE))

#define LK_UNLCK  0     // unlock file region
#define LK_LOCK   1     // lock file region, try for 10 seconds
#define LK_NBLCK  2     // lock file region, don't block
#define LK_RLCK   3     // same as LK_LOCK
#define LK_NBRLCK 4     // same as LK_NBLCK

#endif

#endif /* __LOCKING_H__ */

