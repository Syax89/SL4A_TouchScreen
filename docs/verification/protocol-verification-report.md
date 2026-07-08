# HID-over-SPI protocol verification report — MSHW0231 (V0 path)

**Independent verifier — 2026-07-05**

Sources examined:
- SPEC: `~/Scrivania/decomp/HidSpiProtocolSpec.pdf` (47 pages; text extracted in full)
- V0 DECOMP (hidspi.sys, with PDB symbols): `~/Scrivania/decomp/clean/hidspi_V0_ReadEntries.txt`,
  `hidspi_V0_ConfigureTransfer.txt`, `hidspi_792c_V0_Initialize.txt`, plus additional
  disassembly done in this session directly from `~/windrivers/hidspi.sys` + `hidspi.pdb`
  (functions `ValidateResponseHeader` @0x140008900, `ValidateResponse` @0x140008454,
  `VerifyDescriptorCompletion` V0 @0x1400092ac, `VerifyPowerCompletion` @0x1400096ac,
  `SendingWriteToBusEntry` @0x140008180, `StartingDescriptorResponseTimerEntry` @0x140008430)
- Cx DECOMP: `hidspicx_a664_ConfigureTransfer.txt` and others
- DOCS: `~/Scrivania/docs/HIDSPI_PROTOCOL.md`, `~/Scrivania/docs/VERIFICATION_PLAN.md`

Premise (confirmed, not re-discussed here): the device uses the **HidSpiDeviceV0** path (wire
format "version 2"), not the pure v1.0 spec. Where the spec and V0 diverge, this is called out
explicitly.

---

## D1 — READ APPROVAL

**Verdict: PARTIALLY DISPROVEN (the "approval bytes 7/8" do NOT exist as protocol fields).**

**Spec** (§7.1.2, PDF p. 23): the Read Approval is **5 bytes**:

| offset | bytes | field |
|--------|-------|-------|
| 0 | 1 | Opcode (READ) |
| 1 | 3 | Input Report Address (header or body), **big-endian** |
| 4 | 1 (8 clock cycles) | Placeholder — "0b1 x 8 cycles" = 0xFF in single-SPI |

The header transaction is 9 bytes total (§7.1.3-7.1.4, p. 23-24): "transmitting a Read
Approval (0x05 bytes) … and reading in an Input Report Header (0x04) bytes as part of a
single 9-byte SPI transaction. In the first 5 bytes … the host transmits the Read Approval …
In the last 4 bytes … the device returns the Input Report Header". **Bytes 5-8 of the
transaction are the RX phase**: on MOSI they're don't-care, not fields.

**V0 decomp** (`ReadingResponseHeaderEntry` @0x140007da0, `hidspi_V0_ReadEntries.txt` lines
5-31): the driver only writes bytes 0-4 into the TX buffer:
- `mov byte [rdx], 0xb` → byte0 = 0x0B
- `or dword [rdx+1], 0xff000000` / `mov dword [rdx+1], ecx` → byte1-3 = BE address, byte4 = 0xFF
- transaction length = 9 (`mov qword [rcx+0xb0], 9`)
Bytes 5-8 of the TX buffer are **never initialized**.

**Where the "bytes 7/8" come from**: the TX buffer (`[dev+0xd8]`) is **reused** between write
and read. After the 10-byte DESCREQ `02 00 00 01 42 00 00 03 00 00`, the following read
overwrites only bytes 0-4 (`0B 00 00 00 FF`) and leaves the leftover bytes 5-9 on the wire:
`00 00 03 00 00` → **MOSI byte7 = 0x03**. Before any write, the buffer has just been
allocated and zeroed (`ExAllocatePool2`) → byte7 = 0x00. This reproduces exactly the table in
VERIFICATION_PLAN §2.6 (0x00 pre-DESCREQ, 0x03 after): these are **buffer leftovers captured
in the CSV, not "approval bytes"**. The runtime value 0x0A is explained by the same mechanism
(leftover from a write with content_len=7 → the len+3 field = 0x0A at byte7).

