# Raw Mode Validation

`raw_mode=1` is an instrumented research path, not a multi-touch feature for
daily use. Keep `raw_mode=0` until all gates below pass for the tested firmware.
Never unload or reload `spi_amd` while the system is running; use a reboot as
the recovery boundary.

## Evidence Rules

Classify every claim before changing the parser:

| Class | Required evidence |
|---|---|
| Confirmed | Two independent sources, including a capture or controlled hardware observation |
| Reverse-engineered | Static Windows binary analysis only |
| Linux observation | Reproducible Linux capture only |
| Hypothesis | Any untested interpretation; it must not drive default coordinates |

The current `72x48` field, its offset 26, the fixed c590 approximation, and
grid-to-screen transform are hypotheses. The Windows processor's 288-column
internal representation is reverse-engineered evidence, not proof of the
on-wire byte layout. Do not use either model as calibration ground truth.

## Required Gates

1. Build the driver with the compiler family used for the target kernel and run
   `make -C tests SANITIZE=1 test`.
2. Cold boot in standard mode and record descriptor fields, PCI configuration,
   module parameters, kernel logs, and a known-good touch/pen event trace.
3. Capture Windows SPB/GPIO traffic and `Microsoft.Surface.TouchAndPen.Prod`
   TraceLogging during labelled 3x3 single-finger and separated two-finger
   touches. Preserve the ETL as well as any export.
4. Account for each raw-frame layer explicitly: SPI transfer, V0 body, report
   payload, metadata, and candidate DFT data. Reconcile 4297, 4299, 4302, 4304,
   and 4309 before changing frame offsets or dimensions.
5. Correlate controlled positions with changed bytes and Windows centroid
   values. Test 72-column, 288-column, and non-rectangular layouts offline.
6. Validate the captured handshake only after cold boot. Log firmware,
   GET_FEATURE response, SET_FEATURE result, first frame, timeout, retry, and
   fallback. Do not vary type, length, or feature-ID fields without new capture evidence.
7. Add golden tests for the proven layout before enabling coordinates: empty,
   noise, one touch, two touches, crossing, edge, hold, lift, and truncated
   frames.
8. Run KASAN, UBSAN, KCSAN, lockdep, and SPI fault injection on a dedicated
   kernel before any extended raw-mode soak test.

## Acceptance

Raw multi-touch can be considered reliable only after repeatable cold-boot
handshakes, zero sanitizer/lockdep reports, stable slot identity, bounded
fallback, and calibrated position measurements from controlled traces. Until
then, failed handshakes and incorrect coordinates are expected experimental
outcomes, not evidence that the parser is correct.
