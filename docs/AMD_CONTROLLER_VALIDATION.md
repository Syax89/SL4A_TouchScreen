# AMD SPI Controller Validation Boundary

This driver currently targets the AMDI0060 controller in the Surface Laptop 4
AMD. The items below are deliberately not generalized until hardware traces
verify them.

## Verified Device-Specific Behavior

| Behavior | Evidence | Driver Policy |
|---|---|---|
| Touchscreen transport uses opcode `0x0b` reads and `0x02` TX-only writes | Windows ETW and decompilation | Supported |
| Controller FIFO holds 70 bytes | MMIO and Windows driver analysis | Chunk reads/writes |
| Windows uses controller CS encoding `1` | amdspi.sys decompilation | Retained as AMDI0060 quirk |
| `HOST_PREF=0x8000d4c0` appears in Windows capture | MMIO comparison | Kept behind `force_host_pref` |

## Not Yet Validated

| Area | Why it must not be changed blindly |
|---|---|
| Linux CS index to controller CS encoding | The controller currently always emits Windows encoding 1; its relationship to SPI-core chip-select numbers is unknown. |
| `SPI_CS_HIGH`, CPOL and CPHA support | The driver does not program those electrical modes even though generic SPI capabilities were historically advertised. |
| Post-trigger completion source | CTRL0 busy polling is known to work for the touch path; STATUS width/bit semantics are not established. |
| PCI B4/B8 restoration | Firmware may require the current layout across driver lifetime. Capture before/after cold boot and remove first. |
| Speed-register reset semantics | Windows uses multiple speed tiers; clearing SPI100/SPD7 without trace validation can alter bus timing. |

## Safe Offline Changes Completed

- Removed the unused exported MMIO debug-poll thread. Its global lifetime could
  race controller removal and access unmapped registers.
- Added a bound check before continuation-read address bytes are copied into the
  fixed eight-byte stack buffer.
- Moved hot-path controller diagnostics to debug severity.

## Required Hardware Trace

Capture a cold boot and a subsequent reboot with the driver in standard mode:

1. SPI-core chip-select and ALT_CS register before every transfer.
2. CTRL0, STATUS, TX/RX count and FIFO bytes immediately before trigger and
   after confirmed completion.
3. LPC PCI B4/B8 and HOST_PREF at boot, probe, remove and re-probe.
4. SPI mode, bits-per-word, requested and programmed speed.

Only then should controller capabilities, CS behavior, completion polling, or
persistent PCI register handling be changed.
