# Raw Mode Activation (SET_FEATURE ID5)

The MSHW0231 touch controller has an observed activation sequence for raw-mode
experiments. Its sufficiency for a reliable raw stream is not yet release
evidence; see `docs/EVIDENCE.md` before treating it as a protocol contract.

## Activation Sequence

### 1. Vendor Initialization

Before the SET_FEATURE command, the driver sends a vendor-initialization
write to register 0xC2 to prepare the device:

```
Host → Device (register 0xC2):
  [02 00 2F 00 00 00 01 00 00 00 00 00]
  content_id  length  vendor_init payload
```

This matches the Windows touch initialization trace
(`surface_init.csv` transaction #267).

### 2. SET_FEATURE Handshake

With `skip_getfeat=0`, the legacy path waits for the Windows-like ~5900ms gap
between RPT_DESC and GET_FEATURE before sending the activation command. The
experimental raw profile uses `skip_getfeat=1` and takes the direct vendor-init
path instead.

```
Host → Device:
  Report Type: Feature (0x03)
  Report ID:   5
  Value:       0x01 (observed raw-mode request)
```

The SET_FEATURE frame is a standard HID feature report with:
- Report type = 0x03 (SET_REPORT / Feature)
- Feature report ID = 5
- Data byte = 0x01 (observed request value)

### 3. Mode Change

SET_FEATURE ID5=01 is part of the observed activation sequence. The current
driver recognizes byte-indexed CapImg frames of roughly 4304 bytes, while older
documentation described a 16-bit 6912-byte raster. This discrepancy is tracked
in `docs/EVIDENCE.md`; do not use either layout as a new protocol contract
without a labelled replay fixture.

## Linux Driver Implementation

### `skip_getfeat=1` (Experimental Raw Profile)

The driver uses a direct vendor-init path:
1. Write vendor-init command (0xC2 opcode)
2. After a short stabilization delay, write SET_FEATURE ID5=01
3. Observe subsequent reports; reliable streaming remains unproven

### `skip_getfeat=0` (Legacy)

The original GET_FEATURE-based path:
1. Wait for device descriptor available
2. Read report descriptor
3. Wait ~5900ms (Windows GET_FEATURE delay)
4. Send GET_FEATURE → device returns current ID5 state
5. Send SET_FEATURE ID5=01 → observe whether a stream follows

The `skip_getfeat=1` path is selected only by `install.sh --raw`. The standard
installer profile remains standard HID mode until the raw validation matrix is
complete.

## Debug Validation

There is no `raw_enabled` parameter. Inspect the SPI HID device's `ready`,
`protocol_stats`, `baseline_status`, and `heatmap_debug` sysfs attributes as
described in `docs/TESTING.md`.

## References

- `tools/surface_tracker.py` — Python oracle with raw mode validation
- `docs/HIDSPI_PROTOCOL.md` — Full HID-over-SPI protocol reference
- Windows `surface_init.csv` trace — Transaction #267 (vendor-init)
