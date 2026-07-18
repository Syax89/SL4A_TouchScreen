# Artifact Manifest

## Subject

| Property | Value |
|---|---|
| Input | `/home/simone/windrivers/TouchPenProcessor0C19.dll` |
| SHA-256 | `f05828f04eb7df02ad5a2eeae136ef19d504f987afe2c06d0375ee1264300b4a` |
| PE | 64-bit native Windows DLL |
| File version | `5.127.137.0` |
| PDB reference | `C:\\w\\69\\b\\Release\\x64\\bin\\TouchPenProcessor.pdb` (not present locally) |
| Analysis mode | Offline static analysis only; no hardware, driver, Windows boot, or device interaction |
| Analyzer | Ghidra 12.1.2_DEV headless; radare2 for raw IEEE-754 constant reads |

## Generated Files

| File | SHA-256 | Description |
|---|---|---|
| `README.md` | Textual report; see final verification hash | Recoverable table/formula report, confidence, and V0 applicability |
| `ccl_consumers.c` | `dcdf9d80f0e87026eed6146e010361017cb5b2f5d3becd076a43393b7e0c2931` | Selected code consumers, 4,389 lines |
| `keyword_xrefs.c` | `03498cb4211c57fa521a293d90409e0793ffed6c2a8dabaf5bb194679fe6eccb` | Two-hop keyword/xref consumers, 53,410 lines |

## Reproduction

The following commands were run locally. The Ghidra project lives outside the repository at `/tmp/touchpen-ghidra`.

```sh
/opt/ghidra/support/analyzeHeadless /tmp/touchpen-ghidra touchpen0c19 \
  -import /home/simone/windrivers/TouchPenProcessor0C19.dll \
  -analysisTimeoutPerFile 900

/opt/ghidra/support/analyzeHeadless /tmp/touchpen-ghidra touchpen0c19 \
  -process TouchPenProcessor0C19.dll -noanalysis \
  -scriptPath /home/simone/spi-hid/tools/ghidra \
  -postScript DecompileByAddress.java ccl_consumers.c \
  18068d620 1805fe480 180605cf0 1806091c0 1805ffd00 180600820 \
  180600c40 180601690 180606040 180606370 180608430 1806067d0 \
  180601c10 180605470 180602e60 180602a90 1806025c0 180602770 \
  1805ff7e0 18068bfc0 1805fb520

/opt/ghidra/support/analyzeHeadless /tmp/touchpen-ghidra touchpen0c19 \
  -process TouchPenProcessor0C19.dll -noanalysis \
  -scriptPath /home/simone/spi-hid/tools/ghidra \
  -postScript DecompileByKeywordIndirect.java keyword_xrefs.c \
  RectAnt ccpSection PenDft PenCenterOfMass TrackLib Kalman TouchDetection \
  ShortAnt LongAnt threshold filter
```

The two-hop sweep reported 137 raw keyword hits, 47 directly/indirectly referenced strings, zero pointer-table slots, and 126 selected functions. A lack of a recovered table is therefore a result of consumer tracing, not merely a lack of matching strings.

Raw constants were read at virtual addresses `0x180716340`, `0x1806c089c`, `0x1806cd848`, `0x1806c08a4`, `0x1806c6458`, and `0x180765c70`. The report only assigns semantics where their selected code consumers establish them.
