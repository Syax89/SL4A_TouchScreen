# CSV Windows Boot — Sequenza Completa Transazioni SPI

Estratto da `surface_boot_auto.csv` — Trace ETW del boot di Windows su Surface Laptop 4 AMD.

## Statistiche Riepilogative

| Metrica | Valore |
|---------|--------|
| Transazioni SPI totali | 2384 |
| Transfer buffer totali | 3779 |
| GPIO interrupt totali | 676 |
| Durata totale boot | ~88 secondi |
| File sorgente | `surface_boot_auto.csv` |
| Timestamp | 100ns units (ETW Clock-Time) |

### Byte di stato protocollo (approval byte a8)
| Valore | Fase |
|--------|------|
| 0x00 | Init / pre-DESCREQ |
| 0x03 | Post-DESCREQ (descrittori ricevuti) |
| 0x04 | Runtime (post init) |

### Dettaglio operazioni firmware
- **120 blocchi FW (0xB0)** per il dispositivo companion, NON per il touchscreen
- **Second binary upload (0x22)** per il dispositivo companion
- Il touchscreen NON riceve firmware upload

### Device descriptor
- Vendor=0x045E (Microsoft), Product=0x0C19, Version=0x0100

---

## Formato CSV

Ogni riga contiene campi separati da virgola. Campi rilevanti:
- **Colonna 1**: Event Name (SPB-ClassExtension, GPIO-ClassExtension)
- **Colonna 2**: Event Type (IoSpbPayloadTdBuffer, InterruptInvokeDeviceIsr, etc.)
- **Colonna 28**: Clock-Time (timestamp in 100ns units)
- **Colonna 33**: Payload total size
- **Colonna 34**: Transfer count (1 = TX only, 2 = TX+RX)
- **Colonna 37**: Payload hex data

## Convenzioni

