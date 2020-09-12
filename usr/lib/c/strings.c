#include <stddef.h>
#include <stdint.h>
#include <mach/param.h>
#include <zero/trix.h>

void
bzero(void *ptr, size_t nb)
{
    int8_t     *bptr = ptr;
    uintptr_t  *uptr = ptr;
    uintptr_t   zero = 0;
    uintptr_t   val;

    val = (uintptr_t)ptr & (sizeof(uintptr_t) - 1);
    if (val) {
        val = sizeof(uintptr_t) - val;
        val = max(val, nb);
        nb -= val;
        while (val--) {
            *bptr = 0;
        }
    }
    if (nb >= 2 * MACH_CL_SIZE) {
        val = (uintptr_t)bptr & (MACH_CL_SIZE - 1);
        if (val) {
            val = sizeof(uintptr_t) - val;
            uptr = (uintptr_t *)bptr;
            nb -= val;
            while (val--) {
                *uptr++ = zero;
            }
        } else {
            uptr = (uintptr_t *)bptr;
        }
        val = nb >> MACH_CL_SIZE_LOG2;
        nb -= val * MACH_CL_SIZE;
        if (nb) {
            do {
                uptr[0] = zero;
                uptr[1] = zero;
                uptr[2] = zero;
                uptr[3] = zero;
                uptr[4] = zero;
                uptr[5] = zero;
                uptr[6] = zero;
                uptr[7] = zero;
                uptr += 8;
            } while (--nb);
        }
    }
    val = nb / sizeof(uintptr_t);
    if (nb) {
        while (nb--) {
            *uptr++ = zero;
        }
    }
    bptr = (int8_t *)uptr;
    while (nb--) {
        *bptr++ = 0;
    }

    return;
}

