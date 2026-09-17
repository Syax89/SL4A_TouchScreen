# Frame matrix — every frame this driver sends, against its source

Written after a frame audit that compared three independent things: the frames
this driver builds (`driver/spi-hid-wire-frames.h`), the frames in the reference
captures (`traces/*.csv`, via `tools/parse_spi.py`), and the frames the Windows
minidriver constructs (`hidspi` V0, decompiled under `~/sl4a-analysis/decomp`).

The rule the audit enforces: **every frame must name its source.** A frame with
no source is a guess wearing a constant's clothes.

## Handshake reads

| frame | bytes | source |
|---|---|---|
| read, reference shape | `0B 00 00 00 FF 00 00 <reg> 00` (9B) | the reference's own boot reads: register 0 for its reset and drain, register 3 for the descriptor's responses |
| read, legacy shape | `0B <reg3> FF` (5B) | the field only — kept because this panel answers it; the sweep keeps testing it |
| read, **stream** | `0B 00 00 00 FF 00 03 0A 00 56` (9B) | the reference's DATA reads: offset 7 = **0x0A**, dozens of them once the stream is up (TXN#869+). A first version of this table listed only registers 0 and 3 — a leg caught the omission, the same one that drove the wrong register change, because the stream is read through the same helper. |

The three registers, per phase, are the whole story: **0** while the driver is in WAIT_RESET (the reset and its drain), the descriptor register (**3** on this device) through the descriptor phases, and the stream register (**0x0A**) once the sequencer is DONE.

The register these go to was wrong for the whole campaign and was fixed last:
in raw mode the sequencer reads **register 0**, unconditionally, because that is
where the reference reads its reset and its drain. Two earlier attempts (a flag,
then a state gate) were each falsified by one field run. The decisive evidence
had been in the log all along: a WAIT_DESC read on the stream register answered
`01 ff ee ff ff ff ff ff 32` — a frame **three bytes behind a native prefix**,
its sync (`5A`) three bytes past the end of that nine-byte read. (This paragraph
said "nine bytes out of position" until a cross-family leg proved it wrong with
a harness over this very source: there is no shift, and the bytes behind the
prefix are the reference's own frame — header at offset 8, sync at 11.)

## Descriptor handshake writes

| frame | bytes | source |
|---|---|---|
| DESCREQ, device descriptor | `02 00 00 01 42 00 00 03 00 00` | trace, byte for byte (`surface_boot_auto.csv`, and V0's `ConfiguringDescriptorTransferEntry` builds `02 <reg> 42 00 00 03 00 00` at length 10) |
| DESCREQ, report descriptor | `02 00 00 02 42 00 00 03 00 00` | trace, byte for byte |

Both are the trace's exactly. This is the one part of the protocol that was
never in doubt after the parser learned to print bytes.

## Feature commands

| frame | bytes | source |
|---|---|---|
| GET_FEATURE 6 | `02 00 00 03 42 00 04 03 00 06` (10B) | both: the boot trace's own bytes, and Clock-Time `134276314683821940` (len 10, prefix `02 00 00 03 42`) |
| SET_FEATURE 5 | `02 00 00 03 82 00 03 04 00 05 01 0C EE 5B` (14B) | capture: Clock-Time `134276314683843667` (len 14, prefix `02 00 00 03 82`) |
* SET_FEATURE 0x56 enable | `02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00` (18B) | capture: Clock-Time `134276314683446347`, `captures/wintrace/surface_init.csv` (row shows len 18 and the `02 00 00 03 C2` prefix) |
| SET_FEATURE 0x56 stop | same with payload all-`FF` | ETW capture; this is what stops the device before the power sequence |

## Power frames

| frame | bytes | source |
|---|---|---|
| SET_POWER D0 | `02 00 00 04 82 00 00 04 00 01 01 0C EE 5B` (14B) | capture: Clock-Time `134276314634377432`, `captures/wintrace/surface_init.csv` (prefix `02 00 00 04 82`, payload byte `01`) |
| SET_POWER D2 | same with `02` in the payload byte | **inferred from the D0 twin** — no D2 row exists in any capture (case-insensitive search over `captures/` and `traces/`); the single-byte delta is the whole evidence |

**An apparent conflict, resolved.** V0's `ConfigurePowerTransfer` builds a frame
in a zeroed buffer — `02 <reg> 82 00 00 04 00 01 <D0\|D2>` at length 14, tail
zeros — which looks like a contradiction with the ETW-cited `0C EE 5B`. It is
not — and this paragraph said the opposite until a leg reconstructed both
frames from their bytes: the two functions build the **same command**. Indices
0..10 are identical in V0's form and this driver's shipped form; they differ
only in the three tail bytes. V0 allocates a zeroed buffer and never writes a
tail, which is why its excerpt ends in zeros; the capture shows the tail this
device is actually sent (TXN 634377432, the 14-byte SET_POWER D0 buffer), and the
capture wins for what reaches the device. The same leg found the one open
point the old sentence hid: the **suspend** path (`spi_hid_set_power`) still
sends the V0 zero tail for the same opcode, register and selector, and no
sleep/resume capture exists to adjudicate — do not align one with the other
without that capture.

## The honest limit of every register claim here

The reference's registers live at **offset 7 of a nine-byte read** — the shape
Windows sends. The dialect this panel accepts is the **five-byte read, register
in the address field** — measured, repeatedly, and the reason the legacy variant
exists. **Mapping one onto the other is an interpretation, not a measurement**:
under the panel's own decoding, the reference's "register 3" reads carry zero in
the address field and would look like register-0 reads. So the register table
above is what the trace says Windows asked for, translated into the dialect this
panel answers — and the field is what decides whether the translation is right.

Two independent legs checked this boundary and one of them went further, calling
offset 6 the register. It is not: the parser's own labels are `a7` for offset 6
and `a8` for offset 7, and the feature response reads (`0B 00 00 00 FF 00 04 03
00`, TXN#221) carry **content type 4** at offset 6 and **register 3** at offset
7 — the same register as the descriptor. The distinction matters because the
wrong reading would have sent the feature path to a register that does not
exist in this protocol.

One difference remains and is **not expressible in the winning dialect**: the
reference marks feature-response reads with content type 4, while the five-byte
form this panel answers has no content-type field at all. Recorded here rather
than patched, because the form that could carry it is the form the panel ignores.

## What is not in this driver, and should not be

The traces carry whole families this driver never sends: `0x24` (calibration),
`0x25` (the touch reports — ascending address pairs), `0x26` (the firmware
upload, in 20-byte chunks), `0x28`/`0x29`, `0x84`. They belong to the Windows
driver's own stream and firmware paths. The raw path this driver implements is a
different mode; those frames are recorded here as *known and intentional*
absences, not as gaps.

## The two lessons of this audit

1. **An inventory that is empty for a whole category is a broken inventory.**
   The audit's first pass "proved" the traces contain no power frames — because
   the extraction script had dropped every `0x02`-family frame. The empty
   category was the signal, and I read past it. The frames were there, cited by
   transaction id, in the test file.
2. **A derivation is not a source.** An earlier assertion demanded the power
   trailer *because the enable key contains the same field* — an analogy. It
   turned out to be right, but for a reason nobody had written down; the audit
   only established that when it found the ETW transaction number. Write the
   source, not the reasoning.
