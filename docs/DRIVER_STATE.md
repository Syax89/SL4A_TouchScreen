# MSHW0231 HID-over-SPI Driver — Stato 2026-07-06

> **Repository**: https://github.com/Syax89/SL4A_TouchScreen
> **Fonte di verità**: [GROUND_TRUTH.md](GROUND_TRUTH.md) (verificato CSV × decomp × ACPI × test)

---

## Stato complessivo

| Componente | Stato | Dettaglio |
|-----------|-------|-----------|
| Read path | ✅ FUNZIONANTE | RESET_RSP header `32 10 00 5A` + body `03 00 00 00` letti correttamente |
| Write path | ❌ BLOCCATO | Qualsiasi opcode 0x02 ignorato dal device — DESCREQ, vendor init, tutto |
| Registri SPI | ✅ IDENTICI a Windows | Verificato bit-per-bit: CTRL0, CTRL1, ENA, ALT_CS, 0x44, 0x45, 0x22 |
| Sequenza MMIO | ✅ IDENTICA a Windows | Verificato nel decomp hidspi.sys (0x54d0, 0x4bac) |
| Decomp driver | ✅ Corretto | hidspi.sys (143KB, PDB) — path V0. amdspi.sys (232KB) solo boilerplate WDF |
| Interferenze kernel | ✅ NESSUNA | No upstream spi-amd, no pinctrl, no IRQ/iomem conflict |
| Init mancante | ✅ NESSUNA | SpbCx non tocca registri, _INI solo GPIO, nessun firmware upload al touchscreen |

---

## Fix applicati al driver spi-amd.c

1. `readw & 0xFF` → `readb` (dati corretti)
2. TX_COUNT=3 per read approval, strip 0xFF
3. 6 sync byte (non 5), header read 10B, parser a `&hdr[6]`
4. Body read `blen + 6` byte
5. Ordine Windows exec_segment: opcode → secret bits → 0x44 dance
6. FIFO clear single-set (non toggle)
7. 0x44 dance solo per write (opcode 0x02), mask 0x00FF
8. Opcode re-write dopo 0x44 write16
9. Trigger toggle su 0x47 (preserva altri bit)
10. CS1 hardcoded `(reg & 0xFC) | 0x01`
11. RX_COUNT=0 per write (TX-only)
12. Save/restore 0x22 (Windows fcn.0x6fc0/0x6f84)
13. Opcode restore 0x0B post-transfer (Windows fcn.0x4684)
14. Re-apply secret bits dopo FIFO fill
15. Full-duplex write test (RX_COUNT=tx_len+1)
16. Retry loop 0x80→0x4B, 0x02→0x45, ripoll STATUS
17. Busy poll su CTRL0 bit31 (più affidabile di Windows che non aspetta)

---

## Tentativi falliti

- _RST, _PS3→_PS0→_RST, zero-touch BIOS state, delay 5s
- Vendor init @0x04 `02 00 00 04 82 00 00 04 00 01 01 0C EE 5B`
- Read da 0x04, input register 0x04
- DESCREQ sincrono IRQ (~70µs) vs workqueue
- SPI speed 800 KHz, 33 MHz
- CS0, CS1 forzato

---

## Valori registri verificati

| Registro | Indirizzo | BIOS/Linux | Windows | Match |
|----------|----------|-----------|---------|-------|
| CTRL0 | 0x00 | 0x6f8ca90b | += 0x60040000 | ✅ |
| CTRL1 | 0x0C | 0x02000000 | MAI toccato | ✅ |
| ALT_CS | 0x1D | 0xB1 | (r&0xFC)\|0x01 = 0xB1 | ✅ |
| ENA | 0x20 | 0x11110713 | low nibble=3 usato per 0x44 | ✅ |
| 0x44 | 0x44 | 0x0200 | →0x3300 dance, poi restore | ✅ |
| 0x45 | 0x45 | 0x0B/0x02 | opcode ×2 per transfer | ✅ |
| 0x22 | 0x22 | 0x1111 | save/restore per transfer | ✅ |

---

## Prossimi passi

1. **Logic analyzer** — confrontare segnali SPI fisici tra Windows e Linux (unico modo per trovare differenza elettrica)
2. **Test con kernel diverso** (LTS, vanilla) per escludere regressione
3. **Test con patches AMD FCH SPI** dal kernel upstream
4. **Analizzare se il device richiede una sequenza di init GPIO specifica** che ACPI non sta eseguendo
