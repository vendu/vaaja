#ifndef DECK_CONF_H
#define DECK_CONF_H

/* extensions; comment out or #undef to disable */
#define DECK_MM_EXTENSION       1
#define DECK_MP_EXTENSION       1
#define DECK_FP_EXTENSION       1
#define DECK_FX_EXTENSION       1

#define DECK_SDRAM_SIZE         (8192 * 1024 * 1024)

#if !defined(DECK_RELEASE)
#   define DECK_SANITY_CHECK    1
#endif

#endif /* DECK_CONF_H */

