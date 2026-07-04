# GROUND TRUTH — Modello verificato incrociando CSV ETW × decomp amdspi × decomp hidspi V0 × spec PDF

> **Data verifica: 2026-07-05.** Tre verificatori indipendenti; report completi in
> [verification/](verification/). Questo documento SOSTITUISCE le affermazioni in
> conflitto in HIDSPI_PROTOCOL.md / CSV_SEQUENCE.md / VERIFICATION_PLAN.md §2.5-2.6 /
> DRIVER_STATE.md §6-7. Ogni claim qui sotto è confermato da ≥2 fonti indipendenti.

## 1. Modello di transazione (il punto che sblocca tutto)

**Write = TX-only. Risposta = read separata, gated su interrupt GPIO. SEMPRE.**

- Spec §7.2.3 + §6.2.2: output report → device consuma → interrupt entro 1 s → host
  legge header con read approval → legge body. Tre transazioni SPI distinte.
- hidspi V0 `SendingWriteToBusEntry` @0x140008180: singolo transfer da 10 byte,
  poi `Timer::Start(2000ms)` e attesa evento interrupt, poi `ReadingResponseHeaderEntry`.
- amdspi write path 0x54d0: `write8(0x4B, 0)` esplicito (RX_COUNT=0), **mai** letture
  dalla FIFO dopo il trigger.

**Il "MISO inline" delle write nel CSV ETW è un artefatto**: hidspi riusa i buffer tra
transazioni; il MISO loggato per la DESCREQ (`FF×5 03 00 00 00 00`) è il contenuto della
transazione *precedente* (body della reset response). Stesso artefatto per gli "approval
bytes" (v. §3).

## 2. Sequenza di boot corretta (CSV, canale HID = handle 0x7F74AA5D37F8)

```
IRQ ─46µs→ TXN0  read hdr  9B  → 32 10 00 5A            (RESET_RESP header, type=3)
           TXN1  read body 9B  → 03 00 00 00            (RESET_RESP body: len16=3, ContentID 0)
           TXN2  WRITE 10B     02 00 00 01 42 00 00 03 00 00   (DESCREQ, TX-only)
IRQ +58µs→ TXN3  read hdr  9B  → 72 80 00 5A            (DEVICE_DESC header, type=7, body=32B)
           TXN4  read body 37B → 3 byte prefisso + 28B descriptor + 1 pad
           TXN5  WRITE 10B     02 00 00 02 42 00 00 03 00 00   (DESCREQ2 → wReportDescRegister)
IRQ+727µs→ TXN6  read hdr  9B  → 82 B0 0E 5A            (RPT_DESC header, type=8, body=940B)
           TXN7  read body 945B→ 3 byte prefisso + 936B report descriptor + pad
→ runtime: per OGNI input report: IRQ → read hdr 9B (type=1 DATA) → read body (5+blen)
  cadenza mediana runtime ~10 ms; 676 IRQ ↔ 677 header read nel boot trace
```

- **Non esiste un report "ACK type 0"**: `03 00 00 00` è il body della reset response
  (hidspi `ValidateResponse` ammette solo type {1,3,4,5,7,8}).
- **Un interrupt per ogni input report** (spec §7.1.6); le body read NON hanno un secondo
  IRQ: header+body si leggono nello stesso servizio dell'interrupt.
- **RESET_RESP non sollecitata ripetuta** (il nostro caso, ~109 ms — periodo firmware,
  non in spec) = il device segnala errore → per spec §6.1.3 l'host RIPARTE DALLA
  DESCRIPTOR REQUEST (non dal reset).

## 3. Read approval: 5 byte, niente "approval bytes"

- Formato (spec §7.1.2 + `ReadingResponseHeaderEntry` @0x7da0):
  `[0x0B][addr24 BE][0xFF]` = 5 byte trasmessi; i successivi byte della transazione
  sono fase RX (header a partire dal 6° byte).
- Header read = transazione da 9 byte (5 TX + 4 RX header).
  Body read = 5 + blen, con `blen = (u16(hdr[1..2]) >> 4) * 4`
  (identica a `(hdr[1]>>4 | hdr[2]<<4)*4`). Validare: `hdr[3]==0x5A`, `(hdr[1]&0xF)==0`.
- **I "byte 7/8 di approval" (0x00/0x03/0x0A/0x04 nel CSV) sono spazzatura del buffer TX
  riusato di hidspi** — il device non campiona MOSI dopo il 5° byte. Qualsiasi logica
  `approval_byte7/8` nel driver va rimossa.
- Indirizzo di read: 0x000000 fino al parse del device descriptor, poi wInputRegister
  dal descriptor (sul nostro device resta 0x0000).

## 4. Come amdspi programma il controller (decomp, per exec_segment)

