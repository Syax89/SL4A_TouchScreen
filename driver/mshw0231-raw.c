// SPDX-License-Identifier: GPL-2.0
/* Raw payload consumer. Transport and lifecycle ownership remain in core. */
#include <linux/errno.h>
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/math.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "spi-hid-core.h"
#include "spi-hid-capimg.h"
#include "mshw0231-raw.h"

extern int debug_level;
#define seq_dbg(shid, level, fmt, ...) \
	do { if (debug_level >= (level)) \
		dev_info(&(shid)->spi->dev, "TRACE[hid:%d] " fmt, (level), ##__VA_ARGS__); } while (0)

/* ── Screen calibration ────────────────────────────────────────── */

static bool invert_x;
module_param(invert_x, bool, 0444);
MODULE_PARM_DESC(invert_x, "Experimental raw-pipeline X-axis inversion (load-time only)");

static bool invert_y;
module_param(invert_y, bool, 0444);
MODULE_PARM_DESC(invert_y, "Experimental raw-pipeline Y-axis inversion (load-time only)");

static bool swap_xy;
module_param(swap_xy, bool, 0444);
MODULE_PARM_DESC(swap_xy, "Experimental raw-pipeline axis swap (load-time only)");

/* ── Blob detection tunables (runtime-validated) ────────────────── */

static int blob_min_weight = 1000;
module_param(blob_min_weight, int, 0444);
MODULE_PARM_DESC(blob_min_weight,
	"Experimental raw-pipeline minimum blob signal rise (load-time only)");

static int ema_alpha = 7;
module_param(ema_alpha, int, 0444);
MODULE_PARM_DESC(ema_alpha,
	"Experimental raw-pipeline EMA smoothing (load-time only)");

static int dfa_data_offset;
module_param(dfa_data_offset, int, 0444);
MODULE_PARM_DESC(dfa_data_offset,
	"DFT antenna frame data offset in bytes (0 = decoded raster, no offset)");

static int touch_threshold_pct = 10;
module_param(touch_threshold_pct, int, 0444);
MODULE_PARM_DESC(touch_threshold_pct, "Unused experimental placeholder");

static int ghost_dist = 6;
module_param(ghost_dist, int, 0444);
MODULE_PARM_DESC(ghost_dist,
	"Experimental raw-pipeline coalescence radius (load-time only)");

static int grid_cols = 0;  /* 0 = default 72 */
module_param(grid_cols, int, 0444);
MODULE_PARM_DESC(grid_cols, "Experimental raw grid columns (0=current unvalidated 72x48 fallback)");

static int grid_rows = 0;  /* 0 = default 48 */
module_param(grid_rows, int, 0444);
MODULE_PARM_DESC(grid_rows, "Experimental raw grid rows (0=current unvalidated 72x48 fallback)");

static int calib_scale_x = 0;  /* 0 = use default: SCREEN_MAX/(cols-1) */
module_param(calib_scale_x, int, 0444);
MODULE_PARM_DESC(calib_scale_x, "Experimental raw X scale x1000 (load-time only)");

static int calib_scale_y = 0;
module_param(calib_scale_y, int, 0444);
MODULE_PARM_DESC(calib_scale_y, "Experimental raw Y scale x1000 (load-time only)");

static int calib_offset_x = 0;  /* screen pixel offset */
module_param(calib_offset_x, int, 0444);
MODULE_PARM_DESC(calib_offset_x, "Experimental raw X offset (load-time only)");

static int calib_offset_y = 0;
module_param(calib_offset_y, int, 0444);
MODULE_PARM_DESC(calib_offset_y, "Experimental raw Y offset (load-time only)");

static int blob_debounce = 3;
module_param(blob_debounce, int, 0444);
MODULE_PARM_DESC(blob_debounce, "Experimental raw new-contact debounce (load-time only)");

static int blob_lift_frames = 3;
module_param(blob_lift_frames, int, 0444);
MODULE_PARM_DESC(blob_lift_frames, "Experimental raw missed frames before lift (load-time only)");

static int hold_frames = 0;
module_param(hold_frames, int, 0444);
MODULE_PARM_DESC(hold_frames,
	"Experimental raw contact hold grace frames (load-time only)");

static int pre_assoc_ratio = 0;
module_param(pre_assoc_ratio, int, 0444);
MODULE_PARM_DESC(pre_assoc_ratio,
	"Experimental raw pre-association ratio x1000 (load-time only)");

static int blob_max_distance = 3;
module_param(blob_max_distance, int, 0444);
MODULE_PARM_DESC(blob_max_distance,
	"Experimental raw slot reassignment distance (load-time only)");

static void release_all_slots(struct input_dev *input, u8 *slot_state,
		unsigned int max_slots)
{
	bool had_touch = false;

	for (unsigned int s = 0; s < max_slots; s++) {
		if (slot_state[s] >= 1) {
			input_mt_slot(input, s);
			input_mt_report_slot_state(input, MT_TOOL_FINGER, false);
			had_touch = true;
		}
	}
	if (had_touch) {
		input_mt_sync_frame(input);
		input_report_key(input, BTN_TOUCH, 0);
		input_sync(input);
	}
}

void mshw0231_raw_reset(struct spi_hid *shid)
{
	if (shid->touch_input)
		release_all_slots(shid->touch_input, shid->blob_slot_state,
				  HEATMAP_MAX_SLOTS);

	shid->heatmap_have_baseline = false;
	shid->heatmap_baseline_cells = 0;
	shid->heatmap_baseline_frames = 0;
	memset(shid->heatmap_baseline, 0, sizeof(shid->heatmap_baseline));
	memset(shid->blob_slot_state, 0, sizeof(shid->blob_slot_state));
	memset(shid->blob_slot_duration, 0, sizeof(shid->blob_slot_duration));
	memset(shid->blob_slot_gx, 0, sizeof(shid->blob_slot_gx));
	memset(shid->blob_slot_gy, 0, sizeof(shid->blob_slot_gy));
	memset(shid->blob_slot_weight, 0, sizeof(shid->blob_slot_weight));
	memset(shid->blob_slot_missed, 0, sizeof(shid->blob_slot_missed));
	memset(shid->eigmaj, 0, sizeof(shid->eigmaj));
	memset(shid->eigmin, 0, sizeof(shid->eigmin));
	memset(shid->eigori, 0, sizeof(shid->eigori));
	memset(shid->heatmap_touched, 0, sizeof(shid->heatmap_touched));
	memset(shid->heatmap_signal, 0, sizeof(shid->heatmap_signal));
	memset(shid->blob_slot_hx, 0, sizeof(shid->blob_slot_hx));
	memset(shid->blob_slot_hy, 0, sizeof(shid->blob_slot_hy));
	memset(shid->blob_slot_hpos, 0, sizeof(shid->blob_slot_hpos));
	memset(shid->blob_slot_hcount, 0, sizeof(shid->blob_slot_hcount));
	memset(shid->blob_slot_stationary, 0, sizeof(shid->blob_slot_stationary));
	memset(shid->blob_slot_blob, 0, sizeof(shid->blob_slot_blob));
	memset(shid->blob_x, 0, sizeof(shid->blob_x));
	memset(shid->blob_y, 0, sizeof(shid->blob_y));
	memset(shid->blob_wsum, 0, sizeof(shid->blob_wsum));
	memset(shid->blob_active, 0, sizeof(shid->blob_active));
	shid->heatmap_last_frame_jiffies = 0;
}

/* Screen mapping: logical range 0..32767 for both X and Y.
 * Reverse-engineered from TouchPenProcessor0C19.dll (GROUND_TRUTH.md §22.3):
 * raw frame is 288 columns (stride 0x120), rows auto-detected from data size.
 * Each byte indexes a float[256] lookup table (c590) for actual signal level.
 * Signal = c590[raw_byte] = max_signal - (index * step + offset).
 *
 * Grid calibration formula (§22.8):
 *   screen_x = grid_x * scale_x  where scale_x = (phys_x * SCALE) / (grid_w - 1)
 *   screen_y = grid_y * scale_y  where scale_y = (phys_y * SCALE) / (grid_h - 1)
 *
 * Using max range 0..32767 with aspect ratio from HID descriptor (2934×1652).
 *
 * Grid geometry derived from analyzing captured raw frames:
 * frames): the content_id=0x0C report carries a contiguous 3456-byte cell
 * field starting at dfa_data_offset=26, laid out as 72 columns × 48 rows
 * (row-major, 72-wide fast axis). This is a clean 3:2 grid matching the
 * landscape display — confirmed by connected-component / compactness analysis
 * across 150 touch frames (W=72 gave a single compact blob per finger; other
 * widths fragmented the blobs). The earlier 288×~14 assumption stretched Y
 * onto ~14 rows and scrambled the X/Y mapping. The trailing ~820 bytes of the
 * frame past cell 3456 are footer/metadata and must NOT be treated as cells.
 */
#define GRID_COLS_DEFAULT   72     /* default, overridden by auto-detect or module param */
#define GRID_ROWS_DEFAULT   48
#define GRID_CELLS_DEFAULT  (GRID_COLS_DEFAULT * GRID_ROWS_DEFAULT)  /* 3456 */
#define GRID_CELLS  (288 * 50)     /* max buffer allocation (headroom) */
#define GRID_ROW_STRIDE_DEFAULT GRID_COLS_DEFAULT

/* Signal lookup table (c590[256]). Extracted from TouchPenProcessor0C19.dll .rdata:
 *   c590[i] = 1.0 - (i * 0.00222035428 + 0.600000024)
 * Range: 0.4 (resting) down to ~-0.166 (byte 255).
 * Fixed-point: c590[i] = (10000 - (i * 22204 / 1000 + 6000)), clamped >= 0.
 * Scaled to [0, 4000] with 4 decimal digits of precision. */
#define C590_BASE   10000
#define C590_STEP_NUM 22204   /* 0.00222035428 * 10000000 / 1000 */
#define C590_STEP_DEN 1000
#define C590_OFFSET 6000      /* 0.600000024 * 10000 ≈ 6000 */

void mshw0231_raw_init(struct spi_hid *shid)
{
	int i;
	for (i = 0; i < 256; i++) {
		s32 v = 10000 - ((s32)i * 22 + 6000);
		shid->c590_lut[i] = (s16)(v > 0 ? v : 0);
	}
	seq_dbg(shid, 1, "HEATMAP: c590 lookup table initialized (range %d..%d)\n",
		(int)shid->c590_lut[0], (int)shid->c590_lut[255]);
}

/* Fixed-point atan2 approximation. Returns angle in degrees * 100.
 * Uses rational approximation: atan2(y,x) ≈ (y * 18000) / (PI * (abs(x) + abs(y) + 1))
 * with quadrant correction. Range: [-18000, 18000] (i.e., [-180.00°, 180.00°]). */
static s32 atan2_approx(s32 y, s32 x)
{
	s32 ax = x < 0 ? -x : x;
	s32 ay = y < 0 ? -y : y;
	s32 angle;

	if (ax + ay == 0)
		return 0;

	angle = (s32)(((s64)ay * 9000) / (ax + ay));

	if (x < 0)
		angle = 18000 - angle;
	if (y < 0)
		angle = -angle;

	return angle;
}

/*
 * Process a raw heatmap frame through the full CCL touch pipeline.
 *
 * Pipeline stages:
 *   1. Baseline subtraction + noise floor
 *   2. Peak detection gate (cross-shaped +/-5 cells)
 *   3. CCL flood-fill (4-connected BFS)
 *   4. Velocity rejection + edge penalty + blob splitting
 *   5. Centroid + eigenvalues computation
 *   6. Pre-merge (ghost rejection)
 *   7. Hungarian assignment with multi-finger radii
 *   8. Slot state machine + EMA + deadband + stationary lock
 *   9. MT protocol emission
 *
 * The function modifies blob_* and blob_slot_* arrays in shid.
 */
static void mshw0231_raw_process_samples(struct spi_hid *shid, const u8 *data,
				 u32 data_len, u8 content_id)
{
	struct device *dev = &shid->spi->dev;
	u32 i, cell_count, ncols, nrows;
	u16 nlabels;
	int data_offset;
	int configured_cols, configured_rows;
	int frame_ema_alpha;
	int touched_count = 0;

	/* Parameters are fixed at module load; validate the frame before caching geometry. */
	data_offset = READ_ONCE(dfa_data_offset);
	configured_cols = READ_ONCE(grid_cols);
	configured_rows = READ_ONCE(grid_rows);
	frame_ema_alpha = READ_ONCE(ema_alpha);
	if (frame_ema_alpha < 0 || frame_ema_alpha > 10000)
		frame_ema_alpha = 3;
	if (data_offset < 0 || data_offset >= data_len)
		return;

	/* Clamp unsafe parameters to prevent division by zero
	 * and undefined behavior. Use local variables to avoid
	 * overwriting user-set sysfs values every frame. */
	{
		s32 val;
		val = READ_ONCE(blob_min_weight); if (val < 1) val = 1;
		blob_min_weight = val;  /* one-time clamp only */
		val = READ_ONCE(ema_alpha); if (val < 0 || val > 10000) val = 3;
		ema_alpha = val;
		val = READ_ONCE(blob_debounce); if (val < 1) val = 3;
		blob_debounce = val;
		val = READ_ONCE(blob_lift_frames); if (val < 1) val = 3;
		blob_lift_frames = val;
		val = READ_ONCE(hold_frames); if (val < 0) val = 0;
		hold_frames = val;
		val = READ_ONCE(blob_max_distance); if (val < 1) val = 3;
		blob_max_distance = val;
		val = READ_ONCE(ghost_dist); if (val < 1) val = 6;
		ghost_dist = val;
		val = READ_ONCE(touch_threshold_pct); if (val < 0) val = 0;
		if (val > 100) val = 100;
		touch_threshold_pct = val;
	}

	/* The candidate cell field begins after metadata. Its geometry is not yet
	 * proven, so never infer cells from a short or malformed frame. */
	{
		u32 avail = data_len - data_offset;
		cell_count = avail;
	}

	/* The 72×48 default is an unvalidated experimental candidate. It remains
	 * configurable only for controlled capture comparison, never calibration. */
	if (!shid->heatmap_grid_cols || !shid->heatmap_grid_rows) {
		if (configured_cols > 1)
			ncols = configured_cols;
		else
			ncols = GRID_COLS_DEFAULT;
		if (configured_rows > 1)
			nrows = configured_rows;
		else if (configured_cols > 1)
			nrows = cell_count / ncols;   /* custom cols: derive rows */
		else
			nrows = GRID_ROWS_DEFAULT;
		if (ncols < 2) {
			dev_warn(dev, "HEATMAP: invalid grid columns %u, using default\n", ncols);
			ncols = GRID_COLS_DEFAULT;
		}
		if (nrows < 2) {
			dev_warn(dev, "HEATMAP: invalid grid rows %u, using default\n", nrows);
			nrows = GRID_ROWS_DEFAULT;
		}
		if (nrows > HEATMAP_MAX_CELLS / ncols || ncols * nrows > cell_count) {
			dev_warn(dev, "HEATMAP: frame has %u bytes, insufficient for %ux%u grid\n",
				 cell_count, ncols, nrows);
			return;
		}
		shid->heatmap_grid_cols = ncols;
		shid->heatmap_grid_rows = nrows;
		seq_dbg(shid, 1, "HEATMAP: grid %u cols × %u rows (offset %d, frame avail %u cells)\n",
			 ncols, nrows, data_offset, cell_count);
	}
	ncols = shid->heatmap_grid_cols;
	nrows = shid->heatmap_grid_rows;

	/* Every frame must cover the cached grid. Otherwise stale cells from a
	 * larger prior frame could become phantom touches. */
	if (!ncols || nrows > HEATMAP_MAX_CELLS / ncols) {
		dev_warn(dev, "HEATMAP: invalid grid %ux%u\n", ncols, nrows);
		return;
	}
	if (cell_count < ncols * nrows) {
		dev_warn(dev, "HEATMAP: frame has %u cells, need %u for cached grid\n",
			 cell_count, ncols * nrows);
		mshw0231_raw_reset(shid);
		return;
	}
	cell_count = ncols * nrows;

	/* At the default 100 Hz stream rate, six missing frames are roughly
	 * 60 ms. Use elapsed time rather than a local counter that increments
	 * only when a frame has already arrived. */
	if (shid->heatmap_last_frame_jiffies &&
	    time_after(jiffies, shid->heatmap_last_frame_jiffies +
		       msecs_to_jiffies(60))) {
		release_all_slots(shid->touch_input, shid->blob_slot_state,
				  HEATMAP_MAX_SLOTS);
		memset(shid->blob_slot_state, 0, sizeof(shid->blob_slot_state));
		memset(shid->blob_slot_duration, 0, sizeof(shid->blob_slot_duration));
		memset(shid->blob_slot_gx, 0, sizeof(shid->blob_slot_gx));
		memset(shid->blob_slot_gy, 0, sizeof(shid->blob_slot_gy));
		memset(shid->blob_slot_weight, 0, sizeof(shid->blob_slot_weight));
		memset(shid->blob_slot_missed, 0, sizeof(shid->blob_slot_missed));
		memset(shid->blob_slot_stationary, 0, sizeof(shid->blob_slot_stationary));
		memset(shid->blob_slot_hcount, 0, sizeof(shid->blob_slot_hcount));
		memset(shid->blob_slot_hpos, 0, sizeof(shid->blob_slot_hpos));
		memset(shid->eigmaj, 0, sizeof(shid->eigmaj));
		memset(shid->eigmin, 0, sizeof(shid->eigmin));
		memset(shid->eigori, 0, sizeof(shid->eigori));
	}
	shid->heatmap_last_frame_jiffies = jiffies;

	if (cell_count > HEATMAP_MAX_CELLS) {
		dev_warn(dev, "HEATMAP: frame too large (%u cells > %u max)\n", cell_count, HEATMAP_MAX_CELLS);
		return;
	}

	/* Store raw frame for sysfs debug */
	if (!shid->heatmap_buf || shid->heatmap_capacity < data_len) {
		u8 *new_buf = kmalloc(data_len, GFP_KERNEL);

		if (new_buf) {
			kfree(shid->heatmap_buf);
			shid->heatmap_buf = new_buf;
			shid->heatmap_capacity = data_len;
		}
	}
	if (shid->heatmap_buf && shid->heatmap_capacity >= data_len) {
		memcpy(shid->heatmap_buf, data, data_len);
		shid->heatmap_len = data_len;
		shid->heatmap_content_id = content_id;
	}

	/* Max-tracking baseline: track the per-cell maximum raw byte index
	 * over ~30 frames to establish the resting (no-touch) reference.
	 * A higher raw byte index maps to a lower c590 signal.
	 *
	 * After baseline is established, use Exponential Moving Average (EMA)
	 * to slowly track thermal drift: baseline[i] = (baseline[i]*7 + raw)/8.
	 * This prevents single-count thermal shifts from being detected as
	 * false touches (the 60% default threshold would catch a 1-count shift
	 * at 62 vs baseline threshold 64, but EMA further smooths this). */
	if (!shid->heatmap_have_baseline) {
		if (content_id == 0x0C && cell_count >= 1000) {
			shid->heatmap_baseline_frames++;
			for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
				u8 raw = data[data_offset + i];
				if (shid->heatmap_baseline_frames == 1 || raw > shid->heatmap_baseline[i])
					shid->heatmap_baseline[i] = raw;
			}
			if (shid->heatmap_baseline_frames >= 30) {
				shid->heatmap_baseline_cells = cell_count;
				shid->heatmap_have_baseline = true;
				seq_dbg(shid, 1, "HEATMAP: baseline stabilized after %u frames (%u cells)\n",
					 shid->heatmap_baseline_frames, cell_count);
			}
		}
		return;
	}
	/* Asymmetric baseline tracking. Resting raw is the per-cell maximum
	 * (a touch only *lowers* the raw byte). So:
	 *  - raw >= baseline: recover toward resting quickly (EMA 12.5%), which
	 *    also absorbs slow upward thermal drift;
	 *  - raw <  baseline: this is (probably) an active touch — do NOT let the
	 *    baseline chase it down, otherwise a held finger fades into the
	 *    baseline within a few frames and stops being detected. Allow only a
	 *    very slow decay (~1 count / many frames) to track genuine downward
	 *    thermal drift without swallowing touches. */
	for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
		u8 base = shid->heatmap_baseline[i];
		u8 raw = data[data_offset + i];

		if (raw >= base) {
			u16 cur = (u16)base * 7 + (u16)raw;
			shid->heatmap_baseline[i] = (u8)(cur / 8);
		}
	}

	if (!shid->touch_input) return;

	/* Step 1: compute signal rise per cell once, reuse everywhere. */
	memset(shid->heatmap_touched, 0, cell_count);
	for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
		s16 base = shid->c590_lut[shid->heatmap_baseline[i]];
		s16 curr = shid->c590_lut[data[data_offset + i]];
		s16 rise = curr - base;
		shid->heatmap_signal[i] = rise;
		/* Noise floor (Windows DAT_1806c08c8 = 0.04):
		 * absolute c590 < 400 → class 5 (suppressed).
		 * DLL config table +0xecc = 0.04 confirms. */
		shid->heatmap_touched[i] = (rise >= 200 && curr >= 400) ? 1 : 0;
	}

	/* Step 2+3: Connected-component labeling + centroid + eigenvalues
	 * (matching Windows FUN_180600c40 CCL pipeline).
	 * 4-connected flood-fill — each connected region of signal-above-
	 * baseline becomes one blob candidate.
	 *
	 * Pre-filter: peak-detection gate (Windows FUN_1805fba00).  If no
	 * peak is found at all, the entire CCL pass is skipped — residual
	 * noise after finger lift never creates phantom blobs. */
	memset(shid->blob_wsum, 0, sizeof(shid->blob_wsum));
	memset(shid->blob_active, 0, sizeof(shid->blob_active));
	memset(shid->heatmap_label, 0, cell_count * sizeof(shid->heatmap_label[0]));
	nlabels = 0;
	touched_count = 0;

	/* Peak-detection gate: cross-shaped ±5, min_rise=200.
	 * Lowered from 300 to catch weaker fingers at 3+ density.
	 * The cross-check (±5, higher neighbor) naturally suppresses
	 * flat noise — only genuine signal maxima pass through.
	 * Collect all peaks for velocity rejection. */
	{
		struct { u16 col; u16 row; } peaks[16];
		u8 npeaks = 0;
		for (i = 0; i < cell_count && npeaks < 16; i++) {
			u16 col, row;
			s16 rise;
			bool ok;

			if (!shid->heatmap_touched[i]) continue;
			col = i % ncols; row = i / ncols;
			if (row >= nrows) break;
			rise = shid->heatmap_signal[i];
			if (rise < 200) continue;

			ok = true;
			if (col >= 5 && shid->heatmap_touched[i - 5] &&
			    shid->heatmap_signal[i - 5] > rise) ok = false;
			if (col + 5 < ncols && shid->heatmap_touched[i + 5] &&
			    shid->heatmap_signal[i + 5] > rise) ok = false;
			if (row >= 5 && shid->heatmap_touched[i - 5 * ncols] &&
			    shid->heatmap_signal[i - 5 * ncols] > rise) ok = false;
			if (row + 5 < nrows && shid->heatmap_touched[i + 5 * ncols] &&
			    shid->heatmap_signal[i + 5 * ncols] > rise) ok = false;
			if (ok) {
				peaks[npeaks].col = col;
				peaks[npeaks].row = row;
				npeaks++;
			}
		}

		if (npeaks > 0) {
		u16 *queue = shid->heatmap_queue;
		u16 next_label = 1;
		u32 ci;

		for (ci = 0; ci < (u32)cell_count; ci++) {
			u32 col, row;

			if (!shid->heatmap_touched[ci]) continue;
			if (shid->heatmap_signal[ci] <= 0) continue;
			if (shid->heatmap_label[ci] != 0) continue;
			if (nlabels >= HEATMAP_MAX_BLOBS) break;

			{
				u16 head = 0, tail = 0;
				s64 sx = 0, sy = 0, sw = 0;
				s64 sxx = 0, syy = 0, sxy = 0;
				s32 min_r = 9999, max_r = -1, min_c = 9999, max_c = -1;
				u32 pixel_count = 0;
				s16 max_rise = 0;
				u16 label = next_label;

				queue[tail++] = ci;
				shid->heatmap_label[ci] = label;

				while (head < tail) {
					u32 idx = queue[head++];
					s16 w;
					u16 r, c;
					u32 nxt;

					col = idx % ncols; row = idx / ncols;
					if (row >= nrows) continue;
					w = shid->heatmap_signal[idx];
					if (w <= 0) continue;
					if (w > max_rise) max_rise = w;

					sx += (s64)col * w;
					sy += (s64)row * w;
					sw += w;
					pixel_count++;
					r = (u16)row; c = (u16)col;
					if (r < min_r) min_r = r;
					if (r > max_r) max_r = r;
					if (c < min_c) min_c = c;
					if (c > max_c) max_c = c;

					/* 4-connected neighbors */
					if (col > 0) {
						nxt = idx - 1;
						if (shid->heatmap_touched[nxt] &&
						    shid->heatmap_signal[nxt] > 0 &&
						    shid->heatmap_label[nxt] == 0 &&
						    tail < HEATMAP_MAX_CELLS) {
							shid->heatmap_label[nxt] = label;
							queue[tail++] = nxt;
						}
					}
					if (col + 1 < ncols) {
						nxt = idx + 1;
						if (shid->heatmap_touched[nxt] &&
						    shid->heatmap_signal[nxt] > 0 &&
						    shid->heatmap_label[nxt] == 0 &&
						    tail < HEATMAP_MAX_CELLS) {
							shid->heatmap_label[nxt] = label;
							queue[tail++] = nxt;
						}
					}
					if (row > 0) {
						nxt = idx - ncols;
						if (shid->heatmap_touched[nxt] &&
						    shid->heatmap_signal[nxt] > 0 &&
						    shid->heatmap_label[nxt] == 0 &&
						    tail < HEATMAP_MAX_CELLS) {
							shid->heatmap_label[nxt] = label;
							queue[tail++] = nxt;
						}
					}
					if (row + 1 < nrows) {
						nxt = idx + ncols;
						if (shid->heatmap_touched[nxt] &&
						    shid->heatmap_signal[nxt] > 0 &&
						    shid->heatmap_label[nxt] == 0 &&
						    tail < HEATMAP_MAX_CELLS) {
							shid->heatmap_label[nxt] = label;
							queue[tail++] = nxt;
						}
					}
				}

				/* Filter noise: at least 2 pixels, peak signal >= 300,
				 * and total weight >= blob_min_weight. The max_rise check
				 * alone rejects residual noise after lift (typically
				 * 2-5 pixels at <200 rise). */
				if (pixel_count < 2 || max_rise < 200 || sw < blob_min_weight)
					continue;

				/* Velocity rejection (Windows FUN_180600c40):
				 * blob centroid must be within 6 grid cells of at
				 * least one detected peak. Rejects CCL artifacts
				 * far from any genuine signal maximum. */
				{
					u32 gx = (u32)(sx / sw); /* integer cell */
					u32 gy = (u32)(sy / sw);
					u8 p;
					bool near_peak = false;
					for (p = 0; p < npeaks; p++) {
						s32 dx = (s32)gx - (s32)peaks[p].col;
						s32 dy = (s32)gy - (s32)peaks[p].row;
						if (dx < 0) dx = -dx;
						if (dy < 0) dy = -dy;
						if ((u32)dx <= 6 && (u32)dy <= 6) {
							near_peak = true;
							break;
						}
					}
					if (!near_peak) continue;
				}

				{
					s32 bi = nlabels;
					u32 cx, cy;

					shid->blob_x[bi] = (u32)(sx * 100 / sw);
					shid->blob_y[bi] = (u32)(sy * 100 / sw);
					shid->blob_wsum[bi] = (u32)sw;

					/* Blob splitting: if this CCL blob contains
					 * 2+ peaks at >= 4 cells apart, it's likely
					 * two merged fingers. Split them into separate
					 * blobs using the peak positions (Windows
					 * FUN_180602770 per-neighbor sub-centroids). */
					if (npeaks >= 2 && pixel_count >= 8) {
						u8 p, split_count = 0;
						u8 split_peaks[16];
						for (p = 0; p < npeaks && split_count < 16; p++) {
							u32 pix = (u32)peaks[p].row * ncols + (u32)peaks[p].col;
							if (shid->heatmap_label[pix] == label)
								split_peaks[split_count++] = p;
						}
						if (split_count >= 2 && split_count <= 4) {
							bool too_close = true;
							for (p = 1; p < split_count && too_close; p++) {
								u8 q;
								for (q = 0; q < p; q++) {
									s32 dx = (s32)peaks[split_peaks[p]].col -
										 (s32)peaks[split_peaks[q]].col;
									s32 dy = (s32)peaks[split_peaks[p]].row -
										 (s32)peaks[split_peaks[q]].row;
									if (dx < 0) dx = -dx;
									if (dy < 0) dy = -dy;
									if ((u32)dx >= 4 || (u32)dy >= 4) {
										too_close = false;
										break;
									}
								}
							}
							if (!too_close) {
								/* The current blob has not been committed yet: append
								 * split blobs without decrementing the prior count. */
								for (p = 0; p < split_count && nlabels < HEATMAP_MAX_BLOBS; p++) {
									s32 pr = peaks[split_peaks[p]].row;
									s32 pc = peaks[split_peaks[p]].col;
									s64 ssx = 0, ssy = 0, ssw = 0;
									s32 r, c;
									for (r = max(0, pr - 2); r <= min((s32)nrows - 1, pr + 2); r++) {
										for (c = max(0, pc - 2); c <= min((s32)ncols - 1, pc + 2); c++) {
											u32 idx = (u32)r * ncols + (u32)c;
											s16 w;
											if (shid->heatmap_label[idx] != label) continue;
											w = shid->heatmap_signal[idx];
											if (w <= 0) continue;
											ssx += (s64)c * w;
											ssy += (s64)r * w;
											ssw += w;
										}
									}
									if (ssw > 0) {
										s32 sbi = nlabels;
										shid->blob_x[sbi] = (u32)(ssx * 100 / ssw);
										shid->blob_y[sbi] = (u32)(ssy * 100 / ssw);
										shid->blob_wsum[sbi] = (u32)ssw;
										shid->blob_active[sbi] = true;
										shid->blob_eigmaj[sbi] = 0;
										shid->blob_eigmin[sbi] = 0;
										shid->blob_eigori[sbi] = 0;
										nlabels++;
										touched_count++;
									}
								}
								continue; /* skip normal single-blob path */
							}
						}
					}

					/* Edge-contact weight penalty (Windows DLL config):
					 * +0x8D0=0.967 top edge — mild penalty
					 * +0x8D4=0.228 bottom edge — harsh penalty
					 * (panel connector bezel produces false touches).
					 * Top/left/right: keep ~97% weight.
					 * Bottom (max_r near nrows): keep ~23% weight. */
					if (min_r <= 1 || max_r >= (s32)nrows - 2 ||
					    min_c <= 1 || max_c >= (s32)ncols - 2) {
						if (max_r >= (s32)nrows - 2)
							shid->blob_wsum[bi] = shid->blob_wsum[bi] * 23 / 100;
						else
							shid->blob_wsum[bi] = shid->blob_wsum[bi] * 97 / 100;
					}

					shid->blob_active[bi] = true;
					nlabels++;
					touched_count++;

					/* Eigenvalues: second moments around centroid.
					 * Use the blob's bounding box instead of the
					 * full grid scan for performance. */
					cx = shid->blob_x[bi] / 100;
					cy = shid->blob_y[bi] / 100;
					{
						s32 r, c;
						for (r = min_r; r <= max_r; r++) {
							for (c = min_c; c <= max_c; c++) {
								u32 idx = (u32)r * ncols + (u32)c;
								s16 w;
								s32 dx, dy;

								if (shid->heatmap_label[idx] != label) continue;
								w = shid->heatmap_signal[idx];
								if (w <= 0) continue;
								dx = c - (s32)cx;
								dy = r - (s32)cy;
								sxx += (s64)dx * dx * w;
								syy += (s64)dy * dy * w;
								sxy += (s64)dx * dy * w;
							}
						}
					}
					{
						s64 cov_xx = div_s64(sxx, sw);
						s64 cov_yy = div_s64(syy, sw);
						s64 cov_xy = div_s64(sxy, sw);
						s64 diff = cov_xx - cov_yy;
						u64 disc = (u64)(diff * diff) +
						4ULL * (u64)(cov_xy * cov_xy);
						s32 sq = (s32)int_sqrt(disc);
						s64 major = (cov_xx + cov_yy + sq) / 2;
						s64 minor = (cov_xx + cov_yy - sq) / 2;

						shid->blob_eigmaj[bi] = clamp_t(s64, major, 0, S32_MAX);
						shid->blob_eigmin[bi] = clamp_t(s64, minor, 0, S32_MAX);
						if (diff != 0 || cov_xy != 0) {
							s32 deg = atan2_approx((s32)(2 * cov_xy),
									       (s32)(cov_yy - cov_xx)) / 2;
							if (deg >= 18000) deg -= 18000;
							else if (deg <= -18000) deg += 18000;
							shid->blob_eigori[bi] = deg;
						} else {
							shid->blob_eigori[bi] = 0;
						}
					}
				}
			}
			next_label++;
		}
		}
	} /* have_peak */

	/* Step 5: emit multitouch events with EMA smoothing and slot tracking.
	 * Grid → screen mapping per GROUND_TRUTH.md §22.8.  Calibration
	 * parameters describe the final screen axes, so choose the source grid
	 * extent after swap_xy has selected which grid axis feeds each output. */
	{
		struct input_dev *input = shid->touch_input;
		bool any_touch = false;
		struct { u32 gx; u32 gy; u32 w; u8 idx; } sorted[HEATMAP_MAX_BLOBS];
		u8 sorted_count = 0;
		const u32 SCREEN_MAX = 32767;
		u32 scale_x, scale_y;
		u32 screen_x_cells = swap_xy ? nrows : ncols;
		u32 screen_y_cells = swap_xy ? ncols : nrows;

		if (calib_scale_x > 0)
			scale_x = (u32)calib_scale_x;
		else
			scale_x = (SCREEN_MAX * 1000) / (screen_x_cells - 1);

		if (calib_scale_y > 0)
			scale_y = (u32)calib_scale_y;
		else
			scale_y = (SCREEN_MAX * 1000) / (screen_y_cells - 1);

		for (i = 0; i < HEATMAP_MAX_BLOBS; i++) {
			if (!shid->blob_active[i] || shid->blob_wsum[i] < blob_min_weight)
				continue;
			sorted[sorted_count].gx = shid->blob_x[i];
			sorted[sorted_count].gy = shid->blob_y[i];
			sorted[sorted_count].w = shid->blob_wsum[i];
			sorted[sorted_count].idx = i;
			sorted_count++;
		}
		for (i = 0; i + 1 < sorted_count; i++)
			for (u8 j = i + 1; j < sorted_count; j++)
				if (sorted[j].w > sorted[i].w) {
					typeof(sorted[0]) t = sorted[i];
					sorted[i] = sorted[j]; sorted[j] = t;
				}
		if (sorted_count > HEATMAP_MAX_SLOTS)
			sorted_count = HEATMAP_MAX_SLOTS;

		/* Pre-association filter (Windows DLL:
		 * +0x8C0=0.611, +0x8C4=0.755, +0x8C8=0.831, +0x8CC=0.871).
		 * Discards blobs weaker than (max_weight * pre_assoc_ratio/1000)
		 * before Hungarian assignment — catches noise blobs that survived
		 * initial filtering but are order-of-magnitude weaker than real
		 * fingers. Disabled by default (pre_assoc_ratio=0). */
		if (pre_assoc_ratio > 0 && sorted_count >= 2) {
			u32 min_w = (u64)sorted[0].w * (u32)pre_assoc_ratio / 1000;
			u8 keep = 0;
			for (i = 0; i < sorted_count; i++) {
				if (sorted[i].w >= min_w) {
					if (i != keep) sorted[keep] = sorted[i];
					keep++;
				}
			}
			sorted_count = keep;
		}

		for (i = 0; i < sorted_count; i++) {
			u16 screen_gx = swap_xy ? sorted[i].gy : sorted[i].gx;
			u16 screen_gy = swap_xy ? sorted[i].gx : sorted[i].gy;

			seq_dbg(shid, 2, "CALIB: blob[%u] grid=(%u,%u) screen=(%u,%u) weight=%u scale=(%ux%u)\n",
				 i, sorted[i].gx, sorted[i].gy,
				 (screen_gx * scale_x + 500) / 1000,
				 (screen_gy * scale_y + 500) / 1000,
				 sorted[i].w, scale_x, scale_y);
		}
		if (sorted_count || touched_count)
			seq_dbg(shid, 2, "CALIB: blobs=%u cells_touched=%d\n",
				 sorted_count, touched_count);

		/* Ghost rejection (GROUND_TRUTH.md §22.7 step 3):
		 * Merge blobs closer than ghost_dist threshold (grid cells).
		 * Windows uses Euclidean squared distance (dx²+dy² < ghost_dist²),
		 * not axis-aligned box. If two blobs are within radius, keep the
		 * heavier one, discard the lighter. */
		{
			u8 a, b, j;
			u32 gdsq = (u32)ghost_dist * (u32)ghost_dist * 10000; /* ×100 scale */

			for (a = 0; a < sorted_count; a++) {
				if (sorted[a].w == 0)
					continue;
				for (b = a + 1; b < sorted_count; b++) {
					s32 dx, dy;

					if (sorted[b].w == 0)
						continue;
					dx = (s32)sorted[a].gx - (s32)sorted[b].gx;
					dy = (s32)sorted[a].gy - (s32)sorted[b].gy;
					if ((u32)(dx * dx) + (u32)(dy * dy) <= gdsq) {
						if (sorted[b].w > sorted[a].w) {
							sorted[a].w = 0;
							break;
						} else {
							sorted[b].w = 0;
						}
					}
				}
			}
			j = 0;
			for (a = 0; a < sorted_count; a++) {
				if (sorted[a].w > 0) {
					if (a != j)
						sorted[j] = sorted[a];
					j++;
				}
			}
			sorted_count = j;
		}

		/* Hungarian global assignment (matching Windows TouchPenProcessor0C19).
			 * Replaces the old greedy nearest-neighbor with minimum-cost bipartite
			 * matching.  Each blob is assigned to exactly one slot, minimizing
			 * Euclidean squared distance. Empty slots receive a uniform penalty
			 * so that new slots are only created when no claimed slot is nearby.
			 *
			 * Windows config values (per-device, from decomp):
			 *   Normal association radius:  config+0x8dc = 0.545 grid units
			 *   Continuity radius (1 track): config+0x8e0 = 1.218 grid units
			 *   Coalesce threshold squared: frame_data+0x0c = 36.0
			 * blob_max_distance=3 maps to ~0.545; single-track uses 2.2x. */
		{
			u8 assigned_slot[HEATMAP_MAX_BLOBS];
			u32 new_gx[HEATMAP_MAX_SLOTS], new_gy[HEATMAP_MAX_SLOTS];
			bool new_active[HEATMAP_MAX_SLOTS];
			u8 row_match[HEATMAP_MAX_BLOBS];
			u8 col_match[HEATMAP_MAX_SLOTS];
			u16 row_val[HEATMAP_MAX_BLOBS], col_val[HEATMAP_MAX_SLOTS];
			u8 row_cover[HEATMAP_MAX_BLOBS], col_cover[HEATMAP_MAX_SLOTS];
			s32 dx, dy;
			u8 n_blobs = sorted_count;
			u8 active_slots = 0;
			u8 round, row, col;
			u16 min_val;
			u32 bmd;

			memset(new_active, 0, sizeof(new_active));
			memset(new_gx, 0, sizeof(new_gx));
			memset(new_gy, 0, sizeof(new_gy));
			memset(row_match, 0xFF, sizeof(row_match));
			memset(col_match, 0xFF, sizeof(col_match));
			memset(row_val, 0, sizeof(row_val));
			memset(col_val, 0, sizeof(col_val));

			/* Count claimed slots for single-track continuity radius. */
			for (col = 0; col < HEATMAP_MAX_SLOTS; col++)
				if (shid->blob_slot_state[col] >= 2)
					active_slots++;

			/* Scale association radius by finger count
			 * (matching DLL config table DAT_1808e0460):
			 *   1 finger: 1.218 / 0.545 = 2.23x (continuity +0x8E0)
			 *   2 fingers: 0.545 = 1.00x (normal +0x8DC)
			 *   3 fingers: 1.549 / 0.545 = 2.84x (+0x8E4)
			 *   4 fingers: 1.845 / 0.545 = 3.39x (+0x8E8)
			 *   5+ fingers: 2.161 / 0.545 = 3.97x (+0x8EC)
			 * More fingers → each blob has lower signal → wider
			 * search needed for Hungarian to maintain tracking. */
			bmd = (u32)blob_max_distance * 100;
			if (active_slots == 1)
				bmd = bmd * 22 / 10;   /* ×2.2 */
			else if (active_slots == 3)
				bmd = bmd * 28 / 10;   /* ×2.8 */
			else if (active_slots == 4)
				bmd = bmd * 34 / 10;   /* ×3.4 */
			else if (active_slots >= 5)
				bmd = bmd * 40 / 10;   /* ×4.0 */

			/* Build cost matrix: matching Python oracle Hungarian.
			 * In-range: 10*sqrt(dx²+dy²), out-of-range: 100,
			 * empty/new slot: 1000. */
			for (row = 0; row < n_blobs; row++) {
				row_val[row] = U16_MAX;
				for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
					s32 dx, dy;

					if (shid->blob_slot_state[col] >= 1) {
						dx = (s32)sorted[row].gx - (s32)shid->blob_slot_gx[col];
						dy = (s32)sorted[row].gy - (s32)shid->blob_slot_gy[col];
						if (dx < 0) dx = -dx;
						if (dy < 0) dy = -dy;
						if ((u32)dx <= bmd && (u32)dy <= bmd) {
							u32 d = (u32)dx * (u32)dx + (u32)dy * (u32)dy;
							shid->cost[row][col] = (u16)(int_sqrt((u64)d) / 10);
						} else {
							shid->cost[row][col] = 100;
						}
					} else {
						shid->cost[row][col] = 1000;
					}
					if (shid->cost[row][col] < row_val[row])
						row_val[row] = shid->cost[row][col];
				}
				/* Subtract row minimum. */
				for (col = 0; col < HEATMAP_MAX_SLOTS; col++)
					shid->cost[row][col] -= row_val[row];
			}

			/* Hungarian algorithm iterations. */
			memset(row_cover, 0, sizeof(row_cover));
			memset(col_cover, 0, sizeof(col_cover));
			for (round = 0; round < n_blobs; round++) {
				memset(row_cover, 0, sizeof(row_cover));
				memset(col_cover, 0, sizeof(col_cover));

				for (;;) {
					u8 changed = 0;

					/* Select uncovered row with zero in an uncovered column. */
					for (row = 0; row < n_blobs; row++) {
						if (row_cover[row]) continue;
						for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
							if (col_cover[col]) continue;
							if (shid->cost[row][col] == 0) {
								row_match[row] = col;
								col_match[col] = row;
								row_cover[row] = 1;
								col_cover[col] = 1;
								changed = 1;
								break;
							}
						}
					}
					if (changed) continue;

					/* All uncovered costs > 0: find min and update. */
					min_val = U16_MAX;
					for (row = 0; row < n_blobs; row++) {
						if (row_cover[row]) continue;
						for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
							if (col_cover[col]) continue;
							if (shid->cost[row][col] < min_val)
								min_val = shid->cost[row][col];
						}
					}
					if (min_val == U16_MAX)
						break;

					for (row = 0; row < n_blobs; row++) {
						for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
							if (row_cover[row] && col_cover[col])
								shid->cost[row][col] += min_val;
							else if (!row_cover[row] && !col_cover[col])
								shid->cost[row][col] -= min_val;
						}
					}
				}
			}

			/* Build final assignments from the matching.
			 * Only keep matches with cost below the empty-slot penalty. */
			for (row = 0; row < n_blobs; row++)
				assigned_slot[row] = 0xFF;
			for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
				row = col_match[col];
				if (row >= n_blobs) continue;
				if (shid->blob_slot_state[col] >= 1) {
					dx = (s32)sorted[row].gx - (s32)shid->blob_slot_gx[col];
					dy = (s32)sorted[row].gy - (s32)shid->blob_slot_gy[col];
					if (dx < 0) dx = -dx;
					if (dy < 0) dy = -dy;
					if ((u32)dx <= bmd && (u32)dy <= bmd)
						assigned_slot[row] = col;
				} else {
					assigned_slot[row] = col;
				}
			}

			/* Process slot state transitions (GROUND_TRUTH §22.4):
			 * 0=empty, 1=new, 2=claimed, 3=lift */
			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				u8 bi = 0xFF;

				for (i = 0; i < sorted_count; i++) {
					if (assigned_slot[i] == s) { bi = i; break; }
				}

				if (bi != 0xFF) {
					u32 gx = sorted[bi].gx;
					u32 gy = sorted[bi].gy;
					u32 w  = sorted[bi].w;
					u8 old_state = shid->blob_slot_state[s];
					u32 old_gx = shid->blob_slot_gx[s];
					u32 old_gy = shid->blob_slot_gy[s];
					bool was_claimed = (old_state >= 2);
					u8 blob_idx = sorted[bi].idx;

			/* Sanity: reject jumps beyond association radius
				 * + 2 cells for claimed slots — noise blobs can't
				 * steal existing finger assignments (3+ finger).
				 * Scales with the per-finger-count radius. */
				if (was_claimed) {
						s32 jdx = (s32)gx - (s32)shid->blob_slot_gx[s];
						s32 jdy = (s32)gy - (s32)shid->blob_slot_gy[s];
						u32 jmax = bmd + 200;
						if (jdx < 0) jdx = -jdx;
						if (jdy < 0) jdy = -jdy;
						if ((u32)jdx > jmax || (u32)jdy > jmax)
							goto slot_unassigned;
					}

					/* Copy per-blob eigenvalues to the assigned slot. */
					if (blob_idx < HEATMAP_MAX_BLOBS &&
					    shid->blob_eigmaj[blob_idx] > 0) {
						shid->eigmaj[s] = shid->blob_eigmaj[blob_idx];
						shid->eigmin[s] = shid->blob_eigmin[blob_idx];
						shid->eigori[s] = shid->blob_eigori[blob_idx];
					}

					switch (shid->blob_slot_state[s]) {
					case 0:
						shid->blob_slot_state[s] = 1;
						shid->blob_slot_duration[s] = 1;
						shid->blob_slot_stationary[s] = 0;
						break;
					case 1:
						shid->blob_slot_duration[s]++;
						if (shid->blob_slot_duration[s] >= (u32)blob_debounce)
							shid->blob_slot_state[s] = 2;
						break;
					case 2:
						shid->blob_slot_duration[s]++;
						break;
					case 3:
						shid->blob_slot_state[s] = 1;
						shid->blob_slot_duration[s] = 1;
						shid->blob_slot_stationary[s] = 0;
						break;
					case 4:
						/* Hold recovery: only accept a substantial blob.
						 * Noise after finger lift produces low-weight
						 * blobs (w < 4000) that should not re-claim the
						 * slot. Let them expire through hold→lift instead. */
						if (w < 4000 || shid->blob_slot_missed[s] < 2)
							goto slot_unassigned;
						shid->blob_slot_state[s] = 2;
						shid->blob_slot_duration[s] = 1;
						shid->blob_slot_stationary[s] = 0;
						break;
					}
					shid->blob_slot_missed[s] = 0;
					shid->blob_slot_gx[s] = gx;
					shid->blob_slot_gy[s] = gy;
					/* EMA on blob weight (matching Windows: weight_smoothed = (old*7+new)/8).
					 * Only for continuously-claimed slots — reset weight after hold/lift. */
					if (old_state == 2)
						shid->blob_slot_weight[s] = (shid->blob_slot_weight[s] * 7 + w) / 8;
					else
						shid->blob_slot_weight[s] = w;

					new_active[s] = (shid->blob_slot_state[s] >= 2);

					if (new_active[s]) {
						/* EMA + deadband + stationary lock.
						 * EMA alpha=7 (weight 1/8) for smooth tracking.
						 * Deadband ±80 (0.8 cells, ~3.7 px) suppresses
						 * antenna-noise jitter during slow holds.
						 * After 6 consecutive stationary frames the
						 * position is frozen until a real move occurs. */
						if (old_state == 2) {
							u32 egx = (old_gx * frame_ema_alpha + gx) /
								  (frame_ema_alpha + 1);
							u32 egy = (old_gy * frame_ema_alpha + gy) /
								  (frame_ema_alpha + 1);
							s32 ddx = (s32)egx - (s32)old_gx;
							s32 ddy = (s32)egy - (s32)old_gy;

							if (ddx >= -80 && ddx <= 80 &&
							    ddy >= -80 && ddy <= 80) {
								u8 c = shid->blob_slot_stationary[s];
								if (c < 6) {
									c++;
									shid->blob_slot_stationary[s] = c;
								}
								if (c >= 6) {
									new_gx[s] = old_gx;
									new_gy[s] = old_gy;
								} else {
									new_gx[s] = egx;
									new_gy[s] = egy;
								}
							} else {
								shid->blob_slot_stationary[s] = 0;
								new_gx[s] = egx;
								new_gy[s] = egy;
							}
						} else {
							new_gx[s] = gx;
							new_gy[s] = gy;
						}
						shid->blob_slot_gx[s] = new_gx[s];
						shid->blob_slot_gy[s] = new_gy[s];

						/* Push to history ring for lift lookback. */
						{
							u8 hp = shid->blob_slot_hpos[s];
							shid->blob_slot_hx[s][hp] = new_gx[s];
							shid->blob_slot_hy[s][hp] = new_gy[s];
							shid->blob_slot_hpos[s] = (hp + 1) % SLOT_HISTORY_DEPTH;
							if (shid->blob_slot_hcount[s] < SLOT_HISTORY_DEPTH)
								shid->blob_slot_hcount[s]++;
						}
					}
				} else {
slot_unassigned:				switch (shid->blob_slot_state[s]) {
					case 1:
						shid->blob_slot_state[s] = 0;
						shid->blob_slot_duration[s] = 0;
						break;
					case 2:
						if (hold_frames < 1) {
							/* Lift lookback: use position from 2 frames
							 * ago when the finger was still fully down. */
							u8 hc = shid->blob_slot_hcount[s];
							if (hc >= 2) {
								u8 hp = shid->blob_slot_hpos[s];
								u8 back = (hp + SLOT_HISTORY_DEPTH - 2) % SLOT_HISTORY_DEPTH;
								shid->blob_slot_gx[s] = shid->blob_slot_hx[s][back];
								shid->blob_slot_gy[s] = shid->blob_slot_hy[s][back];
							}
							shid->blob_slot_state[s] = 3;
							shid->blob_slot_missed[s] = 0;
						} else {
							shid->blob_slot_state[s] = 4;
							shid->blob_slot_missed[s] = 1;
						}
						break;
					case 4:
						shid->blob_slot_missed[s]++;
						if (shid->blob_slot_missed[s] >= (u32)hold_frames) {
							/* Lift lookback: use position from 2 frames
							 * ago (when the finger was still fully down). */
							u8 hc = shid->blob_slot_hcount[s];
							if (hc >= 2) {
								u8 hp = shid->blob_slot_hpos[s];
								u8 back = (hp + SLOT_HISTORY_DEPTH - 2) % SLOT_HISTORY_DEPTH;
								shid->blob_slot_gx[s] = shid->blob_slot_hx[s][back];
								shid->blob_slot_gy[s] = shid->blob_slot_hy[s][back];
							}
							shid->blob_slot_state[s] = 3;
							shid->blob_slot_missed[s] = 0;
						}
						break;
					case 3:
						shid->blob_slot_missed[s]++;
						if (shid->blob_slot_missed[s] >=
						    (u32)blob_lift_frames) {
							shid->blob_slot_state[s] = 0;
							shid->blob_slot_missed[s] = 0;
						}
						break;
					case 0:
						break;
					}

				/* Hold and lift states remain active — emit last
				 * known position (hold) or lookback position (lift). */
				if (shid->blob_slot_state[s] == 4 ||
				    shid->blob_slot_state[s] == 3) {
					new_active[s] = true;
					new_gx[s] = shid->blob_slot_gx[s];
					new_gy[s] = shid->blob_slot_gy[s];
				} else {
					new_active[s] = false;
				}
		}
	}

			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				input_mt_slot(input, s);
				input_mt_report_slot_state(input, MT_TOOL_FINGER, new_active[s]);
				if (new_active[s]) {
					s64 fx, fy, tmp;
					u32 screen_gx = new_gx[s];
					u32 screen_gy = new_gy[s];
					/* Orient first: scales and offsets always address final X/Y. */
					if (swap_xy) {
						tmp = screen_gx;
						screen_gx = screen_gy;
						screen_gy = tmp;
					}
					/* Fixed-point ×100: divide by 100000 not 1000. */
					fx = ((s64)screen_gx * scale_x + 50000) / 100000;
					fy = ((s64)screen_gy * scale_y + 50000) / 100000;
					fx += calib_offset_x;
					fy += calib_offset_y;
					if (invert_x) fx = (s64)SCREEN_MAX - fx;
					if (invert_y) fy = (s64)SCREEN_MAX - fy;
					if (fx < 0) fx = 0;
					if (fy < 0) fy = 0;
					if (fx > SCREEN_MAX) fx = SCREEN_MAX;
					if (fy > SCREEN_MAX) fy = SCREEN_MAX;
					input_report_abs(input, ABS_MT_POSITION_X, (u16)fx);
					input_report_abs(input, ABS_MT_POSITION_Y, (u16)fy);
					any_touch = true;

					/* Emit touch ellipse for this slot. */
					if (shid->eigmaj[s] > 0) {
						u32 major, minor;
						s32 ori;

						major = ((u32)int_sqrt(shid->eigmaj[s]) * scale_x + 500) / 1000;
						minor = ((u32)int_sqrt(shid->eigmin[s]) * scale_y + 500) / 1000;
						ori = shid->eigori[s] / 100;
						if (swap_xy) {
							u32 t = major; major = minor; minor = t;
							ori = -ori;
						}
						if (major < 1)
							major = 1;
						if (minor < 1)
							minor = 1;
						if (major > SCREEN_MAX)
							major = SCREEN_MAX;
						if (minor > SCREEN_MAX)
							minor = SCREEN_MAX;
						if (ori > 89) ori = 89;
						if (ori < -89) ori = -89;
						input_report_abs(input, ABS_MT_TOUCH_MAJOR, major);
						input_report_abs(input, ABS_MT_TOUCH_MINOR, minor);
						input_report_abs(input, ABS_MT_ORIENTATION, ori);
					}
				}
			}
		}

		/* Bug fix: only HEATMAP_MAX_SLOTS slots were
		 * ever allocated via input_mt_init_slots() — the loop above
		 * already reports the correct active/inactive state for all of
		 * them. Calling input_mt_slot() with an out-of-range index here
		 * was a no-op in the input core (slot left unchanged at the last
		 * valid one, HEATMAP_MAX_SLOTS-1), so this loop was re-clearing
		 * that last real slot immediately after it was just reported
		 * active, silently deactivating the second finger every frame. */
		input_mt_sync_frame(input);
		input_report_key(input, BTN_TOUCH, any_touch ? 1 : 0);
		input_sync(input);
	}
}

