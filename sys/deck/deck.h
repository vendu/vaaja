#ifndef DECK_DECK_H
#define DECK_DECK_H

#include <deck/conf.h>
#include <stdio.h>
#include <stdint.h>

#include <deck/endian.h>
#include <deck/cdefs.h>
#include <deck/intr.h>
#include <deck/regs.h>
#include <deck/io.h>
#include <deck/inst.h>
#if defined(DECK_MM_EXTENSION) || defined(DECK_MP_EXTENSION)
#include <deck/inst-ext.h>
#endif
#if defined(DECK_MM_EXTENSION)
#include <deck/mmu.h>
#endif
#if defined(DECK_FP_EXTENSION)
#include <deck/fpu.h>
#endif
#if defined(DECK_FP_EXTENSION)
#include <deck/fxp.h>
#endif
#if defined(DECK_SANITY_CHECK)
#include <deck/sanity.h>
#endif

#endif /* DECK_DECK_H */

