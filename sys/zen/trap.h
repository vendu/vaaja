#ifndef __SYS_ZEN_TRAP_H__
#define __SYS_ZEN_TRAP_H__

#include <sys/zen/signal.h>
#if defined(__v0__)
#include <v0/trap.h>
#endif

#define kintron(void)  kpanic("kintron not implemented yet", SIGSYS);
#define kintroff(void)  kpanic("kintron not implemented yet", SIGSYS);

#endif /* __SYS_ZEN_TRAP_H__ */

