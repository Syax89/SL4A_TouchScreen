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
`01 ff ee ff ff ff ff ff 32` — a reset frame **nine bytes out of position**.

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
| SET_POWER D2 | `02 00 00 04 82 00 00 04 00 01 02 0C EE 5B` (14B) | capture: Clock-Time `134276314634377432` (len 14, prefix `02 00 00 04 82`); the D0 twin differs in the payload byte |
| SET_POWER D0 | same with `01` | same transaction |

**An apparent conflict, resolved.** V0's `ConfigurePowerTransfer` builds a frame
in a zeroed buffer — `02 <reg> 82 00 00 04 00 01 <D0\|D2>` at length 14, tail
zeros — which looks like a contradiction with the ETW-cited `0C EE 5B`. It is
not: the register comes from a different source in V0 and it never emits this
trailer, so the two functions build **different commands**. The wire capture wins
for the frame it captures; V0 wins for the frame it builds.

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
