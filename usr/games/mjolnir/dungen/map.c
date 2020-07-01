#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "map.h"

typedef int                 (*const dirfunc)(const void *, const void *);

#define mapeqpnt(a, b)      ((a).x == (b).x && (a).y == (b).y)
#define mapextpnt(p, w, h)  ((p).x < 0 || (p).y < 0 || (p).x >= w || (p).y >= h)
#define maptruncf(x)        (float)((int)x - (x < (int)(x)))
#define mapsnappnt(p, g)                                                \
    (struct pnt){ maptruncf((p).x / (g)) * (g),  maptruncf((p).y / (g)) * (g) }
#define mapsubpnt(a, b)     (struct pnt){ (a).x - (b).x, (a).y - (b).y }
#define mapmagpnt(p)        sqrtf((p).x * (p).x + (p).y * (p).y)
#define mapnewpntset(n)                                                 \
    (struct pntset){ malloc((n) * sizeof(struct pnt)), 0, (n) }
#define mapeqtri(a, b)      (mapeqpnt((a).p1, (b).p1)                   \
                             && mapeqpnt((a).p2, (b).p2))
#define mapreveqtri(a, b)   (mapeqpnt((a).p1, (b).p2)                   \
                             && mapeqpnt((a).p2, (b).p1))
#define mapalndtri(a, b)    (mapeqtri(a, b) || mapreveqtri(a, b))
#define mapedgelen(e)       mapmagpnt(mapsubpnt((e).p2, (e).p1))
#define mapnewtriset(n)                                                 \
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
mapfail(const char *const message, ...)
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
mapfill(char **blk, const int w, const int h, const int val)
{
    int row;
    int col;
    
    for (row = 0; row < h; row++) {
        for (col = 0; col < w; col++) {
            blk[row][col] = val;
        }
    }
    
    return blk;
}

static char **
mapnewblk(const int w, const int h, const int val)
{
    char  **blk = malloc(h * sizeof(char *));
    int     row;
    
    for (row = 0; row < h; row++) {
        blk[row] = malloc(w * sizeof(char));
    }
    mapfill(blk, w, h, val);

    return blk;
}

static struct map
mapnew(const int w, const int h)
{
    struct map  map;

    map.wall = mapnewblk(w, h, '#');
    map.w = w;
    map.h = h;
    
    return map;
}

static struct pntset
mapaddpnt(struct pntset set, const struct pnt pnt)
{
    if (set.cnt != set.max) {
        set.tab[set.cnt++] = pnt;
    } else {
        mapfail("pntset count limitation reached\n");
    }
    
    return set;
}

static int
mapfindpnt(const struct pntset set, const struct pnt pnt)
{
    int i;
    
    for (i = 0; i < set.cnt; i++) {
        if (mapeqpnt(set.tab[i], pnt)) {
            
            return 1;
        }
    }

    return 0;
}

static struct triset
mapaddtri(struct triset set, const struct tri tri)
{
    if (set.cnt != set.max) {
        set.tab[set.cnt++] = tri;
    } else {
        mapfail("triset count limitation reached\n");
    }
        
    return set;
}

// Flags alligned edges.
static void
mapmarkedges(struct triset edges, const struct flags flags)
{
    int i;
    int j;
    
    for (i = 0; i < edges.cnt; i++) {
        const struct tri    edg = edges.tab[i];
        
        for (j = 0; j < edges.cnt; j++) {
            if (i != j) {
                const struct tri    tri = edges.tab[j];
                
                if (mapalndtri(edg, tri)) {
                    edges.tab[j].p3 = flags.one;
                }
            }
        }
    }

    return;
}

// Creates new triangles from unique edges and appends to tset.
static struct triset
mapjoinedges(struct triset set, const struct triset edges, const struct pnt pnt,
             const struct flags flags)
{
    int i;

    for (i = 0; i < edges.cnt; i++) {
        const struct tri    edg = edges.tab[i];

        if (mapeqpnt(edg.p3, flags.zero)) {
            const struct tri    tri = { edg.p1, edg.p2, pnt };

            set = mapaddtri(set, tri);
        }
    }

    return set;
}

