# ID5-Only CapImg Activation Evidence

## Scope

This is the single authorized Linux hardware observation performed on
2026-07-18 after static decompilation identified report ID `0x05`, Usage
`0xc8`, value `1` as `HeatCore`'s `SetHeatReportingMode(Heatmap)` operation.

The boot-time harness sent exactly one controller-boundary GET ID6, waited for
its response, then sent exactly one controller-boundary SET ID5 with semantic
value `01`. It did not send ID56, retry, recover, reset, power-cycle, enable
the beta input device, or issue a follow-up write.

## Harness Result

Saved `isolated_set_status`:

```text
enabled=1
armed=1
attempted=1
state=4
get_sent=1
set_sent=1
write_failed=0
timeout=1
reset=0
captured=10
overflow=2
next_slot=2
```

`timeout=1` means the fixed 45-second observation ended normally. `reset=0`
and `write_failed=0` rule out a reset or SPI write failure during that window.
The small isolated ring retained ten transport records and overwrote two older
ones; it is not used as the raw-frame evidence.

## Raw-Frame Result

The saved raw-capture status, recorded after the harness observation, reports:

```text
raw_input_beta=0
frame_body_length=4304
captured=484
invalid=0
raw_input_invalid=0
```

Thus, after ID5-only activation, the driver received 484 complete, structurally
valid ID0c bodies with no malformed body and without publishing beta input.

The bounded ring was immediately copied before returning to the passive boot
profile. `tools/extract_raw_capture.py` accepted all eight slots. The status
snapshot and the binary ring are separate close-in-time observations, so their
slot sequence numbers are not asserted to be identical.

## Preserved Artifacts

All files are under `captures/id5-20260718/`:

| Artifact | SHA-256 | Meaning |
| --- | --- | --- |
| `raw_capture.bin` | `713f3c778a4a8a9f2a2ad390d12d47c78d781fc6b0e7b4321d179e6ef0dcc83f` | Eight-slot binary ring dump. |
| `frames/raw-slot-0.v0` | `bdbd1d4338683567aa0ec0eeed8a63681fac33734ae7120c429a7019d0f021cb` | Valid complete ID0c body. |
| `frames/raw-slot-1.v0` | `351369ad8018e1829c848d534920a6159e9c309fd729931db280465fb3a30811` | Valid complete ID0c body. |
| `frames/raw-slot-2.v0` | `477a0657f9afff5c732736f00322a695ed20b52ccc57ddc523a455f5218cdd67` | Valid complete ID0c body. |
| `frames/raw-slot-3.v0` | `2fda9e09300ef4aa388b01d720d9e5a46405a53c0acebd8f12a2c7020f9a24aa` | Valid complete ID0c body. |
| `frames/raw-slot-4.v0` | `1bee929b2b7da8eb0fdfccaf4e2b1f06823f629991c593b801694e9c9bee3dd6` | Valid complete ID0c body. |
| `frames/raw-slot-5.v0` | `e4ac891c92b178a574adbd8922a0344b71f1188dea715acffe711d40821a2110` | Valid complete ID0c body. |
| `frames/raw-slot-6.v0` | `6566e4ba6b7055e18071cdad1a8a92464ee813de76ab8faea1c99c368232153d` | Valid complete ID0c body. |
| `frames/raw-slot-7.v0` | `460f8b044bc098538c5e946e64bc5c421fbf3cadfe8606167b9bfdf8b23a0601` | Valid complete ID0c body. |

## Conclusion

For this MSHW0231 device and the corrected Linux controller framing, ID5 value
`01` is sufficient to start the ID0c CapImg stream. The previously observed
Windows ID56 write is not required for this result and remains a separate,
unresolved protocol operation.

This proves acquisition only. It does not validate contact extraction,
coordinates, tracking, or beta multitouch event quality.

## Recovery

The following boot restored the passive profile:

```text
raw_mode=N
raw_capture_only=Y
raw_input_beta=N
raw_transition_once=N
isolated_set_test=N
```

MSHW0231 again enumerated through normal HID discovery as
`001C:045E:0C19.0004` with `hidraw3`.
