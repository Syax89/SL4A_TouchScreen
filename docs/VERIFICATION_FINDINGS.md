# Risultati Verifica Completa — 2026-07-04

## Executive Summary

Tre sub-agenti hanno verificato in modo indipendente SPI controller, HID state machine e ACPI/init flow.
Di seguito i **8 bug critici** da fixare PRIMA di ogni altra modifica.

---

## Riepilogo Bug Critici

| # | Componente | Bug | File:Line | Impatto |
|---|-----------|-----|-----------|---------|
| C1 | **SPI** | `amd_spi_select_chip()`: usa `cs & 0x03`, Windows forza `OR 0x01` | spi-amd.c:106-108 | CRITICAL — trasferimenti con CS=0 falliscono |
| C2 | **SPI** | `amd_spi_setup_v2_regs()` chiamata in `host_transfer`, NON in `exec_segment` | spi-amd.c:426-427 | HIGH — secret bits non applicati per ogni segmento |
| C3 | **SPI** | Strobe 0x49/0x4A non presenti in Windows, potenzialmente dannosi | spi-amd.c:269-270 | MEDIUM |
| C4 | **HID** | `memcpy(&raw, hdr+off, s)` anziché `hdr+off+4` — descrittore corrotto | spi-hid-core.c:1237 | **CRITICAL** — tutti i campi descrittore shiftati di 4 byte |
| C5 | **HID** | `input_register = 0x1000` per letture pre-descriptor — dovrebbe essere `0x0000` | spi-hid-core.h:76, spi-hid-core.c:1814 | **CRITICAL** — desync completo state machine |
| C6 | **HID** | `approval7 = 0x03` a runtime — dovrebbe essere `0x0A` | spi-hid-core.c:1129 | **CRITICAL** — device rifiuta report |
| C7 | **HID** | `approval8 = 0x0A` a runtime — dovrebbe essere `0x00` | spi-hid-core.c:1131 | **CRITICAL** — device rifiuta report |
| C8 | **ACPI** | GPIO pin documentato `0x15` ma DSDT dice `0x55` (85 dec, trigger Edge/ActiveLow) | DSDT / DRIVER_STATE.md | MEDIUM — driver legge da ACPI a runtime, ma documentazione errata |

---

## Dettaglio Bug Critici

### C1 — ALT_CS Encoding (SPI)

```c
// Codice attuale (spi-amd.c:106-108)
static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs) {
    amd_spi_setclear_reg8(amd_spi, AMD_SPI_ALT_CS_REG,
                          cs, AMD_SPI_ALT_CS_MASK);
}

// Comportamento Windows (da fcn.0x4bac decomp)
// read8(0x1D) → AND 0xFC → OR 0x01 → write8(0x1D)
```

**Problema**: Il bit 0 di 0x1D potrebbe essere un "strobe enable" necessario, non un vero CS select.
Windows forza sempre `0x01`. Il nostro driver con cs=0 produce `0x00` (strobe disabilitato).

**Fix**:
```c
static void amd_spi_select_chip(struct amd_spi *amd_spi, u8 cs) {
    u8 tmp = amd_spi_readreg8(amd_spi, AMD_SPI_ALT_CS_REG);
    tmp = (tmp & 0xFC) | 0x01;  // pattern Windows
    amd_spi_writereg8(amd_spi, AMD_SPI_ALT_CS_REG, tmp);
}
```

### C2 — Secret Bits non in exec_segment (SPI)

```c
// Codice attuale: setup_v2_regs chiamata solo in host_transfer (linea 427)
// MAI in amd_spi_exec_segment (linea 234-292)

// Fix: chiamare amd_spi_setup_v2_regs all'inizio di amd_spi_exec_segment
// Windows setta i secret bits DENTRO fcn.0x2be4 (transfer_data) per ogni segmento
```

### C4 — memcpy offset (HID)

```c
// Codice attuale (spi-hid-core.c:1237):
memcpy(&raw, hdr + off, s);  // copia dall'header HID!

// Fix:
memcpy(&raw, hdr + off + 4, min(s - 4, sizeof(raw)));
// +4 salta l'header [type][len_low][len_high][0x5A]
```

### C5 — input_register iniziale (HID)

