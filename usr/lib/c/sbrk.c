#include <features.h>
#include <stdint.h>
#include <unistd.h>
#include <zero/unix.h>

static void *g_curbrk;

void *
sbrk(intptr_t incr)
{
    uint8_t *retptr = g_curbrk;
    uint8_t *u8ptr = g_curbrk;

    if (u8ptr) {
        u8ptr += incr;
        if (incr) {
            if (!brk(u8ptr)) {
                
                g_curbrk = u8ptr;
            } else {
                
                return SBRK_FAILED;
            }
        }
    }

    return retptr;
}

