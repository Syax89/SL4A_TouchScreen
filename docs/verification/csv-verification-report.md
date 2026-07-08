# Independent verification — Ground truth from the ETW CSVs vs. the documentation

Sources: `~/Scrivania/traces/surface_boot_auto.csv` (46,115 lines, 2,384 SPB transactions,
9,507 GPIO events) and `~/Scrivania/traces/surface_init.csv` (513 transactions). Parser:
`parse_etw.py` (same directory as this report). Times are relative to the first transaction
(t=0 = Clock-Time 134276452171753048, CSV line 58).

## KEY FINDING (not present in ANY document)

The boot trace contains **TWO distinct SPB devices**:

| | Device handle | Connection ID | Opcodes seen | Txn |
|---|---|---|---|---|
| **Device A** — touchscreen HID-over-SPI | `0x7F74AA5D37F8` | `0x…0b` | 0x0B (read), 0x02 (write) | 1,357 |
| **Device B** — firmware upload | `0x7F74AA5D7B88` | `0x…18`, `0x…19`, `0x…1a` | 0x00, 0x70, 0xB0, 0xB1, 0x28, 0x22, 0x24, 0x25, 0x26, 0x29 | 1,027 |

"PHASE 3 Activation" and "PHASE 4 Firmware" from the documents (TXN#9–#13+, opcodes
0x00/0x70/0xB0/…) **do not happen on the touchscreen's HID target** — they happen on a
different SPB target (different handle and connection ID). Device A **never** receives the
0x00/0x70/0xB0/0xB1/0x22 opcodes. Whether this is a physically different chip or the same
chip on a different chip-select/ACPI resource can't be determined from the CSV alone, but the
SPB connection is definitely distinct.

Additionally, **all** of device A's transactions are **full-duplex**: request-type field = `8`
in the `Start` event (ID 1010), a single `IoSpbPayloadStart` (ID 1021) with 2 TDs of
**identical length** (ToDevice + FromDevice). MISO shows `FF` during the first 5 bytes (while
the master clocks the header) and real data right after: this is one single SPI transfer, not
write-then-read. On device B, on the other hand, the types are 1=Read, 2=Write, 3=Sequence.

---

## (A) Chronological table — first 20 transactions + GPIO (boot)

Times in ms from t=0. `⚡` = `InterruptInvokeDeviceIsrStart` (pin 0x15, flags 0x409).

