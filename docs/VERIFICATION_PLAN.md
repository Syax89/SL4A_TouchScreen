# Piano di Verifica Completo — MSHW0231 Linux Driver
>
> ## Sommario
>
> Verifica sistematica di **ogni** componente del driver Linux per Surface Laptop 4 AMD touchscreen,
> confrontando con la decompilazione dei driver Windows (`amdspi.sys` + `hidspi.sys`).
>
> **Principio**: Misura mille volte, modifica una. Ogni modifica deve essere verificata da
> un sub-agente che confronta con la decomp Windows.

---

## 0. Pre-verifica: Raccolta Hardware

### 0.1 ACPI DSDT
- [ ] File: `~/dsdt.dsl` — cercare `MSHW0231` per estrarre:
  - Indirizzi registro SPI: `_DSM` functions 1/2/3
  - GPIO pin e configurazione IRQ (pin 0x15, flags 0x409)
  - MMIO base SPI controller: `0xFEC10000`
- [ ] File: `~/ssdt*.dsl` — cercare `AMDI0060` per configurazione SPI controller

### 0.2 Report Descriptor
- [ ] File: `~/Scrivania/wintrace/mshw0231_report_descriptor.txt`
  - Verificare lunghezza (936 byte)
  - Verificare Usage Pages e Report IDs

---

## 1. SPI Controller (spi-amd.c) — Verifica Completa

### 1.1 Mappa Registri (confronto spi-amd.c vs decomp)

| Registro | Offset | Linux (spi-amd.c) | Windows (amdspi.sys) | Match? |
|----------|--------|-------------------|----------------------|--------|
| CTRL0 | 0x00 | AMD_SPI_CTRL0_REG | read32/write32 | [ ] |
| ALT_CS | 0x1D | AMD_SPI_ALT_CS_REG | read8/write8 | [ ] |
| ENA_REG | 0x20 | AMD_SPI_ENA_REG | presente | [ ] |
| REG_PREFIX | 0x22 | read16 in probe | fcn.0x6fc0 (16-bit!) | [ ] |
| SPEED_OPCODE | 0x44 | **NON USATO** | USATO (16-bit write) | [ ] |
| OPCODE_REG | 0x45 | AMD_SPI_OPCODE_REG | write8 singola/doppia | [ ] |
| CMD_TRIGGER | 0x47 | AMD_SPI_CMD_TRIGGER_REG | toggle bit7 | [ ] |
| TX_COUNT | 0x48 | AMD_SPI_TX_COUNT_REG | write8 | [ ] |
| STROBE_V2_A | 0x49 | writeb(0x00) | presente in decomp? | [ ] |
| STROBE_V2_B | 0x4A | writeb(0x00) | presente in decomp? | [ ] |
| RX_COUNT | 0x4B | AMD_SPI_RX_COUNT_REG | write8 | [ ] |
| STATUS | 0x4C | AMD_SPI_STATUS_REG | read8 in fcn.0x3c20 | [ ] |
| SPEED_REG | 0x6C | AMD_SPI_SPEED_REG | presente | [ ] |
| FIFO_BASE | 0x80 | AMD_SPI_FIFO_BASE | 70 byte | [ ] |

### 1.2 CTRL0 Bit Map

| Bit | Nome Linux | Valore | Windows Decomp | Corretto? |
|-----|-----------|--------|----------------|-----------|
| 31 | BUSY | poll per wait | BIT(31) wait | [ ] |
| 30 | SECRET_30 | 0x40000000 | parte di SECRET_BITS | [ ] |
| 29 | SECRET_29 | 0x20000000 | parte di SECRET_BITS | [ ] |
| 23 | TXMODE | 0x00800000 | Windows **NON** lo setta | [ ] |
| 21 | PRESERVED | 0x00200000 | Windows lo preserva | [ ] |
| 20 | FIFO_CLEAR | clear→set toggle | fcn.0x4bac: clear→set | [ ] |
| 18 | SECRET_18 | 0x00040000 | parte di SECRET_BITS | [ ] |
| 16 | EXEC_CMD | clear→set toggle | V1 path only | [ ] |

### 1.3 Sequenza Esecuzione Segmento (fcn.0x4bac decomp)

**Confronto passo-passo con `amd_spi_exec_segment()` in spi-amd.c:**

