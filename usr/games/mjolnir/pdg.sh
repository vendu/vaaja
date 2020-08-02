#! /bin/sh

gcc -DDEBUG_PDG -DTEST_PDG -I.. -I../../lib -g -Wall -Wextra -O -o pdg pdg.c ../../lib/prng/randmt32.c

