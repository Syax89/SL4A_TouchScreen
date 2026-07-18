# Surface TouchPenProcessor0C19 — Kernel Pipeline

Documento canonico della pipeline multitouch implementata nel driver Linux
per MSHW0231, allineata al tracker Surface TouchPenProcessor0C19 decompilato
e al Python oracle (`tools/surface_tracker.py`).

## Panoramica

La pipeline replica il flusso Windows validato dal tracker Python con mapping
~85% fedele al DLL Surface. Una heatmap 72×48 (3456 celle) viene convertita da
raw 16-bit a segnale c590, poi processata in 9 stage per produrre contatti
multi-touch Linux.

File sorgente: `driver/spi-hid-core.c`, funzione `heatmap_process_frame()`.

## Pipeline completa

### Stage 1 — c590 LUT

Mappa gli i valori raw 16-bit della cella in fixed-point ×10000:
```
c590[i] = max(0, 10000 - (i * 22 / 1000 + 6000) / 1000)
```

Definizioni: `C590_BASE=10000`, `C590_STEP_NUM=22204`, `C590_STEP_DEN=1000`,
`C590_OFFSET=6000`.

**Kernel:** `spi-hid-core.c` definizioni c590, applicato a ogni cella in ingresso.

---

### Stage 2 — Baseline

Baseline asimmetrica per-cell: EMA 7/8 verso l'alto su celle non toccate,
le celle sotto contatto sono protette (non aggiornate). 30 frame di warm-up,
poi `baseline_ready = true`.

**Kernel:** `spi-hid-core.c` baseline tracking (pre-pipeline, ~60 linee)

---

### Stage 3 — Peak Detection

**Windows:** `FUN_1805fba00`

Croce a 5 celle nelle direzioni N/S/E/W a distanza ±5, min_rise=350 (c590 units),
max 16 candidati, ordinati per signal rise decrescente.

Una cella è un picco se il suo signal rise > 350 e nessuna delle quattro celle a distanza ±5
ha un rise SUPERIORE.

**Kernel:** `spi-hid-core.c:2260-2307`

```
min_rise = 350
max_peaks = 16
direction = 5 (cross, cardinal axes only)
```

---

### Stage 4 — 5×5 Local Centroid (fixed-point ×100)

**Windows:** `FUN_180602e60`

Per ogni picco: finestra ±2 celle (5×5), media pesata sul segnale:
```
cx = Σ(w · x) / Σw  (in fixed-point ×100)
cy = Σ(w · y) / Σw
```

Solo celle con segnale > baseline contribuiscono. La risoluzione sub-cella
(×100) elimina lo "scalino" visibile tra celle adiacenti, portando la
granularità effettiva da ~461 px a ~4.6 px sul pannello 2256×1504.

**Kernel:** `spi-hid-core.c:2310-2370`

```
window = 2 (5×5 window)
fixed_point_scale = 100
```

---

### Stage 5 — Eigenellipsis (per-blob)

**Windows:** interno a `FUN_180602e60`

Second moments sulla finestra 5×5 del picco:
```
Mxx = Σ(dx² · w), Myy = Σ(dy² · w), Mxy = Σ(dx · dy · w)
```
Decomposizione autovalori per touch major/minor e orientazione (gradi×100).
Solo blob con ≥2 pixel contribuenti.

**Kernel:** `spi-hid-core.c:2370-2390`

---

### Stage 6 — Pre-Merge

Coalesce blob entro distanza² < 36 celle (6 celle di raggio), mantiene il più
pesante (signal weight maggiore). Sort per peso decrescente prima del merge.

**Kernel:** `spi-hid-core.c:2392-2441`

---

### Stage 7 — Hungarian Assignment

**Windows:** `FUN_1805fd090` (cost matrix) / `FUN_1805fd230` (Hungarian solver)

Cost matrix matching Windows oracle:
- In-radius (dist² ≤ max_dist²): `cost = int_sqrt(dx² + dy²)` 
- Out-of-radius: 1000000
- Dummy per slot vuoto: 1000

`blob_max_distance = 3` (default, match Windows ~0.545 grid units).

