#include <mjolnir/conf.h>
#include <mjolnir/mjol.h>

#if (MJOLNIR_VGA_TEXT) && (__ZERO__)

static struct evkbdqchar *kbdevq;

int
mjolgetch(void)
{
    int ch;

    if (!kbdevq) {
        kbdevq = evreg(EVKEYDOWNMASK, EVKEYASCII);
        if (!kbdevq) {
            fprintf(stderr, "cannot allocate keyboard event queue\n");

            exit(1);
        }
    }
    ch = evdeqkbdchar(kbdevq);
    while (!ch) {
        /* wait for signal; Zero delivers SIGIO on keyboard (and mouse) input */
        pause();
        ch = evdeqkbdchar(kbdevq);
    }

    return ch;
}

#endif /* (MJOLNIR_VGA_TEXT) && (__ZERO__) */