**Does the approval change between phases?** No. Neither the spec nor the decomp defines an
"approval value" that changes. The only thing that changes is the **address**: in the spec,
header and body have distinct ACPI addresses (example p. 16-17: func1=0x1000 header,
func2=0x1004 body); in V0 there's a single address that moves from 0x000000 to
`word[dev+0x50]` (wInputRegister from the device descriptor) once the flag `byte[dev+0x64]`
is set at the end of the descriptor parse (`ReadingResponseHeaderEntry`
@0x140007dc8-0x140007de7; flag set in `VerifyDescriptorCompletion` @0x1400094d3
`mov byte [rdi+0x64], bl`(=1)).

**Impact on Linux**: sending 9 bytes is fine *as long as* bytes 5-8 are treated as dummy TX
of a full-duplex transaction (that's where the header arrives on MISO). Sending fixed 0x00 is
correct; any `spi_hid_approval_byte7`-style logic that changes value with state should be
removed (harmless to the device, but it's code based on an artifact).

---

## D2 — Interrupt before every read; descriptor latency

**Verdict: CONFIRMED (interrupt-gated for every input report; max latency 1 s per the spec,
2 s timeout in the Windows V0 driver).**

- Spec §7.1 (p. 22): "When the DEVICE has active data … it will assert the Interrupt line …
  When the HOST receives the Interrupt, it will then perform the read sequence". §7.1.3
  (p. 23): "**In response to an interrupt**, the host shall conduct a bus transfer in order
  to retrieve the Input Report header".
- One interrupt per response/fragment: §7.1.6 (p. 26): "the DEVICE shall assert an interrupt
  **for every fragment** in a transfer"; and "shall raise an interrupt for a subsequent
  fragment within 1 second".
- Interrupt timing (§7.1.1, p. 22-23): the device holds INT LOW "at least until the start of
  an input report (the HOST has written the read approval opcode of the input report
  header)"; no new assertion during transmission of an input report until after the body's
  approval.
- Request→response latency: §6.1.2 (p. 19) step 5: "**Within 1 second**, the device shall
  signal an interrupt and make available … the Device Descriptor". §7.2 (p. 29): "The device
  shall respond to an output report within 1 second of completion of the bus transaction".
  (t1 = 1 ms on p. 13-14 is just an illustrative assumption for throughput calculations, not
  a requirement.)
- V0 decomp: after the write, the driver enters `StartingDescriptorResponseTimerEntry`
  @0x140008430 → `Timer::Start(0x7d0)` = **2000 ms** timeout, then waits for the interrupt
  event; there's a state/transition `c_readingOnInterrupt` (hidspi_symbols.txt lines 358,
  472) and the header read is a separate state entry reached on that event. Even after a
  reset: `ResettingSyncEntry` @0x140008080 → `Fdo::ResetDevice` + `Timer::Start(2000)`, and
  the read starts on the interrupt, not immediately.

**Impact on Linux**: every (header) read must be triggered by the GPIO IRQ, with a ~2 s
timeout. Don't blind-poll between write and read.

---

## D3 — DEVICE DESCRIPTOR request

**Verdict: CONFIRMED (the 10-byte frame is exact for the V0 path; address 0x000001 is
correct).**

**Spec v1.0** (§7.2.1 p. 29 + example §7.3 p. 32) — NOT used by this device:
`[02][addr24 BE][type=0x01][len16=0x0000][id=0x00]` = 8 bytes (already a multiple of 4).
Output Report Type 0x01 = device descriptor request; example address 0x002000 from ACPI.

**V0 decomp** (`ConfiguringDescriptorTransferEntry` @0x140007230,
`hidspi_V0_ConfigureTransfer.txt` lines 216-239): buffer zeroed for 10 bytes, then:
- byte0 = 0x02 (WRITE opcode)
- byte1-3 = `dword[dev+0x68]` big-endian-encoded over 24 bits → `00 00 01`. `[dev+0x68]` =
  _DSM(rev1) func1 = 1 (`hidspi_792c_V0_Initialize`, cf. FINDINGS §2) → **0x000001 correct**,
  also confirmed by the boot ETW (DESC_REQ @0x000001).
- word byte4-5 = 0x0042 (`mov word [r8+4], 0x42`)
- word byte7-8 = 0x0003 (`mov word [r8+7], 3`); byte6 and byte9 stay 0
- length = 0x0A (`mov qword [r9+0xb0], 0xa`)

→ frame **`02 00 00 01 42 00 00 03 00 00`**, identical to the Linux driver's. Field-by-field
breakdown of the V0 body (derived by comparing every `ConfigureTransfer` @0x140006f6c case):

| byte | value | V0 meaning |
|------|--------|----------------|
| 0 | 0x02 | WRITE opcode |
| 1-3 | 00 00 01 | Output/Descriptor-Request Address, BE |
| 4-5 (LE u16) | 0x0042 | `(body_len << 4) \| version` → body_len=4 (bytes 6-9), version=2. Verified: in the case with content, the code does `shl ax,4; or cx,2` with ax = aligned content_len + 4 (@0x14000715e-0x140007172) |
| 6 | 0x00 | report type (0 for descriptor requests; 3=SET_FEATURE, 4=GET_FEATURE, 5=OUTPUT in other cases: @0x140007181-0x14000718e, @0x1400071b2) |
| 7-8 (LE u16) | 0x0003 | content_length + 3 (0 here) — same pattern in every write: `add di, si(=3); mov word [r14+7], di` @0x14000718b |
| 9 | 0x00 | Content ID (0 for descriptor requests) |

The **report descriptor** request in V0 uses the exact same body but at address
`word[dev+0x4e]` = field at offset 6 of the device descriptor (wReportDescRegister) — on our
device 0x000002 (boot ETW). So DESCREQ2 = `02 00 00 02 42 00 00 03 00 00` ✓, consistent with
HIDSPI_PROTOCOL.md.

**Impact on Linux**: no change to the frame needed. (The actual failure is downstream, in the
physical write path — out of scope for this report.)

---

## D4 — Inline response, or a separate READ after the write?

**Verdict: CONFIRMED — a separate READ, gated on an interrupt. The idea of "TX+RX same
opcode in the same transaction" is DISPROVEN.**

- Spec §7.2.3 (p. 31), Sending Output Reports sequence: Step 1 host writes the output
  report; Step 2 device consumes it; Step 3 "DEVICE asserts interrupt within 1 second after
  completion of the bus transaction"; Step 4 "HOST issues a Read Approval and reads the
  Input Report Header"; Step 5 same for the body. **Three distinct SPI transactions.**
  Reiterated in §6.2.2 (p. 20): "The DEVICE shall then signal an interrupt … The HOST shall
  then read an input report from the Input Report addresses".
- V0 decomp: `SendingWriteToBusEntry` @0x140008180 submits **a single**
  `SpbBusWrapper::Transfer` of length `[dev+0xb0]` = 10 (exactly the write frame, no extra RX
  bytes), then the state machine goes through `CheckingWriteCompletionStatus` →
  `StartingDescriptorResponseTimer` (2 s) → waits for the interrupt event →
  `ReadingResponseHeaderEntry` (a separate 9-byte transaction).
- Any "inline ACK" seen in the CSV during the 0x02 transaction is just whatever the device
  happens to drive on MISO during a write: it's not defined by the protocol and Windows
  never reads it.

**Impact on Linux**: HIDSPI_PROTOCOL.md's "Option B" (write-only → wait for IRQ → 0x0B read)
is the right one and is what Windows does. "Option A" (exec_segment combining TX+RX under
opcode 0x02) has no counterpart in the Windows driver.

---

## D5 — RESET RESPONSE

**Verdict: format CONFIRMED; the host does NOT need to send any ACK; the ~109 ms retry is
NOT in the spec.**

**Format** — Spec (p. 25 §point 1.c and 2.b; example p. 31-32): input report type 0x3,
Content Length 0, Content ID 0; header with Length=1 (4-byte body). **V0 decomp**
(`ValidateResponse` @0x140008454):
- header type == 3 (`shr cl,4; cmp cl, r13b(=3)` @0x14000849d)
- body length from the header == 4 (`cmp rdx, 4` @0x1400084dc)
- body word[0..1] == 0x0003 (`movzx r9d, word [r15+5]; cmp r9w, r13w` @0x14000854e — the
  body starts at buffer+5, after the 5 approval bytes)
→ expected wire bytes: header `32 10 00 5A`, body `03 00 00 00`. NB: in V0 the first 2 bytes
of the body are an LE **length field = content_len+3** (3 = len16+ContentID), not a "type":
the generic validation for other types checks
`body_len_header == align4(word body[0..1])` (@0x1400086a0-0x1400086cd). That the value
happens to be 3 for the reset is a coincidence with type 3.

**What the host does** — Spec §6.1.2 (p. 19): reads the reset response (header+body) and
then goes straight to step 4 (writing the descriptor request). **No ACK/consumption
command**: consumption is the read itself (the interrupt is released once the host writes
the approval opcode — §7.1.1 p. 22). V0 decomp: after validation it increments a counter
(`lock inc [rbp+0x88]`) and proceeds to `ConfiguringDescriptorTransfer`; there's a
`CheckingResetRetryCountEntry` for host-side retries.

**Unsolicited reset response** — Spec §6.1.3 (p. 19-20): "When the device detects an error
condition, it may interrupt and make available … an unsolicited Reset Response. After
receiving an unsolicited Reset Response, the host shall initiate the request procedure
**from step (4)**" → i.e. resend the **descriptor request**, not re-reset the device. This is
exactly the observed behavior: the device that keeps re-emitting RESET_RESP every ~109 ms is
signaling an error/an unreceived request.

**Timeout/retry ~109 ms** — CANNOT BE DETERMINED from the spec: the spec doesn't define any
re-emission interval (the interrupt is held LOW until the host reads, with no maximum). The
~109 ms cycle is device-firmware-specific behavior. The spec's timeouts are: 1 s for the
interrupt after a reset and after an output report; the Windows V0 driver uses 2000 ms timers
(`0x7d0` in ResettingSync/StartingDescriptorResponseTimer/ReadingResponse*).

---

## D6 — DEVICE DESCRIPTOR

**Verdict: DISPROVEN for the v1.0 spec (24 bytes, no registers), CONFIRMED for the V0
variant (28 bytes, HID-over-I2C-style layout with wInputRegister/wOutputRegister).**

**Spec v1.0** (§6.1.1, p. 17-19): a **fixed 24-byte** descriptor (wDeviceDescLength=0x18):
wDeviceDescLength(0), bcdVersion(2)=0x0300, wReportDescLength(4), wMaxInputLength(6),
wMaxOutputLength(8), wMaxFragmentLength(10), wVendorID(12), wProductID(14), wVersionID(16),
wFlags(18), 4 reserved bytes(20). **The v1.0 spec has no wInputRegister/wOutputRegister
fields at all** (those addresses come from ACPI). Looking for those fields in the spec is a
modeling error (they're HID-over-I2C names).

**V0 decomp** (`VerifyDescriptorCompletion` @0x1400092ac — disassembled in this session):
- header type == 7 (`and al,0xf0; cmp al,0x70`)
- body length from the header == **0x20** (32 bytes) (`cmp rcx, 0x20` @0x140009378)
- first content word == **0x1C** (28) (`movzx r9d, word [rdx+8]; cmp r9w, r10w(=0x1c)`
  @0x14000940a-0x140009419; rdx = RX buffer, content at offset 8 = 5 approval bytes + 3 body
  prefix bytes)
- copies **28 bytes** of the descriptor into `dev+0x48..0x63` (`movups [rdi+0x48], xmm0`
  from rdx+8; `movsd [rdi+0x58]` from rdx+0x18; `mov [rdi+0x60], eax` from rdx+0x20
  @0x1400094a4-0x1400094bb)
- sets `byte[dev+0x64] = 1` (enables the input address from the descriptor) and
  re-allocates the transfer buffers based on `word[dev+0x52]` and `word[dev+0x56]`
  (@0x1400095a5-0x140009636).

V0 layout (28 bytes) reconstructed from field usage:

| offset | field (dev ctx) | verified usage |
|--------|-----------------|----------------|
| 0 | wDeviceDescLength = 0x001C | validated ==28 |
| 2 | bcdVersion (dev+0x4a) | trace only |
| 4 | wReportDescLength (dev+0x4c) | — |
| 6 | **wReportDescRegister** (dev+0x4e) | report-descriptor request address (`ConfigureTransfer` case 1 @0x1400071bd `movzx edx, word [rbx+0x4e]`); on our device = 0x0002 (boot ETW) |
| 8 | **wInputRegister** (dev+0x50) | address for reads after the parse (`ReadingResponse*` gated on flag +0x64) |
| 10 | wMaxInputLength (dev+0x52) | buffer sizing @0x1400095a5 |
| 12 | **wOutputRegister** (dev+0x54) | output/set-feature/get-feature address (`ConfigureTransfer` @0x140007136, @0x1400071ae) |
| 14 | wMaxOutputLength (dev+0x56) | buffer sizing @0x1400095cf |
| 16 | **wCommandRegister** (dev+0x58) | SET_POWER address (`ConfigurePowerTransfer` @0x140006f11) |
| 18/20/22 | wVendorID / wProductID / wVersionID (dev+0x5a/0x5c/0x5e) | trace @0x140009522-0x140009530 |
| 24-27 | flags dword (dev+0x60) | bit7→dev+0x7c, bit6→dev+0x7d (power flag), bits5:0 ×2→dev+0x80 @0x1400094b8-0x1400094db |

**On the Linux parse's "4-byte header [len_lo][len_hi][??][0x5A]"**: DISPROVEN. The 4-byte
V0 header is (from `ValidateResponseHeader` @0x140008900):
- byte0 = [TYPE 7:4 | VERSION 3:0] (trace: `and r11d,0xf` version, `shr eax,4` type
  @0x140008af4-0x140008b0c) — **not len_lo**
- byte1-2 = LE u16: bits 3:0 **must be 0** (`test dl, 0xf` → reject @0x1400089f7), bits 15:4
  = body length **in units of 4 bytes**
- byte3 = mandatory 0x5A (`cmp dl, 0x5a` @0x140008940; header saved at `[dev+0xc8]`
  @0x140008b4e)

Also, the descriptor body does NOT start with the descriptor itself: it has a **3-byte**
prefix `[len16 = content+3][ContentID]`, and the content (the 28 bytes, which themselves
start with wDeviceDescLength) starts at **body offset 3** (= buffer offset 8, counting the
approval bytes). A parse that skips a fixed 4 "header" bytes inside the body is off by 1.

**Impact on Linux**: the descriptor to parse is 28 bytes starting at body+3; from there,
extract wReportDescRegister (for DESCREQ2), wInputRegister (the new read address),
wOutputRegister, wMaxInput/OutputLength (read sizing), wCommandRegister (SET_POWER).

---

## D7 — SET_POWER

**Verdict: CONFIRMED to exist in both sources but with different formats; the documented V0
frame is nearly right — errors on length and address.**

**Spec v1.0** (§7.2.2, p. 30): Set Power = a command, i.e. an output report with Output
Report Type **0x07**, Content ID = **0x01** (Set Power command ID), Content = 1 byte (0x01
ON, 0x02 SLEEP, 0x03 OFF). The device responds (input report type 0x04, same content ID)
**only** to Set Power ON.

**V0 decomp** (`ConfigurePowerTransfer` @0x140006efc, `hidspi_V0_ConfigureTransfer.txt`
lines 188-215):
- byte0 = 0x02; byte1-3 = **`word[dev+0x58]`** BE = wCommandRegister **from the device
  descriptor** (offset 16) — NOT `[dev+0x68]`/0x000001
- word4-5 = 0x0082 → body_len 8, version 2
- byte6 = 0x00; word7-8 = 0x0004 (= content 1 + 3); byte9 = 0x01 (Set Power command ID);
  byte10 = state (only written if ∈ {1,2})
- transfer length = **0x0E = 14 bytes** (`mov qword [rcx+0xb0], 0xe`) → 3 trailing padding
  bytes

Corrections to FINDINGS/docs: (a) the frame is 14 bytes on the wire
(`02 aa aa aa 82 00 00 04 00 01 ss 00 00 00`), not 11; (b) the address is wCommandRegister
from the descriptor: **before the descriptor is parsed, `dev+0x58` is 0**, so a test done
with SET_POWER at 0x000001 pre-enumeration doesn't replicate Windows. (c) The boot ETW
confirms Windows **never sends any SET_POWER** during enumeration (FINDINGS §5-ter) — it only
sends it if the descriptor's flags indicate power-command support (states
`CheckingIfDeviceSupportsPowerCommand*`, flags from desc[24] bit6/bit7 →
dev+0x7d/0x7c).

---

## D8 — Body-length formula from the input header

**Verdict: CONFIRMED for the V0 path (different from the v1.0 spec, which doesn't apply
here though).**

Linux driver: `blen = (((hdr[1]>>4) & 0xF) | (hdr[2] << 4)) * 4`. Given
`u16 = hdr[1] | hdr[2]<<8`, this is exactly `(u16 >> 4) * 4`.

**V0 decomp** (`ReadingResponseRemainingEntry` @0x140007e50, `hidspi_V0_ReadEntries.txt`
lines 46-54):
```
movzx eax, word [rcx + 0xc9]   ; u16 = header byte1..2
shr rax, 2
and rax, 0x3ffffffffffffffc    ; = (u16 >> 4) * 4
add rax, 5                     ; + 5 read-approval bytes
```
→ transfer body = `(u16>>4)*4 + 5`, i.e. body = `(u16>>4)*4`. Identical to the driver's
formula. Cross-check in the same function's trace: `shr rax,4; lea rax,[rax*4 - 3]` = content
length = body−3 (@0x140007f25-0x140007f29). And `ValidateResponseHeader` @0x1400089f7
requires `(u16 & 0xF) == 0`, so the low nibble of hdr[1] discarded by the formula must still
be 0 (the Linux driver should validate this too, along with hdr[3]==0x5A).

**Spec v1.0** (§7.1.3, p. 23): length = bits 13:0 of u16, in 4-byte units →
`(u16 & 0x3FFF) * 4`, with bit14 = Last Fragment Flag. Different from V0 (where the 4 low
bits are reserved-zero and the length lives in bits 15:4). The two formulas only agree
numerically when `u16 & 0xF == 0` and `u16 < 0x4000`… actually no: they don't agree at all
(they differ by a factor of 16); this is further proof the device does NOT speak v1.0:
`32 10 00 5A` under v1.0 would mean body = 0x1000*4 = 16 KB.

---

## Errors found in HIDSPI_PROTOCOL.md

1. **Line 15 — "Every device response starts with sync bytes (0xFF) followed by the
   header"**: FALSE. 0xFF is the placeholder **transmitted by the host** as the 5th byte of
   the Read Approval (spec §7.1.2 p. 23; decomp `or dword[buf+1], 0xff000000`). The header
   is the 4 RX bytes and the sync is 0x5A at the end (byte3), not 0xFF at the front.
2. **Lines 22, 43, 118-121 — "type 0 = ACK/Ready" and "ACK pattern: 03 00 00 00 (no 0x5A!
   special pattern)"**: FALSE. There is no "ACK" type-0 report: `ValidateResponse` only
   accepts types {1,4,5,7,8} besides 3 (bitmask 0x1B2 @0x140008686). `03 00 00 00` is the
   **reset response's body** (len16=3=content 0+3, ContentID 0, 1 pad), read in the second
   transaction of the SAME reset response. The narrative "two reads, the device changes
   state on its own between RESET_RSP and ACK" actually describes header-read + body-read
   of a single input report.
