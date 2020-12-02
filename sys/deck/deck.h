#ifndef DECK_DECK_H
#define DECK_DECK_H

#include <deck/conf.h>
#include <deck/cdefs.h>
#include <deck/intr.h>
#include <deck/regs.h>
#include <deck/io.h>
#include <deck/inst.h>
#if defined(DECK_SANITY_CHECK)
#include <stdio.h>
#include <deck/sanity.h>
#endif

#if defined(DECK_SANITY_CHECK)
#   define DECK_PRINT_SANITY()  fprintf(stderr, "SANITY CHECKS PASSED\n")
#endif

#endif /* DECK_DECK_H */

