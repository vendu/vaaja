#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/zen/fs0.h>

#if defined(TEST_FS0)
int
main(int argc, char *argv[])
{
    printf("sizeof(struct zenfs0blktab) == %d\n", ZEN_FS0_BLK_TAB_SIZE);
    assert(sizeof(struct zenfs0blktab) == ZEN_FS0_BLK_TAB_SIZE);
    printf("sizeof(union zenfs0link) == %d\n", ZEN_FS0_LINK_SIZE);
    assert(sizeof(union zenfs0link) == ZEN_FS0_LINK_SIZE);
    printf("sizeof(struct zenfs0common) == %d\n", ZEN_FS0_COMMON_SIZE);
    assert(sizeof(struct zenfs0common) == ZEN_FS0_COMMON_SIZE);

    exit(0);
}
#endif

