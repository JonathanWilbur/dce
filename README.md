# Open Software Foundation (OSF) Distributed Computing Environment (DCF)

This project has its own
[Wikipedia Page](https://en.wikipedia.org/wiki/Distributed_Computing_Environment).

There is no point in re-stating what the Wikipedia page already says just fine,
so here you go:

The Distributed Computing Environment (DCE) is a software system developed in
the early 1990s from the work of the Open Software Foundation (OSF), a
consortium founded in 1988 that included Apollo Computer (part of
Hewlett-Packard from 1989), IBM, Digital Equipment Corporation, and
others. The DCE supplies a framework and a toolkit for developing
client/server applications.

 The framework includes:

- A remote procedure call (RPC) mechanism known as DCE/RPC
- A naming (directory) service
- A time service
- An authentication service
- A distributed file system (DFS) known as DCE/DFS

## Jonathan Wilbur's Contributions to this Repository

I have added a `Makefile` so you can build the documentation as PDFs if you
have the `gs` command installed. I seem to have this by default on my Ubuntu
desktop computer. Just run `make pdf` and you should get PDFs in `docout/`.
To clean up the mess of PDFs you just created, run `make cleandoc`.

## Why the interest in this codebase?

I am interested in this codebase because it has an OSI networking stack as well
as an X.500 directory implementation. Though there is virtually no hope of me
ever porting all of this to any modern operating system, the code alone can be
a useful reference for validating my OSI stack implementation in
[Meerkat DSA](https://wildboar-software.github.io/directory/).

## Notes

- The original ODE-based DCE daemons do not build on modern Linux.
- A GNU Make **Linux userspace port** lives in `linux/`. It follows the
  *OSF DCE Porting and Testing Guide* (`make pdf` → `docout/PORTING_GUIDE.pdf`):
  native NPTL instead of CMA user threads, 32-bit NDR `long` types on LP64,
  and new makefiles instead of ODE. Build and test with `make linux` and
  `make linux-test`. See `linux/README.md`.
- The OSI networking stack seems to have been developed by Siemens, so it might
  be an ancestor of the code used in Dir.X.
- The OSI networking stack can run over TCP/IP (IETF RFC 1006 / ITOT). I
  confirmed this in `INTRO.pdf`, Section 3.3.3.5.

## Code Metrics

Lines of code, according to the `cloc` utility:

```
   27457 text files.
   11327 unique files.                                          
   16469 files ignored.

github.com/AlDanial/cloc v 2.06  T=7.32 s (1548.0 files/s, 558730.7 lines/s)
---------------------------------------------------------------------------------------
Language                             files          blank        comment           code
---------------------------------------------------------------------------------------
C                                     5109         323971         877742        1677301
C/C++ Header                          2177          49389         186492         185874
Bourne Shell                           448          13736          35258          61658
Tcl/Tk                                 483          23322          41555          56399
IDL                                    443           5547             16          56264
Korn Shell                             246          10220          24990          51104
C++                                    125          11816          11536          44700
Text                                   317          16448              0          36071
Objective-C++                          132            284             31          35477
Perl                                   137           5832           8130          28957
make                                  1196           9782          64824          28654
Gencat NLS                              12           1533           1034          10910
MATLAB                                   3           1098              0           9475
yacc                                    15           1521           1754           8697
Nemerle                                 67            159              0           7080
Expect                                  30            617           1572           6249
lex                                     18           1076           1636           5581
Standard ML                             15            170             72           4884
Pawn                                     1              0              0           3403
HTML                                    15            414             23           2752
Assembly                                24            918           6680           2608
DOS Batch                              165            231              0           2086
awk                                     24            174           1282           1950
Templ                                    2            438              0           1881
C Shell                                 27            425           1199           1660
TeX                                      4            434            135           1511
Rust                                    34             65              0           1314
sed                                     12             98            243           1151
Oracle PL/SQL                            9             23              0           1035
Fortran 77                               3            313             71            680
Smalltalk                                1             76              0            584
m4                                       2             16              5            552
Verilog-SystemVerilog                    5              0              0            525
Rexx                                     2            122             39            491
Windows Module Definition                2             47              0            491
SQL                                     13              0              0            392
Python                                   1             45              0            305
R                                        1              0              0            273
DTD                                      1             74             89            249
Clean                                    1             20              0             98
Protocol Buffers                         1             10              0             86
Markdown                                 1             11              0             34
diff                                     1              8             16             28
Windows Resource File                    1              3              0             23
Prolog                                   1              7              0             13
---------------------------------------------------------------------------------------
SUM:                                 11327         480493        1266424        2341510
---------------------------------------------------------------------------------------
```