3. **Lines 58-60 — approval bytes APPR7/APPR8 (0x00→0x03)**: FALSE. They aren't fields (see
   D1): they're leftovers of Windows's reused TX buffer (CSV byte 7 = byte7 of the previous
   DESCREQ = 0x03).
4. **Line 64 — "0x001000: input_register (after device descriptor parse)"**: FALSE for this
   device. 0x1000/0x1004/0x2000 are the v1.0 spec's ACPI examples (p. 16-17). In V0 the
   post-parse input register is `word[descriptor+8]` (dev+0x50); on our device it stays
   0x000000.
5. **Line 79 — "The device responds with ACK (03 00 00 00), then sends an IRQ"**: FALSE. The
   write is TX-only; no inline response is defined (spec §7.2.3 p. 31;
   `SendingWriteToBusEntry` = a single 10-byte transfer). Whatever appears on MISO during the
   write isn't protocol.
6. **Lines 197-199, 207-208, and the "Fix summary" table — "In Windows, TX+RX use the SAME
   opcode 0x02 in the same operation" / "TX+RX same opcode" fix**: FALSE (see D4). The
   correct CRITICAL fix is Option B (write-only → IRQ → 0x0B read), which the document
   proposes only as an alternative.
7. **Line 29 — "Byte 1: [LENGTH_LOW_4bit][FRAGMENT_ID_4bit]"**: imprecise. The low nibble of
   byte1 is not a usable fragment ID: `ValidateResponseHeader` **rejects** the header if it's
   ≠ 0 (@0x1400089f7). Should be documented as "reserved, must be 0".
