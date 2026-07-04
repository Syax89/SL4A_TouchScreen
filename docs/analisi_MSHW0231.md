# Analisi HID-over-SPI — Surface touchscreen (ACPI\MSHW0231)

## Identificazione hardware
- Bus: HID-over-SPI (non I2C, non USB classico)
- ACPI HID: `MSHW0231`, driver Windows: `hidspi.sys` ("SPI HID Device")
- Vendor/Product/Version ID: 0x045E / 0xC19 / 0x4
- Protocollo pubblico: "HID Over SPI Protocol Specification v1.0" (Microsoft, Community Promise) — allegato `HidSpiProtocolSpec.pdf`

## Driver Linux esistente
`linux-surface/spi-hid` (adattato dai sorgenti Android del Surface Duo 2).
Match ACPI attuale: `MSHW0134`, `MSHW0162`, `MSHW0235`, `PNP0C51` (generico).
`MSHW0231` non è elencato esplicitamente ma può agganciarsi tramite il `_CID` generico `PNP0C51` se il DSDT lo espone.

## Verifica protocollo via cattura ETW (SPB-ClassExtension + GPIO-ClassExtension)
Byte osservati sul bus SPI, confrontati con le costanti in `spi-hid-core.h`:

| Campo | Valore osservato | Costante driver Linux | Match |
|---|---|---|---|
| Read approval opcode | 0x0B | `SPI_HID_READ_APPROVAL_OPCODE_READ` | OK |
| Read approval constant | 0xFF | `SPI_HID_READ_APPROVAL_CONSTANT` | OK |
| Output write opcode | 0x02 | `SPI_HID_OUTPUT_HEADER_OPCODE_WRITE` | OK |
| Input header sync byte | 0x5A | `SPI_HID_INPUT_HEADER_SYNC_BYTE` | OK |
| Input header version | 0x02 | `SPI_HID_INPUT_HEADER_VERSION` | OK |

**Il protocollo software è quindi confermato conforme alla spec pubblica.**

## Report Descriptor completo (estratto da evento ETW HIDCLASS, non serve WinDbg)
Lunghezza: 936 byte. File allegati: `mshw0231_report_descriptor.bin`, `mshw0231_report_descriptor.txt` (decodificato con `hiddump.py`).

Contenuto rilevante:
- **Collection Pen standard** (Report ID 1): InRange/TipSwitch/BarrelSwitch/Invert/Eraser + X/Y + TipPressure — Digitizer HID puro.
- **Collection TouchScreen standard** (Report ID 0x40 / 64): TipSwitch + X/Y — HID puro, ma **mai osservato sul bus** in nessuna delle catture.
- **~10 Report ID "grezzi"** sotto Digitizer/vendor page (6, 7, 8, 10, 11, 12, 13, 26, 28...): campo 16 bit + blocco enorme marcato "costante" (da 11 a 7485 byte). Report ID 6 è un **Feature "DeviceMode"** (119 byte).

## Cosa manda davvero il dispositivo (confermato su 3 catture: touch a caldo, disable/enable, boot a freddo)
- **~90% dei report DATA → Report ID 12**, blob grezzo da **4299 byte** (2 byte "SurfaceSwitch" + 4297 byte marcati costanti)
- Una minoranza → Report ID 8, blob grezzo da 211 byte
- **0% → Report ID 0x40** (il report TouchScreen standard con X/Y non viene mai inviato)

**Conclusione**: il digitizer trasmette **frame grezzi stile Intel IPTS**, non coordinate elaborate. Serve un demone userspace (tipo `iptsd`) oltre al driver di trasporto.

## Mappa registri SPI (osservata su 3 catture indipendenti)

| Registro | Uso osservato |
|---|---|
| `0x0` | **Solo read-approval** (opcode `0x0B`). Mai una write. Indirizzo di input dedicato — questo è il "ctrl0". |
| `0x1` | Write COMMAND, content_id=0 — parte dell'handshake subito dopo il reset |
| `0x2` | Write COMMAND, content_id=0 — idem |
| `0x3` | Canale Feature: `GET_FEATURE` su Report ID 6 (DeviceMode), `SET_FEATURE` su Report ID 5 e Report ID 86 (0x56) |
| `0x4` | Richiesta Device Descriptor |

Il driver Linux usa come default hardcoded `SPI_HID_DEFAULT_INPUT_REGISTER = 0x1000` prima di leggere il descrittore — con questo hardware il registro reale è `0x0`, quindi se `spi_hid_probe()` non legge l'indirizzo vero da ACPI `_DSM` *prima* della primissima read-approval, la sincronizzazione iniziale fallisce.

## Nessun comando di cambio modalità
In tutta la sequenza di boot, Windows esegue **solo un `GET_FEATURE` sul Report ID 6 (DeviceMode)**, mai un `SET_FEATURE` per modificarlo. Non esiste quindi un comando host-side che forza la modalità "contatti elaborati": il dispositivo parte già configurato (verosimilmente da firmware) per streammare frame grezzi.

## Prossimo passo
1. Verifica diretta di `_DSM` per MSHW0231 via WinDbg (`!amli de \_SB.<path>.MSHW0231._DSM` con debug kernel locale attivo) per confermare *perché* la mappa registri è questa (le funzioni 1/2/3 del `_DSM` dovrebbero restituire questi stessi indirizzi).
2. Decodifica completa del payload del Report ID 12 per capire il formato del frame grezzo (dimensioni griglia, bit per cella, eventuale header di calibrazione) — necessaria per scrivere l'equivalente di `iptsd` per questo hardware.