```
Decomp Windows (fcn.0x4bac):          vs    Linux (amd_spi_exec_segment):

1. read8(0x1D)                              ❌ MANCA in exec_segment
   AND 0xFC, OR 0x01                        (solo in select_chip: cs & 0x3, NON forza 0x01)
   write8(0x1D)

2. Clear FIFO (clear→set bit20)          ✅ amd_spi_clear_fifo_ptr (toggle)

3. write8(opcode, 0x45)                   ✅ amd_spi_set_opcode (singola)

4. Secret bits:                           ⚠️  amd_spi_setup_v2_regs (solo in host_transfer,
   ctrl0 |= 0x60040000                       NON in exec_segment!)
   write32(CTRL0)

5. Speed config:                          ❌ MANCA in exec_segment
   write16(0x44, config)                  (fatto solo in host_setup via amd_set_spi_freq
                                            usando ENA_REG + SPEED_REG, mai 0x44)

6. write8(TX_COUNT, 0x48)                 ✅ presente

7. Fill FIFO (0x80+)                      ✅ presente

8. write8(RX_COUNT, 0x4B)                 ✅ presente

9. write8(opcode, 0x45) AGAIN             ❌ MANCA (doppia scrittura dopo 0x44 overwrite)

10. Secret bits AGAIN:                    ❌ MANCA
    ctrl0 |= 0x60040000

11. write8(0x80, 0x47) Trigger            ✅ toggle via amd_spi_execute_opcode

12. Strobe 0x49, 0x4A                    ✅ writeb(0x00)

13. Wait busy (CTRL0 bit31)               ✅ amd_spi_busy_wait

14. Restore 0x22 (fcn.0x6f84)            ⚠️  letto in probe ma MAI ripristinato
```

### 1.4 Funzione `amd_spi_setup_v2_regs()`

**Codice attuale** (spi-amd.c:170-176):
```c
static void amd_spi_setup_v2_regs(struct amd_spi *amd_spi) {
    amd_spi_setclear_reg32(amd_spi, AMD_SPI_CTRL0_REG,
                           AMD_SPI_SECRET_BITS, 0);
}
```

**Problemi:**
- Chiamata solo in `host_transfer` (line 427), NON in `exec_segment`
- `setclear` con clear=0 significa OR, non preserva bit 21
- **Dovrebbe preservare bit 21** (come fa Windows leggendo prima ctrl0)

**Fix proposto:**
```c
static void amd_spi_setup_v2_regs(struct amd_spi *amd_spi) {
    u32 ctrl0 = amd_spi_readreg32(amd_spi, AMD_SPI_CTRL0_REG);
    ctrl0 |= 0x60040000;  // bits 30+29+18
    // NON tocca bit 21 (preservato), NON setta bit 23 (TXMODE)
    amd_spi_writereg32(amd_spi, AMD_SPI_CTRL0_REG, ctrl0);
}
```

### 1.5 ALT_CS (0x1D) — Comportamento

**Windows** (fcn.0x3c20 decomp):
```
read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
```
Forza **sempre** cs=1 con bit0=1. Il bit0 potrebbe essere uno strobe di enable.

**Linux** (spi-amd.c:106-109):
```c
static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs) {
    amd_spi_setclear_reg8(amd_spi, AMD_SPI_ALT_CS_REG,
                          cs, AMD_SPI_ALT_CS_MASK);
}
```
Usa `cs & 0x3` come puro CS select. Supporta cs=0,1,2,3.

**Potenziale problema**: Se il touchscreen è su cs=0, il valore sarebbe `OR 0x00` = nessun bit settato, mentre Windows scrive `OR 0x01`.

### 1.6 RX FIFO Offset

**Linux**: `fifo_pos + 4` (offset +4 fisso)
**Verifica**: Necessario. Il controller sovrascrive le prime posizioni del FIFO con dati RX. Con TX=0, i dati RX compaiono a offset 0. Con TX>0, i dati RX dovrebbero comparire dopo i dati TX? Il fix +4 fissa una posizione dopo l'header. **Da verificare con test reale**.

### 1.7 Funzione `amd_spi_host_transfer` — Multi-segment