| # | CSV line | t | Dev | Type | MOSI | MISO |
|---|---|---|---|---|---|---|
| ⚡ | 55 | −0.046 | GPIO | ISR | — | — |
| 0 | 58 | 0.000 | A | FD 9/9 | `0B 00 00 00 FF 00 00 00 00` | `FF×5 32 10 00 5A` |
| 1 | 75 | +0.128 | A | FD 9/9 | `0B 00 00 00 FF 00 00 00 00` | `FF×5 03 00 00 00` |
| 2 | 92 | +0.322 | A | FD 10/10 | `02 00 00 01 42 00 00 03 00 00` (DESCREQ) | `FF×5 03 00 00 00 00` |
| ⚡ | 120 | +0.489 | GPIO | ISR | (57.8 µs after #2's IoComplete) | — |
| 3 | 123 | +0.499 | A | FD 9/9 | `0B 00 00 00 FF 00 00 03 00` | `FF×5 72 80 00 5A` |
| 4 | 140 | +0.621 | A | FD 37/37 | `0B 00 00 00 FF 00 00 03 00` + 28×00 | `FF×5 1F 00 00` + `1C 00 00 01 A8 03 02 00 00 00 00 20 03 00 00 02 04 00 5E 04 19 0C 04 00 81 00 00 00 00` |
| 5 | 157 | +0.846 | A | FD 10/10 | `02 00 00 02 42 00 00 03 00 00` (DESCREQ2) | `00×10` (all zero) |
| ⚡ | 185 | +1.641 | GPIO | ISR | (727.0 µs after #5's IoComplete) | — |
| 6 | 188 | +1.691 | A | FD 9/9 | `0B 00 00 00 FF 00 00 03 00` | `FF×5 82 B0 0E 5A` |
| 7 | 205 | +1.854 | A | FD 945/945 | `0B 00 00 00 FF 00 00 03 00` + 936×00 | `FF×5 AB 03 00` + HID report descriptor (936 B, starts `75 08 15 00 26 FF 00…`) + 1 pad |
| 8 | 228 | +964.35 | **B** conn 0x18 | Write 5 | `00 0E 00 00 00` | — |
| 9 | 241 | +964.52 | **B** conn 0x19 | Write 1 | `00` | — |
| 10 | 254 | +964.61 | **B** conn 0x19 | Read 3 | — | `84 26 AA` |
| 11 | 267 | +964.93 | **B** conn 0x1a | Seq W1+R14 | `70` | `0E 00 42 01 02 03 FF 00…` |
| 12 | 289 | +989.08 | **B** conn 0x1a | Write 241 | `B0` + 240 B firmware | — |
| 13–19 | 302… | +992.6…+1013.8 | **B** | Write 241 | `B0` + 240 B (subsequent blocks, one every ~3.5 ms) | — |

Relevant continuation: `B0` blocks up to #131 (241 B) + #132 (125 B, line 7? ; +1412.9 ms),
#133 `B1 31 0F 92 60` (+1414.8 ms), #134/135 `70`→`0E 00 4F 01 50 A6 41 01…` (state
0x42→0x4F). Device A resumes at #220 (+5902.3 ms, ~line 3210).

Subsequent key transactions on device A:

| # | t (ms) | MOSI | MISO |
|---|---|---|---|
| 220 | +5902.28 | `02 00 00 03 42 00 04 03 00 06` | `FF×5 AB 03 00 75 08` |
| 221 | +5902.54 | `0B 00 00 00 FF 00 04 03 00` | `FF×5 52 F0 01 5A` |
| 222 | +5902.70 | `0B 00 00 00 FF 00 04 03 00 06 00 00 00…` (129 B) | `FF×5 7A 00 06 77 00…` (124 B body) |
| 223 | +5907.30 | `02 00 00 03 82 00 03 04 00 05 01 00 00 00` | `FF×5 7A 00 06 77 00…` |
| 224 | +5909.17 | `0B 00 00 00 FF 00 03 04 00` | `FF×5 12 60 03 5A` |
| 225 | +5909.33 | `0B 00 00 00 FF 00 03 04 00 05 01 00 00 00…` (221 B) | `FF×5 D6 00 08…` (216 B body) |
| 531 | +13671.91 (line 7367) | `02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00` | `FF×5 D6 00 08…` |
| 868 | +42195.6 | `0B 00 00 00 FF 00 03 0A 00` | `FF×5 12 60 03 5A` |
| 873 | +42218.0 (line 11937) | `0B 00 00 00 FF 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00…` (4309 B) | `FF×5 CE 10 0C…` (4304 B body) |

---

## (B) Answers to the 6 questions

### 1. DESCREQ: single transaction or separate write+read?
**(a) A SINGLE full-duplex transaction.** TXN#2 (CSV lines 92–122): `Start` event ID 1010 with
type field = **8** (full-duplex; on device B the values are 1=Read, 2=Write, 3=Sequence), a
single `IoSpbPayloadStart` ID 1021 with user data `20, 2` (20 total bytes, 2 TDs), then:
- `IoSpbPayloadTdBuffer` TD0 ToDevice, 10 B: `02 00 00 01 42 00 00 03 00 00`
- `IoSpbPayloadTdBuffer` TD1 FromDevice, 10 B: `FF FF FF FF FF 03 00 00 00 00`
- a single `IoComplete` (ID 1015, status 0x0).

The two TDs have **the same length** and MISO contains `FF` for exactly the 5 MOSI header
bytes: a simultaneous transfer (`IOCTL_SPB_FULL_DUPLEX`), not a write followed by a read. This
holds for ALL 1,357 device A transactions, including the 0x0B "reads" (MOSI zero-padded to the
full length). **Confidence: high.**

### 2. First 12 transactions
See table (A). On the touchscreen (device A): **exactly 2 0x0B reads before the first
write** (TXN#0 → `32 10 00 5A`, TXN#1 → `03 00 00 00` = RESET_RSP header+body, identical MOSI
headers `0B 00 00 00 FF 00 00 00 00`). Then DESCREQ (MISO shows `FF×5 03 00 00 00 00`), a 9 B
read (→ `72 80 00 5A`), a 37 B read (device descriptor: contains `5E 04 19 0C` = VID 0x045E /
PID 0x0C19), DESCREQ2 (→ all zero), a 9 B read (→ `82 B0 0E 5A`), a 945 B read (report
descriptor). TXN #8–#11, chronologically next, are on device B. **Confidence: high.**

### 3. GPIO interrupt
All ISRs: pin **0x15**, flags **0x409**, pin-state mask `0x200000` (bit 21) on `\_SB.GPIO`
instance 1.
- ISR at **t=−46.1 µs, BEFORE TXN#0** (line 55): even the very first read is interrupt-driven.
- ISR at +489.4 µs = **57.8 µs after** DESCREQ's IoComplete (431.6 µs); TXN#3 starts **10.0 µs
  after** the ISR.
- ISR at +1641.4 µs = **727.0 µs after** DESCREQ2's IoComplete (914.4 µs); TXN#6 starts
  **49 µs after**.
- Runtime: **676 total ISRs, each followed by a 9 B 0x0B read** (677 total headers; the extra
  one is TXN#1, a second drain after the single initial ISR). ISR→read delta: min 10 µs,
  median 66 µs, max 338 µs. The "body" reads (37/129/221/945/4309 B) follow the header
  **without a second interrupt**. **Confidence: high.**

### 4. Buffer echo (bytes 7 and 8 of the 0x0B headers — NOT "approval bytes")

**DISPROVEN**: bytes 6-8 of the read-approval frame are NOT "approval" protocol fields.
They are an **ECHO of bytes 6-8 of the last 0x02 write** (the Windows driver reuses its TX
buffer).

| Phase (boot) | Last write (bytes 5–8) | Read header (bytes 5–8) | byte7 | byte8 |
|---|---|---|---|---|
| Pre-descriptor | (none) | `00 00 00 00` | 0x00 | 0x00 |
| Post-DESCREQ (`…01 42 00 00 03 00`) | `00 00 03 00` | `00 00 03 00` | **0x03** | 0x00 |
| Post write #220 (`…03 42 00 04 03 00`) | `00 04 03 00` | `00 04 03 00` | **0x03** (but byte6=0x04) | 0x00 |
| Post write #223 (`…03 82 00 03 04 00`) | `00 03 04 00` | `00 03 04 00` | **0x04** | 0x00 |
| Runtime, post write #531 (`…03 C2 00 03 0A 00`) | `00 03 0A 00` | `00 03 0A 00` | **0x0A** | 0x00 |

In `surface_init.csv` (after disable/enable) the first txn is the write
`02 00 00 04 82 00 00 04 00 01 01 0C 00 00` and ALL runtime reads use `…FF 00 00 04 00`
(byte7=**0x04**, not 0x0A) and regularly receive 221/4309 B reports. So these values are
**not phase constants**: they're whatever the host last wrote in its last SET. byte8 = 0x00
always, in both traces. **Confidence: high on the bytes; also high on the echo mechanism
(confirmed by 5/5 writes, including the long tail).**

### 5. Registers/addresses
- 0x0B read: bytes 1–2 = `00 00` **always** (all 1,352 reads, both phases) → address 0x0000:
  **confirmed**.
- 0x02 write: bytes 1–2 are also `00 00`; the byte that varies is **byte 3**: `0x01`
  (DESCREQ), `0x02` (DESCREQ2), `0x03` (the three subsequent boot writes: 0x42/0x82/0xC2),
  `0x04` (write in the init trace). There is no write with "address 0x0004" anywhere in the
  boot trace; 0x04 at byte 3 only appears in `surface_init.csv`. Byte 4 (`0x42/0x82/0xC2`)
  encodes, in its 2 high bits, the payload length in 4-byte words (1/2/3 → len 10/14/18), and
  in its low nibble a `2` (same "version" as the response headers). **Confidence: high on the
  bytes, medium on the field interpretation.**

### 6. 0xB0 firmware upload
**It exists, but on device B (connection 0x1a), NOT on the touchscreen's HID target.**
- **121 blocks**: 120×241 B (opcode 0xB0 + 240 data bytes) + 1 final 125 B block (0xB0 + 124
  data) = 28,924 payload bytes. From +989.1 ms to +1412.9 ms (~424 ms), one block every
  ~3.5 ms.
- Preceded by (conn 0x18/0x19/0x1a): write `00 0E 00 00 00`, write `00`, 3 B read → `84 26 AA`,
  then `70` → `0E 00 42 01 02 03 FF…` (bootloader state).
- Followed by: `B1 31 0F 92 60`, then `70` → `0E 00 4F 01 50 A6 41 01…` (state byte
  0x42→0x4F), then a **second 0x22 binary** (81×241 + 1×213 B) and **17 cycles** of 0x24
  (37 B), 0x25 (25×241+157 B), 0x26 (17×241+17 B), 0x28 (status read), 0x29 (5 B) groups —
  never mentioned in the docs. **Confidence: high on the numbers; medium on device B's
  identity.**

---

## (C) Discrepancies: docs vs. CSV

1. **[SEVERE] No document distinguishes the two SPB devices.** CSV_SEQUENCE PHASES 3–4,
   DRIVER_STATE §6 PHASES 3–4 and §7 State 3, VERIFICATION_PLAN §2.5/§3 present activation
   (0x00/0x70) and firmware (0xB0/0xB1/0x22/0x28) as a continuation of the touchscreen
   sequence. In reality they happen on a different SPB handle/connection; the HID target
   never receives those opcodes. The Linux driver's "state 3 VENDOR_INIT" (cmd1/cmd2/cmd3 on
   the same HID channel) has no counterpart in the CSV.
2. **[SEVERE] DRIVER_STATE §6 and VERIFICATION_PLAN §2.5: "TXN#11 0x0B read → checksum"** —
   false: it's a **plain 3-byte read with no MOSI command at all** (`84 26 AA`), on device B
   conn 0x19. §7's "ACK read(0x0B,16B)" also doesn't exist in the CSV. (CSV_SEQUENCE's own
   TXN#11 correctly describes it as RX-only, though.)
3. **[SEVERE] VERIFICATION_PLAN §2.6's approval table is incomplete/misleading**: it's
   missing the `00 04 03 00` phase (after write #220) and `00 03 04 00` →
   **approval7=0x04** (after write #223); it's also missing that byte 6 changes too; and
   above all, the values aren't phase constants — they're an **echo of the last 0x02
   write** (surface_init: runtime with approval7=0x04, not 0x0A). A driver that hardcodes
   0x03/0x0A per state only works if it replicates Windows's exact writes.
4. **[MEDIUM] CSV_SEQUENCE TXN#5: "vendor=0x03A8, product=0x0002, version=0x0320"** —
   wrong: 0x03A8=936 is the **report descriptor length**; the real VID/PID = `5E 04 19 0C` =
   0x045E/0x0C19, present in the same payload (consistent with DRIVER_STATE §1 and with bug
   C4 about the +4 offset — the error here comes from mis-parsing the body).
5. **[MEDIUM] VERIFICATION_PLAN §2.4/§2.5: "TXN#3 DESCREQ → ACK (all zeros)"** —
   contradicts the CSV and CSV_SEQUENCE itself: DESCREQ#1 shows `FF×5 03 00 00 00 00` on
   MISO; "all zeros" is only true for DESCREQ2. Either way, the Windows driver doesn't read
   the FIFO after a write (RX_COUNT=0), so MISO's content during DESCREQ is irrelevant.
6. **[MEDIUM] GPIO timing in CSV_SEQUENCE**: "GPIO IRQ → 0x0B read ~112 µs" and "~185 µs" —
   in reality the read **starts** 10 µs (resp. 49 µs) after the ISR; 112/185 µs are only
   obtained by measuring up to IoComplete. "DESCREQ→IRQ ~58 µs" (57.8) and "DESCREQ2→IRQ
   ~727 µs" (727.0) are exact.
7. **[MEDIUM] None of the docs mention the ISR at −46 µs before TXN#0** (line 55): even the
   boot's first read is interrupt-driven. Relevant to the "wait for GPIO IRQ" fix.
8. **[MEDIUM] CSV_SEQUENCE runtime: "interval between reads ~110 ms"** — false: median is
   **10.0 ms** between runtime headers (min 5.9, max 580; 662 headers between +42.2 s and
   +68.1 s).
9. **[LOW] CSV_SEQUENCE TXN#13 "0x28 7-byte TX+RX command"** — it's actually a 1-byte write
   (`28`) + a 6 or 18 byte read.
10. **[LOW] "~6 µs" per-transaction durations** — this is the span of the ETW
    payload-logging events, not the actual SPI time: TXN#0 Start→IoComplete = 108.8 µs.
11. **[LOW] "0x0A runtime ~52 s after boot"** — the first 0x0A header is at +42.2 s from
    TXN#0 (depends on the reference point; order of magnitude is fine).
12. **[LOW] Line references** (TXN#1 "lines 63-73" → actually 58–74) and "Column 28
    Clock-Time" (it's the 17th column in this dump): off by a few, but the cited bytes are
    still correct.
13. **Confirmed by the CSVs** (for completeness): 2 0x0B drains before DESCREQ ✓; DESCREQ is
    a single full-duplex TX+RX transaction (bug #4 and fix D in DRIVER_STATE) ✓; the
    `body_length` formula `((len_low>>4)|(len_high<<4))*4` ✓ (also verified against
    `12 40 43 5A`→4304 and `52 F0 01 5A`→124); header `[TYPE][LEN][5A]` and types 3/7/8 ✓;
    pin 0x15/0x409 in the trace ✓ (the DSDT says something else, as already noted in C8);
    120×241+125 B0 ✓; register read = 0x0000 and approval8=0x00 ✓; 936 B report descriptor ✓
    (body 940 = 3 B subheader `AB 03 00` + 936 + 1 pad byte).

## (D) Confidence

| Conclusion | Confidence |
|---|---|
| DESCREQ and all device A transactions = single full-duplex transaction | **High** (type=8, equal TDs, MISO FF during header — 1,357/1,357 txns) |
| 2 0x0B reads before write, exact bytes of the first 12 txns | **High** (bytes read directly from the CSV) |
| Interrupt: pin 0x15/0x409, 1 ISR → 1 header read, ISR even before TXN#0 | **High** |
| Approval = echo of bytes 5–N of the last 0x02 write | **High** (5/5 writes in the boot + init trace, including the 13-byte tail) |
| Read register 0x0000; write byte3 = 01/02/03/04 | **High** (bytes); **Medium** ("register" semantics) |
| FW upload on a separate SPB device | **High** that it's a different connection/handle; **Medium** on what it physically is |
| B0/0x22/0x24-0x29 cycle block counts | **High** |
| ~10 ms runtime interval | **High** (662 samples) |
