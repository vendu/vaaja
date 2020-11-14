#! /bin/sh

INCLUDES="-I../.. -I../../usr/lib -I../../usr/lib/c"
CFLAGS+="-std=c99 -Werror -Wundef -D__voima__ -D__zen__ -g -nostdinc -nostdlib --freestanding"
CCOPTFLAGS="-O"
CCWARNFLAGS="-D_FORTIFY_SOURCE=2 -Wall -Wundef -Wpointer-arith -Wuninitialized -Winit-self -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wstrict-overflow -Wfloat-equal -Wshadow -Wbad-function-cast -Wcast-align -Wwrite-strings -Wdate-time -Waggregate-return -Wstrict-prototypes -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Winline -Wvla -Wstack-protector -Woverlength-strings -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"
#CCWARNFLAGS="-D_FORTIFY_SOURCE=2 -Wall -Wundef -Wpointer-arith -Wuninitialized -Wlong-long -Winit-self -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wstrict-overflow -Wfloat-equal -Wshadow -Wbad-function-cast -Wcast-align -Wwrite-strings -Wdate-time -Waggregate-return -Wstrict-prototypes -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Winline -Wvla -Wstack-protector -Woverlength-strings -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"
GNUWARNFLAGS="-Wtrampolines -Wstack-usage=16384 -Wunsafe-loop-optimizations -Wc99-c11-compat -Wjump-misses-init -Wlogical-op -Wnormalized -Wno-designated-init -Wsuggest-attribute=const -Wsuggest-attribute=pure -Wsuggest-attribute=noreturn -Wsuggest-attribute=format"
# generate nice-to-priority lookup table
#cd gen
#./gennice.sh
#cd ..
# compile the system
#gcc $CFLAGS $CCOPTFLAGS $CCWARNFLAGS $GNUWARNFLAGS -I../.. -I../../usr/lib -o zen *.c sched/*.c net/*.c ../../usr/lib/zen/fastudiv16.c -pthread
gcc $CFLAGS $CCOPTFLAGS $CCWARNFLAGS $INCLUDES -o zen *.c sched/*.c net/*.c ../../usr/lib/zen/fastudiv16.c -pthread

