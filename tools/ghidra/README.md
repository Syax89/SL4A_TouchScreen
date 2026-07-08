# Ghidra headless decompilation

Ghidra is installed locally (Arch/CachyOS: `pacman -S ghidra`, pulls in `jdk-openjdk`).
No Windows machine needed anymore to decompile a `.sys`/`.dll` — do it here.

## Gotchas that cost time the first run

- `-postScript foo.py` fails with `Ghidra was not started with PyGhidra. Python is not
  available` — this build's headless mode does not support Python postScripts. Use a
  `.java` GhidraScript instead (see `DecompileAll.java` in this folder).
- `-scriptPath` must be an **absolute** path. `.` silently fails to find the script.

## Usage

```sh
mkdir -p /tmp/ghidra_proj
/opt/ghidra/support/analyzeHeadless /tmp/ghidra_proj some_project_name \
  -import /path/to/binary.sys \
  -scriptPath /home/simone/spi-hid/tools/ghidra \
  -postScript DecompileAll.java /tmp/binary_decompiled.c
```

Re-running on an already-imported binary (skip re-analysis, much faster):

```sh
/opt/ghidra/support/analyzeHeadless /tmp/ghidra_proj some_project_name \
  -process binary.sys -noanalysis \
  -scriptPath /home/simone/spi-hid/tools/ghidra \
  -postScript DecompileAll.java /tmp/binary_decompiled.c
```

`DecompileAll.java` decompiles every function in the program and writes a single `.c` file
with `// ==== name @ address ====` markers — good enough for `grep`/`strings`-style triage
before deciding whether a binary deserves a deeper manual look in the GUI.

Proprietary Microsoft binaries themselves (`.sys`/`.dll`/`.pdb`) live in `~/windrivers/`
(local, git-ignored) — never commit them to this public repo, only the decompiled
notes/findings derived from them (`docs/decomp/`).
