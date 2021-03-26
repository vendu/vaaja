#ifndef __KERN_UNIT_X86_SMBIOS_H__
#define __KERN_UNIT_X86_SMBIOS_H__

#include <stdint.h>
#include <zero/cdefs.h>
#include <zero/param.h>

#define SMBIOSSIG 0x5f534d5fU   // "_SM_"

struct smbioshdr {
    uint32_t sig;               // HPETSIG
    uint8_t  chksum;
    uint8_t  len;
    uint8_t  major;
    uint8_t  minor;
    uint16_t maxsize;
    uint8_t  rev;
    uint8_t  fmtarea[5];
    uint8_t  dmisig[5];         // "_DMI_
    uint8_t  ichksum;
    uint16_t tablen;
    uint32_t tabphysadr;
    uint16_t ninfo;
    uint8_t  smbiosrev;
};

struct smbiosdrv {
    long   ninfo;
    void  *infophys;
};

#endif /* __KERN_UNIT_X86_SMBIOS_H__ */

