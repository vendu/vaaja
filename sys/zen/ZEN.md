# vaaja source code roadmap
- [vaaja on github](https://github.com/vendu/vaaja/)

# data
- deq           - dual-end queue with minimal use of atomic synchronization
- htab          - hash table with cache-friendly table-chains for colllisions
- rbt           - red-black tree; memory/resource allocator book-keeping

# zen
- [zen on github](https://github.com/vendu/vaaja/blob/master/sys/zen)

## the zen kernel
- zen
- usr
- sys
- parm

## tao; high-level scheduler
[tao implementation](https://github.com/vendu/vaaja/blob/master/sys/zen/sched/)

### ule         - adopted freebsd scheduler

### bvt         - borrowed virtual time scheduler

## vfs
- node
  - reg
  - chr
  - blk
  - dir
  - link
  - sock
  - mq

## fs

### fs0

### fat32

### iso9660

## dev

### chr
- null
- zero
- tty
- kbd
- ptr
- cam
- pkt
- tap

### blk
- tape
- disk
- opt
- ssd
- net

### ev
- kbd
- ptr
- 

## hw - hardware interface
- unit
- trap
- sig
- tmr
- page
- bus
- io

## mem
- vm
- shm
- blk
- run
- slab
- big
- buf
- map
  - dev
  - nod