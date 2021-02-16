#! /bin/sh

SCHEDULERS="ule spt"
echo "generating nice table"
gcc -D__v0__ -I../../.. -I../../../usr/lib -O -o gennicetab gennicetab.c
chmod 755 gennicetab
for sched in ${SCHEDULERS}
do
    source=../sched/sched-${sched}.c
    head=${sched}-head.c
    tail=${sched}-tail.c
    if [ -f ${head} ]; then
        echo "storing $head for $sched into $source"
        cat ${head} > $source
    fi
    if [ ${sched} = "ule" ]; then
        echo "storing nice tables for $sched into $source"
        ./gennicetab ${sched} >> $source
    fi
    if [ -f ${tail} ]; then
        echo "storing $tail for $sched into $source"
        cat ${tail} >> $source
    fi
done