static int mapincircntr(const struct tri tri, const struct pnt pnt)
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
mapgetedges(struct triset edges, const struct triset in,
            const struct flags flags)
{
    int i;

    for (i = 0; i < in.cnt; i++) {
        const struct tri    tri = in.tab[i];
        const struct tri    ab = { tri.p1, tri.p2, flags.zero };
        const struct tri    bc = { tri.p2, tri.p3, flags.zero };
        const struct tri    ca = { tri.p3, tri.p1, flags.zero };

        edges = mapaddtri(edges, ab);
        edges = mapaddtri(edges, bc);
        edges = mapaddtri(edges, ca);
    }

    return edges;
}

static struct triset
mapdlntri(const struct pntset pset, const int w, const int h, const int max,
          const struct flags flags)
{
    struct triset       set = mapnewtriset(max);
    struct triset       in = mapnewtriset(max);
    struct triset       out = mapnewtriset(max);
    struct triset       edges = mapnewtriset(max);
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

    set = mapaddtri(set, super);
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

            if (mapincircntr(tri, pnt)) {
                // Get triangles where point lies inside their circumcenter...
                in = mapaddtri(in, tri);
            } else {
                // Get triangles where point lies outside of their circumcenter.
                out = mapaddtri(out, tri);
            }
        }
        // Collect all triangle edges where point was inside circumcenter.
        edges = mapgetedges(edges, in, flags);
        // Flag edges that are non-unique.
        mapmarkedges(edges, flags);
        // Construct new triangles with unique edges.
        out = mapjoinedges(out, edges, pnt, flags);
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
maprandpnt(const int w, const int h, const int max, const int grid,
           const int border)
{
    struct pntset   set = mapnewpntset(max);
    int             i;
    
    for (i = set.cnt; i < set.max; i++)
    {
        const struct pnt pnt = {
            (float) (rand() % (w - border) + border / 2),
            (float) (rand() % (h - border) + border / 2)
        };
        const struct pnt snapped = mapsnappnt(pnt, grid);

        set = mapaddpnt(set, snapped);
    }

    return set;
}

static int mapcmptri(const void* a, const void* b)
{
    const struct tri    edg1 = *(const struct tri *)a;
    const struct tri    edg2 = *(const struct tri *)b;

    if (mapedgelen(edg1) < mapedgelen(edg2)) {

        return 1;
    } else if (mapedgelen(edg1) > mapedgelen(edg2)) {

        return -1;
    }

    return 0;
}

static void
mapsort(const struct triset edges, const dirfunc dir)
{
    qsort(edges.tab, edges.cnt, sizeof(struct tri), dir);
}

