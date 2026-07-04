# Verifica indipendente — Ground truth dai CSV ETW vs documentazione

Fonti: `~/Scrivania/traces/surface_boot_auto.csv` (46.115 righe, 2.384 transazioni SPB, 9.507 eventi GPIO) e `~/Scrivania/traces/surface_init.csv` (513 transazioni). Parser: `parse_etw.py` (stessa directory di questo report). Tempi relativi alla prima transazione (t=0 = Clock-Time 134276452171753048, riga CSV 58).

## SCOPERTA CHIAVE (non presente in NESSUN documento)

Nel trace di boot ci sono **DUE device SPB distinti**:

| | Handle device | Connection ID | Opcode visti | Txn |
|---|---|---|---|---|
| **Device A** — touchscreen HID-over-SPI | `0x7F74AA5D37F8` | `0x…0b` | 0x0B (read), 0x02 (write) | 1.357 |
| **Device B** — upload firmware | `0x7F74AA5D7B88` | `0x…18`, `0x…19`, `0x…1a` | 0x00, 0x70, 0xB0, 0xB1, 0x28, 0x22, 0x24, 0x25, 0x26, 0x29 | 1.027 |

Le "FASE 3 Attivazione" e "FASE 4 Firmware" dei documenti (TXN#9–#13+, opcode 0x00/0x70/0xB0/…) **non avvengono sul target HID del touchscreen** ma su un target SPB diverso (handle e connection ID differenti). Il device A non riceve MAI gli opcode 0x00/0x70/0xB0/0xB1/0x22. Se sia un chip fisico diverso o lo stesso chip su un chip-select/risorsa ACPI diversa non è determinabile dal CSV, ma la connessione SPB è certamente distinta.

Inoltre **tutte** le transazioni del device A sono **full-duplex**: campo tipo richiesta = `8` nell'evento `Start` (ID 1010), un solo `IoSpbPayloadStart` (ID 1021) con 2 TD di **lunghezza identica** (ToDevice + FromDevice). Il MISO mostra `FF` durante i primi 5 byte (mentre il master clocca l'header) e i dati subito dopo: è un unico trasferimento SPI, non write-then-read. Sul device B invece i tipi sono 1=Read, 2=Write, 3=Sequence.

---

## (A) Tabella cronologica — prime 20 transazioni + GPIO (boot)

Tempi in ms da t=0. `⚡` = `InterruptInvokeDeviceIsrStart` (pin 0x15, flags 0x409).

