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

## Discrepanza indirizzi (probabile causa di malfunzionamento)
- Registro di **input** osservato: `0x000000`
- Registro di **output** osservato: `0x000004`
- Il driver Linux usa come default hardcoded `SPI_HID_DEFAULT_INPUT_REGISTER = 0x1000` prima di leggere il descrittore — se non legge l'indirizzo reale da ACPI `_DSM` (funzioni 1/2/3) *prima* della primissima read-approval, la sincronizzazione iniziale con questo hardware fallisce.
- **Azione consigliata**: verificare/patchare `spi_hid_probe()` in `spi-hid-core.c` perché usi l'indirizzo letto da ACPI anche per il primissimo `read_approval`, non solo per `hid_desc_addr`.

## Report Descriptor completo (estratto, non serve WinDbg)
Estratto direttamente dall'evento ETW `Microsoft-Windows-Input-HIDCLASS` (informativo, logga il descrittore di ogni dispositivo HID all'avvio della trace). Lunghezza: 936 byte.
File allegati: `mshw0231_report_descriptor.bin` (grezzo), `mshw0231_report_descriptor.txt` (decodificato con `hiddump.py`, incluso).

Contenuto rilevante:
- **Collection Pen standard** (Report ID 1): InRange/TipSwitch/BarrelSwitch/Invert/Eraser + X/Y + TipPressure — Digitizer HID puro, nessun problema atteso per Linux.
- **Collection TouchScreen standard** (Report ID 0x40 / 64): TipSwitch + X/Y — anche questo HID puro.
- **~10 Report ID "grezzi"** sotto Digitizer/vendor page (6, 7, 8, 10, 11, 12, 13, 26, 28...): campo 16 bit + blocco enorme marcato "costante" (da 11 a 7485 byte). Report ID 6 è un **Feature "DeviceMode"** (119 byte) — probabile switch di modalità.

## Cosa manda davvero il dispositivo durante il touch (dati osservati)
Su 549 report DATA catturati in 15 secondi di tocchi:
- **537 (98%) → Report ID 12**, blob grezzo da **4299 byte** (2 byte "SurfaceSwitch" + 4297 byte marcati costanti)
- 13 → Report ID 8, blob grezzo da 211 byte
- **0 → Report ID 0x40** (il report TouchScreen standard con X/Y non è mai stato inviato)

**Conclusione**: il digitizer, così com'è, trasmette **frame grezzi stile Intel IPTS**, non coordinate elaborate. Un driver di solo trasporto SPI-HID (anche perfetto) non basta: serve probabilmente un demone userspace (tipo `iptsd`) che interpreti i frame da 4299 byte e ne estragga le coordinate delle dita — a meno che il Feature Report ID 6 "DeviceMode" non permetta di forzare il dispositivo a inviare direttamente il Report ID 0x40.

## Prossimo passo
Catturare l'inizializzazione completa del driver (dal boot o da un reset del dispositivo in Gestione dispositivi → Disabilita/Abilita), per vedere l'eventuale `SET_FEATURE` su Report ID 6 (DeviceMode) che Windows invia prima di iniziare a ricevere i frame — questo rivelerebbe se esiste un modo per richiedere la modalità "contatti elaborati" invece del flusso raw.