8. **Lines 190-192 and 159-161 — "State 4: type==7 → hid_input_report()" / "Phase 5: 0x0B
   read → HID report type=7"**: almost certainly FALSE. Type 7 = DEVICE_DESC; runtime input
   reports are type **1** (Data) both per the spec (p. 25) and V0
   (`VerifyAndCompleteTransfer` @0x140008ba8 compares the header's type against 1 as the main
   case). The boot ETW labels the runtime traffic as "DATA".
9. **Lines 248-250 — "ConfigureTransfer (fcn.0xa664): sets approval bytes (0x00 → 0x03)"**:
   FALSE. In `hidspicx_a664_ConfigureTransfer.txt` (lines 137-162) the values 2/3/4/5 written
   to `byte[buf]` are the v1.0 spec's **Output Report Types** (2=report desc request, 3=set
   feature, 4=get feature, 5=output report), not approval bytes. (This section refers to the
   Cx path anyway, not applicable to this device.)
10. **The "Windows driver — hidspicx.sys" section in general**: wrong model for this device
    (already known from FINDINGS): it describes HidSpiCx/spec v1.0, including the doubt
    about State2 type==4 (which in Cx is the command response, irrelevant to the V0 path).
11. Inconsistency between documents: HIDSPI_PROTOCOL.md line 131 says vendor=0x03A8
    product=0x0002, VERIFICATION_PLAN.md line 332 says vendor=0x045E product=0x0C19. One of
    the two is wrong (can't be determined from spec/decomp alone; needs checking against the
    descriptor dump).

