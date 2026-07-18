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
