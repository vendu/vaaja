#! /bin/sh

clang -DTEST_ICE -g -Wall -Wextra -Wundef -I.. -I../../lib -O -o ice ice.c d20.c chr.c ../../lib/prng/randmt32.c
