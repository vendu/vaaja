#include <stdlib.h>
#include <assert.h>
#include <zen/fs0.h>

int
main(int argc, char *argv[])
{
    assert(sizeof(struct zenfs0blktab) == ZEN_FS0_BLK_TAB_SIZE);
    assert(sizeof(union zenfs0link) == ZEN_FS0_LINK_SIZE);
    assert(sizeof(struct zenfs0common) == ZEN_FS0_COMMON_SIZE);

    exit(0);
}