```c
// spi-hid-core.h:76:
#define SPI_HID_DEFAULT_INPUT_REGISTER  0x1000

// spi-hid-core.c:1814:
shid->desc.input_register = SPI_HID_DEFAULT_INPUT_REGISTER;

// Fix: usare 0x0000 in state 0 (pre-descriptor)
// Dopo il parse del descriptor, l'input_register vero viene dal dispositivo
```

### C6/C7 — Approval Bytes Errati (HID)

```c
// Codice attuale (spi-hid-core.c:1129-1131):
static u8 spi_hid_approval_byte7(struct spi_hid *shid)
{ return (shid->seq_state == 0) ? 0x00 : 0x03; }
static u8 spi_hid_approval_byte8(struct spi_hid *shid)
{ return (shid->seq_state == 4) ? 0x0A : 0x00; }

// CSV Windows a runtime: approval7=0x0A, approval8=0x00
// Fix:
static u8 spi_hid_approval_byte7(struct spi_hid *shid)
{ return (shid->seq_state == 0) ? 0x00 : ((shid->seq_state >= 4) ? 0x0A : 0x03); }
static u8 spi_hid_approval_byte8(struct spi_hid *shid)
{ return 0x00; }
```

---

## Cosa È STATO CONFERMATO OK

| Area | Conferma |
|------|---------|
| Offset registri SPI (tutti) | Corretti |
| Dimensioni accesso MMIO (8/16/32 bit) | Corrette |
| Toggle FIFO_CLEAR (bit20 clear→set) | Corretto |
| Toggle CMD_TRIGGER (0x47 bit7 clear→set) | Corretto, più robusto di Windows |
| RX FIFO offset +4 | Corretto |
| Busy wait (CTRL0 bit31, timeout 2s) | Corretto |
| Secret bits (30+29+18 = 0x60040000) | Identici a Windows |
| Bit 21 preservato (read-modify-write) | OK (clear mask = 0 in setclear) |
| DESCREQ payload format (10 byte) | Esatto |
| DESCREQ2 payload format | Esatto |
| cmd1/cmd2/cmd3 payload | Esatti |
| Double drain (2x 0x0B read) prima DESCREQ | Corretto |
| State machine transition logic | Corretta |
| Report descriptor buffer size (8K) | Sufficiente (936 byte) |
| ACPI match table (AMDI0060 → V2) | Corretto |
| MMIO base 0xFEC10000 | Corretto |
| SPI speed 33.33 MHz | Corretto |
| reg_prefix (ioread16 da 0x22) | Corretto |
| GPIO IRQ thread registration | Corretto |
| DSDT _DSM UUID match | Corretto |

---

## Stato Fix (2026-07-06 — FINALE)

| # | Bug | Stato | File |
|---|-----|--------|------|
| C4 | memcpy offset +4 | **FIXED** | spi-hid-core.c:1237 |
| C5 | input_register=0x1000 | **FIXED** | spi-hid-core.h:76 |
| C6 | approval7 runtime | **FIXED** | spi-hid-core.c:1129 |
| C7 | approval8 runtime | **FIXED** | spi-hid-core.c:1131 |
| C1 | ALT_CS encoding | **FIXED** | spi-amd.c:106-108 |
| C2 | secret bits in exec_segment | **FIXED** | spi-amd.c:250-251, rimosso da host_transfer |
| C3 | Strobe 0x49/0x4A | **RIMOSSI** (non usati da Windows) | spi-amd.c:272-273 |
| C4-C7,C1-C3 | Build | **COMPILA** | Entrambi i moduli |
| DESCREQ | Write path | **ESAURITO** — software fix insufficiente. Serve logic analyzer. | — |

---

## Esito Finale Test (2026-07-06)

Tutti i fix software sono stati applicati. Tuttavia, **il device ignora qualsiasi write (opcode 0x02)**.
Il blocco e' a livello fisico (CTRL0 bits[15:8] hardwired a 0xA9 vs 0x0E Windows).

**Next step**: logic analyzer su SCK/MOSI/MISO/CS tra Windows e Linux.

---

## Principio Operativo

```
MISURA MILLE VOLTE, MODIFICA UNA.
```

Ogni modifica al codice deve:
1. Essere confrontata con la decomp Windows corrispondente
2. Essere verificata da un sub-agente indipendente
3. Essere documentata in questo file e in DRIVER_STATE.md
4. Essere testata con `make && insmod` PRIMA di procedere al fix successivo
