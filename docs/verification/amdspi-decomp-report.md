# amdspi.sys — Verifica indipendente del percorso TX+RX (controller AMD FCH SPI V2)

Analisi su binario `~/windrivers/amdspi.sys` (nessun PDB) disassemblato con radare2, incrociata
con i dump testuali in `~/Scrivania/decomp/amdspi/`. Tutti gli offset citati sono RVA del binario.

## Convenzioni verificate

Helper MMIO (confermati leggendo il codice, `~/windrivers/amdspi.sys`):

| fcn | semantica | prova |
|-----|-----------|-------|
| `0x19c0` | **read8**  `mov al,[rax]` | 0x1400019ca |
| `0x19d0` | **read32** `mov eax,[rax]` | 0x1400019da |
| `0x19e0` | **read16** `movzx eax,word[rax]` | 0x1400019ea |
| `0x1fbc` | **write8**  `mov [rax],cl` + `lock or` barrier | 0x140001fce |
| `0x1fd8` | **write32** `mov [rax],ecx` + barrier | 0x140001fea |
| `0x7ba0` | **write16** `mov [rax],cx` + barrier | 0x140007bb4 |
| `0x1000` | get_context (ritorna l'oggetto registri) | — |
| `0x6fc0` | salva `read16(BASE+0x22)` in ctx+0x58 | 0x140006fe0 |
| `0x6f84` | ripristina `write16(BASE+0x22, ctx+0x58)` | 0x140006fa4 |
| `0x2be4`, `0x1d24`, `0x758c`, `0x7714`, `0x7858`, `0x1e34`, `0x79e0` | **SOLO tracing WPP** (`WppAutoLogTrace`) — NON toccano MMIO | 0x140002d1d |
| `0x74e4` | calcola timeout per `KeDelayExecutionThread` | — |

`BASE` = `*(qword*)ctx` = puntatore virtuale al MMIO del controller. Ogni accesso registro è
`helper(BASE + offset)`. **Attenzione:** i moltissimi `call 0x2be4/0x1d24/...` con `r9w=0x16,0x17,…`
sono messaggi di trace WPP, non I/O: vanno ignorati nella ricostruzione della sequenza.

## Le QUATTRO funzioni di transfer e come vengono scelte

Dispatch (WDF/SPB callbacks) in `0x5ee0`–`0x61c0`, selezione su `request+0x48` (tipo transfer):

| fcn | opcode 0x45 | ruolo | RX | dove legge la risposta |
|-----|-------------|-------|----|-----|
| **`0x54d0`** | **0x02** | WRITE (PIO, chunk 70B) | `0x4B = 0` | **non legge** |
| **`0x4684`** | **0x02** | WRITE dentro SEQUENCE | `0x4B = 0` | **non legge** |
| **`0x3c20`** | **0x0B** | READ (resto PIO + bulk via DMA) | `0x4B = rx_len` | FIFO **+3** (`0x80+TX_COUNT`) |
| **`0x4bac`** | **0x0B** | READ con speed-config (PIO, chunk 64B) | `0x4B = rx_len + 1` | FIFO **+4** (`0x80+4`) |

Callers: `0x6004→54d0`, `0x602d/0x60c1→3c20`, `0x60dc/0x61ad→4684`, `0x5f26→4bac`. La sequenza
SPB (0x6040) tratta ogni segmento separatamente: `+0x48==1`→read `3c20`, `+0x48==2`→write `4684`.

**Nessuna funzione con opcode 0x02 imposta mai RX_COUNT>0 o rilegge la FIFO.** Le sole letture
combinate TX+RX in un unico trigger sono i due percorsi READ (opcode 0x0B), con TX fisso = 3 byte.

---

## (A) Write path `0x54d0` (opcode 0x02) — sola sequenza MMIO

Prologo: `ctx = get_context(); var_48=ctx`. `num_chunks = ceil(tx_len/70)` (`0x55a1`, div 0x46).
`tx_len` = arg3.

```
[una volta prima del loop]
  6fc0(dev)                          ; salva read16(BASE+0x22) -> ctx+0x58          @0x5673
  a = read8 (BASE+0x1D)              ; ALT_CS                                        @0x5687
  a = (a & 0xFC) | 0x01
  write8(BASE+0x1D, a)               ;                                               @0x56bd

[loop su ogni chunk, var_80 = 0..num_chunks-1]                                       @0x56cf
  c = read32(BASE+0x00)              ; CTRL0                                         @0x56f8
  c |= (1<<20)                       ; bts 20  (FIFO clear) — UNA sola scrittura     @0x570b
  write32(BASE+0x00, c)                                                             @0x5725
  write8 (BASE+0x45, 0x02)           ; OPCODE (1a volta)                            @0x573b
  c = read32(BASE+0x00)              ; CTRL0                                         @0x5748
  ; (salva bit30/29/18 correnti in var_7c — mai riusato)
  c |= 0x60000000                    ; bit30+29                                     @0x5771
  c |= (1<<18)                       ; bts 18                                        @0x577e
  write32(BASE+0x00, c)              ; "secret bits" = 0x60040000, UNA volta/chunk  @0x5792
  w = read16(BASE+0x44)              ; speed/opcode word                            @0x57a6
  n = read8(BASE+0x20) & 0x0F        ; nibble ENA                                   @0x57b5
  w = (w & 0xF0FF) | (n<<8)
  w = (w & 0x0FFF) | (n<<12)         ; entrambi i nibble alti = n
  write16(BASE+0x44, w)              ; scrive 0x44 (sovrascrive anche 0x45!)        @0x581c
  chunk = min(rest,70)               ; rest parte da tx_len
  write8 (BASE+0x48, chunk)          ; TX_COUNT                                     @0x5868
  for i in 0..chunk-1:
      write8(BASE+0x80+i, txbuf[base+i]) ; riempimento FIFO                         @0x58bf
  write8 (BASE+0x4B, 0x00)           ; RX_COUNT = 0  <<< nessuna ricezione          @0x5943
  write8 (BASE+0x45, 0x02)           ; OPCODE (2a volta, dopo 0x44)                 @0x5959
  s = read8(BASE+0x4C)               ; STATUS (byte!)                               @0x596d
  retries = 30 (0x1E)
  ; loop: while ((s>>31)&1) && retries: read8(BASE+0x4C); KeDelay; retries--        @0x59e9
  if !((s>>31)&1):                                                                  @0x5aa2
      write8(BASE+0x47, 0x80)        ; TRIGGER (bit7)                               @0x5ac1
      ; loop: while ((s>>31)&1) read8(BASE+0x4C)                                    @0x5ac6
  ; -> chunk successivo

[una volta dopo il loop]                                                            @0x5b2f
  a = read8(BASE+0x1D); a &= 0xFC; write8(BASE+0x1D, a)   ; ALT_CS (NO OR 1)        @0x5b68
  6f84(dev)                          ; ripristina write16(BASE+0x22, ctx+0x58)      @0x5b75
```

Nessuna lettura FIFO. RX_COUNT sempre 0. **Write = solo TX.**

---

## (B) Read path `0x3c20` (opcode 0x0B) — ramo "resto" PIO (rx_len < 64)

`ctx = get_context(); var_50=ctx`. La parte allineata a 64B va via **DMA** (`fcn.0x3528`, che NON
tocca MMIO: costruisce un pacchetto descrittore 'AeiC'/'ALDT' e lo invia via IOCTL `0x32c004`,
`fcn.0x7c48`). Il **resto** `rx_len = len%64` (`var_60`) è gestito in PIO qui sotto:

```
  6fc0(dev)                          ; salva 0x22                                   @0x3dea
  read8(BASE+0x4C)                   ; STATUS (byte) -> globale                     @0x3dfe
  ALT_CS: read8(0x1D); &0xFC; |0x01; write8(0x1D)                                   @0x3f11..3f47
  read8(BASE+0x1D)                   ; (riletto, salvato)                           @0x3f5b
  ; [se esiste parte bulk var_68] write8(0x45,0x0B); read8(0x45); DMA fcn.0x3528    @0x3f9a
  ; ---- ramo RESTO (var_60 = rx_len) @0x414e ----
  write8 (BASE+0x45, 0x0B)           ; OPCODE (1a)                                  @0x416a
  read8  (BASE+0x45)                                                               @0x417e
  c = read32(BASE+0x00); c|=(1<<20); write32(BASE+0x00, c)  ; FIFO clear (1 set)    @0x41b8
  read32(BASE+0x00)                  ; (riletto)                                    @0x41c5
  c = read32(BASE+0x00); c|=0x60000000; c|=(1<<18); write32(BASE+0x00,c) ; secret   @0x4232
  write8 (BASE+0x48, 0x03)           ; TX_COUNT = 3 (fisso)                         @0x4248
  for _ in 0..2: write8(BASE+0x80, 0x00)  ; 3 byte dummy in FIFO[0] (stesso addr)   @0x427b
  write8 (BASE+0x4B, rx_len)         ; RX_COUNT = rx_len (esatto)                   @0x4295
  write8 (BASE+0x45, 0x0B)           ; OPCODE (2a, dopo)                            @0x42ab
  read8  (BASE+0x45)                                                               @0x42bf
  retries = 10 (0x0A)
  s = read8(BASE+0x4C)               ; STATUS (byte)                               @0x42f1
  ; loop pre-trigger: while ((s>>31)&1) && retries: read8(0x4C); KeDelay; retries-- @0x42ff
  if !((s>>31)&1):                                                                  @0x4376
      write8(BASE+0x47, 0x80)        ; TRIGGER                                      @0x4397
      ; loop post-trigger: while ((s>>31)&1) read8(0x4C)                            @0x439c
  for i in 0..rx_len-1:                                                             @0x43de
      resp[i] = read8(BASE + i + 0x83)   ; <<< legge FIFO a 0x80 + TX_COUNT(3)      @0x4410/0x4447
  ALT_CS: read8(0x1D); &0xFC; write8(0x1D)   ; NO OR 1                              @0x45f2
  6f84(dev)                          ; ripristina 0x22                             @0x45ff
```

Variante `0x4bac` (READ con speed-config, chunk 64B, usata dal wrapper `0x5f26`): identica MA
- scrive **0x44** (speed) come il write path (`0x4fd6/0x5047/0x5060`, write16 `0x7ba0`);
- `write8(0x4B, rx_len + 1)`  — RX_COUNT = rx_len **+1** (`inc eax` @0x5100);
- rilegge la FIFO a **`BASE + i + 0x84`** (offset **+4**) (`0x52fe`, read8).

---

## (C) Diff write(0x54d0) vs read(0x3c20 / 0x4bac)

| passo | write `0x54d0` (0x02) | read `0x3c20` (0x0B) | read `0x4bac` (0x0B) |
|-------|-----------------------|----------------------|----------------------|
| ALT_CS 0x1D pre | read/&0xFC/\|1/write | idem | idem |
| CTRL0 FIFO-clear (bit20) | 1 read+1 set+1 write | idem | idem |
| CTRL0 secret 0x60040000 | 1 volta/chunk | 1 volta | 1 volta |
| **0x44 speed** | **SÌ** (RMW 16-bit) | **NO** | **SÌ** |
| opcode 0x45 | 0x02 ×2 | 0x0B ×2 | 0x0B ×2 |
| TX_COUNT 0x48 | = chunk (≤70) | = **3** fisso | = **3** fisso |
| FIFO TX | dati reali, `0x80+i` | 3×0 su `0x80` | 3×0 su `0x80` |
| **RX_COUNT 0x4B** | **0** | **rx_len** | **rx_len + 1** |
| opcode 0x45 (2°) | 0x02 | 0x0B | 0x0B |
| STATUS poll 0x4C | read8, bit31 | idem | idem |
| TRIGGER 0x47 | 0x80 | 0x80 | 0x80 |
| **lettura FIFO** | **nessuna** | `0x80 + TX_COUNT` = **0x83** | `0x80 + 4` = **0x84** |
| ALT_CS post | &0xFC (no \|1) | idem | idem |
| 0x22 save/restore | 6fc0/6f84 | idem | idem |

Differenze sostanziali: **solo il read path legge la FIFO**; il write path forza RX_COUNT=0. Le due
varianti read **non concordano** sull'offset di rilettura (+3 vs +4) né su RX_COUNT (esatto vs +1).

---

## (D) Risposte alle 8 domande (con evidenza)

**1. Il write path 0x54d0 (opcode 0x02) setta RX_COUNT>0? Legge la FIFO dopo il trigger?**
NO e NO. `write8(BASE+0x4B, 0)` a **0x140005943** (`xor edx,edx`). Dopo il trigger (0x5ac1) c'è solo
il poll di STATUS; **nessun** `read8(BASE+0x8x)`. Il write è puramente TX.

**2. TX e RX sono due esecuzioni separate del controller?**
SÌ, a livello SPB. Il write (0x02) e il read (0x0B) sono funzioni/richieste WDF distinte con due
trigger separati (`0x5ac1` per il write, `0x4397`/`0x528e` per il read). Il DESCREQ tipo
"scrivi 9 byte + leggi 10 byte" = **due transazioni**: una WRITE 0x02 (TX=9, RX=0) e una READ 0x0B
(TX=3 dummy, RX=10). Da notare: **dentro** la singola transazione READ, TX(3)+RX(N) avvengono in
**un unico trigger** (il controller mette la risposta nella stessa FIFO dopo i byte TX).

**3. La risposta arriva nella STESSA FIFO? A che offset?**
Stessa FIFO. Offset = **`0x80 + TX_COUNT`**:
- `0x3c20`: TX_COUNT=3 → legge a **0x83** (`lea rax,[rax+rcx+0x83]`, **0x140004410 / 0x140004447**).
- `0x4bac`: legge a **0x84** = 0x80+4 (**0x1400052fe**), coerente col suo RX_COUNT=rx_len+1.
Il write path invece scrive i TX a `0x80+i` (0x58bf) e non rilegge nulla.

**4. Sequenza completa ordinata (write vs read).** Vedi (A) e (B). Differenze chiave in (C).

**5. ALT_CS 0x1D: pattern read8 → AND 0xFC → OR 0x01 → write8?**
SÌ, esatto, in **tutte** le funzioni di transfer:
- 0x54d0: OR 1 in pre (`0x140005695`/`0x1400056a3`), solo AND 0xFC in post (`0x140005b4c`).
- 0x3c20: `0x140003f1f`/`0x140003f2d` (OR 1), post `0x1400045d6` (solo AND).
- 0x4684: `0x140004b04` … ; 0x4bac: `0x140004e5b`+.
Il `| 0x01` compare **solo in ingresso**; in uscita si azzerano i bit 1-0 (`& 0xFC`) senza rimettere 1.

**6. Scritture a 0x49 / 0x4A ("strobe")?**
**NON ESISTONO in tutto il binario.** Ricerca byte esaustiva su `add rax,0x49/0x4a`,
`add rcx,0x49/0x4a`, `lea/mov/[rax+0x49]`, `[rax+0x4a]`: **zero occorrenze**. I "3 byte" del read
path vanno tutti a `BASE+0x80` (FIFO), non a 0x49/0x4A. Gli offset registro effettivamente usati sono
solo: 0x1D, 0x20, 0x22, 0x44, 0x45, 0x47, 0x48, 0x4B, 0x4C, 0x80+.

**7. "Secret bits" CTRL0 (0x60040000 = bit30+29+18): dove e quante volte? bit 21?**
Applicati **una sola volta per segmento/chunk**: `or 0x60000000` + `bts 18` seguito da un unico
`write32(CTRL0)` (write: **0x140005771/577e/5792**; read 3c20: **0x140004209/421a/4232**;
4bac: **0x140004fa0/4fb1/4fc9**). **Non** c'è una seconda applicazione ("secret again"). Il codice
salva i bit 30/29/18 preesistenti in una variabile (`var_7c`, es. 0x140005755-69) ma non la riusa.
**Bit 21 (0x200000): mai letto/settato/clearato.** È solo *preservato per omissione* (il driver fa OR
di 0x60040000 su ciò che c'è, quindi il valore hardware di bit21 sopravvive). Ricerca di `0x800000`
(bit23 TXMODE) e `0x200000`: **nessuna** manipolazione esplicita in nessuna delle funzioni transfer.

**8. Registro 0x44: scritto nel write path? valore? ordine vs 0x45?**
- Write `0x54d0`: **SÌ**. `read16(0x44)` (0x57a6) → RMW → `write16(0x44, w)` (0x581c). Formula:
  `w = (w & 0xF0FF)|((ENA&0xF)<<8)` poi `w = (w & 0x0FFF)|((ENA&0xF)<<12)` (ENA = read8(BASE+0x20)&0xF).
  Ordine: **DOPO** il primo `write8(0x45,0x02)` (0x573b) e dopo i secret bits.
- **Seconda scrittura opcode 0x45 dopo 0x44: SÌ.** `write8(0x45, 0x02)` di nuovo a **0x140005959**
  (perché la write16 su 0x44 sovrascrive anche il byte 0x45). Idem read: 2a scrittura a 0x42ab (3c20)
  / 0x511d (4bac).
- Read `0x3c20`: **NON** scrive 0x44. Read `0x4bac`: **SÌ** scrive 0x44 (come il write path).

---

## (E) VERDETTO — cosa deve fare il driver Linux per TX+RX come Windows

**Punto 1 — modello concettuale.** Windows **non** legge mai la risposta dentro una transazione
opcode 0x02. Un DESCREQ (scrivi comando + leggi risposta) è **due transazioni SPI separate**:
1. WRITE: opcode `0x45=0x02`, `TX_COUNT(0x48)=N`, FIFO[0..N-1]=payload, `RX_COUNT(0x4B)=0`, trigger.
2. READ:  opcode `0x45=0x0B`, `TX_COUNT(0x48)=3`, FIFO[0..2]=0 (dummy), `RX_COUNT(0x4B)=M`, trigger,
   poi rileggi la FIFO. Se il tuo driver le fa già come due `spi_transfer`/messaggi, sei allineato.

**Sequenza registri per una READ combinata (l'unico caso TX+RX in un trigger che Windows usa):**
```
1.  read8(0x1D); &=0xFC; |=0x01; write8(0x1D)        ; seleziona CS
2.  ctrl0=read32(0x00); ctrl0|=BIT(20); write32(0x00) ; FIFO clear — UN SOLO set (niente toggle)
3.  write8(0x45, 0x0B)                                ; opcode
4.  ctrl0=read32(0x00); ctrl0|=0x60040000; write32(0x00) ; secret bits (una volta)
5.  (opzionale, come 0x4bac) read16(0x44); RMW nibble ENA; write16(0x44); poi RE-write8(0x45,0x0B)
6.  write8(0x48, TX_COUNT)                            ; = 3 (byte comando/indirizzo dummy)
7.  FIFO[0..TX_COUNT-1] = byte TX (Windows scrive 0x00)
8.  write8(0x4B, RX_COUNT)                            ; = M
9.  write8(0x45, 0x0B)                                ; RE-write opcode (obbligatorio se hai fatto passo 5)
10. write8(0x47, 0x80)                                ; trigger
11. attendi busy   [vedi nota sotto]
12. leggi risposta da FIFO a  0x80 + TX_COUNT         ; (Windows 3c20 = +3; 4bac = +4 con RX_COUNT+1)
```

**Punto 2 — offset di rilettura + RX_COUNT (fonte della confusione +3 vs +4).** Le due varianti sono
autoconsistenti: la risposta utile inizia a `0x80 + TX_COUNT + skip`, dove `skip = RX_COUNT - rx_len`.
- `0x3c20`: TX_COUNT=3, RX_COUNT=rx_len (skip=0) → **offset 0x83**.
- `0x4bac`: TX_COUNT=3, RX_COUNT=rx_len+1 (skip=1) → **offset 0x84**.
Regola robusta per Linux: **leggi a `FIFO + TX_COUNT`** e programma **`RX_COUNT = rx_len`** (variante
3c20, la più semplice e coerente). L'offset fisso `+4` del vostro `SPI_REGISTERS.md` funziona solo
perché finora TX_COUNT vale 3 e usate RX_COUNT+1 (variante 4bac); non è una costante universale.

**Punto 3 — busy poll (IMPORTANTE).** I loop "attendi busy" di Windows leggono `0x4C` con **read8** e
testano **bit 31** di un valore a 8 bit → la condizione `(byte>>31)&1` è **sempre 0**: i poll sono di
fatto **no-op** (nessuna vera attesa né prima né dopo il trigger; il ritardo reale lo danno le call di
trace WPP interposte). Prove: read8 a 0x4C in 0x596d/0x59a2 (54d0), 0x42f1/0x4325 (3c20),
0x5165/0x51de/0x52a4 (4bac); test `shr 0x1f` in 0x59e9/0x5aa6, 0x42ff/0x437c, 0x51a3-ecc.
Conseguenza pratica: **NON** modellare il completamento su `STATUS(0x4C) bit31`. Il vostro driver fa
già bene a pollare `CTRL0(0x00) bit31` (il vero busy). Mantenete quello + un `udelay` post-trigger.

**Punto 4 — dettagli da replicare/evitare:**
- Secret bits `0x60040000`: una volta per segmento, **dopo** l'opcode, **prima** di TX/RX count. Va bene una volta per messaggio.
- FIFO clear (bit20): **un solo set**, non il toggle clear→set. (Se la vostra HW richiede il fronte 0→1, è una scoperta *vostra* lato Linux, non presente nel driver Windows.)
- TXMODE bit23 e bit21: **il driver Windows non li tocca**. Se a voi serve bit23 per opcode non-0x0B, resta una vostra scoperta hardware, giusto tenerla separata dai "fatti da decomp".
- ALT_CS: `|0x01` solo in apertura, solo `&0xFC` in chiusura.
- 0x22: salva prima / ripristina dopo (wrappa l'intero messaggio/sequence, non il singolo segmento).

---

## (F) Errori trovati nella documentazione esistente

### `AMDSPI_DECOMP.md`
1. **Indice funzioni errato:** `0x3528` NON "gestisce TX+RX nello stesso CS" — non tocca alcun MMIO,
   costruisce un descrittore DMA 'AeiC'/'ALDT' e lo invia via IOCTL `0x32c004` (`fcn.0x7c48`). `0x4684`
   NON è "cleanup": è un **WRITE handler (opcode 0x02) dentro una SPB sequence** (RX_COUNT=0). `0x54d0`
   è il WRITE handler completo, non un semplice wrapper che "chiama 0x6f84".
2. **Sequenza di `0x4bac`, passo 4 (FIFO clear "clear→set / rising edge"): FALSO.** Il codice fa un
   solo `read32; bts 20; write32` (0x4f37/…). Nessuna scrittura di clear preventiva.
3. **Passo 12 "Secret bits AGAIN": FALSO.** I secret bits sono applicati **una sola volta** per
   segmento. Non esiste una seconda scrittura.
4. **Passo 14 "while (read32(CTRL0) & BIT(31))": DOPPIAMENTE errato.** Il poll legge **0x4C** (non
   CTRL0) con **read8** (non read32); testando bit31 di un byte il loop è un **no-op**. Il vero busy
   bit è CTRL0(0x00) bit31, ma il driver Windows **non lo polla**.
5. Riepilogo "TX+RX con stesso opcode in una singola operazione (0x02 TX + 0x02 RX)": **FALSO.** Con
   opcode 0x02 RX_COUNT è sempre 0. La combinazione TX+RX in un trigger esiste **solo** con opcode 0x0B.
6. Corretto invece: restore di 0x22 (non 0x44), 0x1D strobe, secret bits (30/29/18), niente TXMODE,
   speed su 0x44 con re-write dell'opcode.

### `SPI_REGISTERS.md`
1. **0x49 / 0x4A "STROBE_V2 — scrivi 0x00": NON confermato dal decomp.** amdspi.sys non scrive **mai**
   a 0x49/0x4A. La sezione "V2 Path … Scrivi 0x00 a STROBE 0x49/0x4A" e la riga tabella "Strobe 0x49,
   0x4A" descrivono comportamento del *vostro* driver Linux, non di Windows. Da marcare come tale.
2. **STATUS 0x4C "bit31 = busy su V2 (ioread32)": fuorviante.** Windows lo legge come **byte** e il suo
   poll è inefficace. Il busy affidabile è CTRL0 bit31. La tabella dà 0x4C come 32bit; Windows lo tratta
   come 8bit.
3. **Bit 21 "PRESERVED — Windows lo preserva (legge e mantiene)": impreciso.** Windows non lo legge né
   lo maschera deliberatamente; fa `|= 0x60040000`, quindi bit21 sopravvive per pura omissione. I valori
   osservati tipo `0x6fac100b`/`0x6f8c080b` sono stato hardware, non scritti dal driver.
4. **FIFO "Lettura RX … offset +4 … costante": solo parzialmente vero.** È +4 nella variante `0x4bac`
   (che usa RX_COUNT+1); nella variante `0x3c20` è **+3 = 0x80+TX_COUNT** con RX_COUNT esatto. L'offset
   corretto è `0x80 + TX_COUNT (+skip)`, non una costante +4.
5. **CTRL0 bit20 "richiede transizione 0→1 / clear→set": non è ciò che fa Windows** (un solo set). Se
   è un requisito, è una scoperta lato Linux.
6. Corretto: opcode in 0x45, trigger 0x47 bit7, secret bits 30/29/18, 0x44 come speed+opcode 16bit con
   necessità di re-write opcode, mappa base FIFO 0x80.
