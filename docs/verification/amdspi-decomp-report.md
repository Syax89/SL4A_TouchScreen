# amdspi.sys — Independent verification of the TX+RX path (AMD FCH SPI V2 controller)

Analysis of the `~/windrivers/amdspi.sys` binary (no PDB) disassembled with radare2, cross-checked
against the textual dumps in `~/Scrivania/decomp/amdspi/`. All offsets cited are binary RVAs.

## Verified Conventions

MMIO helpers (confirmed by reading the code, `~/windrivers/amdspi.sys`):

| fcn | semantics | evidence |
|-----|-----------|-------|
| `0x19c0` | **read8**  `mov al,[rax]` | 0x1400019ca |
| `0x19d0` | **read32** `mov eax,[rax]` | 0x1400019da |
| `0x19e0` | **read16** `movzx eax,word[rax]` | 0x1400019ea |
| `0x1fbc` | **write8**  `mov [rax],cl` + `lock or` barrier | 0x140001fce |
| `0x1fd8` | **write32** `mov [rax],ecx` + barrier | 0x140001fea |
| `0x7ba0` | **write16** `mov [rax],cx` + barrier | 0x140007bb4 |
| `0x1000` | get_context (returns the register object) | — |
| `0x6fc0` | saves `read16(BASE+0x22)` into ctx+0x58 | 0x140006fe0 |
| `0x6f84` | restores `write16(BASE+0x22, ctx+0x58)` | 0x140006fa4 |
| `0x2be4`, `0x1d24`, `0x758c`, `0x7714`, `0x7858`, `0x1e34`, `0x79e0` | **WPP tracing ONLY** (`WppAutoLogTrace`) — do NOT touch MMIO | 0x140002d1d |
| `0x74e4` | computes the timeout for `KeDelayExecutionThread` | — |

`BASE` = `*(qword*)ctx` = virtual pointer to the controller's MMIO. Every register access is
`helper(BASE + offset)`. **Note:** the very many `call 0x2be4/0x1d24/...` with `r9w=0x16,0x17,…`
are WPP trace messages, not I/O: they should be ignored when reconstructing the sequence.

## The FOUR transfer functions and how they're selected

Dispatch (WDF/SPB callbacks) in `0x5ee0`–`0x61c0`, selection based on `request+0x48` (transfer type):

| fcn | opcode 0x45 | role | RX | where it reads the response |
|-----|-------------|-------|----|-----|
| **`0x54d0`** | **0x02** | WRITE (PIO, 70B chunk) | `0x4B = 0` | **does not read** |
| **`0x4684`** | **0x02** | WRITE inside SEQUENCE | `0x4B = 0` | **does not read** |
| **`0x3c20`** | **0x0B** | READ (rest PIO + bulk via DMA) | `0x4B = rx_len` | FIFO **+3** (`0x80+TX_COUNT`) |
| **`0x4bac`** | **0x0B** | READ with speed-config (PIO, 64B chunk) | `0x4B = rx_len + 1` | FIFO **+4** (`0x80+4`) |

Callers: `0x6004→54d0`, `0x602d/0x60c1→3c20`, `0x60dc/0x61ad→4684`, `0x5f26→4bac`. The SPB
sequence (0x6040) treats each segment separately: `+0x48==1`→read `3c20`, `+0x48==2`→write `4684`.

**No function with opcode 0x02 ever sets RX_COUNT>0 or re-reads the FIFO.** The only combined
TX+RX reads in a single trigger are the two READ paths (opcode 0x0B), with a fixed TX = 3 bytes.

---

## (A) Write path `0x54d0` (opcode 0x02) — MMIO sequence only

Prologue: `ctx = get_context(); var_48=ctx`. `num_chunks = ceil(tx_len/70)` (`0x55a1`, div 0x46).
`tx_len` = arg3.

