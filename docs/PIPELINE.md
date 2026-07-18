# Surface TouchPenProcessor0C19 — Kernel Pipeline

Documento canonico della pipeline multitouch implementata nel driver Linux
per MSHW0231, allineata al tracker Surface TouchPenProcessor0C19.

## Panoramica

La pipeline replica il flusso Windows decompilato dalla DLL Surface, con
mapping 1:1 tra funzioni Windows (`FUN_180xxxxx`) e implementazione kernel.

File sorgente: `driver/spi-hid-core.c`, funzione `heatmap_process_frame()`.

## Pipeline completa

### Stage 1 — Baseline

Baseline asimmetrica per-cell: EMA 7/8 verso l'alto su celle non toccate,
protezione celle attive. Costruita sui primi 30 frame.

**Kernel:** `spi-hid-core.c` baseline tracking (pre-pipeline)

---

### Stage 2 — Peak Detection

**Windows:** `FUN_1805fba00`

Raggio 5 celle, threshold minima 500 (c590 fixed-point ×10000).
Max 16 candidati, ordinati per signal rise decrescente.

**Kernel:** `spi-hid-core.c:2217-2282`

```
PEAK_MAX_CANDIDATES = 16
PEAK_MIN_RISE = 500
```

---

### Stage 3 — Centroide Signal-Weighted (Stage 9e)

**Windows:** `FUN_180602e60`

Flood-fill 4-connesso dal picco su celle con segnale > baseline.
Centroide pesato sul segnale: `Σ(weight * x) / Σ(weight)`.
Risoluzione sub-cella fixed-point ×256.

**Kernel:** `spi-hid-core.c:2284-2394`

- Flood-fill BFS dal picco, labeling blob connesso
- Noise floor = c590(baseline)
- Output: `blob_x[pi], blob_y[pi]` in ×256 fixed-point, `blob_wsum[pi]`

---

### Stage 4 — Eigenellipsis (per-blob)

**Windows:** Stage 9e (interno a `FUN_180602e60`)

Second moments sul blob flood-filled: `Σ(dx·dx·w)`, `Σ(dy·dy·w)`, `Σ(dx·dy·w)`.
Decomposizione autovalori per touch major/minor e orientazione.

**Kernel:** `spi-hid-core.c:2342-2390`

- Per ogni blob con ≥2 pixel: eigenvalue decomposition
- `eigmaj[pi]`, `eigmin[pi]` (cell²·signal), `eigori[pi]` (gradi×100)

---

### Stage 5 — Blob Sorting e Pre-Merge

**Windows:** sort per signal weight (implicito)

**Kernel:** `spi-hid-core.c:2426-2441`

Sort per `blob_wsum` decrescente, merge pre-assignment entro 6 celle
(squared distance < 36), mantiene il blob più pesante.

---

### Stage 6 — Assegnazione Globale (Hungarian)

**Windows:** `FUN_1805fd090` (cost matrix) / `FUN_1805fd230` (Hungarian solver)

Raggio associazione: 3 celle (0.545 grid units, da `config+0x8dc`).
Costo in-radius: `int_sqrt(dx²+dy²)`, out-of-radius: 1000000.
Dummy per slot vuoto: 1000.

**Kernel:** `spi-hid-core.c:1727-1824` (`heatmap_assign_active_slots`)

```
blob_max_distance = 3  (default, match Windows 0.545)
```

---

### Stage 7 — Track Update (State Machine)

**Windows:** `FUN_180608000` (track update) / `FUN_180607c60` (track allocation)

| Stato | Significato |
|-------|-------------|
| 0 | Empty |
| 1 | New (debounce, 3 frame, X-range stability < 768) |
| 2 | Claimed (active) |
| 3 | Lift (history lookback, 2 frame hold) |

**Kernel:** `spi-hid-core.c:2521-2670`

Feature per-slot:
- **EMA weight filter:** `weight = (old*7 + new)/8` (`FUN_180608000`)
- **History ring:** 10-sample (`FUN_180608000`, `track+0x58`)
- **Moving-average smoothing:** media mobile 3-campioni su history
- **Stationary detection:** lock posizione se movimento < 1 cella per ≥5 frame
- **Lift history lookback:** `FUN_180601dd0`, emette lift a posizione di N frame fa
- **Frame-gap reset:** gap > 5 frame rilascia tutti gli slot

---

### Stage 8 — Coalescenza Post-Emission (Stage 10)

**Windows:** `FUN_1806025c0`

Sopprime il contatto più debole tra coppie entro 6 celle (distanza² < 36),
emulando il group-rewriting di Windows.

**Kernel:** `spi-hid-core.c:2672-2694`

---

### Stage 9 — Emissione

**Windows:** `FUN_180601c10` / `FUN_180601dd0` / `FUN_180602a90`

**Kernel:** `spi-hid-core.c:2696-2764`

Protocollo Linux MT (input_mt_slot, input_mt_report_slot_state,
ABS_MT_POSITION_X/Y, ABS_MT_TOUCH_MAJOR/MINOR, ABS_MT_ORIENTATION).

---

## Parametri configurabili (module params)

| Parametro | Default | Descrizione |
|-----------|---------|-------------|
| `blob_max_distance` | 3 | Raggio associazione celle (Windows: 0.545 → 3) |
| `blob_min_weight` | 1000 | Peso minimo blob (c590) |
| `blob_debounce` | 3 | Frame debounce prima del claim (state 1→2) |
| `blob_lift_frames` | 2 | Frame persi prima del lift (state 2→3) |
| `ema_alpha` | 3 | Fattore smoothing posizione (deprecato) |

## Riferimenti

- `docs/decomp/SURFACE_TRACKER_DECOMP.md` — specifica tracker Surface originale
- `docs/decomp/WINDOWS_CONTACT_ABI.md` — ABI contatti Windows
- `docs/decomp/MULTITOUCH_STATIC_DECOMP.md` — decomp statica HeatCore + TouchPenProcessor
- `tools/surface_tracker.py` — oracolo Python validato
