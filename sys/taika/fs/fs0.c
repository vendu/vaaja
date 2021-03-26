#if defined(TEST_FS0)
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#endif
#include <sys/zen/fs0.h>

#if defined(TEST_FS0)
int
main(int argc, char *argv[])
{
    fprintf(stderr, "checking fs0 data structures\n");
    fprintf(stderr, "sizeof(struct zenfs0blktab) should be 64 bytes - ");
    assert(sizeof(struct zenfs0blktab) == 64);
    fprintf(stderr, "PASSED\n");
    fprintf(stderr, "sizeof(union zenfs0link) should be 64 bytes - ");
    assert(sizeof(union zenfs0link) == 64);
    fprintf(stderr, "PASSED\n");
    fprintf(stderr, "sizeof(struct zenfs0common) should be 128 bytes - ");
    assert(sizeof(struct zenfs0common) == 128);
    fprintf(stderr, "PASSED\n");

    exit(0);
}
#endif

