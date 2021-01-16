#! /bin/sh

LANGS="en_US.ISO-8859-1 en_US.UTF-8 fi_FI.ISO-8859-1 fi_FI.UTF-8"
gcc -O -o ctype ctype.c
for lc_ctype in ${LANGS}
do
    header=../lib/ctype-${lc_ctype}.h
    echo "storing locale for $lc_ctype into $header"
    LANG=${lc_ctype} ./ctype >> $header
done

