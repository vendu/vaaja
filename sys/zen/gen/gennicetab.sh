#! /bin/sh

SCHEDULERS="ule spt"
echo "generating nice table"
gcc -D__v0__ -I../../.. -I../../../usr/lib -O -o nicetab nicetab.c
for sched in ${SCHEDULERS}
do
    source=../sched/sched-${sched}.c
    echo "storing nice tables for $sched into $source"
    ./nicetab ${sched} > $source
done

