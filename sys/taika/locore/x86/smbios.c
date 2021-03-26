#include <kern/conf.h>

#if (SMBIOS)

#include <stddef.h>
#include <stdint.h>
#include <kern/util.h>
#include <kern/unit/x86/smbios.h>

struct smbiosdrv smbiosdrv;

static struct smbioshdr *
smbiosfind(void)
{
    struct smbioshdr *hdr = NULL;
    uint32_t          *ptr = (uint32_t *)0xf0000;
    uint32_t          *lim = (uint32_t *)0xfffff;

    while (ptr < lim) {
        if (*ptr == SMBIOSSIG) {
            hdr = (struct smbioshdr *)ptr;

            return hdr;
        }
        ptr += 4;
    }

    return hdr;
}

void
smbiosinit(void)
{
    struct smbioshdr *hdr;

    hdr = smbiosfind();
    if (!hdr) {
        kprintf("SMBIOS: header not found!\n");

        return;
    }
    kprintf("SMBIOS: header found at %p\n", hdr);
    smbiosdrv.ninfo = hdr->ninfo;
    smbiosdrv.infophys = (void *)(hdr->tabphysadr);

    return;
}

#endif /* SMBIOS */

