#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <v0/v0.h>

int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    V0_PRINT_SANITY();
    printf("hello world\n");

    exit(0);
}

