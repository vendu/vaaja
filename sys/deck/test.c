#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <deck/deck.h>

int
main(C_UNUSED int argc, C_UNUSED char *argv[])
{
    DECK_PRINT_SANITY();
    printf("hello world\n");

    exit(0);
}

