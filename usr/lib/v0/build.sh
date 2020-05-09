#! /bin/sh

./init_headers.sh
gcc -O -I.. -I../../usr/lib -o vm *.c