**Kernel:** `spi-hid-core.c:2450-2560`

HEATMAP_MAX_BLOBS = 20 (previene buffer overflow con 3+ dita).

La cost matrix è allocata nello struct `spi_hid` (non nello stack della
funzione) per evitare stack overflow (~2968 byte rimanenti dopo lo spostamento).

---

### Stage 8 — Track Update (State Machine)

**Windows:** `FUN_180608000` (track update) / `FUN_180607c60` (track allocation)

| Stato | Significato |
|-------|-------------|
| 0 | Empty |
| 1 | New (debounce, 3 frame) |
| 2 | Claimed (active) |
| 3 | Lift (history lookback, 2 frame hold) |

Feature per-slot:
- **Light EMA:** `pos = (old · alpha + new) / (alpha + 1)`, alpha=3
- **History ring:** 10-sample per position lookback
- **Lift lookback:** emette lift a posizione di 2 frame fa (matching Windows `FUN_180601dd0`)
- **Frame-gap reset:** gap > 5 frame rilascia tutti gli slot

**Kernel:** `spi-hid-core.c:2627-2764`

---

### Stage 9 — MT Emission

Protocollo Linux MT:
```
input_mt_slot(dev, slot)
input_mt_report_slot_state(dev, MT_TOOL_FINGER, active)
input_report_abs(dev, ABS_MT_POSITION_X, gx * scale_x + offset_x)
input_report_abs(dev, ABS_MT_POSITION_Y, gy * scale_y + offset_y)
input_report_abs(dev, ABS_MT_TOUCH_MAJOR, sqrt(eigmaj * scale))
input_report_abs(dev, ABS_MT_TOUCH_MINOR, sqrt(eigmin * scale))
input_report_abs(dev, ABS_MT_ORIENTATION, eigori)
input_mt_sync_frame(dev)
input_sync(dev)
```

**Kernel:** `spi-hid-core.c:2766-2800`

---

## Parametri configurabili (module params)

| Parametro | Default | Descrizione |
|-----------|---------|-------------|
| `raw_mode` | 1 | Attiva heatmap + pipeline multi-touch |
| `skip_getfeat` | 1 | Vendor-init + SET_FEATURE (via più affidabile) |
| `getfeat_delay_ms` | 0 | Ritardo prima del GET_FEATURE (Windows: 5900) |
| `blob_max_distance` | 3 | Raggio associazione celle Hungarian |
| `blob_min_weight` | 1000 | Peso minimo blob per emissione |
| `blob_debounce` | 3 | Frame debounce prima del claim (state 1→2) |
| `blob_lift_frames` | 2 | Frame persi prima del lift (state 2→3) |
| `ema_alpha` | 3 | Fattore smoothing posizione |
| `grid_cols` | 72 | Colonne griglia (override) |
| `grid_rows` | 48 | Righe griglia (override) |
| `calib_scale_x/y` | 0 | Fattore scala ×1000 (0=auto) |
| `calib_offset_x/y` | 0 | Offset schermo in logical coords |
| `invert_x/y` | 0 | Inverti asse |
| `swap_xy` | 0 | Scambia assi X/Y |

## Differenze dal Python Oracle

La pipeline kernel omette deliberatamente:
- **DFT processing**: rielaborazione antenna Short/Long (manca l'antenna geometry, ~16+16 antenne)
- **CCL flood-fill**: sostituita da peak detection a croce (più veloce, risultati comparabili)
- **Blob splitting**: causava splitting di dita singole, rimosso
- **Kalman tracking**: sostituito da EMA leggero (alpha=3) + history lookback
- **Dilation**: non necessario con la finestra 5×5 del centroide
- **Ghost rejection**: la pre-merge + Hungarian gestiscono i duplicati

Il Python oracle è in `tools/surface_tracker.py` e serve come ground truth
per validare le modifiche alla pipeline.

## Riferimenti

- `docs/decomp/WINDOWS_CONTACT_ABI.md` — ABI contatti Windows
- `docs/decomp/MULTITOUCH_STATIC_DECOMP.md` — decomp statica HeatCore + TouchPenProcessor
- `tools/surface_tracker.py` — oracolo Python validato
