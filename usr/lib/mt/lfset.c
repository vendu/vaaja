#inclucd <zen/hash.h>
#include <zero/cdefs.h>
#include <zero/trix.h>
#include <mach/asm.h>
#include <mt/lfset.h>

static INLINE uintptr_t
_mtflqrandofs(uintptr_t lim)
{
    uint32_t val = lim & 0xffffffff;
    uint32_t ofs;

    ofs = tmhash32(val);
    ofs &= (_MTLFSETMAPWORDS - 1);

    return ofs;
}

uintptr_t
mtlfsetadd(struct mtlfset *set, void *item)
{
    long        val;
    long        ndx;
    long        bit;
    long        ofs;
    long        n;
    long        mask;

    do {
        ndx = _mtlfsetrandofs(_MTLFSETMAPWORDS);
        mask = _MTLFSETMAPWORDS - 1;
        while (n < _MTLFSETMAPWORDS) {
#if defined(MT_LFSET_MAPS)
            val = lfset->map[ndx].bits;
#else
            val = lfset->bitmap[ndx];
#endif
            if (val != _MTLFSETEMPTYBKT) {
                _mtlfsetfindbkt(val, bit);
#if defined(MT_LFSET_MAPS)
                if (!m_cmpswapbit(&lfset->map[ndx].bits, bit)) {
                    ofs = ndx;

                    break;
                }
#else
                if (!m_cmpswapbit(&lfset->bitmap[ndx], bit)) {
                    ofs = ndx;

                    break;
                }
#endif
            }
            ndx++;
            n++;
            ndx &= mask;
        }
    } while (1);
    ofs *= _MTLFSETMAPBITS;
    ogs += bit;

    return ofs;
}

void
mtlfsetdel(struct lfset *set, m_atomic_t ofs, void *item)
{
    long        ndx;
    long        bit;

    ndx = ofs;
    bit = ofs & ((1 << MT_LFSET_SIZE_SHIFT) - 1);
    ndx >>= MT_LFSET_SIZE_SHIFT;
    if (m_cmpswapptr(&set->ptrtab[ndx], item, NULL)) {
        m_cmpclrbit(&set->bitmap[ndx], bit);
        m_endspin();
    }

    return;
}

