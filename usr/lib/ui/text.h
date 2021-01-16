#ifndef __UI_TEXT_H__
#define __UI_TEXT_H__

#include <stdint.h>

#if !defined(TEXT_T)
#define TEXT_T uint8_t
#endif

struct textrend {
    unsigned fgcolor : 9;       // standard or 256-color xterm palette entry
    unsigned bgcolor : 9;       // standard or 256-color xterm palette entry
    unsigned pad     : 7;       // extra room for later flags
    unsigned atr     : 7;       // specified text attributes
};
typedef struct textrend TEXTREND_T;

struct uitextbuf {
    long              nrow;     // # of buffer rows
    long              ncol;     // # of colums in rows
    TEXT_T          **scrdata;  // on-screen data
    struct textrend **scrrend;  // on-screen text attributes
    long              nbufrow;  // # of buffer rows
    TEXT_T          **data;     // Unicode text data
    struct textrend **rend;     // rendition attribute data
};

void uifreetextbuf(struct uitextbuf *buf);
long uiinittextbuf(struct uitextbuf *buf, long nrow, long ncol, long nbufrow);

#endif /* __UI_TEXT_H__ */

