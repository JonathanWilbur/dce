# AGENTS.md

## Cursor Cloud specific instructions

This repository is the **OSF DCE (Distributed Computing Environment) 1.2.2** source
distribution — a 1990s C/IDL codebase built with OSF's proprietary ODE build system
targeting vintage UNIX platforms (AIX/RIOS, HP-UX, Ultrix, OSF/1, SVR4).

### What can and cannot be run here

- The DCE source (`dce/src`, `ode/`) **does not build on modern Linux** and was never
  ported (see `README.md`). There is no package manifest, no CI, and no modern test
  runner. Do not attempt to compile the daemons/services (`dced`, `secd`, `cdsd`,
  `rpcd`, DFS, GDS, etc.) — that requires a 1990s OSF/AIX/HP-UX toolchain that does not
  exist in this environment.
- The **only functional workflow on modern Linux is the documentation build**, driven
  by the top-level `Makefile`. It uses Ghostscript (`gs`) to convert the bundled
  PostScript books/specs into PDFs.

### Doc build (the runnable "application")

- Build all PDFs: `make pdf` (run from the repo root). Output goes to `docout/`.
- Clean up: `make cleandoc`.
- Requires the `gs` (Ghostscript) command on PATH. The update script installs it.
- A full `make pdf` produces 25 PDFs in `docout/` and takes on the order of ~30s.
- `docout/` and `*.pdf` are gitignored, so build output is never committed.

### Gotchas

- There are many nested `Makefile`s in the tree (e.g. `dce/src/config/Makefile` is an
  ODE makefile that fails to parse with GNU make). **Only the top-level
  `/workspace/Makefile` is meant to be run on Linux** — always run `make pdf` from the
  repo root, not from a subdirectory.
- There is no lint or automated test setup that works on modern Linux; "testing" the
  environment means running `make pdf` and confirming valid PDFs are produced in
  `docout/`.
