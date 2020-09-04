#ifndef __VOIMA_BSP_H__
#define __VOIMA_BSP_H__

/*
 * BSP-unit I/O-instruction format  - 2 16-bit parcels
 *         ---------------------------------
 * field - | op | unit | src | cmd | info  |
 *         ---------------------------------
 * bits -  |  5 |  3   |  4  |  4  |  16   |
 *         ---------------------------------
 *
 * IOP
 * ---
 * - cmd consists of one or more of the following bits
 *   - CLRA                     clear (revoke) all permissions
 *   - SETA                     set (invoke) all permissions
 *   - SETRD                    set I/O read-permission bit (allow access)
 *   - CLRRD                    clear I/O read-permission bit (deny access)
 *   - SETWR                    set I/O write-permission bit (allow access)
 *   - CLRWR                    clear I/O write-permission bit (deny access)
 *
 * IOR, IOW
 * --------
 * - cmd consists of one or more of the following bits
 *   - POLL                     poll port at given frequency
 *   - BLOCK                    block when no new input on port
 *   - PEEK                     check the queue for desired items
 *   - POKE                     push item to the queue to be read back later
 *   - HALF                     16-bit I/O operations
 *
 * IOC
 * ---
 * - cmd consists of one or more of the following bits
 *   - DIR                      list directory contents (r1 = directory-node)
 *   - OPN                      open file for I/O; r1 = file-node, cmd = flags
 *   - FRD                      read part of file or directory entry
 *   - FWR                      write part of file or directory entry
 *   - SEEK                     seek file or directory to position
 *   - TRUN                     truncate file to given length (e.g. 0)
 *   - DEL                      unlink/remove file from filesystem
 *   - TTY                      open pseudo-teletype for terminal I/O
 *
 * MTR
 * ---
 * - cmd consists of one or more of the following bits
 *   - DMA                      direct memory access buffer
 *   - FB                       video framebuffer
 *   - BUF                      generic stream- or memory buffer
 *   - SHM                      shared-memory segment
 *   - MQ                       message queue [structure]
 *   - KEEP                     hint kernel to keep some of the buffers around
 *   - RING                     queue events/requests into a ring buffer
 */
/*
 * EXAMPLE
 * -------
 * ldi  $VOIMA_FB_ADR, %r1      ;; load framebuffer physical address into R1
 * iom  $imm16, %r1             ;; imm16 is embedded into the opcode info-member
 * mtr  $cmd, $npg, %r1         ;; npg is imm16-member of opcode
 */
#define VOIMA_DEV               0x00 // BUSCHK, DEVINIT, DEVGETATR, DEVSETATR
// dev(*atr, cmd, flg);
#define VOIMA_IOP               0x01 // I/O-port permission control
#define VOIMA_IOR               0x02 // info is the port-ID
#define VOIMA_IOW               0x03 // info is the port-ID
#define VOIMA_IOC               0x04 // DIR, OPN, FRD, FWR, SEEK, TRUN, DEL, TTY
#define VOIMA_IOM               0x05 // e.g. iom(VOIMA_FB_ADR, flg);
#define VOIMA_MTR               0x06 // memory-type ranges; SEQ, KEEP, RING
#define VOIMA_CMD               0x07

#endif /* __VOIMA_BSP_H__ */

