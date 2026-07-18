# Next Steps

## Current State

- Standard HID touch e pen funzionano via IRQ-driven sequencer.
- Pipeline multitouch Surface completa implementata nel driver (`spi-hid-core.c`):
  peak detection, flood-fill centroid, Hungarian assignment, EMA weight,
  stationary lock, lift history lookback, post-emission coalescence.
- Nome dispositivo: "Surface Touchscreen" (riconosciuto da KDE).
- `raw_mode=1` di default (passivo, osserva frame in arrivo).
- `raw_capture_only=1` + `raw_input_beta=1` attivi.
- DKMS: `sl4a-touch/1.0.0~standard6rawsafe9`, kernel `7.1.3-2-cachyos`.

## Pipeline implementata

Vedi [`docs/PIPELINE.md`](PIPELINE.md) per il mapping completo degli stadi
Windows Surface TouchPenProcessor0C19 nel driver kernel.

## Priorita'

1. **Classificatore quadratico discriminante** — `FUN_180601690`:
   10 feature, 4 classi di contatto, medie e matrici di covarianza.
   Migliora la qualita' del tracking distinguendo tocchi validi da edge/ghost.

2. **Split suppression** — `FUN_180603510`:
   Rilevamento overlap tra track nello stesso blob fisico.
   Gia' parzialmente coperto dalla coalescenza post-emission.

3. **Risoluzione del warning stack frame** — `spi-hid-core.c:2068`:
   Migrare il BFS queue da stack allocation a heap per ridurre
   lo stack frame sotto il limite di 2048 byte.

4. **Ellissi per-slot** — completare l'emissione di touch major/minor/orientation
   per ogni slot attivo (attualmente emesso per tutti, ma i dati blob
   potrebbero essere raffinati).

5. **Testing esteso multi-dito** — validazione con 2-5 dita simultanee,
   misurazione latenza e accuratezza.

## Documentazione

- `docs/PIPELINE.md` — documento canonico della pipeline
- `docs/decomp/SURFACE_TRACKER_DECOMP.md` — specifica tracker Surface originale
- `docs/decomp/WINDOWS_CONTACT_ABI.md` — ABI contatti Windows