- **Opcode**: il primo byte del TX payload è l'opcode SPI
- **Sync bytes**: 5 byte `0xFF` prima dell'header HID
- **Header HID** (RESET_RSP/DEVICE_DESC/RPT_DESC): 4 byte `[TYPE][LEN_LOW][LEN_HIGH][0x5A]`
- **Header dati descrittore** (TXN#8): 3 byte `[TYPE][LEN_LOW][LEN_HIGH]` — 0x75 è il primo byte dati, NON 0x5A
- **Tipo report**: `(TYPE >> 4) & 0xF`, versione: `TYPE & 0x0F` == 2
- **Approval bytes**: byte 7 e 8 del TX payload 0x0B

---

## FASE 1: Reset e Discovery (~0-2000 µs, 8 transazioni)

### TXN #1 — Primo 0x0B Read (linee 63-73)
```
PayloadStart: total=18 bytes, transfers=2
┌─ TX ToDevice:  9 byte ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 00 00                         │
│ │  └──────────────┘ └┘ └─┘ └─┘                    │
│ opcode  addr=0x0000  FF pad  approval7=0x00        │
│           (register 0)          approval8=0x00      │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 byte ────────────────────────────┐
│ FF FF FF FF FF 32 10 00 5A                          │
│ └─sync (5)──┘ └─header──┘                          │
│ TYPE=0x32 → (3<<4)|2 = RESET_RSP type=3            │
│ body_length = (0x10>>4)|(0x00<<4) = 1 → *4 = 4      │
└─────────────────────────────────────────────────────┘
⏱ Durata totale: ~6.3 µs (63 tick da 100ns)
```

### TXN #2 — Secondo 0x0B Read (linee 80-90)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice:  9 byte ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 00 00   (IDENTICO al primo)   │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 byte ────────────────────────────┐
│ FF FF FF FF FF 03 00 00 00                          │
│ └─sync──┘ └─ACK──┘                                 │
│ ACK pattern: 03 00 00 00 (device PRONTO!)          │
└─────────────────────────────────────────────────────┘
⏱ Gap da TXN #1: ~168 µs
⏱ Durata: ~5.5 µs

🔑 KEY INSIGHT: Il device cambia stato tra TXN #1 e TXN #2 
   senza nessun comando! Il solo tempo trascorso (~168 µs) 
   fa sì che il device passi da RESET_RSP ad ACK.
```

### TXN #3 — DESCREQ (linee 97-108)
```
PayloadStart: total=20, transfers=2
┌─ TX ToDevice: 10 byte ─────────────────────────────┐
│ 02 00 00 01 42 00 00 03 00 00                       │
│ │  └──???──┘ └??┘ └??┘ └??┘ └??┘                   │
│ opcode 0x02 = DESCREQ                               │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 10 byte ───────────────────────────┐
│ FF FF FF FF FF 03 00 00 00 00                       │
│ └─sync──┘ └───ACK────┘                             │
│ DESCREQ acknowledged!                                │
└─────────────────────────────────────────────────────┘
⏱ Gap da TXN #2: ~147 µs
⏱ Durata: ~5.6 µs
```

### 🔴 GPIO IRQ (linee 120-121) — Dopo DESCREQ
```
GPIO-ClassExtension: InterruptInvokeDeviceIsrStart
GPIO-ClassExtension: InterruptInvokeDeviceIsrComplete
⏱ Dopo DESCREQ: ~58 µs
⏱ Durata ISR: ~1 µs
Pin: 0x15, Flags: 0x409 (level-triggered active-high)
```

### TXN #4 — 0x0B Read (post-DESCREQ, linee 128-139)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice: 9 byte ──────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00                          │
│                         └─┘                          │
│              approval7=0x03 (CAMBIATO da 0x00!)     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 byte ────────────────────────────┐
│ FF FF FF FF FF 72 80 00 5A                          │
│ └─sync──┘ └─header──┘                              │
│ TYPE=0x72 → (7<<4)|2 = DEVICE_DESC type=7 !!!      │
│ body_length = (0x80>>4)|(0x00<<4) = 8 → *4 = 32     │
└─────────────────────────────────────────────────────┘
⏱ Gap da GPIO IRQ: ~112 µs
⏱ Durata: ~5.4 µs
```

### TXN #5 — 0x0B Read 37 byte (linee 145-155)
```
PayloadStart: total=74, transfers=2
┌─ TX ToDevice: 37 byte ─────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00 00 00 00 00...          │
│                         └─┘                          │
│              approval7=0x03                          │
│ Padding a 37 byte (zero-filled)                     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 37 byte ───────────────────────────┐
│ FF FF FF FF FF 1F 00 00 1C 00 00 01 A8 03 02 00... │
│ └─sync──┘ └───device descriptor data─────────────┘ │
│ vendor=0x03A8, product=0x0002, version=0x0320       │
└─────────────────────────────────────────────────────┘
⏱ Durata: ~5.6 µs
```

### TXN #6 — DESCREQ2 (linee 162-172)
```
PayloadStart: total=20, transfers=2
┌─ TX ToDevice: 10 byte ─────────────────────────────┐
│ 02 00 00 02 42 00 00 03 00 00                       │
│    └──???──┘                                        │
│    register=0x0002 (diverso da TXN #3: 0x0001)     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 10 byte ───────────────────────────┐
│ 00 00 00 00 00 00 00 00 00 00                       │
│ ALL ZEROS! Device busy/non risponde                  │
└─────────────────────────────────────────────────────┘
⏱ Gap da TXN #5: ~140 µs
⏱ Durata: ~3.0 µs
```

### 🔴 GPIO IRQ (linee 185-186) — Dopo DESCREQ2
```
⏱ Dopo DESCREQ2: ~727 µs
⏱ Durata ISR: ~5 µs
Pin: 0x15
```

### TXN #7 — 0x0B Read (post-DESCREQ2, linee 193-203)
```
PayloadStart: total=18, transfers=2
┌─ TX ToDevice: 9 byte ──────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00                          │
│              approval7=0x03                          │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 9 byte ────────────────────────────┐
│ FF FF FF FF FF 82 B0 0E 5A                          │
│ └─sync──┘ └─header──┘                              │
│ TYPE=0x82 → (8<<4)|2 = RPT_DESC type=8 !!!         │
│ body_length = (0xB0>>4)|(0x0E<<4) = 0xEB → *4 = 940 │
└─────────────────────────────────────────────────────┘
⏱ Gap da GPIO IRQ: ~185 µs
⏱ Durata: ~6.7 µs
```

### TXN #8 — 0x0B Read 945 byte (linee 210-220)
```
PayloadStart: total=1890, transfers=2
┌─ TX ToDevice: 945 byte ────────────────────────────┐
│ 0B 00 00 00 FF 00 00 03 00... (945 byte totali)     │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 945 byte ──────────────────────────┐
│ FF FF FF FF FF AB 03 00 75 08 15 00 26 FF 00 06... │
│ └─sync──┘ └header┘ └──dati descrittore────────────┘ │
│            (3 byte, NO 0x5A! 0x75 è 1° byte dati)    │
│ Contiene Usage Pages, Logical Min/Max, Report IDs...│
└─────────────────────────────────────────────────────┘
⏱ Gap da TXN #7: ~560 µs
⏱ Durata: ~5.4 µs
```

---

## FASE 2: Attivazione (~962ms dopo TXN #8)

### ⏸ GAP di ~962 millisecondi
```
Il driver Windows processa i descrittori, configura l'HID framework,
prepara i buffer, setta le strutture interne.
NESSUNA attività SPI durante questo gap.
```

### TXN #9 — cmd1 (linee 233-235)
```
PayloadStart: total=5, transfers=1 (TX only!)
┌─ TX ToDevice: 5 byte ──────────────────────────────┐
│ 00 0E 00 00 00                                      │
│ │  └─payload──┘                                     │
│ opcode 0x00 = comando di attivazione                │
│ payload: 0x0E 0x00 0x00 0x00                        │
└─────────────────────────────────────────────────────┘
```

### TXN #10 — cmd2
```
PayloadStart: total=1, transfers=1 (TX only!)
┌─ TX ToDevice: 1 byte ──────────────────────────────┐
│ 00                                                  │
│ opcode 0x00 = NOP/padding                           │
└─────────────────────────────────────────────────────┘
```

### TXN #11 — Checksum Response (RX-only, no 0x0B)
```
┌─ RX FromDevice: dati checksum                       │
│ 84 26 AA...                                         │
└─────────────────────────────────────────────────────┘
```

### TXN #12 — cmd3 (0x70 TX+RX)
```
┌─ TX ToDevice: 1 byte ──────────────────────────────┐
│ 70                                                  │
│ opcode 0x70 = richiesta stato FW                    │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: 14 byte ───────────────────────────┐
│ 0E 00 42 01 02 03 FF...                             │
│ └───stato firmware─────────────────────────────────┘ │
⏱ Durata: ~23.5 ms (!! più lento degli altri)        │
```

---

## FASE 3: Firmware Upload (da ~989ms in poi)

### TXN #13+ — B0 Firmware Blocks
```
PayloadStart: total=241, transfers=1 (TX only)
┌─ TX ToDevice: 241 byte ────────────────────────────┐
│ B0 [240 byte di dati firmware]                       │
│ │                                                    │
│ opcode 0xB0 = firmware block write                   │
└─────────────────────────────────────────────────────┘
⏱ Ogni blocco: ~3.4 ms
⏱ Gap tra blocchi: ~100-170 µs
⏱ Totale blocchi: ~120
⏱ Tempo totale FW: ~410 ms
```

---

## FASE 4: Post-FW — Secondo Binario e Runtime (~410ms+)

### TXN #134 — 0xB1 Post-FW Execute (5 byte TX)
```
┌─ TX ToDevice: 5 byte ──────────────────────────────┐
│ B1 [4 byte payload]                                  │
│ │                                                    │
│ opcode 0xB1 = comando esecuzione post-firmware       │
└─────────────────────────────────────────────────────┘
```

### TXN #135-136 — 0x70 Read Status (×2)
```
┌─ TX ToDevice: 1 byte ──────────────────────────────┐
│ 70                                                  │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: N byte ─────────────────────────────┐
│ stato firmware post-upload                           │
└─────────────────────────────────────────────────────┘
```

### TXN #137 — 0x28 Comando Sconosciuto (7 byte TX+RX)
```
┌─ TX ToDevice: 7 byte ──────────────────────────────┐
│ 28 [6 byte payload]                                  │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: N byte ─────────────────────────────┐
│ risposta comando 0x28                                │
└─────────────────────────────────────────────────────┘
```

### TXN #138+ — 0x22 Second Binary Upload
```
┌─ TX ToDevice: N byte ──────────────────────────────┐
│ 22 [payload]                                         │
│ opcode 0x22 = upload secondo binario                 │
└─────────────────────────────────────────────────────┘
⏱ Blocchi multipli, simile a FASE 3
```

### Runtime — 0x0B Read con approval7=0x0A
```
┌─ TX ToDevice: 9+ byte ─────────────────────────────┐
│ 0B ... 0A 00                                        │
│        └─┘                                          │
│   approval7=0x0A (runtime)                           │
└─────────────────────────────────────────────────────┘
┌─ RX FromDevice: N byte ─────────────────────────────┐
│ dati sensore / report HID                           │
└─────────────────────────────────────────────────────┘
⏱ Intervallo tra letture: ~110 ms (resync periodico)
```

---

## RIEPILOGO TIMING

| Evento | Durata/Gap |
|--------|-----------|
| 0x0B read (9+9 byte) | ~6 µs |
| DESCREQ (10+10 byte) | ~6 µs |
| DESCREQ → GPIO IRQ | ~58 µs |
| GPIO IRQ → 0x0B read | ~112 µs |
| GPIO ISR duration | ~1-5 µs |
| DESCREQ2 (10+10 byte) | ~3 µs |
| DESCREQ2 → GPIO IRQ | ~727 µs |
| GPIO IRQ → 0x0B read | ~185 µs |
| Report descriptor read (1890 byte) | ~5.4 µs |
| Descriptor → cmd1 gap | ~962 ms (!) |
| FW block write (241 byte) | ~3.4 ms |
| FW inter-block gap | ~100-170 µs |
| Resync byte frequency (runtime) | ogni ~110 ms |

---

## PATTERN RICORRENTI

### Opcode e Significato
| Opcode | Nome | TX | RX | Note |
|--------|------|----|----|------|
| 0x0B | Read Register | 9+ byte | n byte | Approval a byte7/8, address a byte1-3 |
| 0x02 | DESCREQ | 10 byte | 10 byte | Richiede descrittore dispositivo |
| 0x00 | Attivazione | 1-5 byte | — | cmd1/cmd2 |
| 0x70 | Read Status | 1 byte | 14 byte | Stato firmware |
| 0xB0 | FW Block | 241 byte | — | Firmware write |
| 0xB1 | Post-FW Execute | 5 byte | — | Esecuzione post-firmware |
| 0x28 | Unknown | 7 byte | N byte | Comando sconosciuto post-FW |
| 0x22 | Second Binary | N byte | — | Upload secondo binario |

### Approval Bytes
```
Byte7 (approval7):
  - 0x00: stato iniziale (prima di DESCREQ)
  - 0x03: dopo aver ricevuto almeno un descrittore
  - 0x0A: runtime (~52s dopo il boot)

Byte8 (approval8):
  - 0x00: SEMPRE (non cambia mai)
```

### Formula Body Length
```
body_length = ((len_low >> 4) | (len_high << 4)) * 4
```
Esempi:
- RESET_RSP: len_low=0x10, len_high=0x00 → (1|0)*4 = 4
- DEVICE_DESC: len_low=0x80, len_high=0x00 → (8|0)*4 = 32
- RPT_DESC: len_low=0xB0, len_high=0x0E → (0x0B|0xE0)*4 = 0xEB*4 = 940

### Header HID
```
Formato: [TYPE][LEN_LOW][LEN_HIGH][0x5A]

TYPE byte:
  bit 3-0: versione protocollo (= 2 per HID-over-SPI)
  bit 7-4: tipo report

Tipi report:
  0 = ACK/Ready (pattern: 03 00 00 00, non ha 0x5A!)
  3 = RESET_RSP   (es: 32 10 00 5A)
  7 = DEVICE_DESC  (es: 72 80 00 5A)
  8 = RPT_DESC     (es: 82 B0 0E 5A)
```
