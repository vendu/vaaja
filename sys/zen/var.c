#include <sys/zen/conf.h>
#include <stdint.h>
#include <mach/param.h>
#include <zero/cdefs.h>
#include <sys/zen/var.h>

/* set and query system configuration parameters (both static and dynamic) */

struct zenvars  k_zenvars C_ALIGNED(MACH_PAGE_SIZE);

/* TODO: map the rest of the variables from decice or address spaces */

void
kinitvars(void)
{
    k_zenvars.hirestmr = kmaphirestmr();
    k_zenvars.rtctime = kmaprtc();
    k_zenvars.tmrhz = ZEN_TMR_HZ;
    k_zenvars.openmax = ZEN_OPEN_MAX;
    k_zenvars.clsize = MACH_CL_SIZE;
    k_zenvars.wordsize = MACH_WORD_SIZE;
    k_zenvars.adrbits = MACH_ADR_BITS;
    k_zenvars.banksize = MACH_BANK_SIZE;
    k_zenvars.pagesize = MACH_PAGE_SIZE;
    k_zenvars.shmmax = ZEN_SHM_MAX;
    k_zenvars.evqmax = ZEN_EVQ_MAX;

    return;
}

void
ksetvar(int id, int val)
{
    struct zenproc *proc = k_getcurproc();

    if (!proc->zenvars) {
        kinitvars();
        kmemcpy(proc->zenvars, k_zenvars, sizeof(struct zenvars));
    }
    switch (id) {
        case ZEN_VAR_OPEN_MAX:
            p_zenvars->openmax = val;

            break;
        case ZEN_VAR_SHM_MAX:
            p_zenvars->shmmax = val;

            break;
        default:

            kpanic("invalid argument for ksetvar()", SIGSYS);
    }
}

void
kgetvar(int16_t ofs, int16_ size)
{
    struct zenproc *proc = k_getcurproc();

    if (!proc->zenvars) {
        kinitvars();
        kmemcpy(proc->zenvars, k_zenvars, sizeof(struct zenvars));
    }
    if (ofs < 0 || ofs > sizeof(struct zenvars)) {

            kpanic("invalid ofs-argument for kgetvar()", SIGSYS);
    }
    switch (size) {
        case 4:

            return (int32_t *)(proc->zenvars)[ofs];
        case 2:

            return (int16_t *)(proc->zenvars)[ofs];
        case 1:

            return (int8_t *)(proc->zenvars)[ofs];
        default:

            kpanic("invalid size-argument for kgetvar()", SIGSYS);
    }
}

