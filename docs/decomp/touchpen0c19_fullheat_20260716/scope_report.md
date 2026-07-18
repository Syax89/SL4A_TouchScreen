# TouchPenProcessor0C19 focused offline Ghidra export

## Scope
Keyword predicate: `SurfaceHeatProcessor|Heatmap|VirtualHid|Heat` (case-insensitive, `Heat` word-boundary constrained). Direct internal calls are transitively included. Vtable candidates are runs of at least three initialized non-executable 64-bit function pointers; a candidate is included only when it contains a keyword-root method. Indirect dispatch is recorded but cannot be proven as a direct call edge.

Roots: 18  |  Scoped functions: 468  |  Keyword strings: 83

## Exports
12 PE external-entry symbols; see `exports.tsv`.

## Vtables
1 candidate tables, 13 method slots; see `vtables.tsv`.

## Call Graph
1090 direct internal edges; see `callgraph.tsv`.

## Static Data
6027 direct code-to-static-data references; see `static_data_references.tsv`. Context/member offsets are preserved verbatim in each complete decompilation.

## Files
- `exports.tsv`: all PE exports.
- `strings.tsv`: scoped keyword strings plus all strings referenced by scoped functions.
- `vtables.tsv`: relevant vtable candidates and slots.
- `callgraph.tsv`: direct internal call edges and caller status.
- `static_data_references.tsv`: every direct non-executable address reference from scoped code.
- `functions/*.c`: complete Ghidra decompiler output for every scoped function.
- `functions/index.tsv`: function classification, roots, callers, and direct callees.

