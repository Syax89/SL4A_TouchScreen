# amdspi.sys — Windows AMD SPI Driver (Decompilato)

Driver Windows per il controller AMD FCH SPI. Responsabile della comunicazione SPI di basso livello.

## File Sorgente

Tutti i file in `~/Scrivania/decomp/amdspi/` sono funzioni decompilate da `amdspi.sys` (x64).

---

## Indice Funzioni

| Indirizzo | Nome File | Descrizione |
|-----------|-----------|-------------|
| 0x1000 | 0x1000_get_context.txt | Ottiene il device context |
| 0x19c0 | 0x19c0_write8.txt | Scrittura MMIO 8-bit |
| 0x19d0 | 0x19d0_read8.txt | Lettura MMIO 8-bit |
| 0x19e0 | 0x19e0_read8b.txt | Lettura MMIO 8-bit (variante) |
| 0x1e34 | 0x1e34.txt | (utility) |
| 0x1fbc | 0x1fbc_write_mmio.txt | Write MMIO generico |
| 0x1fd8 | 0x1fd8_write_mmio2.txt | Write MMIO generico 2 |
| 0x21c0 | 0x21c0.txt | (utility) |
| 0x2ad4 | 0x2ad4.txt | (setup transfer) |
| 0x2be4 | 0x2be4_transfer_data.txt | Riempimento FIFO con dati TX |
| 0x2d28 | 0x2d28.txt | (gestione buffer) |
| 0x2ff4 | 0x2ff4.txt | (gestione completamento) |
| 0x3528 | 0x3528_sub_transfer.txt | Sub-transfer: gestisce TX+RX nello stesso CS |
| 0x3994 | 0x3994.txt | (gestione errori) |
| **0x3c20** | **0x3c20_transfer.txt** | **Esecuzione trasferimento SPI completo** |
| 0x4684 | 0x4684.txt | (cleanup) |
| **0x4bac** | **0x4bac.txt** | **Esecuzione segmento (TX+RX)** |
| **0x54d0** | **0x54d0_submit_handler.txt** | **Entry point: submit I/O request** |
| 0x6d50 | 0x6d50.txt | (timer/cleanup) |
| 0x6f84 | 0x6f84_restore_register_prefix.txt | Ripristina 0x22 e 0x44 dopo trasferimento |
| 0x6fc0 | 0x6fc0_read_register_prefix.txt | Legge register prefix da 0x22 |
| 0x7714 | 0x7714.txt | (completamento I/O) |
| 0x7ba0 | 0x7ba0.txt | (utility) |

---

## Funzioni Chiave

### fcn.0x3c20 — Transfer (Esecuzione Completa)

**Chiamata da**: 0x54d0 (submit handler)

