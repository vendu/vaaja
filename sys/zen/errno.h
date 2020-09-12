#ifndef __SYS_ZEN_ERRNO_H__
#define __SYS_ZEN_ERRNO_H__

#define ENOSYS                  1  // function not implemented
#define EINTR                   2  // interrupted system call
#define ENOMEM                  3  // out of memory
#define EAGAIN                  4  // try again
//#if defined(_BSD_SOURCE)
#define EWOULDBLOCK             EAGAIN // synonymous with EAGAIN
//#endif
#define EACCES                  5  // permission denied
#define EIO                     6  // I/O error
#define ELOOP                   7  // too many symbolic links encountered
#define ENAMETOOLONG            8  // file name too long
#define ENOENT                  9  // no such file or directory
#define ENOTDIR                 10 // not a directory
#define EOVERFLOW               11 // value too large for defined data type
#define EBADF        			12 // bad file number
#define EFBIG        			13 // file too large
#define EINVAL       			14 // invalid argument
#define ENODEV       			15 // no such device
#define ENOSPC       			16 // no space left on device
#define ESPIPE       			17 // illegal seek
#define EILSEQ       			18 // illegal byte sequence
#define EDOM         			19 // math argument out of domain of function
#define ERANGE       			20 // math result not representable
#define EMSGSIZE     			21 // message too long
#define ETIMEDOUT    			22 // connection timed out
#define ENOLCK       			23 // no record locks available
#define EDEADLK      			24 // resource deadlock would occur
#define EDEADLOCK               EDEADLK // synonymous with EDEADLK
/* new errno-values */
#define ECANCELED    			25 // operation canceled
#define EOWNERDEAD   			26 // owner died
#define ERFKILL      			27 // operation not possible due to RF-kill
#define EHWPOISON    			28 // memory page has hardware error
#define ENOTEMPTY    			29 // directory not empty
#define ENOMSG       			30 // no message of desired type
#define EIDRM        			31 // identifier removed
#define ECHRNG       			32 // channel number out of range
#define EL2NSYNC     			33 // level 2 not synchronised
#define EL3HLT       			34 // level 3 halted
#define EL3RST       			35 // level 3 reset
#define ELNRNG       			36 // link number out of range
#define EUNATCH      			37 // protocol driver not attached
#define ENOCSI       			38 // no CSI structure available
#define EL2HLT       			39 // level 2 halted
#define EBADE        			40 // invalid exchange
#define EBADR        			41 // invalid request descriptor
#define EXFULL       			42 // exchange full
#define ENOANO       			43 // no anode
#define EBADRQC      			44 // invalid request code
#define EBADSLT      			45 // invalid slot
#define ESRCH        			46 // no such process
#define EISDIR       			47 // is a directory
#define ENOTUNIQ     			48 // name not unique on network
#define EBADFD       			49 // file descriptor in bad state
#define EPERM        			50 // operation not permitted
#define ENXIO        			51 // no such device or address
#define E2BIG        			52 // argument list too long
#define ENOEXEC      			53 // exec format error
#define ECHILD       			54 // no child processes
#define EFAULT       			55 // bad address
#define ENOTBLK      			56 // block device required
#define EBUSY        			57 // device or resource busy
#define EEXIST       			58 // file exists
#define EXDEV        			59 // cross-device link
#define ENFILE       			60 // file table overflow
#define EMFILE       			61 // too many open files
#define ENOTTY       			62 // not a typewriter/teletype
#define ETXTBSY      			63 // text file busy
#define EROFS        			64 // read-only filesystem
#define EMLINK       			65 // too many links
#define EPIPE        			66 // broken pipe

#define NERRNO       			67 // 1 bigger than any errno value */

#if defined(__zen__)
//#define __seterrno(e) (e)
#endif

#endif /* __SYS_ZEN_ERRNO_H__ */

