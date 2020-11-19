#include <mjolnir/conf.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <zero/trix.h>
#include <mjolnir/chr.h>
#include <mjolnir/obj.h>
#include <mjolnir/dng.h>

typedef int                 (*const dirfunc)(const void *, const void *);

#define dngeqpnt(a, b)      ((a).x == (b).x && (a).y == (b).y)
#define dngextpnt(p, w, h)  ((p).x < 0 || (p).y < 0 || (p).x >= w || (p).y >= h)
#define dngtruncf(x)        (float)((int)x - (x < (int)(x)))
#define dngsnappnt(p, g)                                                \
    (struct pnt){ dngtruncf((p).x / (g)) * (g),  dngtruncf((p).y / (g)) * (g) }
#define dngsubpnt(a, b)     (struct pnt){ (a).x - (b).x, (a).y - (b).y }
#define dngmagpnt(p)        sqrtf((p).x * (p).x + (p).y * (p).y)
#define dngnewpntset(n)                                                 \
    (struct pntset){ malloc((n) * sizeof(struct pnt)), 0, (n) }
#define dngeqtri(a, b)      (dngeqpnt((a).p1, (b).p1)                   \
                             && dngeqpnt((a).p2, (b).p2))
#define dngreveqtri(a, b)   (dngeqpnt((a).p1, (b).p2)                   \
                             && dngeqpnt((a).p2, (b).p1))
#define dngalndtri(a, b)    (dngeqtri(a, b) || dngreveqtri(a, b))
#define dngedgelen(e)       dngmagpnt(dngsubpnt((e).p2, (e).p1))
#define dngnewtriset(n)                                                 \
    (struct triset){ malloc((n) * sizeof(struct tri)), 0, (n) }

struct pnt {
    float       x;
    float       y;
};

struct pntset {
    struct pnt *tab;
    int         cnt;
    int         max;
};

struct tri {
    struct pnt  p1;
    struct pnt  p2;
    struct pnt  p3;
};

struct triset {
    struct tri *tab;
    int         cnt;
    int         max;
};

struct flags {
    struct pnt  zero;
    struct pnt  one;
};

static void
dngfail(const char *const message, ...)
{
    va_list args;

    va_start(args, message);
    vprintf(message, args);
    va_end(args);

#if defined(NDEBUG)
    exit(1);
#else
    abort();
#endif
}

static char **
dnginit(char *blk, const int w, const int h, const int val)
{
    int                 lim = w * h;
    int                 ndx;

    for (ndx = 0; ndx < lim; ndx++) {
        blk[ndx] = val;
    }

    return blk;
}

static char **
dngnewblk(const int w, const int h, const int val)
{
    char               *blk = malloc(w * h * sizeof(char));

    dnginit(blk, w, h, val);

    return blk;
}

static struct dng
dngnew(const int w, const int h)
{
    struct dng      dng;
    unsigned int    seed = time(NULL);

    dng.bdng = calloc(roundup2(w * h, 8) / CHAR_BIT, sizeof(uint8_t));
    dng.celltab = dngnewblk(w, h, MJOLNIR_ITEM_WALL);
    dng.nempty = 0;
    dng.w = w;
    dng.h = h;
    dng.seed = seed;

    return dng;
}

static struct pntset
dngaddpnt(struct pntset set, const struct pnt pnt)
{
    if (set.cnt != set.max) {
        set.tab[set.cnt++] = pnt;
    } else {
        dngfail("pntset count limitation reached\n");
    }

    return set;
}

static int
dngfindpnt(const struct pntset set, const struct pnt pnt)
{
    int i;

    for (i = 0; i < set.cnt; i++) {
        if (dngeqpnt(set.tab[i], pnt)) {

            return 1;
        }
    }

    return 0;
}

static struct triset
dngaddtri(struct triset set, const struct tri tri)
{
    if (set.cnt != set.max) {
        set.tab[set.cnt++] = tri;
    } else {
        dngfail("triset count limitation reached\n");
    }

    return set;
}

// Flags alligned edges.
static void
dngmarkedges(struct triset edges, const struct flags flags)
{
    int i;
    int j;

    for (i = 0; i < edges.cnt; i++) {
        const struct tri    edg = edges.tab[i];

        for (j = 0; j < edges.cnt; j++) {
            if (i != j) {
                const struct tri    tri = edges.tab[j];

                if (dngalndtri(edg, tri)) {
                    edges.tab[j].p3 = flags.one;
                }
            }
        }
    }

    return;
}

