# Raw Mode Activation (SET_FEATURE ID5)

The MSHW0231 touch controller requires a specific activation sequence
to enter raw heatmap mode. Without this sequence, the device produces
only pre-processed single-touch coordinates (Report ID 0x40).

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

After reading the HID report descriptor, the driver waits for the
device to stabilize (matching the Windows ~5900ms delay between
RPT_DESC and GET_FEATURE), then sends the activation command:

```
Host → Device:
  Report Type: Feature (0x03)
  Report ID:   5
  Value:       0x01 (enable raw mode)
```

The SET_FEATURE frame is a standard HID feature report with:
- Report type = 0x03 (SET_REPORT / Feature)
- Feature report ID = 5
- Data byte = 0x01 (enable)

### 3. Mode Change

After receiving SET_FEATURE ID5=01, the device switches from standard
HID mode to raw heatmap mode. All subsequent input reports contain:

- Headers containing frame type markers
- 72×48 mutual-capacitance heatmap (3456 cells × 16-bit raw = 6912 bytes)
- Frame metadata (timestamp, frame counter, touch state flags)

## Linux Driver Implementation

### `skip_getfeat=1` (Default, Recommended)

The driver uses a direct vendor-init path:
1. Write vendor-init command (0xC2 opcode)
2. After a short stabilization delay, write SET_FEATURE ID5=01
3. Begin processing raw input reports

### `skip_getfeat=0` (Legacy)

The original GET_FEATURE-based path:
1. Wait for device descriptor available
2. Read report descriptor
3. Wait ~5900ms (Windows GET_FEATURE delay)
4. Send GET_FEATURE → device returns current ID5 state
5. Send SET_FEATURE ID5=01 → device enters raw mode

The `skip_getfeat=1` path is preferred because it avoids the ~5.9s
delay and matches the working cold boot trace more closely.

## Debug Validation

The activation can be confirmed by reading the raw mode flag:

```bash
cat /sys/module/spi_hid/parameters/raw_enabled
# 1 = raw mode active, 0 = standard mode
```

Without activation, only single-touch coordinates are produced and
`raw_enabled` reads 0.

## References

- `tools/surface_tracker.py` — Python oracle with raw mode validation
- `docs/HIDSPI_PROTOCOL.md` — Full HID-over-SPI protocol reference
- Windows `surface_init.csv` trace — Transaction #267 (vendor-init)
