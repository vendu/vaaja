#! /bin/sh

CC=gcc
INCLUDES="-I.. -I../../usr/lib"
CFLAGS="-std=c99"
CCOPTFLAGS="-O"
CCWARNFLAGS=" -D_FORTIFY_SOURCE=2 -Wall -Wundef -Wpointer-arith -Wuninitialized -Winit-self -Wparentheses -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wstrict-overflow -Wfloat-equal -Wshadow -Wbad-function-cast -Wcast-align -Wwrite-strings -Wdate-time -Waggregate-return -Wstrict-prototypes -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Winline -Wvla -Wstack-protector -Woverlength-strings -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"
#CCWARNFLAGS=" -D_FORTIFY_SOURCE=2 -Wall -Wundef -Wpointer-arith -Wuninitialized -Wlong-long -Winit-self -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wstrict-overflow -Wfloat-equal -Wshadow -Wbad-function-cast -Wcast-align -Wwrite-strings -Wdate-time -Waggregate-return -Wstrict-prototypes -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Winline -Wvla -Wstack-protector -Woverlength-strings -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"
GNUWARNFLAGS=" -Wtrampolines -Wstack-usage=16384 -Wunsafe-loop-optimizations -Wc99-c11-compat -Wjump-misses-init -Wlogical-op -Wnormalized -Wno-designated-init -Wsuggest-attribute=const -Wsuggest-attribute=pure -Wsuggest-attribute=noreturn -Wsuggest-attribute=format"

${CC} ${INCLUDES} ${CCOPTFLAGS} ${CCWARNFLAGS} ${GNUWARNFLAGS} -fPIC -shared -o libv0.so *.c