// Creates new triangles from unique edges and appends to tset.
static struct triset
dngjoinedges(struct triset set, const struct triset edges, const struct pnt pnt,
             const struct flags flags)
{
    int i;

    for (i = 0; i < edges.cnt; i++) {
        const struct tri    edg = edges.tab[i];

        if (dngeqpnt(edg.p3, flags.zero)) {
            const struct tri    tri = { edg.p1, edg.p2, pnt };

            set = dngaddtri(set, tri);
        }
    }

    return set;
}

static int dngincircntr(const struct tri tri, const struct pnt pnt)
{
    const float ax = tri.p1.x - pnt.x;
    const float ay = tri.p1.y - pnt.y;
    const float bx = tri.p2.x - pnt.x;
    const float by = tri.p2.y - pnt.y;
    const float cx = tri.p3.x - pnt.x;
    const float cy = tri.p3.y - pnt.y;
    const float det =
        (ax * ax + ay * ay) * (bx * cy - cx * by) -
        (bx * bx + by * by) * (ax * cy - cx * ay) +
        (cx * cx + cy * cy) * (ax * by - bx * ay);

    if (det > 0.0f) {

        return 1;
    }

    return 0;
}

// Collects all edges from given triangles.
static struct triset
dnggetedges(struct triset edges, const struct triset in,
            const struct flags flags)
{
    int i;

    for (i = 0; i < in.cnt; i++) {
        const struct tri    tri = in.tab[i];
        const struct tri    ab = { tri.p1, tri.p2, flags.zero };
        const struct tri    bc = { tri.p2, tri.p3, flags.zero };
        const struct tri    ca = { tri.p3, tri.p1, flags.zero };

        edges = dngaddtri(edges, ab);
        edges = dngaddtri(edges, bc);
        edges = dngaddtri(edges, ca);
    }

    return edges;
}

static struct triset
dngdlntri(const struct pntset pset, const int w, const int h, const int max,
          const struct flags flags)
{
    struct triset       set = dngnewtriset(max);
    struct triset       in = dngnewtriset(max);
    struct triset       out = dngnewtriset(max);
    struct triset       edges = dngnewtriset(max);
    // Shallow copies are exploited here for quick array concatentations.
    // In doing so, the original tris triangle is lost. This dummy pointer
    // will keep track of it for freeing at a later date.
    struct tri         *dummy = set.tab;
    // The super triangle will snuggley fit over the screen.
    const struct tri    super = { { (float) -w, 0.0f },
                                  { 2.0f * w, 0.0f },
                                  { 0.5f * w, 2.0f * h } };
    int i;
    int j;

    set = dngaddtri(set, super);
    for (i = 0; i < pset.cnt; i++)
    {
        const struct pnt    pnt = pset.tab[i];

        in.cnt = 0;
        out.cnt = 0;
        edges.cnt = 0;
        // For all triangles...
        for (j = 0; j < set.cnt; j++)
        {
            const struct tri    tri = set.tab[j];

            if (dngincircntr(tri, pnt)) {
                // Get triangles where point lies inside their circumcenter...
                in = dngaddtri(in, tri);
            } else {
                // Get triangles where point lies outside of their circumcenter.
                out = dngaddtri(out, tri);
            }
        }
        // Collect all triangle edges where point was inside circumcenter.
        edges = dnggetedges(edges, in, flags);
        // Flag edges that are non-unique.
        dngmarkedges(edges, flags);
        // Construct new triangles with unique edges.
        out = dngjoinedges(out, edges, pnt, flags);
        // Update triangle list.
        // FAST SHALLOW COPY - ORIGINAL POINTER LOST.
        set = out;
    }
    free(dummy);
    free(in.tab);
    free(edges.tab);

    return set;
}

static struct pntset
dngrandpnt(const int w, const int h, const int max, const int grid,
           const int border)
{
    struct pntset   set = dngnewpntset(max);
    int             i;

    for (i = set.cnt; i < set.max; i++)
    {
        const struct pnt pnt = {
            (float) (getrand() % (w - border) + border / 2),
            (float) (getrand() % (h - border) + border / 2)
        };
#if 0
        const struct pnt pnt = {
            (float) (getrand() % (w - border) + border),
            (float) (getrand() % (h - border) + border)
        };
#endif
        const struct pnt snapped = dngsnappnt(pnt, grid);

        set = dngaddpnt(set, snapped);
    }

    return set;
}