static int
mapisconnd(const struct pnt a, const struct pnt b, const struct triset edges,
           const struct flags flags)
{
    struct pntset   todo = mapnewpntset(edges.max);
    struct pntset   done = mapnewpntset(edges.max);
    struct triset   reach = mapnewtriset(edges.max);
    int             conn = 0;
    int             i;
    
    todo = mapaddpnt(todo, a);
    do {
        const struct pnt    removed = todo.tab[--todo.cnt];
        
        done = mapaddpnt(done, removed);
        // Get reachable edges from current point.
        reach.cnt = 0;
        for (i = 0; i < edges.cnt; i++) {
            const struct tri    edg = edges.tab[i];
            
            if (!mapeqpnt(edg.p3, flags.one)
                && mapeqpnt(edg.p1, removed)) {
                reach = mapaddtri(reach, edg);
            }
        }
        // For all reachable edges
        for (i = 0; i < reach.cnt; i++) {
            // Was the destination reached?
            if (mapeqpnt(reach.tab[i].p2, b)) {
                conn = 1;

                break;
            }
            // Otherwise add point of reachable edge to todo list.
            if (!mapfindpnt(done, reach.tab[i].p2)) {
                todo = mapaddpnt(todo, reach.tab[i].p2);
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
mapdelrev(struct triset edges, const int w, const int h,
          const struct flags flags)
{
    int i;
    
    mapsort(edges, mapcmptri);
    for (i = 0; i < edges.cnt; i++) {
        struct tri *edg = &edges.tab[i];
        
        edg->p3 = flags.one;
        if (mapextpnt(edg->p1, w, h)
            || mapextpnt(edg->p2, w, h)) {

            continue;
        }
        // Break the connection.
        // If two points are not connected in anyway then reconnect.
        // Due to edge duplicates the occasional loop will occur. The occasional loop benefits
        // the dungeon design else the explorer will get bored dead end after dead end.
        if (!mapisconnd(edg->p1, edg->p2, edges, flags)) {
            edg->p3 = flags.zero;
        }
    }

    return;
}

static void
mapchkdups(const struct triset edges, const struct flags flags)
{
    int i;
    int j;

    for (i = 0; i < edges.cnt; i++) {
        for (j = 0; j < edges.cnt; j++) {
            if (mapeqpnt(edges.tab[j].p3, flags.one)
               || mapeqpnt(edges.tab[i].p3, flags.one)) {
                
                continue;
            }
            if (mapreveqtri(edges.tab[i], edges.tab[j])) {
                edges.tab[j].p3 = flags.one;
            }
        }
    }

    return;
}

static void
mapmkroom(const struct map map, const struct pnt where,
          const int w, const int h)
{
    const int   x = where.x;
    const int   y = where.y;
    int         xx;
    int         yy;
    int         i;
    int         j;

    for (i = -w; i < w; i++) {
        for (j = -h; j < h; j++) {
            xx = x + i;
            yy = y + j;

            map.wall[yy][xx] = ' ';
        }
    }

    return;
}

static void
mapmkcorr(const struct map map, const struct pnt a, const struct pnt b)
{
    const struct pnt    step = mapsubpnt(b, a);
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
    int                 i;
    
    for (i = 0; i < sx; i++) {
        x += dx;
        map.wall[y][x] = ' ';
    }
    for (i = 0; i < sy; i++) {
        y += dy;
        map.wall[y][x] = ' ';
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
mapmkbone(const struct map map, const struct tri edg, const int w, const int h)
{
    mapmkroom(map, edg.p1, w, h);
    mapmkroom(map, edg.p2, w, h);
    mapmkcorr(map, edg.p1, edg.p2);

    return;
}

// Carve all bones from solid map.
static void
mapcarve(const struct map map, const struct triset edges,
         const struct flags flags, const int grid)
{
    int i;
    
    for (i = 0; i < edges.cnt; i++) {
        const struct tri    edg = edges.tab[i];
        
        if (!mapeqpnt(edg.p3, flags.one)) {
            // Min room size ensures room will not be smaller than min x min.
            //            const int min = 2;
            const int   min = 2;
            const int   size = grid / 2 - min;
            const int   w = min + rand() % size;
            const int   h = min + rand() % size;
            
            mapmkbone(map, edg, w, h);
        }
    }

    return;
}

struct map
mapgen(const int w, const int h, const int grid, const int max)
{
    const struct map    map = mapnew(w, h);
    const struct flags  flags = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
    const int           border = 2 * grid;
    struct pntset       pset;
    struct triset       tset;
    struct triset       edges;
    
    srand(time(0));
    pset = maprandpnt(w, h, max, grid, border);
    tset = mapdlntri(pset, w, h, 9 * max, flags);
    edges = mapgetedges(mapnewtriset(27 * max), tset, flags);
    mapdelrev(edges, w, h, flags);
    mapchkdups(edges, flags);
    mapcarve(map, edges, flags, grid);
    free(tset.tab);
    free(pset.tab);
    free(edges.tab);

    return map;
}

void mapclose(const struct map map)
{
    int row;
    
    for (row = 0; row < map.h; row++) {
        free(map.wall[row]);
    }
    free(map.wall);

    return;
}

void mapprint(const struct map map)
{
    int row;
    int col;
    
    for (row = 0; row < map.h; row++) {
        for (col = 0; col < map.w; col++) {
            printf("%c%s", map.wall[row][col], col == map.w - 1 ? "\n" : "");
        }
    }
    putchar('\n');

    return;
}

