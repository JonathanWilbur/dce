/*
 * RPC runtime system configuration for Linux.
 *
 * Closest ancestors from PORTING_GUIDE.pdf:
 *   AT386  — little-endian Intel, BSD sockets
 *   SVR4   — System V family, reentrant libc
 *
 * Linux NPTL provides kernel threads and a reentrant C library, so
 * CMA I/O wrappers are not required (ch. 4: _CMA_THREAD_SYNC_IO_).
 */
#ifndef _SYSCONF_H
#define _SYSCONF_H 1

#include <dce/dce.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/file.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <string.h>
#include <dce/cma.h>
#include <dce/pthread_exc.h>

#define USE_PROTOTYPES		1
#define STDARG_PRINTF		1
#define NO_VARARGS_PRINTF	1

#ifndef MSG_MAXIOVLEN
#define MSG_MAXIOVLEN		16
#endif

#ifndef PROT_NCACN
#define PROT_NCACN		1
#endif
#ifndef PROT_NCADG
#define PROT_NCADG		1
#endif
#ifndef NAF_IP
#define NAF_IP			1
#endif

#define RPC_DEFAULT_NLSPATH	"/usr/share/locale/%s/LC_MESSAGES/%s.cat"

#define ATFORK_SUPPORTED
#define ATFORK(handler)		rpc__cma_atfork(handler)

extern void rpc__cma_atfork _DCE_PROTOTYPE_((void *));

#endif /* _SYSCONF_H */
