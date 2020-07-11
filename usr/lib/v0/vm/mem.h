#ifndef __V0_MEM_H__
#define __V0_MEM_H__

#include <stddef.h>
#include <zero/cdefs.h>

/* BRAM is only initialized at run-time and never freed */
static C_INLINE void *
v0balloc(size_t size)
{
    volatile void  *ptr;
    size_t          ofs;

    ;
}

#endif /* __V0_MEM_H__ */

