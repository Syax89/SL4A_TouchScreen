# HID-over-SPI Protocol — MSHW0231 Touchscreen

## Panoramica

Il touchscreen Surface Laptop 4 AMD usa il protocollo HID-over-SPI (sezione 7.2.5.7 specifica HID SPI).
Il dispositivo è un controller touchscreen che richiede:
1. Sequenza di discovery (reset → descrittori → report descriptor)
2. Attivazione vendor-specific (cmd1/cmd2/cmd3)
3. Upload firmware B0 (120 blocchi da 241 byte)
4. Inizializzazione (~41 secondi)
5. Funzionamento normale (report HID)

## Formato Header HID

Ogni risposta dal dispositivo inizia con sync bytes (0xFF) seguiti dall'header:

```
Byte 0: [TYPE_4bit][VERSION_4bit]
        versione = 2 (HID-over-SPI)
        tipo report:
          0 = ACK/Ready
          3 = RESET_RSP
          7 = DEVICE_DESC  
          8 = RPT_DESC

Byte 1: [RESERVED_4bit][LENGTH_LOW_4bit]
Byte 2: [LENGTH_HIGH_8bit]
Byte 3: 0x5A (marker)

Body length = (((Byte1 >> 4) << 0) | (Byte2 << 4)) * 4
```

### Esempi

```
RESET_RSP:  32 10 00 5A    type=3, body_len = ((1>>4)<<0 | 0x00<<4)*4 = 0
DEVICE_DESC: 72 80 00 5A    type=7, body_len = ((8>>4)<<0 | 0x00<<4)*4 = 0  
RPT_DESC:   82 B0 0E 5A    type=8, body_len = ((0xB>>4)<<0 | 0x0E<<4)*4 = 0xE0*4 = 896

ACK pattern: 03 00 00 00    (non ha 0x5A! Pattern speciale)
```

---

## Comandi SPI

### 0x0B — Read Register (Approval Read)

```
TX: [0x0B] [ADDR_H] [ADDR_M] [ADDR_L] [0xFF] [0x00] [0x00] [APPR7] [APPR8]
      opcode  └──── register address (24bit) ────┘  └pad─┘ └─ approval ─┘

9 byte totali. Il dispositivo risponde con i dati dal registro specificato.

Approval bytes:
  APPR7: 0x00 stato iniziale, 0x03 dopo primo descrittore, 0x0A dopo attivazione
  APPR8: 0x00 default

Indirizzi registro:
  0x000000: registro di input (pre-definito)
  0x001000: input_register (dopo device descriptor parse)
```

### 0x02 — DESCREQ (Device Descriptor Request)

```
TX: [0x02] [0x00] [0x00] [0x01] [0x42] [0x00] [0x00] [0x03] [0x00] [0x00]
      opcode  └───???────┘ └─??─┘ └─???──────────────┘ └─??─┘

10 byte totali.

DESCREQ2 (per report descriptor):
TX: [0x02] [0x00] [0x00] [0x02] [0x42] [0x00] [0x00] [0x03] [0x00] [0x00]
                      ↑ registro 0x02 invece di 0x01

Il dispositivo risponde con ACK (03 00 00 00), poi invia IRQ.
Dopo IRQ, il successivo 0x0B read restituisce il descrittore.
```

### 0x00 — Attivazione (cmd1, cmd2)

```
cmd1: [0x00] [0x0E] [0x00] [0x00] [0x00]    5 byte
cmd2: [0x00]                                   1 byte (NOP/padding)

TX only, nessuna risposta inline.
```

### 0x70 — Read Status (cmd3)

```
TX: [0x70]                                     1 byte
RX: 14 byte di stato firmware

TX+RX nella stessa transazione.
```

### 0xB0 — Firmware Block Write

```
TX: [0xB0] [240 byte di dati firmware]         241 byte totali

TX only, nessuna risposta inline.
120 blocchi + 125 byte di coda = 29.045 byte totali.
```

---

## Sequenza di Boot Completa

### Fase 1: Reset (0-200 µs)
```
┌─ 0x0B read → RESET_RSP (32 10 00 5A)
│  Il dispositivo è in reset, invia il report di reset.
├─ 0x0B read → ACK (03 00 00 00)  
│  Dopo ~168 µs, il dispositivo è pronto.
│  NESSUN comando inviato tra le due letture!
└─ Il dispositivo cambia stato da solo (timeout interno)
```

### Fase 2: Device Discovery (~200-2000 µs)
```
┌─ DESCREQ (0x02) → ACK (03 00 00 00 00)
├─ GPIO IRQ (~58 µs dopo)
├─ 0x0B read → DEVICE_DESC type=7 (72 80 00 5A)
│  approval byte7 cambia a 0x03
├─ 0x0B read 37B → device descriptor data
│  vendor=0x03A8, product=0x0002, ver=0x0320
├─ DESCREQ2 (0x02) → all zeros (device busy)
├─ GPIO IRQ (~727 µs dopo)
├─ 0x0B read → RPT_DESC type=8 (82 B0 0E 5A)
├─ 0x0B read 945B → full HID report descriptor
└─ ~962ms gap (driver processing interno)
```

### Fase 3: Attivazione (~962ms)
```
┌─ cmd1 (0x00, 5 byte)
├─ cmd2 (0x00, 1 byte)  
├─ 0x0B read → checksum response
├─ cmd3 (0x70, TX+RX 14B) → FW status
└─ (possibili altri comandi vendor: 0x28, 0x24, 0x29, 0x22, 0x25, 0x26)
```

### Fase 4: Firmware Upload (~410ms)
```
┌─ B0 block #1 (241 byte)
├─ B0 block #2
├─ ... (120 blocchi totali)
├─ B0 tail (125 byte)
└─ Attesa 41 secondi per inizializzazione dispositivo
```

