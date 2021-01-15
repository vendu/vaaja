#ifndef SYS_ZEN_KERN_H
#define SYS_ZEN_KERN_H

#if defined(__v0__) && defined(__kernel__)
#include <sys/zen/trap.h>
#include <sys/zen/tmr.h>
#include <sys/zen/task.h>
#include <sys/zen/sched.h>
#include <sys/zen/var.h>
#include <sys/zen/perm.h>#include <sys/zen/drv.h>
#include <sys/zen/dev.h>
#include <sys/zen/vfs.h>
#include <sys/zen/fs0.h>
#include <sys/zen/buf.h>
#include <sys/zen/io.h>
#include <sys/zen/page.h>
#include <sys/zen/vm.h>
#include <sys/zen/net.h>
#endif

#endif /* SYS_ZEN_KERN_H */

