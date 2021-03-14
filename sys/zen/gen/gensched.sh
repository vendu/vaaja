#! /bin/sh

SCHEDULERS="ule spt uni"
echo "generating nice table"
gcc -D__v0__ -I../../.. -I../../../usr/lib -O -o gennicetab gennicetab.c
chmod 755 gennicetab

gensched()
{
    for sched in ${SCHEDULERS}
    do
        dest=../sched/sched-${sched}.c
        head=${sched}-head.c
        tail=${sched}-tail.c
        echo "DEST: $dest"
        echo "HEAD: $head"
        echo "TAIL: $tail"
        cat /dev/null > $src
        if [ -f ${head} ]; then
            echo "storing $head for $sched into $dest"
            cat ${head} > $dest
            echo "concatenating nice tables for $sched into $dest"
            ./gennicetab ${sched} >> $dest
        else
            echo "storing nice tables for $sched into $dest"
            ./gennicetab ${sched} > $dest
        fi
        if [ -f ${tail} ]; then
            echo "storing $tail for $sched into $dest"
            cat ${tail} >> $dest
        fi
    done
}

gensched $@