```
[once before the loop]
  6fc0(dev)                          ; saves read16(BASE+0x22) -> ctx+0x58          @0x5673
  a = read8 (BASE+0x1D)              ; ALT_CS                                        @0x5687
  a = (a & 0xFC) | 0x01
  write8(BASE+0x1D, a)               ;                                               @0x56bd

[loop over each chunk, var_80 = 0..num_chunks-1]                                     @0x56cf
  c = read32(BASE+0x00)              ; CTRL0                                         @0x56f8
  c |= (1<<20)                       ; bts 20  (FIFO clear) — a SINGLE write         @0x570b
  write32(BASE+0x00, c)                                                             @0x5725
  write8 (BASE+0x45, 0x02)           ; OPCODE (1st time)                             @0x573b
  c = read32(BASE+0x00)              ; CTRL0                                         @0x5748
  ; (saves current bit30/29/18 into var_7c — never reused)
  c |= 0x60000000                    ; bit30+29                                     @0x5771
  c |= (1<<18)                       ; bts 18                                        @0x577e
  write32(BASE+0x00, c)              ; "secret bits" = 0x60040000, ONCE per chunk   @0x5792
  w = read16(BASE+0x44)              ; speed/opcode word                            @0x57a6
  n = read8(BASE+0x20) & 0x0F        ; ENA nibble                                    @0x57b5
  w = (w & 0xF0FF) | (n<<8)
  w = (w & 0x0FFF) | (n<<12)         ; both high nibbles = n
  write16(BASE+0x44, w)              ; writes 0x44 (also overwrites 0x45!)          @0x581c
  chunk = min(rest,70)               ; rest starts at tx_len
  write8 (BASE+0x48, chunk)          ; TX_COUNT                                     @0x5868
  for i in 0..chunk-1:
      write8(BASE+0x80+i, txbuf[base+i]) ; FIFO fill                                @0x58bf
  write8 (BASE+0x4B, 0x00)           ; RX_COUNT = 0  <<< no reception               @0x5943
  write8 (BASE+0x45, 0x02)           ; OPCODE (2nd time, after 0x44)                @0x5959
  s = read8(BASE+0x4C)               ; STATUS (byte!)                               @0x596d
  retries = 30 (0x1E)
  ; loop: while ((s>>31)&1) && retries: read8(BASE+0x4C); KeDelay; retries--        @0x59e9
  if !((s>>31)&1):                                                                  @0x5aa2
      write8(BASE+0x47, 0x80)        ; TRIGGER (bit7)                               @0x5ac1
      ; loop: while ((s>>31)&1) read8(BASE+0x4C)                                    @0x5ac6
  ; -> next chunk

[once after the loop]                                                              @0x5b2f
  a = read8(BASE+0x1D); a &= 0xFC; write8(BASE+0x1D, a)   ; ALT_CS (NO OR 1)        @0x5b68
  6f84(dev)                          ; restores write16(BASE+0x22, ctx+0x58)        @0x5b75
```

No FIFO read. RX_COUNT always 0. **Write = TX only.**

---

## (B) Read path `0x3c20` (opcode 0x0B) — PIO "remainder" branch (rx_len < 64)

`ctx = get_context(); var_50=ctx`. The 64B-aligned part goes via **DMA** (`fcn.0x3528`, which does NOT
touch MMIO: it builds an 'AeiC'/'ALDT' descriptor packet and sends it via IOCTL `0x32c004`,
`fcn.0x7c48`). The **remainder** `rx_len = len%64` (`var_60`) is handled in PIO below:

