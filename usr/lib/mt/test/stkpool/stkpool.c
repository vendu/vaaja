#include <stdio.h>
#include <stdlib.h>
#include <mach/asm.h>
#define STKPOOL_LOCKFREE 1
#include <mt/stkpool.h>
#include <mt/mtx.h>
#include <pthread.h>

#define STKPOOL_THREADS         32
#define STKPOOL_THREAD_POINTERS 2097152

static struct lkpool  stkpool[STKPOOL_THREADS];
static pthread_t      thrtab[STKPOOL_THREADS];
static void         **thrptrtab[STKPOOL_THREADS];
static zerofmtx       thrcntmtx;
static volatile long  thrcnt;

static __inline__ long
thrgetid(void)
{
    long id;

    fmtxlk(&thrcntmtx);
    id = m_fetchadd(&thrcnt, 1);
    fmtxunlk(&thrcntmtx);

    return id;

}

void *
thrstart(void *arg)
{
    struct lkpool  *pool = arg;
    void          **pptr;
    long            id;
    long            ndx;
    void            *ptr;

    id = thrgetid();
    fprintf(stderr, "thrstart(%lx)\n", id);
    lkpoolinit(pool);
    pptr = thrptrtab[id];
    for (ndx = 0 ; ndx < STKPOOL_THREAD_POINTERS ; ndx++) {
        ptr = &pptr[ndx];
        pptr[ndx] = ptr;
    }
    for (ndx = 0 ; ndx < STKPOOL_THREAD_POINTERS ; ndx++) {
        if (!stkpoolpushptr(pool, pptr[ndx])) {
            fprintf(stderr, "stkpoolpushptr() failed\n");

            exit(1);
        }
    }
    for (ndx = 0 ; ndx < STKPOOL_THREAD_POINTERS ; ndx++) {
        if (!stkpoolpop(pool)) {
            fprintf(stderr, "stkpoolpop() failed\n");

            exit(1);
        }
    }

    return arg;
}

int
main(int argc, char *argv[])
{
    long           ndx;
    int            res;
    void          *ptr;

    for (ndx = 0 ; ndx < STKPOOL_THREADS ; ndx++) {
        thrptrtab[ndx] = malloc(STKPOOL_THREAD_POINTERS * sizeof(void *));
        if (!thrptrtab[ndx]) {

            exit(1);
        }
    }
    for (ndx = 0 ; ndx < STKPOOL_THREADS ; ndx++) {
        res = pthread_create(&thrtab[ndx], NULL, thrstart, &stkpool[ndx]);
        if (res) {
            fprintf(stderr, "pthread_create() FAILED\n");

            exit(1);
        }
    }
    for (ndx = 0 ; ndx < STKPOOL_THREADS ; ndx++) {
        res = pthread_join(thrtab[ndx], &ptr);
        if (res) {
            fprintf(stderr, "pthread_join() FAILED\n");

            exit(1);
        }
    }

    exit(0);
}

