/* valhalla assembler configuration and command-line options */

#ifndef __VAS_OPT_H__
#define __VAS_OPT_H__

#include <zero/cdefs.h>

extern void vasinitalign(void);

#define vasalign2(adr, x)                                               \
    (!((x) & (sizeof(x) - 1))                                           \
     ? (adr)                                                            \
     : roundup2(adr,sizeof(x)))
#define vasalignword(adr)                                               \
    vasalign(adr, sizeof(vasword_t))
#define vasaligntok(adr, type)                                          \
    vasalign(adr, vastokalntab[(type)])

#endif /* __VAS_OPT_H__ */