int mshw0231_raw_input_register(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	int ret;

	if (!shid->raw_mode_active)
		return 0;

	{
		shid->touch_input = input_allocate_device();
		if (shid->touch_input) {
			shid->touch_input->name = "MSHW0231 Touchscreen";
			shid->touch_input->phys = "spi-hid/input1";
			shid->touch_input->id.bustype = BUS_SPI;
			shid->touch_input->id.vendor = 0x045E;
			shid->touch_input->id.product = 0x0C19;
			shid->touch_input->dev.parent = &shid->spi->dev; /* attach to physical SPI dev */
			set_bit(INPUT_PROP_DIRECT, shid->touch_input->propbit);
			set_bit(EV_ABS, shid->touch_input->evbit);
			set_bit(EV_KEY, shid->touch_input->evbit);
			set_bit(BTN_TOUCH, shid->touch_input->keybit);
			input_set_abs_params(shid->touch_input, ABS_MT_POSITION_X, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_MT_POSITION_Y, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_X, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_Y, 0, 32767, 0, 0);
			input_abs_set_res(shid->touch_input, ABS_MT_POSITION_X, 112);
			input_abs_set_res(shid->touch_input, ABS_MT_POSITION_Y, 198);
			input_abs_set_res(shid->touch_input, ABS_X, 112);
			input_abs_set_res(shid->touch_input, ABS_Y, 198);
			input_set_abs_params(shid->touch_input, ABS_MT_TOUCH_MAJOR, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_MT_TOUCH_MINOR, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_MT_ORIENTATION, -89, 89, 0, 0);
			ret = input_mt_init_slots(shid->touch_input, HEATMAP_MAX_SLOTS,
						  INPUT_MT_DIRECT);
			if (ret) {
				dev_warn(dev, "HEATMAP: failed to init MT slots (%d)\n", ret);
				input_free_device(shid->touch_input);
				shid->touch_input = NULL;
				return ret;
			}
			if (input_register_device(shid->touch_input)) {
				dev_warn(dev, "HEATMAP: failed to register touch input device\n");
				input_free_device(shid->touch_input);
				shid->touch_input = NULL;
			} else {
				dev_info(dev, "HEATMAP: multitouch input device registered\n");
			}
		}
	}
	return 0;
}

int mshw0231_raw_consume_v0(struct spi_hid *shid, const u8 *body,
			    u32 body_length)
{
	struct spi_hid_capimg_raster raster;
	int ret;

	ret = spi_hid_capimg_decode_v0(body, body_length, &raster);
	if (ret)
		return ret;

	mshw0231_raw_process_samples(shid, raster.samples,
				     SPI_HID_CAPIMG_RASTER_SAMPLES, 0x0c);
	return 0;
}

void mshw0231_raw_consume_samples(struct spi_hid *shid, const u8 *samples,
				  u32 sample_count, u8 content_id)
{
	mshw0231_raw_process_samples(shid, samples, sample_count, content_id);
}