```
  6fc0(dev)                          ; saves 0x22                                   @0x3dea
  read8(BASE+0x4C)                   ; STATUS (byte) -> global                      @0x3dfe
  ALT_CS: read8(0x1D); &0xFC; |0x01; write8(0x1D)                                   @0x3f11..3f47
  read8(BASE+0x1D)                   ; (re-read, saved)                             @0x3f5b
  ; [if a bulk part var_68 exists] write8(0x45,0x0B); read8(0x45); DMA fcn.0x3528    @0x3f9a
  ; ---- REMAINDER branch (var_60 = rx_len) @0x414e ----
  write8 (BASE+0x45, 0x0B)           ; OPCODE (1st)                                 @0x416a
  read8  (BASE+0x45)                                                               @0x417e
  c = read32(BASE+0x00); c|=(1<<20); write32(BASE+0x00, c)  ; FIFO clear (1 set)    @0x41b8
  read32(BASE+0x00)                  ; (re-read)                                    @0x41c5
  c = read32(BASE+0x00); c|=0x60000000; c|=(1<<18); write32(BASE+0x00,c) ; secret   @0x4232
  write8 (BASE+0x48, 0x03)           ; TX_COUNT = 3 (fixed)                         @0x4248
  for _ in 0..2: write8(BASE+0x80, 0x00)  ; 3 dummy bytes in FIFO[0] (same addr)    @0x427b
  write8 (BASE+0x4B, rx_len)         ; RX_COUNT = rx_len (exact)                    @0x4295
  write8 (BASE+0x45, 0x0B)           ; OPCODE (2nd, after)                          @0x42ab
  read8  (BASE+0x45)                                                               @0x42bf
  retries = 10 (0x0A)
  s = read8(BASE+0x4C)               ; STATUS (byte)                               @0x42f1
  ; pre-trigger loop: while ((s>>31)&1) && retries: read8(0x4C); KeDelay; retries-- @0x42ff
  if !((s>>31)&1):                                                                  @0x4376
      write8(BASE+0x47, 0x80)        ; TRIGGER                                      @0x4397
      ; post-trigger loop: while ((s>>31)&1) read8(0x4C)                            @0x439c
  for i in 0..rx_len-1:                                                             @0x43de
      resp[i] = read8(BASE + i + 0x83)   ; <<< reads FIFO at 0x80 + TX_COUNT(3)     @0x4410/0x4447
  ALT_CS: read8(0x1D); &0xFC; write8(0x1D)   ; NO OR 1                              @0x45f2
  6f84(dev)                          ; restores 0x22                               @0x45ff
```

Variant `0x4bac` (READ with speed-config, 64B chunk, used by the `0x5f26` wrapper): identical BUT
- writes **0x44** (speed) like the write path (`0x4fd6/0x5047/0x5060`, write16 `0x7ba0`);
- `write8(0x4B, rx_len + 1)`  — RX_COUNT = rx_len **+1** (`inc eax` @0x5100);
- re-reads the FIFO at **`BASE + i + 0x84`** (offset **+4**) (`0x52fe`, read8).

---

## (C) Diff write(0x54d0) vs read(0x3c20 / 0x4bac)

| step | write `0x54d0` (0x02) | read `0x3c20` (0x0B) | read `0x4bac` (0x0B) |
|-------|-----------------------|----------------------|----------------------|
| ALT_CS 0x1D pre | read/&0xFC/\|1/write | same | same |
| CTRL0 FIFO-clear (bit20) | 1 read+1 set+1 write | same | same |
| CTRL0 secret 0x60040000 | 1×/chunk | 1× | 1× |
| **0x44 speed** | **YES** (16-bit RMW) | **NO** | **YES** |
| opcode 0x45 | 0x02 ×2 | 0x0B ×2 | 0x0B ×2 |
| TX_COUNT 0x48 | = chunk (≤70) | = **3** fixed | = **3** fixed |
| FIFO TX | real data, `0x80+i` | 3×0 at `0x80` | 3×0 at `0x80` |
| **RX_COUNT 0x4B** | **0** | **rx_len** | **rx_len + 1** |
| opcode 0x45 (2nd) | 0x02 | 0x0B | 0x0B |
| STATUS poll 0x4C | read8, bit31 | same | same |
| TRIGGER 0x47 | 0x80 | 0x80 | 0x80 |
| **FIFO read** | **none** | `0x80 + TX_COUNT` = **0x83** | `0x80 + 4` = **0x84** |
| ALT_CS post | &0xFC (no \|1) | same | same |
| 0x22 save/restore | 6fc0/6f84 | same | same |

Substantial differences: **only the read path reads the FIFO**; the write path forces RX_COUNT=0. The two
read variants **do not agree** on the re-read offset (+3 vs +4) or on RX_COUNT (exact vs +1).

---

## (D) Answers to the 8 questions (with evidence)

