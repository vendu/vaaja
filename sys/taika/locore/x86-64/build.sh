#! /bin/sh

CC=gcc
AS=gcc
LD=ld

$CC -O -fno-omit-frame-pointer -Wall -Wextra -Wundef -mcmodel=kernel -m64 -ffreestanding -nostdlib -Wno-unused -DZEROMTX=1 -D_ZERO_SOURCE=1 -Wundef -DNEWSIGNAL=1 -DNEWMP=1 -DNEWTMR=1 -DFASTINTR=1 -DNEWCLI=1 -DKLUDGE=1 -DMEMTEST=0 -D_REENTRANT=1 -DQEMU=1 -g -DKERN32BIT=1 -D__KERNEL__=1 -D__MTKERNEL__ -march=x86-64 -fno-builtin -fno-asynchronous-unwind-tables -ffreestanding -static -Wall -I../../../usr/lib -I../../../usr/lib -I../../.. -I../../../usr/lib/c -g -O -fno-omit-frame-pointer -fno-stack-protector -c *.c ../../*.c ../x86/*.c ../../proc/*.c ../../mem/*.c ../../io/*.c ../../io/drv/chr/cons.c ../../io/drv/pc/*.c ../../io/net/*.c ../i387/math.c ../../../usr/contrib/Zeno/plasma24grid.c

$AS -O -fno-omit-frame-pointer -Wall -Wextra -Wundef -mcmodel=kernel -m64 -ffreestanding -nostdlib -Wno-unused -Wundef -Wextra -DZEROMTX=1 -D_ZERO_SOURCE=1 -DNEWSIGNAL=1 -DNEWMP=1 -DNEWTMR=1 -DFASTINTR=1 -DNEWCLI=1 -DQEMU=1 -g -D__KERNEL__=1 -D__MTKERNEL__ -march=pentium-mmx -fno-builtin -ffreestanding -static -Wall -I../../.. -I../../../usr/lib -I../../../usr/lib/c -g -fno-stack-protector -msse -c ../x86/boot.S *.S

$LD -nostdlib -nodefaultlibs -g -static -T kern.lds -Map zero.sym -o zero *.o


