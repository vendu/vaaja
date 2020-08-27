#! /bin/sh

echo "building fs0 tests..."
gcc -D__zen__ -D__v0__ -DTEST_FS0 -O -I../.. -I../../usr/lib -o test-fs0 test-fs0.c
echo -n "executing fs0 tests..."
./test-fs0 && echo " SUCCESS"

