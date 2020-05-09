#include <stdlib.h>
#include <string.h>
#include <v0/v0.h>

struct v0sys *
v0confsys(struct v0sys *sys, struct v0sysconf *conf)
{
    size_t nbmem = V0_MEM_SIZE;
    size_t ncl = nbmem >> V0_CACHE_LINE_SHIFT;
    size_t ntlb = V0_TLB_ENTRIES;
    size_t npg = nbmem >> V0_PAGE_SHIFT;
    size_t nio = V0_IOMAP_ITEMS;

    if (!sys) {
        sys = calloc(1, sizeof(struct v0sys));
    } else {
        memset(sys, 0, sizeof(struct v0sys));
    }
    if (sys) {
        if (!v0initsys(sys, nbmem, ncl, ntlb, npg, nio)) {
            if (sys->mem) {
                free(sys->mem);
            }
            if (sys->pagetab) {
                free(sys->pagetab);
            }
            if (sys->clbits) {
                free(sys->clbits);
            }
            if (sys->iomap) {
                free(sys->iomap);
            }

            return NULL;
        }
    }

    return sys;
}