**Write path 0x54d0 (opcode 0x02), sequenza MMIO:**
```
1. ALT_CS 0x1D: read8 → &0xFC → |0x01 → write8
2. FIFO clear: CTRL0 bit20 — UN SOLO SET (read-modify-write), non toggle
3. write8(0x45, opcode)                      ← prima scrittura opcode
4. CTRL0 |= 0x60040000 (secret bits, RMW; bit21/23 mai toccati) — una volta per segmento
5. w = read16(0x44); n = read8(0x20) & 0xF;
   w = (w & 0x00FF) | (n<<8) | (n<<12); write16(0x44, w)   ← clobbera 0x45!
6. write8(0x45, opcode)                      ← SECONDA scrittura opcode
7. write8(0x48, tx_len); fill FIFO da 0x80
8. write8(0x4B, 0)                           ← RX_COUNT sempre 0 nelle write
9. write8(0x47, 0x80) trigger
10. poll 0x4C (NB: il poll Windows è buggato/no-op — non aspetta davvero;
    il nostro poll su CTRL0 bit31 è più corretto e va tenuto)
11. uscita: ALT_CS &0xFC (senza OR 1)
```
**Read path 0x3c20 (opcode 0x0B), ramo PIO:** identico tranne: TX_COUNT=3 (i 3 byte
d'indirizzo in FIFO; l'opcode esce da 0x45; il byte 0xFF NON è trasmesso — è il primo
clock RX), NIENTE dance 0x44, RX_COUNT=rx_len, risposta letta a **FIFO + TX_COUNT**
(0x83). La variante 0x4bac usa RX_COUNT=rx_len+1 e legge a 0x84 (regola: offset
risposta = 0x80 + TX_COUNT, skip = RX_COUNT − rx_len).

- **Strobe 0x49/0x4A: NON ESISTONO in amdspi** (ricerca esaustiva nel binario). Solo-Linux.
- L'unica differenza write-vs-read: la **dance del registro 0x44** (passi 5-6).
  Effetto netto sui registri = nullo (0x45 riscritto) → probabile side-effect di latch
  della write16 a 0x44. È il candidato n.1 per il blocco write.
  NB il test precedente (H10) scriveva 0x44 PRIMA dell'opcode → no-op garantito; l'ordine
  Windows (opcode → word 0x44 → opcode) NON è mai stato provato.

## 5. Device descriptor V0 (28 byte, layout tipo HID-over-I2C)

Body della risposta type=7: `[len16 LE = 31][ContentID]` (3 byte) + 28 byte contenuto + pad.
Il contenuto inizia quindi a **body+3** (= header+4+3):

| off | campo | note |
|-----|-------|------|
| 0 | wDeviceDescLength = 0x001C | validare ==28 |
| 2 | bcdVersion | |
| 4 | wReportDescLength | 936 sul nostro device |
| 6 | **wReportDescRegister** | destinazione DESCREQ2 (=0x0002 qui) |
| 8 | **wInputRegister** | nuovo indirizzo read post-parse (=0x0000 qui) |
| 10 | wMaxInputLength | dimensiona le body read |
| 12 | **wOutputRegister** | output/set-get-feature |
| 14 | wMaxOutputLength | |
| 16 | **wCommandRegister** | indirizzo SET_POWER (solo se flags lo dicono) |
| 18/20/22 | VID / PID / bcdVersion | 0x045E / 0x0C19 |
| 24 | dword flags | bit6/bit7 = supporto power command |

- Windows **non manda SET_POWER al boot** (i flag del descriptor decidono; e comunque
  andrebbe a wCommandRegister, che prima del parse vale 0 → i test pre-enumerazione a
  0x000001 non replicavano Windows).
- Input report runtime: **type=1 (DATA)**, non type=7.

## 6. I due device SPB (scoperta CSV)

Il boot trace contiene DUE target SPB: il touchscreen HID (connection 0x0b, solo
opcode 0x0B/0x02) e un **secondo device** (connection 0x18/0x19/0x1a) su cui avvengono
TUTTI gli opcode 0x00/0x70/0xB0/0xB1/0x22/0x24-0x29: vendor init, upload firmware
(121 blocchi 0xB0 = 28.924 B), secondo binario 0x22, cicli 0x24-0x29.
**Lo stato VENDOR_INIT e fw_work del driver Linux mirano al device sbagliato e vanno
rimossi dal path HID.** (Da capire in futuro se/quale nodo ACPI sia il secondo device
e se il touch funzioni senza quell'upload dopo cold boot.)

## 7. Piano fix conseguente (una modifica → un test)

| # | Fix | Fonte |
|---|-----|-------|
| G0 | Baseline: build repo attuale + reload + dmesg | — |
| G1 | Read Windows-shape: approval 5B (0B+addr24BE+FF via TX_COUNT=3+RX include FF), header read 9B, body read 5+blen, validazioni 0x5A/nibble; rimuovere approval_byte7/8 | §3, §4 |
| G2 | Write path exec_segment: dance 0x44 (opcode→word 0x44→opcode), FIFO clear single-set | §4 |
| G3 | Sequencer: header+body per IRQ; DESCREQ dopo body reset; NIENTE read post-write (aspettare IRQ, timeout 2 s); su RESET_RESP ripetuta ri-DESCREQ | §1, §2 |
| G4 | Parse descriptor a body+3 (28B, validare 0x001C); DESCREQ2 a wReportDescRegister; input register/max length dal descriptor | §5 |
| G5 | Rimuovere VENDOR_INIT/fw_work dal path HID; runtime su type=1 | §6, §5 |
