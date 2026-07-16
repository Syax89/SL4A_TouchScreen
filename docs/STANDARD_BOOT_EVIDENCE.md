# Standard Boot Evidence Matrix

This matrix defines allowed standard HID boot behavior for AMDI0060 and
MSHW0231. Windows ETW and cleaned decompilation are the primary sources.

| Action | Evidence | Standard decision |
| --- | --- | --- |
| Map ACPI MMIO resource | `hidspi_344c_EvtPrepareHardware.txt` | Keep |
| Select CS1 | AMD transfer decomp | Keep |
| Configure 33.33 MHz from ACPI device speed | AMD transfer setup | Keep |
| Set FIFO_CLEAR once per V2 segment | `0x4bac.txt`, `0x54d0_submit_handler.txt` | Keep |
| Set FAST_READ and opcode at `0x45` | `0x4bac.txt`, `0x54d0_submit_handler.txt` | Keep |
| Apply `0x44` RMW and rewrite opcode | `0x4bac.txt`, `0x54d0_submit_handler.txt` | Keep |
| Trigger V2 with `0x47 = 0x80` | AMD transfer decomp | Keep |
| Restore `0x22` and release CS | `0x6f84_restore_register_prefix.txt` | Keep |
| Continue a 64-byte PIO read with TX_COUNT=3 and FIFO `+0x84` | `0x4bac.txt` | Keep |
| PCI LPC writes at `B4`/`B8` | No clean decomp evidence | Exclude |
| MMIO HOST_PREF override at `0x2c` | No clean controller evidence | Exclude |
| Probe FIFO clear toggle | No clean decomp evidence | Exclude |
| Forced bus speed or TX-count experiment | No clean decomp evidence | Exclude |
| Probe `_PS3` to `_PS0` cycle | Not present in ETW standard enumeration | Disabled by default |
| GPIO IRQ then `0B 00 00 00 FF` read approval | `CSV_SEQUENCE.md` | Required |
| RESET_RSP, DESCREQ, DEVICE_DESC, DESCREQ2, RPT_DESC | `CSV_SEQUENCE.md` | Required order |
| Raw GET/SET feature or calibration | Not standard enumeration | Forbidden in standard mode |

The controller candidate uses the captured 9-byte response prefix with the V0
header at offset 5. Its descriptor continuation must retain the same TX_COUNT
and FIFO read offset as the Windows PIO path; adding a trailing dummy byte
changes that hardware framing and corrupts the descriptor.
