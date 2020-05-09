#! /bin/sh

echo "bulding test-string"
gcc -I../.. -O -Wall -g -o test-string test-string.c ../string.c ../strings.c
echo "executing test-string"
./test-string

