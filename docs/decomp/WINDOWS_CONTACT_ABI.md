# Windows Heat Contact ABI

## Finalized Frame

`HeatDevice::FinalizeContactFrame` dispatches a plugin frame to its processor
host. The normalized internal header is 0x28 bytes:

| Offset | Type | Meaning |
| ---: | --- | --- |
| `0x00` | `u64` | ABI size marker, `0x28`. |
| `0x08` | `u64` | frame context/identifier; low `u16` reaches HID headers. |
| `0x10` | `u32` | contact layout type: 1, 2, or 3. |
| `0x14` | `u8` | type-2 frame attribute. |
| `0x18` | pointer | contact array. |
| `0x20` | `u32` | contact count. |
| `0x24` | `u16` | internal type-2 attribute. |

See `heatcore_20260716/functions/1800047f0_FUN_1800047f0.c` and
`18000a510_FUN_18000a510.c`.

## Contact Layouts

| Type | Finalized size | HID contacts per report | HID stride |
| --- | ---: | ---: | ---: |
| 1 | `0x3a` | 5 | `0x21` |
| 2 | `0x3c` | 5 | `0x23` |
| 3 | `0x48` | 1 | `0x1d` |

The plugin base contact is 0x32 bytes. HeatCore prefixes it with the finalized
size marker and maps the base contact ID, flags, four X/Y coordinate pairs, and
trailing properties into the Type-1 record. Type 2 adds a `u16` at `+0x3a`;
Type 3 adds geometry/status extension fields through `+0x46`.

The exact semantic order of the four coordinate pairs is unresolved. Translation
code proves that offsets `+0x0b/+0x13/+0x1b/+0x23` are X-like and
`+0x0f/+0x17/+0x1f/+0x27` are Y-like, but does not prove whether they are
corners, bounds, prediction points, or history.

## Type-1 HID Serialization

`HeatHidInjectionProvider::InjectHeatContactFrame` batches up to five Type-1
contacts. The report starts with type byte `1`, count byte, and low `u16` of
the frame identifier. Each `0x21`-byte contact serializes:

| HID offset | Finalized source |
| ---: | --- |
| `0x00` | flags at `+0x0a` |
| `0x01` | contact ID at `+0x08` |
| `0x03`, `0x07` | X-like fields at `+0x13`, `+0x23` |
| `0x0f`, `0x13` | Y-like fields at `+0x17`, `+0x27` |
| `0x0b`, `0x17`, `0x1b`, `0x1d` | unresolved properties at `+0x2b`, `+0x2f`, `+0x33`, `+0x35` |

Evidence: `heatcore_20260716/functions/180015ff0_FUN_180015ff0.c`.

## Boundaries

This ABI describes Windows virtual-HID output, not the Linux event ABI. Linux
should emit standard Type-B events from a stable internal contact model rather
than copy these packed formats. The recovered fields are an oracle for contact
identity, geometry and lifecycle tests, not a reason to emulate Windows HID
injection.
