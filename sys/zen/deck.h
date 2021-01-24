#ifndef SYS_ZEN_DECK_H
#define SYS_ZEN_DECK_H

#include <sys/zen/conf.h>
#if defined(ZEN_GFX_DECK)
#include <gfx/api.h>
#endif
#define ZEN_INVALID_API 0
#define ZEN_TEXT_API    1
#define ZEN_GFX_API     2
struct zendeck {
    long        api;
    struct ev  *evq;
    long        pixfmt;
    void       *fb;
    long        w;
    long        h;
};

#endif /* SYS_ZEN_DECK_H */