static int dngcmptri(const void* a, const void* b)
{
    const struct tri    edg1 = *(const struct tri *)a;
    const struct tri    edg2 = *(const struct tri *)b;

    if (dngedgelen(edg1) < dngedgelen(edg2)) {

        return 1;
    } else if (dngedgelen(edg1) > dngedgelen(edg2)) {

        return -1;
    }

    return 0;
}

static void
dngsort(const struct triset edges, const dirfunc dir)
{
    qsort(edges.tab, edges.cnt, sizeof(struct tri), dir);
}

static int
dngisconnd(const struct pnt a, const struct pnt b, const struct triset edges,
           const struct flags flags)
{
    struct pntset   todo = dngnewpntset(edges.max);
    struct pntset   done = dngnewpntset(edges.max);
    struct triset   reach = dngnewtriset(edges.max);
    int             conn = 0;
    int             i;

    todo = dngaddpnt(todo, a);
    do {
        const struct pnt    removed = todo.tab[--todo.cnt];

        done = dngaddpnt(done, removed);
        // Get reachable edges from current point.
        reach.cnt = 0;
        for (i = 0; i < edges.cnt; i++) {
            const struct tri    edg = edges.tab[i];

            if (!dngeqpnt(edg.p3, flags.one)
                && dngeqpnt(edg.p1, removed)) {
                reach = dngaddtri(reach, edg);
            }
        }
        // For all reachable edges
        for (i = 0; i < reach.cnt; i++) {
            // Was the destination reached?
            if (dngeqpnt(reach.tab[i].p2, b)) {
                conn = 1;

                break;
            }
            // Otherwise add point of reachable edge to todo list.
            if (!dngfindpnt(done, reach.tab[i].p2)) {
                todo = dngaddpnt(todo, reach.tab[i].p2);
            }
        }
    } while ((todo.cnt) && !conn);
    free(todo.tab);
    free(reach.tab);
    free(done.tab);

    return conn;
}

// Graph Theory Reverse Delete algorithm. Kruskal 1956.
static void
dngdelrev(struct triset edges, const int w, const int h,
          const struct flags flags)
{
    int i;

    dngsort(edges, dngcmptri);
    for (i = 0; i < edges.cnt; i++) {
        struct tri *edg = &edges.tab[i];

        edg->p3 = flags.one;
        if (dngextpnt(edg->p1, w, h)
            || dngextpnt(edg->p2, w, h)) {

            continue;
        }
        // Break the connection.
        // If two points are not connected in anyway then reconnect.
        // Due to edge duplicates the occasional loop will occur. The occasional loop benefits
        // the dungeon design else the explorer will get bored dead end after dead end.
        if (!dngisconnd(edg->p1, edg->p2, edges, flags)) {
            edg->p3 = flags.zero;
        }
    }

    return;
}

static void
dngchkdups(const struct triset edges, const struct flags flags)
{
    int i;
    int j;

    for (i = 0; i < edges.cnt; i++) {
        for (j = 0; j < edges.cnt; j++) {
            if (dngeqpnt(edges.tab[j].p3, flags.one)
               || dngeqpnt(edges.tab[i].p3, flags.one)) {

                continue;
            }
            if (dngreveqtri(edges.tab[i], edges.tab[j])) {
                edges.tab[j].p3 = flags.one;
            }
        }
    }

    return;
}

static void
dngmkroom(struct dng dng, const struct pnt where,
          const int w, const int h)
{
    struct room        *room = malloc(sizeof(struct room));
    struct room        *next;
    const int           x = where.x;
    const int           y = where.y;
    int                 ndx;
    int                 xx;
    int                 yy;
    int                 i;
    int                 j;

    if (!room) {
        fprintf(stderr, "failed to allocate room structure\n");

        exit(1);
    }
    room->prev = NULL;
    room->x = x;
    room->y = y;
    room->w = w;
    room->h = h;
    next = dng.roomlist;
    if (next) {
        next->prev = room;
    }
    room->next = next;
    dng.roomlist = room;
#if 0
    i = 2 * w + 2 * h;
    dng.nempty += i;
    for (i = -w; i < w; i++) {
        for (j = -h; j < h; j++) {
            xx = x + i;
            yy = y + j;
            ndx = yy * dng.w + xx;
            setbit(dng.bdng, ndx);
            dng.celltab[ndx] = ' ';
        }
    }
#endif
    i = w + h;
    dng.nempty += i;
    for (i = -w / 2; i < w / 2; i++) {
        for (j = -h / 2; j < h / 2; j++) {
            xx = x + i;
            yy = y + j;
            ndx = yy * dng.w + xx;
            setbit(dng.bdng, ndx);
            dng.celltab[ndx] = ' ';
        }
    }

    return;
}

