# Report di verifica protocollo HID-over-SPI — MSHW0231 (V0 path)

**Verificatore indipendente — 2026-07-05**

Fonti esaminate:
- SPEC: `~/Scrivania/decomp/HidSpiProtocolSpec.pdf` (47 pagine; testo estratto integralmente)
- DECOMP V0 (hidspi.sys, con simboli PDB): `~/Scrivania/decomp/clean/hidspi_V0_ReadEntries.txt`, `hidspi_V0_ConfigureTransfer.txt`, `hidspi_792c_V0_Initialize.txt`, più disassembly aggiuntivo fatto in questa sessione direttamente da `~/windrivers/hidspi.sys` + `hidspi.pdb` (funzioni `ValidateResponseHeader` @0x140008900, `ValidateResponse` @0x140008454, `VerifyDescriptorCompletion` V0 @0x1400092ac, `VerifyPowerCompletion` @0x1400096ac, `SendingWriteToBusEntry` @0x140008180, `StartingDescriptorResponseTimerEntry` @0x140008430)
- DECOMP Cx: `hidspicx_a664_ConfigureTransfer.txt` e altri
- DOCS: `~/Scrivania/docs/HIDSPI_PROTOCOL.md`, `~/Scrivania/docs/VERIFICATION_PLAN.md`

Premessa (confermata, non ridiscussa): il device usa il path **HidSpiDeviceV0** (formato wire "versione 2"), non la spec v1.0 pura. Dove spec e V0 divergono lo indico esplicitamente.

---

## D1 — READ APPROVAL

**Verdetto: parzialmente SMENTITO (i "byte 7/8 di approval" NON esistono come campi di protocollo).**

**Spec** (§7.1.2, PDF pag. 23): la Read Approval è di **5 byte**:

| offset | bytes | campo |
|--------|-------|-------|
| 0 | 1 | Opcode (READ) |
| 1 | 3 | Input Report Address (header o body), **big-endian** |
| 4 | 1 (8 cicli di clock) | Placeholder — "0b1 x 8 cycles" = 0xFF in single-SPI |

La transazione header è di 9 byte totali (§7.1.3-7.1.4, pag. 23-24): "transmitting a Read Approval (0x05 bytes) … and reading in an Input Report Header (0x04) bytes as part of a single 9-byte SPI transaction. In the first 5 bytes … the host transmits the Read Approval … In the last 4 bytes … the device returns the Input Report Header". **I byte 5-8 della transazione sono la fase RX**: sul MOSI sono don't-care, non campi.

**Decomp V0** (`ReadingResponseHeaderEntry` @0x140007da0, `hidspi_V0_ReadEntries.txt` righe 5-31): il driver scrive nel buffer TX **solo i byte 0-4**:
- `mov byte [rdx], 0xb` → byte0 = 0x0B
- `or dword [rdx+1], 0xff000000` / `mov dword [rdx+1], ecx` → byte1-3 = indirizzo BE, byte4 = 0xFF
- lunghezza transazione = 9 (`mov qword [rcx+0xb0], 9`)
I byte 5-8 del buffer TX **non vengono mai inizializzati**.

**Origine dei "byte 7/8"**: il buffer TX (`[dev+0xd8]`) è **riusato** tra write e read. Dopo la DESCREQ da 10 byte `02 00 00 01 42 00 00 03 00 00`, la read successiva riscrive solo i byte 0-4 (`0B 00 00 00 FF`) e lascia sul filo i byte 5-9 residui: `00 00 03 00 00` → **MOSI byte7 = 0x03**. Prima di qualunque write il buffer è appena allocato e azzerato (`ExAllocatePool2`) → byte7 = 0x00. Questo riproduce esattamente la tabella di VERIFICATION_PLAN §2.6 (0x00 pre-DESCREQ, 0x03 dopo): sono **residui di buffer catturati nel CSV, non "approval bytes"**. Il valore 0x0A a runtime è spiegabile con lo stesso meccanismo (residuo di una write con content_len=7 → campo len+3 = 0x0A a byte7).

