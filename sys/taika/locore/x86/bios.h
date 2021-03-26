#ifndef __KERN_UNIT_X86_BIOS_H__
#define __KERN_UNIT_X86_BIOS_H__

#define BIOSWARMREBOOT   0x1234

/* initialisation */
#define RTCBASE          0x70
#define BIOSWRV          0x467          // warm reset vector
#define BIOSREBOOTMODE   0x472

#endif /* __KERN_UNIT_X86_BIOS_H__ */

