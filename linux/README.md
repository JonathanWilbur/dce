# OSF DCE 1.2.2 — Linux userspace port

This directory is the GNU Make build for a Linux port of DCE, following
the *OSF DCE Porting and Testing Guide* (`docout/PORTING_GUIDE.pdf`,
built with `make pdf` from the repository root).

## Why this layout

The guide (ch. 1.7.2) says ODE must be ported before DCE itself, and
that piecemeal ports may supply new makefiles because the ODE makefiles
assume a complete DCE. ODE's make dialect is not GNU make, so this tree
does not try to run `dce/src/config/Makefile`.

Chapter 4.1: if the OS already has kernel pthreads, do not ship CMA
user-level context-switch assembly. Linux NPTL is used instead; the DCE
POSIX 1003.4a draft-4 API and TRY/CATCH package are implemented on top
of it in `src/dcethreads.c`.

Chapter 5 / Table 5-3: NDR `long` is 32 bits. Linux LP64 `long` is 64
bits, so `dce/src/rpc/sys_idl/LINUX/ndrtypes.h` uses `int32_t`.

Suggested order in the guide §1.7.4: tools → headers → threads → RPC.
This milestone covers headers, threads, UUID, and `uuidgen`. Full RPC
(`libnck`), the IDL compiler, CDS, security, and DFS are not built yet
(DFS is a kernel filesystem; the guide places KRPC/DFS last).

## Build

From the repository root:

```
make linux
make linux-test
```

Or here:

```
make
make test
```

Outputs go to `build/` (gitignored):

- `libdcethreads.a` — DCE threads + exceptions + CTS harness
- `libdceuuid.a` — UUID library
- `uuidgen`
- OSF thread tests: `pthread_hello`, `CRVB_THD_001`, `CRVB_MUT_001`
- `linux_selftest`

Compile DCE-using programs with:

```
CFLAGS += -Ilinux/include -D_REENTRANT -D_GNU_SOURCE
LDFLAGS += -pthread -Llinux/build -ldcethreads -ldceuuid
```

`#include <pthread.h>` with `-Ilinux/include` first picks up the DCE
draft-4 header, not glibc's POSIX 1003.1c header.

## Platform files added under `dce/src`

| Path | Guide section |
| --- | --- |
| `dce/LINUX/dce.h` | §1.7 / §12 machine stanza |
| `rpc/sys_idl/LINUX/ndrtypes.h` | §5.2.2 Table 5-3 |
| `rpc/sys_idl/LINUX/ndr_rep.h` | §5.2.2 little-endian IEEE ASCII |
| `rpc/sys_idl/LINUX/marshall.h` | default macros, as AT386 |
| `rpc/runtime/LINUX/sysconf.h` | §5.2.3 `sysconf.h` |
| `rpc/runtime/LINUX/comsoc_sys.h` | BSD sockets (`comsoc_bsd.h`) |
| `rpc/idl/idl_compiler/sysdep.h` | §5.2.1 Linux as UNIX + gcc -E |
| `threads/LINUX/` | ch. 4 native pthreads choice |

Closest reference platforms: AT386 (little-endian Intel) and SVR4
(System V family, reentrant libc).
