#ifndef __SYS_SYSMACROS_H__
#define __SYS_SYSMACROS_H__

#include <features.h>
#include <sys/mkdev.h>
#include <sys/param.h>

#define minor(dev)           ((dev_t)((dev) & MAXMIN)) 
#define major(dev)           ((dev_t)(((dev) >> NBITSMINOR) & MAXMAJ))
#define makedev(maj, min)    ((dev_t)(((dev) << NBITSMINOR) | ((min) & MAXMIN)))
#define makedevice(maj, min) ((dev_t)((maj) << NBITSMINOR) | ((min) & MAXMIN))
#if defined(__KERNEL__)
#define bmajor(dev)          major(dev)
#define getminor(dev)        minor(dev)
#define getmajor(dev)        major(dev)
#endif /* __KERNEL__ */
#define emajor(dev)          ((((dev_t)(dev) >> NBITSMINOR) > MAXMAJ) \
                              ? NODEV \
							  : (((dev_t)(dev) >> NBITSMINOR) & MAXMAJ))
#define eminor(dev)          ((dev_t)((dev) & MAXMIN))
#define getemajor(dev)       emajor(dev)
#define geteminor(dev)       eminor(dev)

#if !defined(__KERNEL__)

#if (_GNU_SOURCE)
extern unsigned int          gnu_dev_major(unsigned long long dev);
extern unsigned int          gnu_dev_minor(unsigned long long dev);
extern unsigned long long    gnu_dev_makedev(unsigned int major,
                                             unsigned int minor);
#endif /* _GNU_SOURCE */

#endif /* !defined(__KERNEL__) */

#define ISP2(x)               (!(((x) & ((x) - 1))))
#define P2ALIGN(x, aln)       ((x) & ~(aln))
#define P2PHASE(x, aln)       ((x) & ((aln) - 1))
#define P2NPHASE(x, aln)      (-(x) & ((align) - 1))
#define P2ROUNDUP(x, aln)     (-(-(x) & -(aln)))
#define P2END(x, aln)         (-(~(x) & -(aln)))
#define P2PHASEUP(x, aln, p)  ((p) - (((p) - (x)) & -(aln)))
#define P2CROSS(x, y, aln)    (((x) ^ (y)) > ((aln) - 1))
#define P2SAMEHIGHBIT(x, y)  (((x) ^ (y)) < ((x) & (y)))

#define INCR_COUNT(var, mtx)  (mtxlk(var), (*(var))++, mtxunlk(mtx))
#define DECR_COUNT(var, mtx)  (mtxlk(var), (*(var))--, mtxunlk(mtx))

#endif /* __SYS_SYSMACROS_H__ */

