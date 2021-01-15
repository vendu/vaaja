#ifndef SYS_ZEN_TRAP_H
#define SYS_ZEN_TRAP_H

#include <mach/trap.h>

#define kintrmask(mask)         kpanic("kintrmask not implemented yet", SIGSYS);
#define kintron(void)           kpanic("kintron not implemented yet", SIGSYS);
#define kintroff(void)          kpanic("kintron not implemented yet", SIGSYS);

#endif /* SYS_ZEN_TRAP_H */