**1. Does the write path 0x54d0 (opcode 0x02) set RX_COUNT>0? Does it read the FIFO after the trigger?**
NO and NO. `write8(BASE+0x4B, 0)` at **0x140005943** (`xor edx,edx`). After the trigger (0x5ac1) there is only
a STATUS poll; **no** `read8(BASE+0x8x)`. The write is purely TX.

**2. Are TX and RX two separate controller executions?**
YES, at the SPB level. The write (0x02) and the read (0x0B) are distinct functions/WDF requests with two
separate triggers (`0x5ac1` for the write, `0x4397`/`0x528e` for the read). A DESCREQ of the type
"write 9 bytes + read 10 bytes" = **two transactions**: a WRITE 0x02 (TX=9, RX=0) and a READ 0x0B
(TX=3 dummy, RX=10). Note: **inside** a single READ transaction, TX(3)+RX(N) happen in
**a single trigger** (the controller places the response in the same FIFO after the TX bytes).

**3. Does the response arrive in the SAME FIFO? At what offset?**
Same FIFO. Offset = **`0x80 + TX_COUNT`**:
- `0x3c20`: TX_COUNT=3 → reads at **0x83** (`lea rax,[rax+rcx+0x83]`, **0x140004410 / 0x140004447**).
- `0x4bac`: reads at **0x84** = 0x80+4 (**0x1400052fe**), consistent with its RX_COUNT=rx_len+1.
The write path instead writes the TX at `0x80+i` (0x58bf) and never re-reads anything.

**4. Full ordered sequence (write vs read).** See (A) and (B). Key differences in (C).

**5. ALT_CS 0x1D: read8 → AND 0xFC → OR 0x01 → write8 pattern?**
YES, exactly, in **all** transfer functions:
- 0x54d0: OR 1 pre (`0x140005695`/`0x1400056a3`), only AND 0xFC post (`0x140005b4c`).
- 0x3c20: `0x140003f1f`/`0x140003f2d` (OR 1), post `0x1400045d6` (AND only).
- 0x4684: `0x140004b04` … ; 0x4bac: `0x140004e5b`+.
The `| 0x01` appears **only on entry**; on exit bits 1-0 are zeroed (`& 0xFC`) without setting 1 back.

**6. Writes to 0x49 / 0x4A ("strobe")?**
**DO NOT EXIST anywhere in the binary.** Exhaustive byte search for `add rax,0x49/0x4a`,
`add rcx,0x49/0x4a`, `lea/mov/[rax+0x49]`, `[rax+0x4a]`: **zero occurrences**. The "3 bytes" of the read
path all go to `BASE+0x80` (FIFO), not to 0x49/0x4A. The register offsets actually used are
only: 0x1D, 0x20, 0x22, 0x44, 0x45, 0x47, 0x48, 0x4B, 0x4C, 0x80+.