### Errors in VERIFICATION_PLAN.md §2.6 (approval bytes)
The table "Before DESCREQ 0x00 / After device desc 0x03 / Runtime 0x0A" is an artifact (see
D1): the correct value to transmit doesn't actually matter (the device doesn't sample MOSI
after approval byte 4); Windows in practice transmits buffer garbage. Should be removed as a
"requirement". The rest of §2.3 ("initial input register 0x0000") is CONFIRMED, though.

### Note on FINDINGS-hidspi-V0-DECODED.md (for completeness)
- §3 SET_POWER: real length is 14 bytes with 3 padding bytes; address = wCommandRegister
  (desc[16]), not necessarily the initial output address (see D7).
- §3 READ body: the formula "length rounded up to a multiple of 4, plus 5" is confirmed at
  the bit level (`(u16>>4)*4+5`).
- §2: confirmed that `[dev+0x50]` isn't written in Initialize, but is written by the
  descriptor parse (desc offset 8) along with the flag `[dev+0x64]` — so "input 0x000000" only
  holds until the descriptor declares a different wInputRegister.

---

## Verdict summary

| # | Question | Verdict |
|---|---------|----------|
| 1 | 9-byte read approval with "approval byte7/8" | Approval = 5 bytes (opcode+addr24BE+0xFF); bytes 5-8 = RX. "Approval" bytes 7/8 DISPROVEN (reused-buffer artifact). No value change between phases; only the address changes after the descriptor parse |
| 2 | Interrupt before every read | CONFIRMED (spec p. 22-23, 26; V0 `c_readingOnInterrupt`, 2 s timer). Descriptor latency: ≤1 s per spec |
| 3 | DESCREQ frame `02 00 00 01 42 00 00 03 00 00` | CONFIRMED byte-for-byte against `ConfiguringDescriptorTransferEntry`; 0x000001 correct (_DSM func1 + ETW) |
| 4 | Inline response vs. separate read | Separate READ gated on IRQ — CONFIRMED; "TX+RX same opcode" DISPROVEN |
| 5 | Reset response | Header `32 10 00 5A` + body `03 00 00 00`; no ACK, proceed straight to DESCREQ; ~109 ms retry NOT in the spec; unsolicited → restart from the descriptor request (spec §6.1.3) |
| 6 | Device descriptor | V0: 32-byte body, 28-byte content at body+3, HID-I2C-style layout with wReportDescRegister/wInputRegister/wOutputRegister/wCommandRegister; the "4-byte [len_lo][len_hi][??][5A]" parse is DISPROVEN (byte0 = type\|version; content at body offset 3) |
| 7 | SET_POWER | Spec: type 0x07/ID 0x01 command. V0: 14-byte frame at wCommandRegister; the documented frame has imprecise length and address; Windows doesn't send it at boot |
| 8 | `blen = ((hdr[1]>>4)&0xF \| hdr[2]<<4)*4` | CONFIRMED identical to the V0 decomp (`(u16>>4)*4`); add hdr[3]==0x5A and (hdr[1]&0xF)==0 validation |
