#include <stdlib.h>
#include <ui/ui.h>
#include <ui/text.h>
#if !defined(FREE)
#define FREE(x)                 free(x)
#endif
#if !defined(MALLOC)
#define MALLOC(x)               malloc(x)
#endif
#if !defined(CALLOC)
#define CALLOC(n, s)            calloc(n, s)
#endif
#if !defined(MEMCPY)
#define MEMCPY(dest, src, len) mempcy(dest, src, len)
#endif

void
uifreetextbuf(struct uitextbuf *buf)
{
    TEXT_T     **data = buf->scrdata;
    TEXTREND_T **rend = buf->scrrend;
    long         nrow = buf->nrow;
    long         nbufrow = buf->nbufrow;
    long         n;

    if (data) {
        for (n = 0 ; n < nrow ; n++) {
            if (data[n]) {
                FREE(data[n]);
            }
        }
        FREE(data);
    }
    if (rend) {
        for (n = 0 ; n < nrow ; n++) {
            if (rend[n]) {
                FREE(rend[n]);
            }
        }
        FREE(rend);
    }
    data = buf->data;
    if (data) {
        for (n = 0 ; n < nbufrow ; n++) {
            if (data[n]) {
                FREE(data[n]);
            }
        }
        FREE(data);
    }
    rend = buf->rend;
    if (rend) {
        for (n = 0 ; n < nbufrow ; n++) {
            if (rend[n]) {
                FREE(rend[n]);
            }
        }
        FREE(rend);
    }

    return;
}

long
uiinittextbuf(struct uitextbuf *buf, long nrow, long ncol, long nbufrow)
{
    long         ndx;
    TEXT_T     **data;
    TEXTREND_T **rend;
    TEXT_T      *dptr;
    TEXTREND_T  *rptr;

    data = MALLOC(nrow * sizeof(TEXT_T *));
    if (!data) {

        return 0;
    }
    rend = MALLOC(nrow * sizeof(TEXTREND_T *));
    if (!rend) {
        uifreetextbuf(buf);

        return 0;
    }
    buf->nrow = nrow;
    buf->ncol = ncol;
    buf->scrdata = data;
    buf->scrrend = rend;
    for (ndx = 0 ; ndx < nrow ; ndx++) {
        dptr = CALLOC(ncol, sizeof(TEXT_T));
        if (!dptr) {
            uifreetextbuf(buf);

            return 0;
        }
        data[ndx] = dptr;
        rptr = CALLOC(ncol, sizeof(TEXTREND_T));
        if (!rptr) {
            uifreetextbuf(buf);

            return 0;
        }
        rend[ndx] = rptr;
    }
    if (!nbufrow) {
        nbufrow = UITEXTDEFBUFNROW;
    }
    data = CALLOC(nbufrow, sizeof(TEXT_T *));
    if (!data) {
        uifreetextbuf(buf);

        return 0;
    }
    rend = CALLOC(nbufrow, sizeof(TEXTREND_T *));
    if (!data) {
        uifreetextbuf(buf);

        return 0;
    }
    buf->nbufrow = nbufrow;
    buf->data = data;
    buf->rend = rend;

    return 1;
}