static void
dngmkcorr(struct dng dng, const struct pnt a, const struct pnt b)
{
    const struct pnt    step = dngsubpnt(b, a);
    const struct pnt    delta = {
        step.x > 0.0f ? 1.0f : step.x < 0.0f ? -1.0f : 0.0f,
        step.y > 0.0f ? 1.0f : step.y < 0.0f ? -1.0f : 0.0f,
    };
    const int           sx = fabsf(step.x);
    const int           sy = fabsf(step.y);
    const int           dx = delta.x;
    const int           dy = delta.y;
    int                 x = a.x;
    int                 y = a.y;
    int                 ndx;
    int                 i;

    i = sx + sy;
    dng.nempty += i;
    for (i = 0; i < sx; i++) {
        x += dx;
        ndx = y * dng.w + x;
        setbit(dng.bdng, ndx);
        dng.celltab[ndx] = ' ';
    }
    for (i = 0; i < sy; i++) {
        y += dy;
        ndx = y * dng.w + x;
        setbit(dng.bdng, ndx);
        dng.celltab[ndx] = ' ';
    }

    return;
}

// ############################################# This is what a bone looks like
// #            ################################ when generated from an edge.
// #            #####################          #
// #  r o o m      c o r r i d o r    r o o m  #
// #     A      #####################    B     #
// #            ################################
// #############################################
static void
dngmkbone(const struct dng dng, const struct tri edg, const int w, const int h)
{
    dngmkroom(dng, edg.p1, w, h);
    dngmkroom(dng, edg.p2, w, h);
    dngmkcorr(dng, edg.p1, edg.p2);

    return;
}

// Carve all bones from solid dng.
static void
dngcarve(const struct dng dng, const struct triset edges,
         const struct flags flags, const int grid)
{
    int i;

    for (i = 0; i < edges.cnt; i++) {
        const struct tri    edg = edges.tab[i];

        if (!dngeqpnt(edg.p3, flags.one)) {
            // Min room size ensures room will not be smaller than min x min.
            //            const int   min = 2;
            const int   min = 4;
            //            const int   size = grid / 2 - min;
            const int   size = grid / 2;
            const int   w = min + (getrand() % size);
            const int   h = min + (getrand() % size);

            dngmkbone(dng, edg, w, h);
        }
    }

    return;
}

struct dng
dnggenlvl(const int w, const int h,
          const int grid, const int max)
{
    const struct dng    dng = dngnew(w, h);
    const struct flags  flags = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
    const int           border = 2 * grid;
    //    const int           border = grid;
    struct pntset       pset;
    struct triset       tset;
    struct triset       edges;

    pset = dngrandpnt(w, h, max, grid, border);
#if 0
    tset = dngdlntri(pset, w, h, 9 * max, flags);
    edges = dnggetedges(dngnewtriset(27 * max), tset, flags);
#endif
    tset = dngdlntri(pset, w, h, 17 * max, flags);
    edges = dnggetedges(dngnewtriset(51 * max), tset, flags);
    dngdelrev(edges, w, h, flags);
    dngchkdups(edges, flags);
    dngcarve(dng, edges, flags, grid);
    free(tset.tab);
    free(pset.tab);
    free(edges.tab);

    return dng;
}

void dngclose(const struct dng dng)
{
    int row;

    free(dng.bdng);
    free(dng.celltab);

    return;
}

void dngprint(const struct dng dng)
{
    int                 lim = dng.w * dng.h;
    int                 ndx;
    int                 col;

    while (ndx < lim) {
        for (col = 0; col < dng.w; col++) {
            printf("%c%s", dng.celltab[ndx], col == dng.w - 1 ? "\n" : "");
            ndx++;
        }
    }
    putchar('\n');

    return;
}

void
dnggen(int w, int h, int gridsize, int maxrooms)
{
    //    const struct dng dng = dnggen(80, 128, 8, 64);
    const struct dng dng = dnggenlvl(1,
                                     w,
                                     h,
                                     gridsize,
                                     maxrooms);
    dngprint(dng);
    dngclose(dng);

    return;
}

#if defined(TEST_DNG)
int
main(int argc, char *argv[])
{
    seedrand(time(NULL));
    dnggen(80,
           //              48 + (getrand() & 0x1f),
           48,
           8 + (getrand() & 0x07),
           //              4 + (getrand() & 0x07),
           16 + (getrand() & 0x1f));

    exit(0);
}
#endif

