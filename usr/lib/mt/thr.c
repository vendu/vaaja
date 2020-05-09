#include <time.h>
#include <sys/time.h>
#include <mach/asm.h>
#include <mt/thr.h>
#include <mt/cond.h>
#include <zero/cdefs.h>
#include <zero/time.h>
#include <vnd/prof.h>

#define DEQLIST_TYPE            mtthr
#define DEQLIST_QTYPE           mtthrqueue
#define DEQLIST_RM_COND(thr)    ((thr)->sleep)
#define DEQLIST_QUEUE(thr)      ((thr)->sleep = MT_THR_ASLEEP)
#define DEQLIST_DEQUEUE(thr)    ((thr)->sleep = MT_THR_AWAKE)
#include <mt/deq.h>

static mtthrqueue               g_thrsleepqueue;
THREADLOCAL mtthr               t_thrself;

void
mtwaitthr1(mtthrqueue *queue)
{
    mtthr      *thr = &t_thrself;

    if (!queue) {
        queue = &g_thrsleepqueue;
    }
    deqlistpush(queue, thr);
    while (m_atomread(&thr->sleep)) {
        mtyieldthr();
    }

    return;
}

long
mtsleepthr2(mtthrqueue *queue, const struct timespec *absts)
{
    mtthr              *thr = &t_thrself;
#if defined(USECLOCKNANOSLEEP)
    struct timespec     tsrem;
#else
    struct timeval      tvout;
    struct timeval      tvcur;
#endif

    if (!queue) {
        queue = &g_thrsleepqueue;
    }
    deqlistpush(queue, thr);
#if defined(USECLOCKNANOSLEEP)
    while (clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, absts, &tsrem)) {
        if (errno == EINTR) {

            continue;
        } else {
            if (m_atomread(&thr->sleep)) {
                deqlistrm(queue, thr);
            }

            return -1;
        }
    }
    if (m_atomread(&thr->sleep)) {
        deqlistrm(queue, thr);
    }
#else
    gettimeofday(&tvcur, NULL);
    tstotv(absts, &tvout);
    do {
        if (tvcmp(&tvout, &tvcur) > 0) {
            if (m_atomread(&thr->sleep)) {
                deqlistrm(queue, thr);
            }

            return 0;
        }
        mtyieldthr();
        gettimeofday(&tvcur, NULL);
    } while (1);
#endif

    return 0;
}

mtthr *
thrwake1(mtthrqueue *queue)
{
    mtthr      *thr = NULL;

    if (!queue) {
        queue = &g_thrsleepqueue;
    }
    deqlistdequeue(queue, &thr);

    return thr;
}

void
thrwakeall(mtthrqueue *queue)
{
    mtthr      *thr = NULL;

    if (!queue) {
        queue = &g_thrsleepqueue;
    }
    do {
        deqlistdequeue(queue, &thr);
    } while (thr);

    return;
}

