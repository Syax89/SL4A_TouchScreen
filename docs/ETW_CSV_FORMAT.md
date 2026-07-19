# Windows ETW CSV Trace Format

The Windows `Microsoft-Windows-HIDSPI` ETW provider captures HID-over-SPI
bus transactions at 0xFFFF verbosity. These traces are the primary ground
truth used to reverse-engineer the protocol and pipeline.

## Trace Capture

ETW traces are captured on a Windows boot of the Surface Laptop 4:

```powershell
# Start HIDSPI tracing
logman start HIDSPI -p {provider_guid} 0xFFFF -o trace.etl -ets

# After collecting data, stop and convert
logman stop HIDSPI -ets
tracerpt trace.etl -o trace.csv -of CSV
```

The resulting CSV files contain raw SPI frames with bus-level detail.

## CSV Format

Each row represents a single SPI bus transaction:

| Column | Type | Description |
|--------|------|-------------|
| Sequence | u32 | Global transaction sequence number |
| Timestamp | u64 | QPC timestamp (100ns ticks) |
| Direction | str | "TX" (host→device) or "RX" (device→host) |
| Length | u16 | Byte count |
| Data | hex | Raw bytes (space-separated hex) |
| Status | u32 | Bus status flags |

## Frame Types

The `content_id` field (first 2 bytes of data) identifies the message type:

| content_id | Name | Example Pattern |
|------------|------|-----------------|
| 0x01 | DESCREQ | TX: [01 00 0A 00 ...] |
| 0x07 | DESCREQ_RESP | RX: [07 00 1E 00 ...] |
| 0x08 | DEVICE_DESC | RX: [08 00 ...] |
| 0x0B | RPT_DESC | RX: [0B 00 ... 936 bytes] |
| 0x0F | COMMAND | TX/RX: [0F 00 ...] |
| 0x11 | INPUT_REPORT | RX: [11 00 ... heatmap data] |

## Trace Files

Key trace files used during development:

| File | Content | Transactions |
|------|---------|-------------|
| `surface_init.csv` | Cold boot initialization sequence | ~300 |
| `surface_touch.csv` | Touch interaction trace | ~1000+ |
| `tools/surface_tracker.py` | Python oracle processing captured frames | N/A |

## Parsing Tools

- `tools/parse_spi.py` — Parse ETW CSV to extract frame sequences
- `tools/surface_tracker.py` — Replay pipeline over captured frames
- The driver's `seq_dbg` infrastructure can export frames in a
  comparable format for cross-validation.

## Reference Sequences

### Init Sequence (surface_init.csv)

```
TX: DESCREQ [01 00 0A 00 00 00 01 00 00 00 00 00]
RX: DEVICE_DESC [07 00 1E 00 ... 28 bytes descriptor]
TX: DESCREQ2 [01 00 ...]
RX: RPT_DESC [0B 00 ... 936 bytes report descriptor]
--- 5900ms gap ---
TX: GET_FEATURE [0F 00 ...]
RX: FEATURE_RESP [0F 00 ...]
TX: SET_FEATURE ID5=01 [0F 00 ...]
RX: FEATURE_ACK [0F 00 ...]
--- raw mode active ---
RX: INPUT_REPORT [11 00 ... 3528 bytes raw frame]
```

### Touch Frame (surface_touch.csv)

Each raw input report follows this structure:

```
[0..1]   content_id = 0x0011 (INPUT_REPORT, little-endian)
[2..3]   length = 3528 (0x0DC8)
[4..5]   content_length + report_type
[6]      Frame type (0x00 = standard, 0x80 = gap)
[7..11]  Frame metadata
[12..]   Heatmap data (72×48 × 2 bytes = 6912 bytes raw)
```

## Validation Methodology

1. Capture ETW trace on Windows of known touch gesture
2. Parse CSV → extract raw frames as byte arrays
3. Feed frames to Python oracle pipeline
4. Compare oracle output with kernel driver output
5. Adjust kernel implementation to match

The `tools/surface_tracker.py` script implements this validation
loop with the full Windows pipeline emulated in Python.
