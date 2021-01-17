#! /bin/sh

. ./build.sh

CFLAGS+=" -m32"

gcc $CFLAGS $CCOPTFLAGS $CCWARNFLAGS $GNUWARNFLAGS $INCLUDES -O -o test test.c lib/hash.c lib/ht.c