La funzione attuale esamina ogni transfer nella lista e:
- Per TX: estrae opcode dal primo byte, esegue chunking FIFO
- Per RX standalone: usa sempre opcode 0x0B

**Problema**: TX e RX sono operazioni **separate** anche quando appartengono alla stessa transazione logica (es. DESCREQ dove TX=0x02 e RX=0x02 dovrebbero essere insieme).

---

## 2. HID Protocol (spi-hid-core.c) — Verifica Completa

### 2.1 State Machine

| Stato | Nostro Driver | Windows Decomp |
|-------|---------------|----------------|
| 0 (WAIT_RESET) | type==3: drain x2 + DESCREQ | VerifyResetResponse: se type==3→avanza e invia DESCREQ |
| 1 (WAIT_DESC) | type==7: parse desc → state=2 | type==7: riceve desc → DESCREQ2 |
| 2 (WAIT_RPT) | type==8: drain → VENDOR_INIT | type==8: riceve report desc (State2 confronta type==4!) |
| 3 (VENDOR_INIT) | cmd1,cmd2,cmd3 | Attivazione vendor dopo descrittori |
| 4 (DONE) | type==7 → hid_input_report | Runtime |

### 2.2 Bug #1: memcpy offset (DRIVER_STATE.md)

**File**: spi-hid-core.c:1237
**Bug**: `memcpy(&raw, hdr+off, s)` copia header invece del body
**Fix**: `memcpy(&raw, hdr+off+4, s)` (+4 per saltare header HID)

### 2.3 Bug #2: input_register iniziale

**File**: spi-hid-core.c
**Bug**: Usa `SPI_HID_DEFAULT_INPUT_REGISTER = 0x1000` sempre
**Windows**: Usa 0x0000 con approval byte7=0x00 prima del descriptor parse

### 2.4 Bug #3: DESCREQ non produce type=7

**Causa radice**: TX opcode 0x02 + RX separata opcode 0x0B
- La RX 0x0B è un **nuovo comando di read**, non la completion del DESCREQ
- Il device processa il DESCREQ durante la RX 0x0B, non prima

**Soluzione Windows**: TX+RX con stesso opcode (0x02) in una singola operazione SPI
- CSV TXN #3: opcode 0x02, TX 10 byte + RX 10 byte, risposta = ACK
- Poi GPIO IRQ → 0x0B read → type=7

### 2.5 Sequenza Inizializzazione (da CSV Windows)

```
Fase 1 — Reset:
  TXN#1: 0x0B read 9B → RESET_RSP (32 10 00 5A)
  TXN#2: 0x0B read 9B → ACK (03 00 00 00)
  (gap ~168 µs, NESSUN comando inviato!)

Fase 2 — Discovery:
  TXN#3: DESCREQ 0x02 TX+RX 10B → ACK (all zeros)
  GPIO IRQ (58 µs dopo)
  TXN#4: 0x0B read 9B → DEVICE_DESC type=7
  TXN#5: 0x0B read 37B → device descriptor data
  TXN#6: DESCREQ2 0x02 TX+RX 10B → all zeros (busy)
  GPIO IRQ (727 µs dopo)
  TXN#7: 0x0B read 9B → RPT_DESC type=8
  TXN#8: 0x0B read 945B → HID report descriptor
  (~962ms gap — driver processing)

Fase 3 — Attivazione:
  TXN#9:  cmd1 0x00 5B TX
  TXN#10: cmd2 0x00 1B TX
  TXN#11: 0x0B read → checksum
  TXN#12: cmd3 0x70 TX+RX 14B

Fase 4 — FW Upload:
  120+ blocchi 0xB0 da 241B
```

### 2.6 Approval Bytes

| Fase | approval7 | approval8 | Registro |
|------|-----------|-----------|----------|
| Prima di DESCREQ | 0x00 | 0x00 | 0x0000 |
| Dopo device desc | 0x03 | 0x00 | 0x0000(?) |
| Runtime | 0x0A | 0x00 | 0x0000 |

### 2.7 GPIO IRQ

- Pin 0x15, level-triggered active-high (flags 0x409)
- Il device alza l'IRQ quando ha dati pronti dopo DESCREQ/DESCREQ2
- L'host deve **attendere** l'IRQ, poi fare 0x0B read

---

## 3. Firmware Upload

### 3.1 File Firmware

