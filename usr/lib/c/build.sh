#! /bin/sh

INCLUDES="-I. -I../../.. -I../../../usr/lib"
CCDEFFLAGS="-D_DEFAULT_SOURCE -D_POSIX_SOURCE"
CCWARNFLAGS="-D_FORTIFY_SOURCE=2 -Wall -Wundef -Wextra -Wpointer-arith -Wuninitialized -Winit-self -Wmissing-include-dirs -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wsuggest-attribute=pure -Wsuggest-attribute=const -Wsuggest-attribute=noreturn -Wsuggest-attribute=format -Wtrampolines -Wfloat-equal -Wshadow -Wstack-usage=16384 -Wunsafe-loop-optimizations -Wbad-function-cast -Wcast-align -Wwrite-strings -Wdate-time -Wjump-misses-init -Wlogical-op -Wstrict-prototypes -Wnormalized -Wopenmp-simd -Wpacked -Wredundant-decls -Wnested-externs -Winline -Wvector-operation-performance -Wvla -Wstack-protector -Woverlength-strings -Wno-designated-init -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute -Wno-implicit-fallthrough"

echo "building libc"
gcc ${INCLUDES} ${CCDEFFLAGS} ${CCWARNFLAGS} -fno-builtin -nostdinc -O -Wall -g -shared -fPIC -o libc.so *.c api/zen/*.c -pthread

