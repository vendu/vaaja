# 0x0C Commandments for Code Optimization
  ---

### Copyright (C) 2018 Tuomo Petteri Venäläinen

## 0x01	Know Your Data.
	- organize data on few cachelines with related items close to each other
	- utilize today's massive cache prefetches
	- align your data properly - have values on boundaries of their size
	- use proper access sizes - bytes might be faster than shifting words
## 0x02	Know Your Memory.
	- virtual memory works on pages
	- caches operate in lines and/or strides
	- aligned access is the way to go
	- keep related data close and compact
	- align structures on cacheline boundaries
	- utilize cache-coloring to hopefully reduce cacheline collisions -
	  randomize cacheline addresses of accessed data
	- leverage massive cache prefetches
		- e.g. hash table chains may be implemented as flat tables
			- no more pointer chase/dependency/fetch for every item
			- tables are iterated over from cache
## 0x03	Buffer.
	- I/O works best with blocks as opposed to raw/character-based I/O
	  - reduces number of system calls and device/file-system operations
	  - operating system kernels buffer I/O blocks for better chances of
	    reuse as well as read- and write-bursts with less seeking on disk
	    and such devices
## 0x04	Know Your Hardware.
	- avoid division and remainder/modulus calculations
	- avoid data dependencies; when you access a variable/register, try not
	  to use the new value right off
	  - example: use ptr[0] = 0; ptr[1] = 0; over *ptr++ = 0; *ptr = 0;
	    - avoid data dependency on new pointer value for better pipelining
## 0x05	Unroll.
	- more linear pipeline execution with fewer branches taken
	- sometimes Duff's devices are in order
	- pay attention to memory access patterns
		- process cachelines
		- optionally prefetch the next cacheline
## 0x06	Inline.
	- do NOT overdo it; consider instruction cache locality/small functions
	- reduce number of function calls to avoid related overhead
## 0x07	Know Your Threads.
	- synchronize access to critical data structures
	- utilize event/message queues for IPC
	  - use shared memory for localhost connections
	- use thread-local storage (TLS) for buffering per-thread data/buffers
## 0x08	Synchronize.
	- do NOT put more than one lock object per cacheline where feasible
	- sometimes it's still good to use lock-bitmaps
	- avoid atomic access in tight loops - poll volatiles in RAM instead of
	  atomic operations
	- try to use lock- or wait-free data structures where possible
## 0x09	Know Your System.
	- Lions Book
	- The Design and Implementation of the UNIX Operating System
	- Advanced Programming in the UNIX Environment
	- The Design and Implementation of the FreeBSD Operating System
## 0x0a	Do Your Research.
	- use your favorite search engine(s) to find lots of documents/papers
	  on the Internet
## 0x0b	Hack!
	[velho](https://github.com/vendu/velho/)
	[zero](https://github.com/vendu/OS-Zero/)
	[resurrection](https://github.com/vendu/Resurrection)
	[vendetta](https://github.com/vendu/vendetta-emacs-theme)
## 0x0c	Explore.