- `~/b0_clean.bin` (29045 byte)
- `~/b0_firmware.bin` (28920 byte)
- `~/b0_firmware_v2.bin` (28920 byte)

### 3.2 Sequenza Upload (da CSV)

- 120 blocchi da 241 byte (0xB0 + 240 dati)
- 125 byte coda
- Intervallo ~100-170 µs tra blocchi
- Durata totale ~410ms
- Dopo FW: 0xB1 execute command, poi 0x70 status check

### 3.3 TXMODE per 0xB0

- Opcode 0xB0 richiede TXMODE=1 perché il controller non riconosce 0xB0 come write
- Windows non usa TXMODE — come fa?
- Possibile che il path V2 con secret bits gestisca automaticamente la direzione

---

## 4. Aspetti da Verificare con Test Reali

### 4.1 Registro 0x44 (Speed Config Windows)

- Non implementato nel driver Linux
- Windows scrive 16-bit a 0x44 che overwrita anche 0x45
- La doppia scrittura dell'opcode (step 9) serve per questo
- Domanda: è necessario per il funzionamento?

### 4.2 Strobe 0x49/0x4A

- Il nostro driver scrive 0x00 a entrambi
- La decomp Windows non mostra esplicitamente queste scritture
- Possibile che siano gestite dall'hardware o implicite nel trigger

### 4.3 Secret Bits

- 0x60040000 = bit 30 + 29 + 18
- Funzione esatta sconosciuta, ma Windows li setta sempre
- Bit 30 e 29 potrebbero essere "enable V2 path"
- Bit 18 potrebbe abilitare l'auto-detection della direzione (TX vs RX)

### 4.4 Restore 0x22

- fcn.0x6fc0 legge 16-bit da MMIO+0x22 all'inizio di ogni transfer
- fcn.0x6f84 ripristina 0x22 dopo il transfer
- Il nostro driver legge 0x22 in probe (ioread16) ma mai lo ripristina
- Potrebbe non essere critico se non modifichiamo 0x22

---

## 5. Piano d'Azione (in ordine di priorità)

### Fase A: Fix SPI Controller (CRITICAL)

1. **Fix `amd_spi_setup_v2_regs`**: leggere ctrl0 prima, preservare bit 21, settare solo 30+29+18
2. **Spostare `setup_v2_regs` in `exec_segment`**: chiamarlo PRIMA di ogni segmento
3. **Aggiungere restore 0x22**: salvare prima, ripristinare dopo in exec_segment
4. **Fix ALT_CS**: usare pattern Windows (AND 0xFC, OR 0x01) invece di puro CS select
5. **Verificare 0x44 speed config**: testare se necessario

### Fase B: Fix HID Protocol

1. **Fix memcpy offset** in parse descrittore (hdr+off → hdr+off+4)
2. **Fix input_register iniziale**: usare 0x0000 con approval7=0x00 nello stato 0
3. **Implementare TX+RX combinato**: modificare `spi_hid_seq_write_then_read` per usare singolo transfer
4. **Aggiungere doppio drain**: due 0x0B read prima di DESCREQ
5. **Attendere GPIO IRQ**: dopo DESCREQ, attendere IRQ prima di leggere risposta

### Fase C: Firmware e Runtime

1. **Riabilitare fw_work** dopo fix SPI
2. **Testare upload B0** con i fix
3. **Verificare report HID** in runtime

---

## 6. Checklist Verifica Finale

- [ ] Il device risponde con RESET_RSP (type=3) alla prima 0x0B read
- [ ] Dopo ~168 µs, seconda 0x0B read restituisce ACK
- [ ] DESCREQ 0x02 TX+RX produce ACK inline
- [ ] GPIO IRQ arriva dopo DESCREQ
- [ ] 0x0B read post-IRQ restituisce DEVICE_DESC type=7
- [ ] 0x0B read 37B restituisce descrittore corretto (vendor=0x045E, product=0x0C19)
- [ ] DESCREQ2 produce RPT_DESC type=8
- [ ] Report descriptor di 936 byte ricevuto correttamente
- [ ] Comandi vendor (cmd1/cmd2/cmd3) eseguiti
- [ ] Firmware B0 upload completato
- [ ] Report HID tipo 7 ricevuti in runtime
