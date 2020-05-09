#! /bin/sh

echo "building mtx-tutorial examples"
gcc -I../../usr/lib -o mtx-tutorial mtx-tutorial.c 2> build.log

