/* velho c library quick-sort implementation */

#include <stddef.h>
#include <stdint.h>
#include <limits.h>

#include <zero/trix.h>

#define _MAXTHRES 4
#define _STKSIZE  (CHAR_BIT * sizeof(size_t))

#define _swap8(ap, bp)                                                  \
    do {                                                                \
        uint8_t *_ap = (uint8_t *)(ap);                                 \
        uint8_t *_bp = (uint8_t *)(bp);                                 \
        uint8_t  _tmp = *_ap;                                           \
                                                                        \
        *_ap = *_bp;                                                    \
        *_bp = _tmp;                                                    \
    } while (0)
#define _swap16(ap, bp)                                                 \
    do {                                                                \
        uint16_t       *_ap = (uint16_t *)(ap);                         \
        uint16_t       *_bp = (uint16_t *)(bp);                         \
        uint16_t        _tmp = *_ap;                                    \
                                                                        \
        *_ap = *_bp;                                                    \
        *_bp = _tmp;                                                    \
    } while (0)
#define _swap32(ap, bp)                                                 \
    do {                                                                \
        uint32_t       *_ap = (uint32_t *)(ap);                         \
        uint32_t       *_bp = (uint32_t *)(bp);                         \
        uint32_t        _tmp = *_ap;                                    \
                                                                        \
        *_ap = *_bp;                                                    \
        *_bp = _tmp;                                                    \
    } while (0)
#define _swap64(ap, bp)                                                 \
    do {                                                                \
        uint64_t       *_ap = (uint64_t *)(ap);                         \
        uint64_t       *_bp = (uint64_t *)(bp);                         \
        uint64_t        _tmp = *_ap;                                    \
                                                                        \
        *_ap = *_bp;                                                    \
        *_bp = _tmp;                                                    \
    } while (0)
#define _swapgen(ap, bp, sz)                                            \
    do {                                                                \
        size_t          _sz = (sz);                                     \
        uint8_t        *_ap = (ap);                                     \
        uint8_t        *_bp = (bp);                                     \
        uint8_t         _tmp;                                           \
                                                                        \
        do {                                                            \
            _tmp = *_ap;                                                \
            *_ap++ = *_bp;                                              \
            *_bp++ = _tmp;                                              \
        } while (--_sz);                                                \
    } while (0)

#define _stkpush(lo, hi, sp)    ((sp)->low = (lo), (sp)->high = (hi), ++sp)
#define _stkpop(lo, hi, sp)     (--(sp), (lo) = (sp)->low, (hi) = (sp)->high)
#define _stknotempty(s, t)      ((s) < (t))
struct _stknode {
    void       *low;
    void       *high;
};

void
qsort(void *const base, size_t nitems, size_t size,
      int (*cmp)(const void *, const void *))
{
    uint8_t            *lo;
    uint8_t            *hi;
    uint8_t            *mid;
    uint8_t            *l;
    uint8_t            *r;
    uint8_t            *end;
    uint8_t            *tmp;
    uint8_t            *thres;
    uint8_t            *run;
    uint8_t            *item;
    const size_t        maxthres = _MAXTHRES * size;
    uint8_t             u8;
    struct _stknode     stk[_STKSIZE];
    struct _stknode    *stktop = stk;

    if (nitems <= 1) {

        return;
    }
    _stkpush(NULL, NULL, stktop);
    if (nitems > maxthres) {
	lo = base;
	hi = lo + (nitems - 1) * size;
	while (_stknotempty(stk, stktop)) {
            mid = lo + (((hi - lo) / size) >> 1) * size;
	    if (cmp(mid, lo) < 0) {
		_swapgen(mid, lo, size);
	    }
	    if (cmp(hi, mid) < 0) {
		_swapgen(mid, hi, size);
		if (cmp(mid, lo) < 0) {
		    _swapgen(mid, lo, size);
		}
	    }
	    l = lo + size;
	    r = hi - size;
	    do {
		while (cmp(l, mid) < 0) {
		    l += size;
		}
		while (cmp(mid, r) < 0) {
		    r -= size;
		}
		if (l < r) {
		    _swapgen(l, r, size);
		    if (mid == l) {
			mid = r;
		    } else if (mid == r) {
			mid = l;
		    }
		    l += size;
		    r -= size;
		} else if (l == r) {
		    l += size;
		    r -= size;

		    break;
		}
	    } while (l <= r);
	    if ((size_t)(r - lo) <= maxthres) {
		if ((size_t)(hi - l) <= maxthres) {
		    _stkpop(lo, hi, stktop);
		} else {
		    lo = l;
		}
	    } else if ((size_t)(hi - l) <= maxthres) {
		hi = r;
	    } else if ((r - lo) > (hi - l)) {
		_stkpush(lo, r, stktop);
		lo = l;
	    } else {
		_stkpush(l, hi, stktop);
		hi = r;
	    }
	}
    }
    tmp = base;
    end = tmp + (nitems - 1) * size;
    thres = min(end, tmp + maxthres);
    for (run = tmp + size ; run <= thres ; run += size) {
	if (cmp(run, tmp) < 0) {
	    tmp = run;
	}
    }
    if (tmp != (uint8_t *)base) {
	_swapgen(tmp, base, size);
    }
    tmp = base;
    run = tmp + size;
    while ((run += size) <= end) {
	tmp = run - size;
	while (cmp(run, tmp) < 0) {
	    tmp -= size;
	}
	tmp += size;
	if (tmp != run) {
	    item = run + size;
	    while (--item >= run) {
		u8 = *item;
		for (hi = lo = item ; (lo -= size) >= tmp ; hi = lo) {
		    *hi = *lo;
		}
		*hi = u8;
	    }
	}
    }

    return;
}

