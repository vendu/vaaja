#ifndef SYS_ZEN_DECK_H
#define SYS_ZEN_DECK_H

#define ZEN_TEXT_API    1
#define ZEN_GFX_API     2
struct zendeck {
    long        api;
    struct evq  evq;
    void       *fb;
    long        w;
    long        h;
};

#endif /* SYS_ZEN_DECK_H */

