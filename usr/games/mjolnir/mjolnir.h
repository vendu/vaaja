#ifndef __MJOLNIR_MJOLNIR_H__
#define __MJOLNIR_MJOLNIR_H__

#include <mjolnir/conf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <deck/lib/cdefs.h>
#include <prng/randmt32.h>
#include <mjolnir/util.h>
#include <mjolnir/tty.h>
#include <mjolnir/d20.h>
#include <mjolnir/deck.h>
#include <mjolnir/obj.h>
#include <mjolnir/chr.h>
#include <mjolnir/item.h>
#include <mjolnir/ice.h>
#include <mjolnir/dng.h>
#include <deck/lib/bitmagic.h>

void                            iceinitchr(struct objchr *chr,
                                           const char *name, long cat);

#endif /* __MJOLNIR_MJOLNIR_H__ */

