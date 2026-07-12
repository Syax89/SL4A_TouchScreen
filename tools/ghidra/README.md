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
before deciding whether a binary deserves a deeper manual look in the GUI. **Only practical
for small binaries** (tens to low hundreds of KB) — for anything multi-MB with no PDB
(thousands+ of stripped functions), use `DecompileByKeyword.java` instead (below); a full
`DecompileAll` there would take far too long and produce an unreviewable dump.

## Triaging huge stripped binaries: `DecompileByKeyword.java`

Added 2026-07-12 while reverse-engineering `TouchPenProcessor0C19.dll` (9.7MB, no PDB, no
named functions — see `docs/GROUND_TRUTH.md` §21). Scans the **raw bytes** of every
initialized memory block for keyword substrings (deliberately does not rely on Ghidra's
auto-analysis having classified those bytes as string Data — many embedded debug/assert
strings in real-world binaries are never auto-typed as strings, so a `DefinedData`-based
scan misses most of them). For every hit, finds the code that directly references that exact
string address and decompiles just those functions — instead of every function in the binary.

```sh
/opt/ghidra/support/analyzeHeadless /tmp/ghidra_proj some_project_name \
  -process binary.dll -noanalysis \
  -scriptPath /home/simone/spi-hid/tools/ghidra \
  -postScript DecompileByKeyword.java /tmp/out.c keyword1 keyword2 ...
```

(First `-import` without `-noanalysis` once, same as above, to let Ghidra's analyzers run —
`DecompileByKeyword.java` itself doesn't need re-analysis on subsequent runs with different
keywords.)

Caveat: only finds **direct** code references to the exact string address. Strings accessed
indirectly (e.g. through a runtime-indexed array-of-pointers table) won't have a function
attached — a real limitation hit while investigating `RectAnt.*`/`ccpSection*` strings in the
same binary (see `GROUND_TRUTH.md` §21.2); would need a follow-up hop (find what reads the
pointer-table slot, not the string itself).

## Decompiling known entry points: `DecompileByAddress.java`

When the useful entry point is an export, a vtable target, or an address found through RTTI,
there may be no string reference to chase. Decompile only those functions by passing their
virtual addresses (including the PE image base):

```sh
/opt/ghidra/support/analyzeHeadless /tmp/ghidra_proj some_project_name \
  -process binary.dll -noanalysis \
  -scriptPath /home/simone/spi-hid/tools/ghidra \
  -postScript DecompileByAddress.java /tmp/entry-points.c 180692900
```

## Structural analysis without strings: `ListFunctionsByScalar.java`, `ListFunctionsByMnemonic.java`, `LocateKeyword.java`

Added 2026-07-12 during the CCL pipeline reverse engineering (see `GROUND_TRUTH.md` §22).

- **`ListFunctionsByScalar.java`** — Finds every function containing a given integer constant
  in its instruction operands (e.g. the per-device context stride `0x195e8`). Used to
  identify the init/reset functions separately from the runtime loop.
  ```sh
  /opt/ghidra/support/analyzeHeadless /tmp/ghidra_proj proj -process binary.dll -noanalysis \
    -scriptPath /home/simone/spi-hid/tools/ghidra \
    -postScript ListFunctionsByScalar.java /tmp/out.txt 0x195e8
  ```
- **`ListFunctionsByMnemonic.java`** — Finds functions containing a SIMD instruction prefix
  (e.g. `MULPS`) with per-function instruction counts. Used to locate the XGBoost pen
  position estimator from its packed float math.
- **`LocateKeyword.java`** — Raw-byte substring search (no Ghidra Data/String typing needed).
  Returns virtual addresses of matches, even for unreferenced strings. Pair with
  `RefsNearAddress.java`.

## Pointer tables and symbols: `DumpPointerTable.java`, `ListSymbols.java`

- **`DumpPointerTable.java`** — Dumps a 64-bit pointer table (vtable, function-pointer array)
  and resolves each entry to a containing function. Used to extract `SurfaceHeatProcessor`
  and `HeatAdaptorWindows` vtables.
- **`ListSymbols.java`** — Lists Ghidra symbols whose names contain a case-insensitive substring.
  Used to locate RTTI symbols (e.g. `SurfacetHeatProcessor::vftable`).

## Finding ETW/TraceLogging provider GUIDs: `ListGuidsNearEventRegister.java` / `DumpTlgProvider.java` / `DumpProviderCallers.java`

Added 2026-07-12 to recover the TraceLogging provider GUID for
`Microsoft.Surface.TouchAndPen.Prod` inside `TouchPenProcessor0C19.dll` (see `GROUND_TRUTH.md`
§21.4 for the full writeup and result). TraceLogging providers (unlike classic manifest-based
ETW) don't have their GUID as a plain compile-time `GUID`-typed constant Ghidra recognizes —
`ListGuidsNearEventRegister.java` lists any it *does* recognize plus every caller of the raw
`EventRegister` import; `DumpTlgProvider.java`/`DumpProviderCallers.java` then walk the call
graph and hex-dump memory around referenced addresses to find the actual metadata blob.

**The layout that worked in practice** (found empirically, not from a spec): a TraceLogging
provider's metadata is laid out as `[16-byte GUID][u16 little-endian total size][provider
name, null-terminated][event field defs...]` — i.e. the GUID sits in the 16 bytes immediately
before a 2-byte size prefix that precedes the provider's name string. Once you've found the
provider name string (e.g. via `strings` or a Python regex over the raw file bytes), the GUID
is just the preceding 16 bytes — no need to trace the call graph at all if you can find the
name string directly. (A first attempt to instead *compute* the GUID from a recalled
public-domain TraceLogging hash algorithm did not match the real bytes — don't trust that
without checking against the actual binary.)

Proprietary Microsoft binaries themselves (`.sys`/`.dll`/`.pdb`) live in `~/windrivers/`
(local, git-ignored) — never commit them to this public repo, only the decompiled
notes/findings derived from them (`docs/decomp/`).
