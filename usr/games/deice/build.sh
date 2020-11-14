#! /bin/sh

CCWARNINGS="-Wextra -Werror -Wundef"

clang ${CCWARNINGS} -DD20_SRAND_TIME=1 -DD20_RANDMT32=1 -I.. -I../../lib -g -O -o deice deice.c ../../lib/prng/randmt32.c