| # | riga CSV | t | Dev | Tipo | MOSI | MISO |
|---|---|---|---|---|---|---|
| ⚡ | 55 | −0.046 | GPIO | ISR | — | — |
| 0 | 58 | 0.000 | A | FD 9/9 | `0B 00 00 00 FF 00 00 00 00` | `FF×5 32 10 00 5A` |
| 1 | 75 | +0.128 | A | FD 9/9 | `0B 00 00 00 FF 00 00 00 00` | `FF×5 03 00 00 00` |
| 2 | 92 | +0.322 | A | FD 10/10 | `02 00 00 01 42 00 00 03 00 00` (DESCREQ) | `FF×5 03 00 00 00 00` |
| ⚡ | 120 | +0.489 | GPIO | ISR | (57.8 µs dopo IoComplete di #2) | — |
| 3 | 123 | +0.499 | A | FD 9/9 | `0B 00 00 00 FF 00 00 03 00` | `FF×5 72 80 00 5A` |
| 4 | 140 | +0.621 | A | FD 37/37 | `0B 00 00 00 FF 00 00 03 00` + 28×00 | `FF×5 1F 00 00` + `1C 00 00 01 A8 03 02 00 00 00 00 20 03 00 00 02 04 00 5E 04 19 0C 04 00 81 00 00 00 00` |
| 5 | 157 | +0.846 | A | FD 10/10 | `02 00 00 02 42 00 00 03 00 00` (DESCREQ2) | `00×10` (tutti zero) |
| ⚡ | 185 | +1.641 | GPIO | ISR | (727.0 µs dopo IoComplete di #5) | — |
| 6 | 188 | +1.691 | A | FD 9/9 | `0B 00 00 00 FF 00 00 03 00` | `FF×5 82 B0 0E 5A` |
| 7 | 205 | +1.854 | A | FD 945/945 | `0B 00 00 00 FF 00 00 03 00` + 936×00 | `FF×5 AB 03 00` + report descriptor HID (936 B, inizia `75 08 15 00 26 FF 00…`) + 1 pad |
| 8 | 228 | +964.35 | **B** conn 0x18 | Write 5 | `00 0E 00 00 00` | — |
| 9 | 241 | +964.52 | **B** conn 0x19 | Write 1 | `00` | — |
| 10 | 254 | +964.61 | **B** conn 0x19 | Read 3 | — | `84 26 AA` |
| 11 | 267 | +964.93 | **B** conn 0x1a | Seq W1+R14 | `70` | `0E 00 42 01 02 03 FF 00…` |
| 12 | 289 | +989.08 | **B** conn 0x1a | Write 241 | `B0` + 240 B firmware | — |
| 13–19 | 302… | +992.6…+1013.8 | **B** | Write 241 | `B0` + 240 B (blocchi successivi, uno ogni ~3.5 ms) | — |

Seguito rilevante: blocchi `B0` fino a #131 (241 B) + #132 (125 B, riga 7?; +1412.9 ms), #133 `B1 31 0F 92 60` (+1414.8 ms), #134/135 `70`→`0E 00 4F 01 50 A6 41 01…` (stato 0x42→0x4F). Device A riprende a #220 (+5902.3 ms, riga 3210 ca.).

Transazioni chiave successive del device A:

| # | t (ms) | MOSI | MISO |
|---|---|---|---|
| 220 | +5902.28 | `02 00 00 03 42 00 04 03 00 06` | `FF×5 AB 03 00 75 08` |
| 221 | +5902.54 | `0B 00 00 00 FF 00 04 03 00` | `FF×5 52 F0 01 5A` |
| 222 | +5902.70 | `0B 00 00 00 FF 00 04 03 00 06 00 00 00…` (129 B) | `FF×5 7A 00 06 77 00…` (124 B body) |
| 223 | +5907.30 | `02 00 00 03 82 00 03 04 00 05 01 00 00 00` | `FF×5 7A 00 06 77 00…` |
| 224 | +5909.17 | `0B 00 00 00 FF 00 03 04 00` | `FF×5 12 60 03 5A` |
| 225 | +5909.33 | `0B 00 00 00 FF 00 03 04 00 05 01 00 00 00…` (221 B) | `FF×5 D6 00 08…` (216 B body) |
| 531 | +13671.91 (riga 7367) | `02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00` | `FF×5 D6 00 08…` |
| 868 | +42195.6 | `0B 00 00 00 FF 00 03 0A 00` | `FF×5 12 60 03 5A` |
| 873 | +42218.0 (riga 11937) | `0B 00 00 00 FF 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00…` (4309 B) | `FF×5 CE 10 0C…` (4304 B body) |

---

## (B) Risposte alle 6 domande

### 1. DESCREQ: singola transazione o write+read separate?
**(a) SINGOLA transazione, full-duplex.** TXN#2 (righe CSV 92–122): evento `Start` ID 1010 con campo tipo = **8** (full-duplex; sul device B i valori sono 1=Read, 2=Write, 3=Sequence), un solo `IoSpbPayloadStart` ID 1021 con user data `20, 2` (20 byte totali, 2 TD), poi:
- `IoSpbPayloadTdBuffer` TD0 ToDevice, 10 B: `02 00 00 01 42 00 00 03 00 00`
- `IoSpbPayloadTdBuffer` TD1 FromDevice, 10 B: `FF FF FF FF FF 03 00 00 00 00`
- un solo `IoComplete` (ID 1015, status 0x0).

I due TD hanno **la stessa lunghezza** e il MISO contiene `FF` esattamente per i 5 byte di header MOSI: trasferimento simultaneo (IOCTL_SPB_FULL_DUPLEX), non write seguita da read. Vale per TUTTE le 1.357 transazioni del device A, incluse le "read" 0x0B (MOSI zero-padded alla lunghezza totale). **Confidenza: alta.**

### 2. Prime 12 transazioni
Vedi tabella (A). Sul touchscreen (device A): **esattamente 2 read 0x0B prima della prima write** (TXN#0 → `32 10 00 5A`, TXN#1 → `03 00 00 00`, header MOSI identici `0B 00 00 00 FF 00 00 00 00`). Poi DESCREQ (→ ACK `03 00 00 00 00`), read 9 B (→ `72 80 00 5A`), read 37 B (device descriptor: contiene `5E 04 19 0C` = VID 0x045E / PID 0x0C19), DESCREQ2 (→ tutti zero), read 9 B (→ `82 B0 0E 5A`), read 945 B (report descriptor). Le txn #8–#11 cronologicamente successive sono sul device B. **Confidenza: alta.**

### 3. GPIO interrupt
Tutti gli ISR: pin **0x15**, flags **0x409**, maschera pin-state `0x200000` (bit 21) su `\_SB.GPIO` istanza 1.
- ISR a **t=−46.1 µs, PRIMA di TXN#0** (riga 55): anche la primissima read è interrupt-driven.
- ISR a +489.4 µs = **57.8 µs dopo l'IoComplete di DESCREQ** (431.6 µs); TXN#3 parte **10.0 µs dopo** l'ISR.
- ISR a +1641.4 µs = **727.0 µs dopo l'IoComplete di DESCREQ2** (914.4 µs); TXN#6 parte **49 µs dopo**.
- Runtime: **676 ISR totali, ognuno seguito da una read 0x0B da 9 B** (677 header totali; l'eccedenza è TXN#1, secondo drain dopo l'unico ISR iniziale). Delta ISR→read: min 10 µs, mediana 66 µs, max 338 µs. Le read "body" (37/129/221/945/4309 B) seguono l'header **senza un secondo interrupt**. **Confidenza: alta.**

### 4. Approval bytes (byte 7 e 8, 0-indexed, degli header 0x0B)
I byte 5–8 dell'header di read sono un **ECO dei byte 5–8 dell'ultima write 0x02** (e le read "body" ecoano anche i byte successivi, fino a `56 BD 0C EE 5B 44 4C` della write #531 — vedi #873):

| Fase (boot) | Ultima write (byte 5–8) | Header read (byte 5–8) | byte7 | byte8 |
|---|---|---|---|---|
| Pre-descriptor | (nessuna) | `00 00 00 00` | 0x00 | 0x00 |
| Post-DESCREQ (`…01 42 00 00 03 00`) | `00 00 03 00` | `00 00 03 00` | **0x03** | 0x00 |
| Post write #220 (`…03 42 00 04 03 00`) | `00 04 03 00` | `00 04 03 00` | **0x03** (ma byte6=0x04) | 0x00 |
| Post write #223 (`…03 82 00 03 04 00`) | `00 03 04 00` | `00 03 04 00` | **0x04** | 0x00 |
| Runtime, post write #531 (`…03 C2 00 03 0A 00`) | `00 03 0A 00` | `00 03 0A 00` | **0x0A** | 0x00 |

In `surface_init.csv` (dopo disable/enable) la prima txn è la write `02 00 00 04 82 00 00 04 00 01 01 0C 00 00` e TUTTE le read runtime usano `…FF 00 00 04 00` (byte7=**0x04**, non 0x0A) e ricevono regolarmente report da 221/4309 B. Quindi i valori **non sono costanti di fase**: sono qualunque cosa l'host abbia scritto nell'ultimo SET. byte8 = 0x00 sempre, in entrambi i trace. **Confidenza: alta sui byte; alta anche sul meccanismo di eco (5 write su 5 lo confermano, incluso il tail lungo).**

### 5. Registri/indirizzi
- Read 0x0B: byte 1–2 = `00 00` **sempre** (tutte le 1.352 read, entrambe le fasi) → indirizzo 0x0000: **confermato**.
- Write 0x02: byte 1–2 = `00 00` anch'essi; il byte che varia è il **byte 3**: `0x01` (DESCREQ), `0x02` (DESCREQ2), `0x03` (le tre write successive del boot: 0x42/0x82/0xC2), `0x04` (write nel trace init). Non esiste nel boot alcuna write con "indirizzo 0x0004"; 0x04 al byte 3 compare solo in surface_init.csv. Il byte 4 (`0x42/0x82/0xC2`) codifica nei 2 bit alti la lunghezza del payload in parole da 4 byte (1/2/3 → len 10/14/18) e nel nibble basso `2` (stessa "versione" degli header di risposta). **Confidenza: alta sui byte, media sull'interpretazione dei campi.**

### 6. Firmware upload 0xB0
**Esiste, ma sul device B (connection 0x1a), NON sul target HID del touchscreen.**
- **121 blocchi**: 120×241 B (opcode 0xB0 + 240 dati) + 1 blocco finale da 125 B (0xB0 + 124 dati) = 28.924 byte di payload. Da +989.1 ms a +1412.9 ms (~424 ms), un blocco ogni ~3.5 ms.
- Preceduto da (conn 0x18/0x19/0x1a): write `00 0E 00 00 00`, write `00`, read 3 B → `84 26 AA`, poi `70` → `0E 00 42 01 02 03 FF…` (stato bootloader).
- Seguito da: `B1 31 0F 92 60`, poi `70` → `0E 00 4F 01 50 A6 41 01…` (byte di stato 0x42→0x4F), poi un **secondo binario 0x22** (81×241 + 1×213 B) e **17 cicli** di gruppi 0x24 (37 B), 0x25 (25×241+157 B), 0x26 (17×241+17 B), 0x28 (read stato), 0x29 (5 B) — mai menzionati nei docs. **Confidenza: alta sui numeri; media sull'identità del device B.**

---

## (C) Divergenze docs vs CSV

1. **[GRAVE] Nessun documento distingue i due device SPB.** CSV_SEQUENCE FASI 3–4, DRIVER_STATE §6 FASI 3–4 e §7 State 3, VERIFICATION_PLAN §2.5/§3 presentano attivazione (0x00/0x70) e firmware (0xB0/0xB1/0x22/0x28) come continuazione della sequenza del touchscreen. In realtà avvengono su handle/connection SPB diversi; il target HID non riceve mai quegli opcode. Lo "state 3 VENDOR_INIT" del driver Linux (cmd1/cmd2/cmd3 sullo stesso canale HID) non ha riscontro nel CSV.
2. **[GRAVE] DRIVER_STATE §6 e VERIFICATION_PLAN §2.5: "TXN#11 0x0B read → checksum"** — falso: è una **plain Read di 3 byte senza alcun comando MOSI** (`84 26 AA`), su device B conn 0x19. Anche §7 "ACK read(0x0B,16B)" non esiste nel CSV. (CSV_SEQUENCE TXN#11 invece lo descrive correttamente come RX-only.)
3. **[GRAVE] Tabella approval VERIFICATION_PLAN §2.6 incompleta/fuorviante**: mancano le fasi `00 04 03 00` (dopo write #220) e `00 03 04 00` → **approval7=0x04** (dopo write #223); manca che anche il byte 6 cambia; e soprattutto i valori non sono costanti di fase ma **eco dell'ultima write 0x02** (surface_init: runtime con approval7=0x04, non 0x0A). Un driver che hardcoda 0x03/0x0A per stato funziona solo se replica esattamente le stesse write di Windows.
4. **[MEDIA] CSV_SEQUENCE TXN#5: "vendor=0x03A8, product=0x0002, version=0x0320"** — errato: 0x03A8=936 è la **lunghezza del report descriptor**; VID/PID reali = `5E 04 19 0C` = 0x045E/0x0C19, presenti nello stesso payload (coerente con DRIVER_STATE §1 e col bug C4 sui +4 di offset — qui l'errore è da mis-parse del body).
5. **[MEDIA] VERIFICATION_PLAN §2.4/§2.5: "TXN#3 DESCREQ → ACK (all zeros)"** — contraddice il CSV e lo stesso CSV_SEQUENCE: DESCREQ#1 riceve `FF×5 03 00 00 00 00`; "all zeros" è solo DESCREQ2.
6. **[MEDIA] Timing GPIO in CSV_SEQUENCE**: "GPIO IRQ → 0x0B read ~112 µs" e "~185 µs" — in realtà la read **parte** 10 µs (risp. 49 µs) dopo l'ISR; 112/185 µs si ottengono solo misurando fino all'IoComplete. "DESCREQ→IRQ ~58 µs" (57.8) e "DESCREQ2→IRQ ~727 µs" (727.0) sono esatti.
7. **[MEDIA] Manca in tutti i docs l'ISR a −46 µs prima di TXN#0** (riga 55): anche la prima read del boot è interrupt-driven. Rilevante per il fix "attendere GPIO IRQ".
8. **[MEDIA] CSV_SEQUENCE runtime: "intervallo tra letture ~110 ms"** — falso: mediana **10.0 ms** tra gli header runtime (min 5.9, max 580; 662 header tra +42.2 s e +68.1 s).
9. **[BASSA] CSV_SEQUENCE TXN#13 "0x28 comando 7 byte TX+RX"** — è write 1 byte (`28`) + read 6 o 18 byte.
10. **[BASSA] Durate "~6 µs" per transazione** — è lo span degli eventi ETW di payload-logging, non il tempo SPI: TXN#0 Start→IoComplete = 108.8 µs.
11. **[BASSA] "0x0A runtime ~52 s dopo il boot"** — primo header 0x0A a +42.2 s da TXN#0 (dipende dal riferimento; ordine di grandezza ok).
12. **[BASSA] Riferimenti riga** (TXN#1 "linee 63-73" → reale 58–74) e "Colonna 28 Clock-Time" (in questo dump è la 17ª colonna): off-by-few, i byte citati sono comunque corretti.
13. **Confermati dai CSV** (per completezza): 2 drain 0x0B pre-DESCREQ ✓; DESCREQ full-duplex TX+RX in un'unica transazione (bug #4 e fix D di DRIVER_STATE) ✓; formula body_length `((len_low>>4)|(len_high<<4))*4` ✓ (verificata anche su `12 40 43 5A`→4304 e `52 F0 01 5A`→124); header `[TYPE][LEN][5A]` e tipi 3/7/8 ✓; pin 0x15/0x409 nel trace ✓ (il DSDT dice altro, come già notato in C8); 120×241+125 B0 ✓; register read = 0x0000 e approval8=0x00 ✓; report descriptor 936 B ✓ (body 940 = subheader 3 B `AB 03 00` + 936 + 1 pad).

## (D) Confidenza

| Conclusione | Confidenza |
|---|---|
| DESCREQ e tutte le txn device A = full-duplex singola transazione | **Alta** (tipo=8, TD equali, MISO FF durante header — 1.357/1.357 txn) |
| 2 read 0x0B pre-write, byte esatti prime 12 txn | **Alta** (byte letti direttamente dal CSV) |
| Interrupt: pin 0x15/0x409, 1 ISR → 1 header read, ISR anche prima di TXN#0 | **Alta** |
| Approval = eco byte 5–N dell'ultima write 0x02 | **Alta** (5/5 write nel boot + init trace, incluso tail 13 byte) |
| Read register 0x0000; write byte3 = 01/02/03/04 | **Alta** (byte); **Media** (semantica "registro") |
| FW upload su device SPB separato | **Alta** che sia connection/handle diverso; **Media** su cosa sia fisicamente |
| Conteggi blocchi B0/0x22/cicli 0x24-0x29 | **Alta** |
| Intervallo runtime ~10 ms | **Alta** (662 campioni) |