### Fase 5: Funzionamento (~41s dopo FW)
```
┌─ GPIO IRQ periodici (~ogni 110ms)
├─ 0x0B read → report HID type=7
├─ hid_input_report() → eventi touch
└─ Loop continuo
```

---

## State Machine del Nostro Driver

### Stato Attuale (v5)

```
State 0 (WAIT_RESET):
  ├── type==3: drain #1 (blen byte) + drain #2 (64 byte)
  └── Sempre: DESCREQ (0x02 TX+RX 10B) → state=1 + drain

State 1 (WAIT_DESC):
  ├── type==7: parse device descriptor → DESCREQ2 → state=2 + drain
  ├── type==3: "still RESET_RSP" (log)
  └── type==0: "ACK in state 1" (log, no action)

State 2 (WAIT_RPT):
  ├── type==8: drain → goto VENDOR_INIT
  └── type==3/7: log and retry

State 3 (VENDOR_INIT):
  ├── cmd1 (0x00, 5B) → cmd2 (0x00, 1B) → ACK read → cmd3 (0x70)
  └── schedule fw_work + create_device_work → state=4

State 4 (DONE):
  └── type==7: forward to hid_input_report()
```

### Problemi Noti

1. **DESCREQ non produce type=7**: TX usa opcode 0x02, RX separata usa 0x0B
   - In Windows: TX+RX usano LO STESSO opcode 0x02 nella stessa operazione
   - Da CSV: DESCREQ TX+RX inline restituisce ACK, poi IRQ, poi type=7

2. **Nessun DESCREQ2**: Manca il secondo DESCREQ per il report descriptor
   - Il codice in state 1 HA DESCREQ2 ma non viene mai eseguito (state 1 non vede type=7)

3. **Drain multiplo assente**: Windows fa 2 letture 0x0B prima di DESCREQ
   - Le due letture drenano RESET_RSP e poi ricevono ACK

4. **TX+RX opcode mismatch**: La write 0x02 e la read 0x0B sono operazioni separate
   - Il device interpreta 0x0B come nuovo comando, non come completion di 0x02

### Fix Proposti

**Opzione A**: Usare `exec_segment(opcode=0x02, tx=9, rx=10)` combinato
  - Stesso opcode per TX e RX in una singola operazione
  - Richiede capire il layout FIFO per RX quando TX>0
  
**Opzione B**: TX 0x02 write-only, attendere GPIO IRQ, poi 0x0B read
  - Il device processa DESCREQ e invia IRQ quando pronto
  - La 0x0B read successiva prende la risposta

**Opzione C**: Usare path V1 (CTRL0) per DESCREQ con TX+RX combinato
  - Il path V1 potrebbe gestire RX per qualsiasi opcode
  - Meno restrizioni del path V2 con TXMODE

---

## Driver Windows — hidspicx.sys

Da decompilazione, il driver HID-over-SPI di Windows implementa:

### Funzioni Principali

1. **VerifyResetResponse** (fcn.0xdd64):
   - Legge header RESET_RSP (type=3)
   - Verifica che il body contenga i dati corretti
   - Se valido, avanza lo stato

2. **HidGetDeviceDescriptor** (fcn.0xf458):
   - Invia DESCREQ (opcode 0x02, register 0x01)
   - Aspetta GPIO IRQ
   - Legge device descriptor via 0x0B

3. **VerifyDescriptorCompletion** (fcn.0xd2f8):
   - Verifica che il descrittore sia completo
   - Invia DESCREQ2 (opcode 0x02, register 0x02)
   - Aspetta GPIO IRQ
   - Legge report descriptor

4. **ConfigureTransfer** (fcn.0xa664):
   - Configura i buffer di trasferimento
   - Setta approval bytes (0x00 → 0x03)

5. **PrepareDescriptor** (fcn.0x12bc0):
   - Alloca buffer per descrittore
   - Prepara la struttura SPI_HID_DEVICE_DESC

### Orchestrator (fcn.0xc8d8) — Loop Principale

```
1. Legge il tipo di report dal buffer
2. Switch sul tipo:
   - type==3 (RESET_RSP): avanza stato e invia DESCREQ
   - type==7 (DEVICE_DESC): riceve descrittore, invia DESCREQ2
   - type==8 (RPT_DESC): riceve report descriptor
   - type==0 (ACK) con body_len>0: verifica completamento
3. Se il descrittore è completo, avvia attivazione
4. Loop su IRQ successive
```

### Chiavi di Lettura Decomp

```
r13d = 3       → costante per confronto type==3
r14d = stato flag (0/1)
rdi = type byte dal buffer
cmp dil, r13b  → confronta type con 3
je 0x...       → se type==3, salta al handler RESET_RSP
```

---

## Riepilogo Fix Necessari

| Priorità | Problema | File | Fix |
|----------|---------|------|-----|
| CRITICAL | DESCREQ non produce type=7 | spi-hid-core.c | TX+RX stesso opcode o attendere IRQ |
| CRITICAL | Device non riceve DESCREQ | spi-amd.c | Verificare TXMODE per 0x02 |
| HIGH | Manca drain multiplo | spi-hid-core.c | Due 0x0B read prima di DESCREQ |
| HIGH | State 1 non vede type=7 | spi-hid-core.c | Aggiungere logica per type=7 |
| MEDIUM | Manca DESCREQ2 | spi-hid-core.c | Già presente ma non raggiunto |
| MEDIUM | Manca VENDOR_INIT completo | spi-hid-core.c | Aggiungere comandi vendor |
| LOW | Firmware non testato | spi-hid-core.c | Riattivare fw_work dopo fix |
