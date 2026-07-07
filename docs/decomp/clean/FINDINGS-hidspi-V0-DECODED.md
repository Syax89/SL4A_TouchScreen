# hidspi.sys DECODED — our device uses HidSpiDeviceV0, NOT the v1.0 path

**Date**: 2026-07-03 evening — decomp with real PDB symbols (`idp hidspi.pdb`)
**Updated**: 2026-07-07 (work in progress)
**Status**: the code has been verified and the Linux driver replicates
the MMIO sequence. Investigation of the write path continues.

---

## 1. HOW WINDOWS CHOOSES THE PATH (Fdo::EvtPrepareHardware @ 0x344c -> GetHidSpiAcpiDefinition @ 0x11060)

`AcpiHelper::GetHidSpiAcpiDefinition`:
1. Calls `_DSM(rev=3, func=0)`. If it returns 0x7F -> "full v1.0" definition
2. Otherwise calls `_DSM(rev=1, func=0)`. If it returns != 0 -> "revision 1"
3. Otherwise error.

Our `_DSM (dsdt.dsl:1576)` responds ONLY to `Arg1==1` (revision 1), func0->0x03, func1->1.
**So field0 = 1 -> HidSpiDeviceV0 path.**

---

## 2. ADDRESSES AND OPCODES FOR OUR DEVICE (from HidSpiDeviceV0::Initialize @ 0x792c)

| Parameter | Value | Source |
|-----------|--------|-------|
| **Input Report Address** | **0x000000** | `[dev+0x50]`, default 0 |
| **Output / Descriptor Request Address** | **0x000001** | `[dev+0x68]` = _DSM func1 = 1 |
| **Read opcode** | **0x0B** | literal in ReadingResponse*Entry |
| **Write opcode** | **0x02** | literal in Configure*Entry |

---

## 3. WIRE FORMAT — DERIVED BYTE-BY-BYTE FROM THE DRIVER

### READ header (ReadingResponseHeaderEntry @ 0x7da0), 9 bytes
```
byte0   = 0x0B                     (opcode READ)
byte1-3 = 00 00 00                 (Input Report Address = 0, big-endian)
byte4   = 0xFF                     (pad / dummy)
byte5-8 = <4 bytes header from the device>
```

### READ body (ReadingResponseRemainingEntry @ 0x7e50)
Same opcode 0x0B, SAME address 0x000000, pad 0xFF.
Length = `(header_u16 >> 4) * 4 + 5`.

### WRITE descriptor request (ConfiguringDescriptorTransferEntry @ 0x7230), 10 bytes
```
byte0   = 0x02                     (opcode WRITE)
byte1-3 = 00 00 01                 (Output Address = 1, big-endian)
byte4   = 0x42                     (V2 body: type/version)
byte5   = 0x00
byte6   = 0x00
byte7   = 0x03
byte8   = 0x00
byte9   = 0x00
```
**Full wire = `02 00 00 01 42 00 00 03 00 00`** (10 bytes)

---

## 4. FULL ENUMERATION SEQUENCE (HidSpiDeviceV0 state machine)

1. Reset (ACPI `_RST`: RESET GPIO low 300ms -> high). Device -> IRQ.
2. Read header @ 0x000000 (`0B 00 00 00 FF` + 4 bytes). Expected `32 10 00 5A`.
3. Read body @ 0x000000. Expected `03 00 00 00`.
4. Write descriptor request @ 0x000001: **`02 00 00 01 42 00 00 03 00 00`**
5. Device -> IRQ. Read header+body @ 0x000000 -> device descriptor (32 bytes, type 7).
6. DESCREQ2 @ 0x000002 -> report descriptor -> Ready.

---

## 5. HW TEST OUTCOME (2026-07-07)

The exact sequence has been implemented in the Linux driver (input 0x000000, output 0x000001,
DESCREQ `02 00 00 01 42 00 00 03 00 00`). Reads work, but **the device ignores
writes (opcode 0x02)**.

The investigation continues:
- ACPI tables IDENTICAL between Windows/Linux (DSDT md5sum `78046fa74c0282ee59db8b04a5204d88`)
- Bit-identical MMIO registers (except CTRL0[15:8] hardware-managed and CTRL1 read-only)
- MMIO sequence identical to the Windows decomp
- The AMD FCH Cezanne controller under Linux write path is under investigation

**Next step**: continue debugging the write path asymmetry.

---

## 6. DISASSEMBLED FILES (in decomp/clean/)
- hidspi_11060_GetHidSpiAcpiDefinition.txt — rev1 vs rev3 choice
- hidspi_344c_EvtPrepareHardware.txt — V0 vs Cx choice
- hidspi_792c_V0_Initialize.txt — addresses/opcodes
- hidspi_V0_ConfigureTransfer.txt — write descriptor/power/feature
- hidspi_V0_ReadEntries.txt — read header/body
- hidspi_symbols.txt / hidspicx_symbols.txt — full PDB symbols