**7. CTRL0 "secret bits" (0x60040000 = bit30+29+18): where and how many times? bit 21?**
Applied **only once per segment/chunk**: `or 0x60000000` + `bts 18` followed by a single
`write32(CTRL0)` (write: **0x140005771/577e/5792**; read 3c20: **0x140004209/421a/4232**;
4bac: **0x140004fa0/4fb1/4fc9**). **There is no** second application ("secret again"). The code
saves the pre-existing bits 30/29/18 into a variable (`var_7c`, e.g. 0x140005755-69) but never reuses it.
**Bit 21 (0x200000): never read/set/cleared.** It is only *preserved by omission* (the driver ORs
0x60040000 onto what's there, so the hardware value of bit21 survives). Searching for `0x800000`
(bit23 TXMODE) and `0x200000`: **no** explicit manipulation in any of the transfer functions.

**8. Register 0x44: written in the write path? value? order vs 0x45?**
- Write `0x54d0`: **YES**. `read16(0x44)` (0x57a6) → RMW → `write16(0x44, w)` (0x581c). Formula:
  `w = (w & 0xF0FF)|((ENA&0xF)<<8)` then `w = (w & 0x0FFF)|((ENA&0xF)<<12)` (ENA = read8(BASE+0x20)&0xF).
  Order: **AFTER** the first `write8(0x45,0x02)` (0x573b) and after the secret bits.
- **Second opcode 0x45 write after 0x44: YES.** `write8(0x45, 0x02)` again at **0x140005959**
  (because the write16 to 0x44 also overwrites byte 0x45). Same for read: 2nd write at 0x42ab (3c20)
  / 0x511d (4bac).
- Read `0x3c20`: does **NOT** write 0x44. Read `0x4bac`: **DOES** write 0x44 (like the write path).

---

## (E) VERDICT — what the Linux driver must do for TX+RX like Windows

**Point 1 — conceptual model.** Windows **never** reads the response inside an opcode 0x02
transaction. A DESCREQ (write command + read response) is **two separate SPI transactions**:
1. WRITE: opcode `0x45=0x02`, `TX_COUNT(0x48)=N`, FIFO[0..N-1]=payload, `RX_COUNT(0x4B)=0`, trigger.
2. READ:  opcode `0x45=0x0B`, `TX_COUNT(0x48)=3`, FIFO[0..2]=0 (dummy), `RX_COUNT(0x4B)=M`, trigger,
   then re-read the FIFO. If your driver already does these as two `spi_transfer`/messages, you're aligned.

**Register sequence for a combined READ (the only TX+RX-in-one-trigger case Windows uses):**
```
1.  read8(0x1D); &=0xFC; |=0x01; write8(0x1D)        ; select CS
2.  ctrl0=read32(0x00); ctrl0|=BIT(20); write32(0x00) ; FIFO clear — a SINGLE set (no toggle)
3.  write8(0x45, 0x0B)                                ; opcode
4.  ctrl0=read32(0x00); ctrl0|=0x60040000; write32(0x00) ; secret bits (once)
5.  (optional, like 0x4bac) read16(0x44); RMW ENA nibble; write16(0x44); then RE-write8(0x45,0x0B)
6.  write8(0x48, TX_COUNT)                            ; = 3 (dummy command/address bytes)
7.  FIFO[0..TX_COUNT-1] = TX bytes (Windows writes 0x00)
8.  write8(0x4B, RX_COUNT)                            ; = M
9.  write8(0x45, 0x0B)                                ; RE-write opcode (mandatory if step 5 was done)
10. write8(0x47, 0x80)                                ; trigger
11. wait for busy   [see note below]
12. read the response from the FIFO at  0x80 + TX_COUNT ; (Windows 3c20 = +3; 4bac = +4 with RX_COUNT+1)
```

**Point 2 — re-read offset + RX_COUNT (source of the +3 vs +4 confusion).** The two variants are
self-consistent: the useful response starts at `0x80 + TX_COUNT + skip`, where `skip = RX_COUNT - rx_len`.
- `0x3c20`: TX_COUNT=3, RX_COUNT=rx_len (skip=0) → **offset 0x83**.
- `0x4bac`: TX_COUNT=3, RX_COUNT=rx_len+1 (skip=1) → **offset 0x84**.
Robust rule for Linux: **read at `FIFO + TX_COUNT`** and program **`RX_COUNT = rx_len`** (the
3c20 variant, the simplest and most consistent one). The fixed `+4` offset in your `SPI_REGISTERS.md`
only works because so far TX_COUNT is 3 and you use RX_COUNT+1 (4bac variant); it is not a universal constant.

**Point 3 — busy poll (IMPORTANT).** Windows's "wait busy" loops read `0x4C` with **read8** and
test **bit 31** of an 8-bit value → the condition `(byte>>31)&1` is **always 0**: the polls are effectively
**no-ops** (no real waiting either before or after the trigger; the real delay comes from the interposed
WPP trace calls). Evidence: read8 at 0x4C in 0x596d/0x59a2 (54d0), 0x42f1/0x4325 (3c20),
0x5165/0x51de/0x52a4 (4bac); `shr 0x1f` test in 0x59e9/0x5aa6, 0x42ff/0x437c, 0x51a3-etc.
Practical consequence: do **NOT** model completion on `STATUS(0x4C) bit31`. Your driver already does
the right thing polling `CTRL0(0x00) bit31` (the real busy). Keep that + a post-trigger `udelay`.

**Point 4 — details to replicate/avoid:**
- Secret bits `0x60040000`: once per segment, **after** the opcode, **before** TX/RX count. Once per message is fine.
- FIFO clear (bit20): **a single set**, not a toggle clear→set. (If your HW requires the 0→1 edge, that's a discovery *you* made on the Linux side, not present in the Windows driver.)
- TXMODE bit23 and bit21: **the Windows driver never touches them**. If you need bit23 for non-0x0B opcodes, that remains a hardware discovery of yours — keep it separate from the "facts from decomp".
- ALT_CS: `|0x01` only on open, only `&0xFC` on close.
- 0x22: save before / restore after (wraps the whole message/sequence, not the individual segment).

---

## (F) Errors found in the existing documentation

### `AMDSPI_DECOMP.md`
1. **Wrong function index:** `0x3528` does NOT "handle TX+RX under the same CS" — it doesn't touch any MMIO,
   it builds an 'AeiC'/'ALDT' DMA descriptor and sends it via IOCTL `0x32c004` (`fcn.0x7c48`). `0x4684`
   is NOT "cleanup": it's a **WRITE handler (opcode 0x02) inside an SPB sequence** (RX_COUNT=0). `0x54d0`
   is the full WRITE handler, not a simple wrapper that "calls 0x6f84".
2. **`0x4bac` sequence, step 4 (FIFO clear "clear→set / rising edge"): FALSE.** The code does a
   single `read32; bts 20; write32` (0x4f37/…). No preventive clear write.
3. **Step 12 "Secret bits AGAIN": FALSE.** The secret bits are applied **only once** per
   segment. There is no second write.
4. **Step 14 "while (read32(CTRL0) & BIT(31))": doubly wrong.** The poll reads **0x4C** (not
   CTRL0) with **read8** (not read32); testing bit31 of a byte makes the loop a **no-op**. The real busy
   bit is CTRL0(0x00) bit31, but the Windows driver **never polls it**.
5. Summary "TX+RX with the same opcode in a single operation (0x02 TX + 0x02 RX)": **FALSE.** With
   opcode 0x02 RX_COUNT is always 0. The TX+RX combination in one trigger exists **only** with opcode 0x0B.
6. Correct instead: restore of 0x22 (not 0x44), 0x1D strobe, secret bits (30/29/18), no TXMODE,
   speed on 0x44 with opcode re-write.

### `SPI_REGISTERS.md`
1. **0x49 / 0x4A "STROBE_V2 — write 0x00": NOT confirmed by the decomp.** amdspi.sys **never** writes
   to 0x49/0x4A. The section "V2 Path … Write 0x00 to STROBE 0x49/0x4A" and the table row "Strobe 0x49,
   0x4A" describe *your* Linux driver's behavior, not Windows's. Should be marked as such.
2. **STATUS 0x4C "bit31 = busy on V2 (ioread32)": misleading.** Windows reads it as a **byte** and its
   poll is ineffective. The reliable busy indicator is CTRL0 bit31. The table lists 0x4C as 32-bit; Windows treats it
   as 8-bit.
3. **Bit 21 "PRESERVED — Windows preserves it (reads and keeps it)": imprecise.** Windows doesn't read it or
   deliberately mask it; it does `|= 0x60040000`, so bit21 survives purely by omission. Observed values
   like `0x6fac100b`/`0x6f8c080b` are hardware state, not written by the driver.
4. **FIFO "RX read … offset +4 … constant": only partially true.** It's +4 in the `0x4bac` variant
   (which uses RX_COUNT+1); in the `0x3c20` variant it's **+3 = 0x80+TX_COUNT** with exact RX_COUNT. The
   correct offset is `0x80 + TX_COUNT (+skip)`, not a fixed +4 constant.
5. **CTRL0 bit20 "requires a 0→1 transition / clear→set": not what Windows does** (a single set). If
   this is a requirement, it's a Linux-side discovery.
6. Correct: opcode at 0x45, trigger 0x47 bit7, secret bits 30/29/18, 0x44 as speed+opcode 16-bit with the
   need to re-write the opcode, FIFO base map at 0x80.