**Logica** (dall'assembly):
1. Alloca buffer DMA (512 byte, tag 'SPI2')
2. Chiama `fcn.0x3528` (sub_transfer) per configurare il trasferimento
3. Se sub_transfer restituisce 0, errore
4. Chiama `fcn.0x6fc0` — **legge register prefix da MMIO+0x22** (16-bit)
5. Chiama `fcn.0x19c0` — write8 per scrivere a registro (context+0x4C)
6. Salva valore letto in variabile globale `[0x14001e814]`
7. Salva `[context+0x20]` — sembra un contatore di segmenti
8. Divide per 64 (0x40) — calcola numero di segmenti?
9. Se resto != 0 → branch condizionale
10. Chiama `fcn.0x2be4` — **transfer_data**: riempie FIFO e esegue

### fcn.0x4bac — Execute Segment

**Ordine operazioni** (confermato dal nostro codice):

```
1. read8(0x1D)                         // leggi ALT_CS
2. AND 0xFC, OR 0x01                   // preserva bit 7-2, azzera CS, setta bit 0
3. write8(0x1D)                         // scrivi ALT_CS (enable strobe)

4. Clear FIFO:
   ctrl0 = read32(CTRL0)
   ctrl0 &= ~BIT(20)                   // clear FIFO_CLEAR
   write32(CTRL0, ctrl0)
   ctrl0 |= BIT(20)                    // set FIFO_CLEAR (rising edge)
   write32(CTRL0, ctrl0)

5. write8(opcode, 0x45)                // opcode in OPCODE_REG

6. Secret bits:
   ctrl0 = read32(CTRL0)
   ctrl0 |= 0x60040000                 // bits 30, 29, 18
   // NON setta bit 23 (TXMODE)!
   // NON modifica bit 21 (PRESERVED)!
   write32(CTRL0, ctrl0)

7. Speed config:
   r44 = read16(0x44)                  // leggi speed+opcode
   r44 = (r44 & 0xF0FF) | (nibble << 8) // modifica speed
   r44 = (r44 & 0x0FFF) | (nibble << 12)
   write16(0x44, r44)                  // scrivi — OVERWRITE 0x45!

8. write8(TX_COUNT, 0x48)             // byte da trasmettere

9. Fill FIFO (0x80+):
   for i in 0..tx_len-1:
     write8(data[i], 0x80 + i)

10. write8(RX_COUNT, 0x4B)            // byte da ricevere
    // Per write: RX_COUNT = 0
    // Per read: RX_COUNT = rx_len

11. write8(opcode, 0x45) AGAIN         // re-write dopo speed config!

12. Secret bits AGAIN:
    ctrl0 = read32(CTRL0)
    ctrl0 |= 0x60040000
    write32(CTRL0, ctrl0)

13. Trigger:
    write8(0x80, 0x47)                // CMD_TRIGGER bit7

14. Wait busy (lfence prima):
    while (read32(CTRL0) & BIT(31))
      pause/spin

15. Restore (fcn.0x6f84):
    write16(0x22, saved_prefix)       // ripristina register prefix
    write16(0x44, saved_44)           // ripristina speed/opcode
```

### fcn.0x54d0 — Submit Handler

**Entry point** per ogni richiesta I/O SPI.

Chiama:
1. `fcn.0x3c20` (transfer) — esecuzione principale
2. `fcn.0x6f84` (restore) — ripristino registri

### fcn.0x2be4 — Transfer Data

**Riempie il FIFO** con i dati TX del trasferimento.
Chiamato da `fcn.0x3c20` per ogni segmento.

### fcn.0x6fc0 — Read Register Prefix

Legge 16-bit da MMIO+0x22 e salva nel context.
Questo valore viene usato da `fcn.0x6f84` per il ripristino.

### fcn.0x6f84 — Restore Register Prefix

Ripristina i valori salvati:
- `write16(0x22, saved_value_from_0x6fc0)`
- `write16(0x44, saved_44)`

Questo garantisce che lo stato dei registri tra un trasferimento e l'altro sia consistente.

---

## hidspicx.sys — HID Protocol Driver

I file in `~/Scrivania/decomp/hidspicx_*.txt` contengono il driver HID-over-SPI:

| File | Funzione |
|------|----------|
| hidspicx_dd64_VerifyResetResponse | Verifica header RESET_RSP (type=3) |
| hidspicx_f458_HidGetDeviceDescriptor | Invia DESCREQ, riceve device descriptor |
| hidspicx_d2f8_VerifyDescriptorCompletion | Verifica descrittore, invia DESCREQ2 |
| hidspicx_a664_ConfigureTransfer | Configura buffer per trasferimento |
| hidspicx_a900_ConfiguringDescriptorTransfer | Configura trasferimento descrittore |
| hidspicx_orchestrator | Loop principale — dispatch per tipo report |
| hidspicx_12bc0_PrepareDescriptor | Alloca buffer e prepara descrittore |
| hidspicx_1ef0_InitTransfer | Inizializza trasferimento |
| hidspicx_da9c_State2 | Gestione stato 2 |

### Orchestrator (fcn.0xc8d8)

Il loop principale del driver hidspi. Dispatch basato sul tipo di report:

```
1. Legge type byte dal buffer ricevuto
2. r13d = 3 (costante per RESET_RSP)
3. cmp type, 3 → se type==3, handler RESET_RSP
4. cmp type, 7 → se type==7, handler DEVICE_DESC  
5. cmp type, 8 → se type==8, handler RPT_DESC
6. Per ogni tipo, chiama la funzione appropriata
7. Loop su IRQ successive
```

Dettagli assembly:
```
mov r13d, 3           ; costante type==3
movzx edi, byte [rcx] ; leggi type byte
cmp dil, r13b          ; type == 3?
jne not_reset_rsp      ; se no, salta
; ... handler RESET_RSP ...
```

Il flag `r14b` controlla se il descrittore è stato ricevuto:
- `r14b = 0` → descrittore non ancora ricevuto
- `r14b = 1` → descrittore ricevuto

Il flag `dil` controlla se il buffer è valido:
- `setne dil` dopo `cmp` — dil=1 se il context pointer è valido

---

## Riepilogo

### Cosa Windows fa che noi NON facciamo

1. **Due 0x0B read prima di DESCREQ** — drenaggio RESET_RSP
2. **TX+RX con stesso opcode** in una singola operazione (0x02 TX + 0x02 RX)
3. **Speed config** a 0x44 che sovrascrive 0x45, poi re-write opcode
4. **Restore 0x22 e 0x44** dopo ogni trasferimento
5. **0x1D strobe** (AND 0xFC, OR 0x01) prima di ogni trasferimento
6. **Secret bits** (30, 29, 18) in CTRL0 — sempre settati
7. **NON setta TXMODE** (bit 23) — Windows non ne ha bisogno
8. **Attende GPIO IRQ** prima di leggere risposte a DESCREQ
9. **DESCREQ2** per richiedere report descriptor dopo device descriptor
10. **~962ms gap** tra ricezione descrittori e comandi di attivazione

### Cosa noi facciamo che Windows NON fa

1. **TX+RX separati** con opcode diversi (0x02 TX, 0x0B RX)
2. **Drain read singolo** invece di doppio
3. **Nessun speed config** (abbiamo rimosso per semplicità)
4. **Nessun restore 0x22/0x44** (non necessario senza speed config)
5. **Nessun 0x1D strobe** (rimosso per debug)
6. **Nessun secret bits** (rimossi per semplicità)
7. **Debug esteso** su ogni operazione SPI