**L'approval cambia tra le fasi?** No. Né la spec né il decomp prevedono un "valore di approval" che cambia. L'unica cosa che cambia è l'**indirizzo**: nella spec header e body hanno indirizzi ACPI distinti (esempio pag. 16-17: func1=0x1000 header, func2=0x1004 body); in V0 c'è un solo indirizzo che passa da 0x000000 a `word[dev+0x50]` (wInputRegister dal device descriptor) quando il flag `byte[dev+0x64]` viene settato a fine parse del descriptor (`ReadingResponseHeaderEntry` @0x140007dc8-0x140007de7; flag settato in `VerifyDescriptorCompletion` @0x1400094d3 `mov byte [rdi+0x64], bl`(=1)).

**Impatto Linux**: i 9 byte inviati vanno bene *purché* i byte 5-8 siano trattati come dummy TX di una transazione full-duplex (lì arriva l'header su MISO). Mandare 0x00 fissi è corretto; qualunque logica `spi_hid_approval_byte7` che cambia valore con lo stato va rimossa (innocua per il device, ma è codice basato su un artefatto).

---

## D2 — Interrupt prima di ogni read; latenza descriptor

**Verdetto: CONFERMATO (interrupt-gated per ogni input report; latenza max 1 s da spec, timeout 2 s nel driver Windows V0).**

- Spec §7.1 (pag. 22): "When the DEVICE has active data … it will assert the Interrupt line … When the HOST receives the Interrupt, it will then perform the read sequence". §7.1.3 (pag. 23): "**In response to an interrupt**, the host shall conduct a bus transfer in order to retrieve the Input Report header".
- Un interrupt per ogni risposta/frammento: §7.1.6 (pag. 26): "the DEVICE shall assert an interrupt **for every fragment** in a transfer"; e "shall raise an interrupt for a subsequent fragment within 1 second".
- Timing interrupt (§7.1.1, pag. 22-23): il device tiene INT LOW "at least until the start of an input report (the HOST has written the read approval opcode of the input report header)"; nessuna nuova assertion durante la trasmissione di un input report fino a dopo l'approval del body.
- Latenza richiesta→risposta: §6.1.2 (pag. 19) passo 5: "**Within 1 second**, the device shall signal an interrupt and make available … the Device Descriptor". §7.2 (pag. 29): "The device shall respond to an output report within 1 second of completion of the bus transaction". (Il t1 = 1 ms a pag. 13-14 è solo un'ipotesi illustrativa per i calcoli di throughput, non un requisito.)
- Decomp V0: dopo la write il driver entra in `StartingDescriptorResponseTimerEntry` @0x140008430 → `Timer::Start(0x7d0)` = **2000 ms** di timeout, poi aspetta l'evento interrupt; esiste lo stato/transizione `c_readingOnInterrupt` (hidspi_symbols.txt righe 358, 472) e la read header è una entry di stato separata raggiunta su evento. Anche dopo il reset: `ResettingSyncEntry` @0x140008080 → `Fdo::ResetDevice` + `Timer::Start(2000)` e la read parte sull'interrupt, non subito.

**Impatto Linux**: ogni read (header) deve essere scatenata dall'IRQ GPIO, con timeout ~2 s. Non fare polling cieco tra write e read.

---

## D3 — Richiesta DEVICE DESCRIPTOR

**Verdetto: CONFERMATO (il frame da 10 byte è esatto per il path V0; indirizzo 0x000001 corretto).**

**Spec v1.0** (§7.2.1 pag. 29 + esempio §7.3 pag. 32) — NON usata da questo device: `[02][addr24 BE][type=0x01][len16=0x0000][id=0x00]` = 8 byte (già multiplo di 4). Output Report Type 0x01 = device descriptor request; indirizzo di esempio 0x002000 da ACPI.

**Decomp V0** (`ConfiguringDescriptorTransferEntry` @0x140007230, `hidspi_V0_ConfigureTransfer.txt` righe 216-239): buffer azzerato per 10 byte, poi:
- byte0 = 0x02 (opcode WRITE)
- byte1-3 = `dword[dev+0x68]` codificato big-endian su 24 bit → `00 00 01`. `[dev+0x68]` = _DSM(rev1) func1 = 1 (`hidspi_792c_V0_Initialize`, cfr. FINDINGS §2) → **0x000001 corretto**, confermato anche dal boot ETW (DESC_REQ @0x000001).
- word byte4-5 = 0x0042 (`mov word [r8+4], 0x42`)
- word byte7-8 = 0x0003 (`mov word [r8+7], 3`); byte6 e byte9 restano 0
- lunghezza = 0x0A (`mov qword [r9+0xb0], 0xa`)

→ frame **`02 00 00 01 42 00 00 03 00 00`**, identico a quello del driver Linux. Scomposizione campo-per-campo del body V0 (derivata confrontando tutti i casi di `ConfigureTransfer` @0x140006f6c):

| byte | valore | significato V0 |
|------|--------|----------------|
| 0 | 0x02 | opcode WRITE |
| 1-3 | 00 00 01 | Output/Descriptor-Request Address, BE |
| 4-5 (LE u16) | 0x0042 | `(body_len << 4) \| versione` → body_len=4 (byte 6-9), versione=2. Verifica: nel caso con contenuto il codice fa `shl ax,4; or cx,2` con ax = content_len allineato + 4 (@0x14000715e-0x140007172) |
| 6 | 0x00 | report type (0 per le descriptor request; 3=SET_FEATURE, 4=GET_FEATURE, 5=OUTPUT negli altri casi: @0x140007181-0x14000718e, @0x1400071b2) |
| 7-8 (LE u16) | 0x0003 | content_length + 3 (qui 0) — stesso pattern in tutte le write: `add di, si(=3); mov word [r14+7], di` @0x14000718b |
| 9 | 0x00 | Content ID (0 per descriptor request) |

La richiesta del **report descriptor** in V0 è lo stesso identico body ma all'indirizzo `word[dev+0x4e]` = campo a offset 6 del device descriptor (wReportDescRegister) — sul nostro device 0x000002 (boot ETW). Quindi DESCREQ2 = `02 00 00 02 42 00 00 03 00 00` ✓ coerente con HIDSPI_PROTOCOL.md.

**Impatto Linux**: nessuna modifica al frame. (Il mancato funzionamento è a valle, nel path di write fisico — fuori scope di questo report.)

---

## D4 — Risposta inline o READ separata dopo la write?

**Verdetto: CONFERMATO — READ separata, gated su interrupt. L'idea "TX+RX stesso opcode nella stessa transazione" è SMENTITA.**

- Spec §7.2.3 (pag. 31), sequenza Sending Output Reports: Step 1 host scrive l'output report; Step 2 device consuma; Step 3 "DEVICE asserts interrupt within 1 second after completion of the bus transaction"; Step 4 "HOST issues a Read Approval and reads the Input Report Header"; Step 5 idem per il body. **Tre transazioni SPI distinte.** Ribadito in §6.2.2 (pag. 20): "The DEVICE shall then signal an interrupt … The HOST shall then read an input report from the Input Report addresses".
- Decomp V0: `SendingWriteToBusEntry` @0x140008180 sottomette **una sola** `SpbBusWrapper::Transfer` con lunghezza `[dev+0xb0]` = 10 (esattamente il frame di write, nessun byte RX extra), poi la macchina a stati passa per `CheckingWriteCompletionStatus` → `StartingDescriptorResponseTimer` (2 s) → attesa evento interrupt → `ReadingResponseHeaderEntry` (transazione da 9 byte separata).
- Qualsiasi "ACK inline" visto nel CSV durante la transazione 0x02 è ciò che il device pilota su MISO durante una write: non è definito dal protocollo e Windows non lo legge.

**Impatto Linux**: l'"Opzione B" di HIDSPI_PROTOCOL.md (write-only → attesa IRQ → read 0x0B) è quella giusta ed è ciò che fa Windows. L'"Opzione A" (exec_segment TX+RX combinato con opcode 0x02) non corrisponde a nulla nel driver Windows.

---

## D5 — RESET RESPONSE

**Verdetto: CONFERMATO il formato; l'host NON deve inviare alcun ACK; il retry ~109 ms NON è nella spec.**

**Formato** — Spec (pag. 25 §punto 1.c e 2.b; esempio pag. 31-32): input report type 0x3, Content Length 0, Content ID 0; header con Length=1 (4 byte di body). **Decomp V0** (`ValidateResponse` @0x140008454):
- header type == 3 (`shr cl,4; cmp cl, r13b(=3)` @0x14000849d)
- lunghezza body dall'header == 4 (`cmp rdx, 4` @0x1400084dc)
- word body[0..1] == 0x0003 (`movzx r9d, word [r15+5]; cmp r9w, r13w` @0x14000854e — il body inizia a buffer+5, dopo i 5 byte di approval)
→ wire atteso: header `32 10 00 5A`, body `03 00 00 00`. NB: in V0 i primi 2 byte del body sono un **campo lunghezza LE = content_len+3** (3 = len16+ContentID), non un "type": la validazione generica per gli altri tipi controlla `body_len_header == align4(word body[0..1])` (@0x1400086a0-0x1400086cd). Che il valore sia 3 per il reset è una coincidenza con il type 3.

**Cosa fa l'host** — Spec §6.1.2 (pag. 19): legge la reset response (header+body) e poi passa direttamente al passo 4 (write della descriptor request). **Nessun ACK/comando di consumo**: il consumo è la lettura stessa (l'interrupt viene rilasciato quando l'host scrive l'opcode dell'approval — §7.1.1 pag. 22). Decomp V0: dopo la validazione incrementa un contatore (`lock inc [rbp+0x88]`) e prosegue verso `ConfiguringDescriptorTransfer`; esiste `CheckingResetRetryCountEntry` per i retry lato host.

**Reset response non sollecitata** — Spec §6.1.3 (pag. 19-20): "When the device detects an error condition, it may interrupt and make available … an unsolicited Reset Response. After receiving an unsolicited Reset Response, the host shall initiate the request procedure **from step (4)**" → cioè ri-inviare la **descriptor request**, non ri-resettare il device. Questo è esattamente il comportamento osservato: il device che ri-emette RESET_RESP ogni ~109 ms sta segnalando errore/richiesta non ricevuta.

**Timeout/retry ~109 ms** — NON DETERMINABILE dalla spec: la spec non prevede alcun intervallo di ri-emissione (l'interrupt è tenuto LOW finché l'host non legge, senza massimo). Il ciclo a ~109 ms è comportamento specifico del firmware del device. I timeout della spec sono: 1 s per interrupt post-reset e post-output-report; il driver Windows V0 usa timer di 2000 ms (`0x7d0` in ResettingSync/StartingDescriptorResponseTimer/ReadingResponse*).

---

## D6 — DEVICE DESCRIPTOR

**Verdetto: SMENTITO per la spec v1.0 (24 byte, senza registri), CONFERMATO in variante V0 (28 byte, layout tipo HID-over-I2C con wInputRegister/wOutputRegister).**

**Spec v1.0** (§6.1.1, pag. 17-19): descriptor di **24 byte fissi** (wDeviceDescLength=0x18): wDeviceDescLength(0), bcdVersion(2)=0x0300, wReportDescLength(4), wMaxInputLength(6), wMaxOutputLength(8), wMaxFragmentLength(10), wVendorID(12), wProductID(14), wVersionID(16), wFlags(18), 4 byte reserved(20). **Nella spec v1.0 NON esistono wInputRegister/wOutputRegister** (gli indirizzi vengono da ACPI). Chiedere quei campi alla spec è un errore di modello (sono nomi HID-over-I2C).

**Decomp V0** (`VerifyDescriptorCompletion` @0x1400092ac — disassemblata in questa sessione):
- header type == 7 (`and al,0xf0; cmp al,0x70`)
- lunghezza body dall'header == **0x20** (32 byte) (`cmp rcx, 0x20` @0x140009378)
- primo word del contenuto == **0x1C** (28) (`movzx r9d, word [rdx+8]; cmp r9w, r10w(=0x1c)` @0x14000940a-0x140009419; rdx = buffer RX, contenuto a offset 8 = 5 approval + 3 di prefisso body)
- copia **28 byte** di descriptor in `dev+0x48..0x63` (`movups [rdi+0x48], xmm0` da rdx+8; `movsd [rdi+0x58]` da rdx+0x18; `mov [rdi+0x60], eax` da rdx+0x20 @0x1400094a4-0x1400094bb)
- setta `byte[dev+0x64] = 1` (abilita l'indirizzo di input dal descriptor) e rialloca i buffer di transfer in base a `word[dev+0x52]` e `word[dev+0x56]` (@0x1400095a5-0x140009636).

Layout V0 (28 byte) ricostruito dagli usi dei campi:

| offset | campo (dev ctx) | uso verificato |
|--------|-----------------|----------------|
| 0 | wDeviceDescLength = 0x001C | validato ==28 |
| 2 | bcdVersion (dev+0x4a) | solo trace |
| 4 | wReportDescLength (dev+0x4c) | — |
| 6 | **wReportDescRegister** (dev+0x4e) | indirizzo della report-descriptor request (`ConfigureTransfer` caso 1 @0x1400071bd `movzx edx, word [rbx+0x4e]`); sul device = 0x0002 (boot ETW) |
| 8 | **wInputRegister** (dev+0x50) | indirizzo delle read dopo il parse (`ReadingResponse*` gated su flag +0x64) |
| 10 | wMaxInputLength (dev+0x52) | dimensionamento buffer @0x1400095a5 |
| 12 | **wOutputRegister** (dev+0x54) | indirizzo di output/set-feature/get-feature (`ConfigureTransfer` @0x140007136, @0x1400071ae) |
| 14 | wMaxOutputLength (dev+0x56) | dimensionamento buffer @0x1400095cf |
| 16 | **wCommandRegister** (dev+0x58) | indirizzo SET_POWER (`ConfigurePowerTransfer` @0x140006f11) |
| 18/20/22 | wVendorID / wProductID / wVersionID (dev+0x5a/0x5c/0x5e) | trace @0x140009522-0x140009530 |
| 24-27 | dword flags (dev+0x60) | bit7→dev+0x7c, bit6→dev+0x7d (flag power), bits5:0 ×2→dev+0x80 @0x1400094b8-0x1400094db |

**Sul parse Linux "header 4 byte [len_lo][len_hi][??][0x5A]"**: SMENTITO. L'header di 4 byte V0 è (da `ValidateResponseHeader` @0x140008900):
- byte0 = [TYPE 7:4 | VERSION 3:0] (trace: `and r11d,0xf` versione, `shr eax,4` type @0x140008af4-0x140008b0c) — **non è len_lo**
- byte1-2 = u16 LE: bit 3:0 **devono essere 0** (`test dl, 0xf` → scarto @0x1400089f7), bit 15:4 = lunghezza body **in unità di 4 byte**
- byte3 = 0x5A obbligatorio (`cmp dl, 0x5a` @0x140008940; header salvato in `[dev+0xc8]` @0x140008b4e)

Inoltre il body del descriptor NON inizia col descriptor: ha un prefisso di **3 byte** `[len16 = content+3][ContentID]`, e il contenuto (i 28 byte, che a loro volta iniziano con wDeviceDescLength) parte a **offset 3 del body** (= offset 8 del buffer con approval). Un parse che salta 4 byte fissi di "header" dentro il body sbaglia di 1.

**Impatto Linux**: il descriptor da parsare è di 28 byte a body+3; da lì vanno estratti wReportDescRegister (per DESCREQ2), wInputRegister (nuovo indirizzo di read), wOutputRegister, wMaxInput/OutputLength (dimensionamento letture), wCommandRegister (SET_POWER).

---

## D7 — SET_POWER

**Verdetto: CONFERMATO che esiste in entrambe le fonti ma con formati diversi; il frame V0 documentato è quasi giusto — errori su lunghezza e indirizzo.**

**Spec v1.0** (§7.2.2, pag. 30): Set Power = comando, cioè output report con Output Report Type **0x07**, Content ID = **0x01** (command ID Set Power), Content = 1 byte (0x01 ON, 0x02 SLEEP, 0x03 OFF). Il device risponde (input report type 0x04, stesso content ID) **solo** a Set Power ON.

**Decomp V0** (`ConfigurePowerTransfer` @0x140006efc, `hidspi_V0_ConfigureTransfer.txt` righe 188-215):
- byte0 = 0x02; byte1-3 = **`word[dev+0x58]`** BE = wCommandRegister **dal device descriptor** (offset 16) — NON `[dev+0x68]`/0x000001
- word4-5 = 0x0082 → body_len 8, versione 2
- byte6 = 0x00; word7-8 = 0x0004 (= content 1 + 3); byte9 = 0x01 (command ID Set Power); byte10 = stato (scritto solo se ∈ {1,2})
- lunghezza transfer = **0x0E = 14 byte** (`mov qword [rcx+0xb0], 0xe`) → 3 byte di padding finali

Correzioni a FINDINGS/docs: (a) il frame è di 14 byte sul filo (`02 aa aa aa 82 00 00 04 00 01 ss 00 00 00`), non 11; (b) l'indirizzo è wCommandRegister dal descriptor: **prima del parse del descriptor `dev+0x58` vale 0**, quindi il test fatto con SET_POWER a 0x000001 pre-enumerazione non replica Windows. (c) Il boot ETW conferma che Windows **non manda alcuna SET_POWER** durante l'enumerazione (FINDINGS §5-ter) — la manda solo se i flag del descriptor indicano supporto power (stati `CheckingIfDeviceSupportsPowerCommand*`, flag da desc[24] bit6/bit7 → dev+0x7d/0x7c).

---

## D8 — Formula body length dall'input header

**Verdetto: CONFERMATO per il path V0; (diversa dalla spec v1.0, che però non si applica).**

Driver Linux: `blen = (((hdr[1]>>4) & 0xF) | (hdr[2] << 4)) * 4`. Posto `u16 = hdr[1] | hdr[2]<<8`, questa è esattamente `(u16 >> 4) * 4`.

**Decomp V0** (`ReadingResponseRemainingEntry` @0x140007e50, `hidspi_V0_ReadEntries.txt` righe 46-54):
```
movzx eax, word [rcx + 0xc9]   ; u16 = header byte1..2
shr rax, 2
and rax, 0x3ffffffffffffffc    ; = (u16 >> 4) * 4
add rax, 5                     ; + 5 byte di read approval
```
→ transfer body = `(u16>>4)*4 + 5`, cioè body = `(u16>>4)*4`. Identica alla formula del driver. Controprova nel trace della stessa funzione: `shr rax,4; lea rax,[rax*4 - 3]` = content length = body−3 (@0x140007f25-0x140007f29). E `ValidateResponseHeader` @0x1400089f7 impone `(u16 & 0xF) == 0`, quindi il nibble basso di hdr[1] scartato dalla formula deve comunque essere 0 (il driver Linux dovrebbe validarlo, insieme a hdr[3]==0x5A).

**Spec v1.0** (§7.1.3, pag. 23): lunghezza = bit 13:0 di u16, in unità di 4 byte → `(u16 & 0x3FFF) * 4`, con bit14 = Last Fragment Flag. Diversa dal V0 (dove i 4 bit bassi sono riservati-zero e la lunghezza sta nei bit 15:4). Le due formule coincidono numericamente solo quando `u16 & 0xF == 0` e `u16 < 0x4000`… no: non coincidono affatto (differiscono di un fattore 16); è un'ulteriore prova che il device NON parla v1.0: `32 10 00 5A` in v1.0 significherebbe body = 0x1000*4 = 16 KB.

---

## Errori trovati in HIDSPI_PROTOCOL.md

1. **Riga 15 — "Ogni risposta dal dispositivo inizia con sync bytes (0xFF) seguiti dall'header"**: FALSO. 0xFF è il placeholder **trasmesso dall'host** come 5° byte della Read Approval (spec §7.1.2 pag. 23; decomp `or dword[buf+1], 0xff000000`). L'header sono i 4 byte RX e il sync è 0x5A in coda (byte3), non 0xFF in testa.
2. **Righe 22, 43, 118-121 — "tipo 0 = ACK/Ready" e "ACK pattern: 03 00 00 00 (non ha 0x5A! Pattern speciale)"**: FALSO. Non esiste un report "ACK" type 0: `ValidateResponse` ammette solo i type {1,4,5,7,8} oltre al 3 (bitmask 0x1B2 @0x140008686). `03 00 00 00` è il **body della reset response** (len16=3=content 0+3, ContentID 0, 1 pad), letto nella seconda transazione della STESSA reset response. La narrazione "due letture, il device cambia stato da solo tra RESET_RSP e ACK" descrive in realtà header-read + body-read di un solo input report.
3. **Righe 58-60 — approval bytes APPR7/APPR8 (0x00→0x03)**: FALSO. Non sono campi (vedi D1): sono residui del buffer TX riusato di Windows (byte 7 del CSV = byte7 della precedente DESCREQ = 0x03).
4. **Riga 64 — "0x001000: input_register (dopo device descriptor parse)"**: FALSO per questo device. 0x1000/0x1004/0x2000 sono gli esempi ACPI della spec v1.0 (pag. 16-17). In V0 l'input register post-parse è `word[descriptor+8]` (dev+0x50); sul nostro device resta 0x000000.
5. **Riga 79 — "Il dispositivo risponde con ACK (03 00 00 00), poi invia IRQ"**: FALSO. La write è TX-only; nessuna risposta inline è definita (spec §7.2.3 pag. 31; `SendingWriteToBusEntry` = singolo transfer da 10 byte). Ciò che appare su MISO durante la write non è protocollo.
6. **Righe 197-199 e 207-208 e tabella "Riepilogo Fix" — "In Windows TX+RX usano LO STESSO opcode 0x02 nella stessa operazione" / fix "TX+RX stesso opcode"**: FALSO (vedi D4). Il fix CRITICAL corretto è l'Opzione B (write-only → IRQ → read 0x0B), che il documento propone solo come alternativa.
7. **Riga 29 — "Byte 1: [LENGTH_LOW_4bit][FRAGMENT_ID_4bit]"**: impreciso. Il nibble basso di byte1 non è un fragment ID utilizzabile: `ValidateResponseHeader` **rifiuta** l'header se ≠ 0 (@0x1400089f7). Da documentare come "riservato, deve essere 0".
8. **Righe 190-192 e 159-161 — "State 4: type==7 → hid_input_report()" / "Fase 5: 0x0B read → report HID type=7"**: quasi certamente FALSO. Type 7 = DEVICE_DESC; i report di input a runtime sono type **1** (Data) sia per la spec (pag. 25) sia per V0 (`VerifyAndCompleteTransfer` @0x140008ba8 confronta il type dell'header con 1 come caso principale). Il boot ETW etichetta il runtime come "DATA".
9. **Righe 248-250 — "ConfigureTransfer (fcn.0xa664): setta approval bytes (0x00 → 0x03)"**: FALSO. In `hidspicx_a664_ConfigureTransfer.txt` (righe 137-162) i valori 2/3/4/5 scritti a `byte[buf]` sono gli **Output Report Type** della spec v1.0 (2=report desc request, 3=set feature, 4=get feature, 5=output report), non approval bytes. (Sezione comunque riferita al path Cx, non applicabile al device.)
10. **Sezione "Driver Windows — hidspicx.sys" in generale**: modello sbagliato per questo device (già noto da FINDINGS): descrive HidSpiCx/spec v1.0, incluso il dubbio su State2 type==4 (che in Cx è la command response, irrilevante per il path V0).
11. Incoerenza tra documenti: HIDSPI_PROTOCOL.md riga 131 dice vendor=0x03A8 product=0x0002, VERIFICATION_PLAN.md riga 332 dice vendor=0x045E product=0x0C19. Una delle due è errata (non determinabile da spec/decomp; da verificare sul dump del descriptor).

### Errori in VERIFICATION_PLAN.md §2.6 (approval bytes)
La tabella "Prima di DESCREQ 0x00 / Dopo device desc 0x03 / Runtime 0x0A" è un artefatto (vedi D1): il valore corretto da trasmettere è indifferente (il device non campiona MOSI dopo il byte 4 dell'approval); Windows di fatto trasmette spazzatura di buffer. Da rimuovere come "requisito". Il resto di §2.3 ("input register iniziale 0x0000") è invece CONFERMATO.

### Nota su FINDINGS-hidspi-V0-DECODED.md (per completezza)
- §3 SET_POWER: lunghezza reale 14 byte con 3 pad; indirizzo = wCommandRegister (desc[16]), non necessariamente l'output address iniziale (vedi D7).
- §3 READ body: la formula "lunghezza arrotondata a multiplo di 4 + 5" è confermata al bit (`(u16>>4)*4+5`).
- §2: confermo che `[dev+0x50]` non è scritto in Initialize, ma viene scritto dal parse del descriptor (desc offset 8) insieme al flag `[dev+0x64]` — quindi "input 0x000000" vale solo finché il descriptor non dichiara un wInputRegister diverso.

---

## Riepilogo verdetti

| # | Domanda | Verdetto |
|---|---------|----------|
| 1 | Read approval 9 byte con "approval byte7/8" | Approval = 5 byte (opcode+addr24BE+0xFF); byte 5-8 = RX. Byte7/8 "di approval" SMENTITI (artefatto buffer riusato). Nessun cambio di valore tra fasi; cambia solo l'indirizzo dopo il parse del descriptor |
| 2 | Interrupt prima di ogni read | CONFERMATO (spec pag. 22-23, 26; V0 `c_readingOnInterrupt`, timer 2 s). Latenza descriptor: ≤1 s da spec |
| 3 | Frame DESCREQ `02 00 00 01 42 00 00 03 00 00` | CONFERMATO byte-per-byte contro `ConfiguringDescriptorTransferEntry`; 0x000001 corretto (_DSM func1 + ETW) |
| 4 | Risposta inline vs read separata | READ separata gated su IRQ — CONFERMATO; "TX+RX stesso opcode" SMENTITO |
| 5 | Reset response | Header `32 10 00 5A` + body `03 00 00 00`; nessun ACK, si prosegue con DESCREQ; retry ~109 ms NON in spec; unsolicited → ripartire dalla descriptor request (spec §6.1.3) |
| 6 | Device descriptor | V0: body 32 byte, contenuto 28 byte a body+3, layout tipo HID-I2C con wReportDescRegister/wInputRegister/wOutputRegister/wCommandRegister; parse "4 byte [len_lo][len_hi][??][5A]" SMENTITO (byte0 = type\|version; contenuto a offset 3 del body) |
| 7 | SET_POWER | Spec: comando type 0x07/ID 0x01. V0: frame 14 byte a wCommandRegister; il frame documentato ha lunghezza e indirizzo imprecisi; Windows non lo manda al boot |
| 8 | `blen = ((hdr[1]>>4)&0xF \| hdr[2]<<4)*4` | CONFERMATO identico al decomp V0 (`(u16>>4)*4`); aggiungere validazioni hdr[3]==0x5A e (hdr[1]&0xF)==0 |
