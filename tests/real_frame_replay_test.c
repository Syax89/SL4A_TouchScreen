/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Replays the recovered Windows touch session (537 real frames extracted
 * from the surface_touch.csv SPB capture; one hex line per frame: 5 bytes
 * of 0xFF padding + the 4304-byte V0 0x0c body) through the driver's OWN
 * code on the host:
 *
 *   - the body is decoded by the real spi_hid_capimg_decode_v0()
 *     (driver/spi-hid-capimg.c), and
 *   - the decoded raster goes through the real mshw0231_raw_consume_v0()
 *     (driver/mshw0231-raw.c) — the same entry point the kernel driver's
 *     core calls for a live 0x0c report — and the contacts it publishes are
 *     read back through the linux/input.h + linux/input/mt.h stubs, i.e. the
 *     same slot/position/BTN_TOUCH state an evdev listener would see.
 *
 * The question is not "is the pipeline correct" but "how many contacts does
 * OUR pipeline get out of real frames, and at which stage does each real one
 * get lost". That cannot be answered from the published MT events alone, so
 * this translation unit #includes the staged copy of driver/mshw0231-raw.c
 * (see the Makefile rule) and calls the driver's own static stage functions
 * on the state the real run left behind:
 *
 *   raw_detect_peaks()    - the peak counter (reads shid->heatmap_*, no writes)
 *   raw_ccl_flood_fill()  - the blob/CCL counter, on a COPY of shid
 *   raw_ghost_merge()     - the merge stage, on the blob list rebuilt from
 *                           shid->blob_* the way mshw0231_raw_process_samples()
 *                           builds it, with the slot state from before the frame
 *
 * Nothing in driver/ is modified and no pipeline DECISION is reimplemented:
 * the peaks, the connected components, the merge and the published contacts
 * all come out of the driver's own functions. What this file does compute
 * itself is (a) the REFERENCE blob count (an independent 4-connected CCL at
 * Windows' 0.1 per-cell threshold = byte < 135 over the decoded raster) and
 * (b) a per-component replay of the four comparisons raw_ccl_flood_fill()
 * applies (HEATMAP_MIN_BLOB_PIXELS, max_rise, blob_min_weight,
 * HEATMAP_VELOCITY_REJECT_RADIUS), so the component a reference blob belongs
 * to can be attributed to the exact comparison that rejected it. Every
 * threshold in those messages is the driver's own constant or module param.
 *
 * Usage: real_frame_replay_test <frames.txt> [--trace]
 *   --trace also enables the driver's seq_dbg() output (sl4a_debug_level = 2)
 *   on stderr, so the reconstructed blob list can be diffed against the
 *   driver's own "CALIB: blobs=N cells_touched=M" line for the same frame.
 *
 * Output: one line per frame
 *   f<idx> ref=<reference blobs> touch=<touched cells> peaks=<peaks>
 *          ccl=<blobs committed by CCL> sorted=<pre-merge list>
 *          merged=<post-ghost-merge list> pub=<published MT slots>
 * a detail block for every frame that publishes fewer contacts than the
 * reference has blobs (the stage that dropped each one, with values), and a
 * summary.
 *
 * The corpus is Microsoft-derived capture data and is deliberately NOT in
 * this repository: when the file is absent the tool prints SKIP and exits 0
 * (the Makefile also runs it behind `|| true`, like the other optional
 * contracts).
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spi-hid-core.h"
#include "mshw0231-raw.h"
#include "mshw0231-raw-constants.h"
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include "mt_record.h"

/*
 * The driver's own source, staged into raw-pipeline-stage/ by the Makefile
 * (a directory without the real spi-hid-core.h, so the stub shadows it) and
 * compiled into THIS translation unit so its static stage functions and
 * module params are reachable from the analysis below. That include is the
 * only reason this test does not simply link driver/mshw0231-raw.c as a
 * separate object the way raw_pipeline_replay_test.c does.
 */
#include "raw-pipeline-stage/mshw0231-raw.c"

#define GRID_COLS 72
#define GRID_ROWS 48
#define GRID_CELLS (GRID_COLS * GRID_ROWS)   /* 3456 samples, the proven layout */
#define BODY_BYTES 4304                      /* V0 0x0c body, as tracked fixtures use */
#define PAD_BYTES 5                          /* SPB 0xFF padding before the body */
#define REF_THRESHOLD 135                    /* Windows threshold 0.1 per cell: byte < 135 */
#define FRAME_JIFFIES 20                     /* measured cadence of this capture (~50 Hz) */
#define MAX_FRAMES 1024
#define MAX_COMPONENTS 128
#define MAX_REF_BLOBS 24

static int passed;
static int failed;

#define CHECK(cond, msg, ...) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL: " msg "\n", ##__VA_ARGS__); \
		failed++; \
	} else { \
		passed++; \
	} \
} while (0)

/* ── The reference signal: an independent CCL over the decoded raster ── */

struct ref_blob {
	int size;          /* cells with byte < REF_THRESHOLD */
	int first_cell;    /* raster index of the first cell found (raster order) */
	int gx100, gy100;  /* signal-weighted centroid, x100 (same units as blob_x[]) */
	int weight;        /* sum of c590 signal over the cells, driver x10^4 units */
	int min_byte;
};

static int ref_grid[GRID_CELLS];   /* scratch label raster for the reference pass */

static int ref_blobs_at_threshold(const unsigned char *raster, const s16 *lut,
				  struct ref_blob *out, int max_out)
{
	static int queue[GRID_CELLS];
	int count = 0;
	int i;

	memset(ref_grid, 0, sizeof(ref_grid));
	memset(out, 0, (size_t)max_out * sizeof(out[0]));

	for (i = 0; i < GRID_CELLS; i++) {
		struct ref_blob *b;
		int head = 0, tail = 0;
		long sx = 0, sy = 0, sw = 0;

		if (ref_grid[i] || raster[i] >= REF_THRESHOLD)
			continue;
		if (count >= max_out)
			break;

		ref_grid[i] = 1;
		queue[tail++] = i;
		b = &out[count];
		b->first_cell = i;
		b->min_byte = 255;

		while (head < tail) {
			int idx = queue[head++];
			int row = idx / GRID_COLS, col = idx % GRID_COLS;
			int w = lut[raster[idx]];

			b->size++;
			if (raster[idx] < b->min_byte)
				b->min_byte = raster[idx];
			sx += (long)col * w;
			sy += (long)row * w;
			sw += w;

			if (col > 0 && !ref_grid[idx - 1] && raster[idx - 1] < REF_THRESHOLD) {
				ref_grid[idx - 1] = 1;
				queue[tail++] = idx - 1;
			}
			if (col + 1 < GRID_COLS && !ref_grid[idx + 1] &&
			    raster[idx + 1] < REF_THRESHOLD) {
				ref_grid[idx + 1] = 1;
				queue[tail++] = idx + 1;
			}
			if (row > 0 && !ref_grid[idx - GRID_COLS] &&
			    raster[idx - GRID_COLS] < REF_THRESHOLD) {
				ref_grid[idx - GRID_COLS] = 1;
				queue[tail++] = idx - GRID_COLS;
			}
			if (row + 1 < GRID_ROWS && !ref_grid[idx + GRID_COLS] &&
			    raster[idx + GRID_COLS] < REF_THRESHOLD) {
				ref_grid[idx + GRID_COLS] = 1;
				queue[tail++] = idx + GRID_COLS;
			}
		}
		b->weight = (int)sw;
		if (sw > 0) {
			b->gx100 = (int)(sx * 100 / sw);
			b->gy100 = (int)(sy * 100 / sw);
		}
		count++;
	}
	return count;
}

/* ── Corpus loading ──────────────────────────────────────────────────── */

struct frame {
	unsigned char body[BODY_BYTES];
	int ref_count;
	struct ref_blob ref[MAX_REF_BLOBS];
};

static struct frame frames[MAX_FRAMES];

static int hex_nibble(int c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;
	return -1;
}

/*
 * One hex line per frame. The tracked corpus is the SPB wire form (5 padding
 * bytes + the 4304-byte body); a bare-body file is accepted as well, decided
 * by where the V0 magic sits, so a wrong assumption cannot silently shift the
 * raster by 5 bytes.
 */
static int hex_line_to_body(const char *line, unsigned char *body)
{
	static unsigned char raw[8192];
	size_t digits = 0, bytes, offset;
	const char *p;

	for (p = line; *p && *p != '\n' && *p != '\r'; p++) {
		int nib = hex_nibble(*p);

		if (*p == ' ' || *p == '\t')
			continue;
		if (nib < 0)
			return -1;
		if (digits >= sizeof(raw) * 2)
			return -1;
		if (digits % 2 == 0)
			raw[digits / 2] = (unsigned char)(nib << 4);
		else
			raw[digits / 2] |= (unsigned char)nib;
		digits++;
	}
	if (!digits || digits % 2)
		return -1;
	bytes = digits / 2;
	if (bytes == BODY_BYTES + PAD_BYTES && raw[PAD_BYTES] == 0xce &&
	    raw[PAD_BYTES + 1] == 0x10 && raw[PAD_BYTES + 2] == 0x0c)
		offset = PAD_BYTES;
	else if (bytes >= BODY_BYTES && raw[0] == 0xce && raw[1] == 0x10 && raw[2] == 0x0c)
		offset = 0;
	else
		return -1;
	memcpy(body, raw + offset, BODY_BYTES);
	return 0;
}

/* Returns the frame count, -1 when the file cannot be opened, or a negative
 * count of unreadable lines. */
static int load_frames(const char *path, int *bad_lines)
{
	char line[16384];
	FILE *f = fopen(path, "r");
	int count = 0;

	*bad_lines = 0;
	if (!f)
		return -1;
	while (fgets(line, sizeof(line), f) && count < MAX_FRAMES) {
		const char *p = line;

		while (*p == ' ' || *p == '\t')
			p++;
		if (*p == '\n' || *p == '\r' || *p == '\0')
			continue;              /* blank line */
		if (hex_line_to_body(line, frames[count].body) == 0)
			count++;
		else
			(*bad_lines)++;
	}
	fclose(f);
	return *bad_lines ? -*bad_lines : count;
}

/* ── Device setup, mirroring what the driver does at probe time ──────── */

static struct spi_hid shid;
static struct spi_hid pre_state;    /* state as it was before the current frame */
static struct spi_device spidev;

static void setup_device(void)
{
	/* SL4 (MSHW0231) tuning: 72x48 grid, 3456 capimg samples, 30-frame
	 * baseline, EMA alpha 7 — the driver's current defaults, the same
	 * raw_pipeline_replay_test.c uses. */
	static const struct spi_hid_dev_cfg sl4_cfg = {
		.capimg_raster_samples   = GRID_CELLS,
		.heatmap_baseline_needed = 30,
		.heatmap_baseline_alpha  = 7,
		.grid_cols               = GRID_COLS,
		.grid_rows               = GRID_ROWS,
	};

	memset(&shid, 0, sizeof(shid));
	memset(&spidev, 0, sizeof(spidev));
	shid.spi = &spidev;
	shid.raw_mode_active = true;
	shid.cfg = &sl4_cfg;

	mshw0231_raw_init(&shid);
	if (mshw0231_raw_input_register(&shid) != 0 || !shid.touch_input) {
		fprintf(stderr, "FATAL: mshw0231_raw_input_register() failed\n");
		exit(1);
	}
}

/* One frame through the driver's real V0 path: its decoder + its pipeline. */
static int feed_body(const unsigned char *body)
{
	int ret;

	jiffies += FRAME_JIFFIES;
	ret = mshw0231_raw_consume_v0(&shid, body, BODY_BYTES);
	if (ret)
		fprintf(stderr, "note: mshw0231_raw_consume_v0() returned %d for a frame\n", ret);
	return ret;
}

/* ── Per-frame analysis ──────────────────────────────────────────────── */

struct component {
	int label;
	int pixels;
	int max_rise;
	int sw;
	int min_r, max_r, min_c, max_c;
	int gx100, gy100;
	int blob_idx;              /* driver blob index, -1 when rejected */
	int merge_loser;           /* pre-merge list index when the merge ate it, else -1 */
	const char *drop;          /* NULL when the component cleared every CCL gate */
	char why[200];             /* the exact comparison that rejected it */
};

static struct {
	int touched_cells;
	int peaks;
	u16 peaks_col[HEATMAP_MAX_PEAKS];
	u16 peaks_row[HEATMAP_MAX_PEAKS];
	int ccl_blobs;
	int ccl_touched;
	int n_components;
	int truncated;
	struct component comp[MAX_COMPONENTS];
	u16 label_raster[GRID_CELLS];
	int active_slots_before;
	int ghost_radius;          /* cells, after the per-finger-count scaling */
	int n_pre;                 /* blob list handed to the ghost merge */
	struct blob_entry pre[HEATMAP_MAX_BLOBS];
	int n_post;                /* list after the ghost merge */
	struct blob_entry post[HEATMAP_MAX_BLOBS];
	int merge_loser[HEATMAP_MAX_BLOBS];   /* pre[] index zeroed by the merge */
	int merge_winner[HEATMAP_MAX_BLOBS];  /* pre[] index it was merged into */
	int n_merges;
	int row_of_blob[HEATMAP_MAX_BLOBS];   /* pre[] row for a driver blob index */
	u8 assigned[HEATMAP_MAX_BLOBS];       /* raw_hungarian_match() result, re-run */
	u32 bmd;                              /* the association radius it used */
	int published;
} ff;

static int count_touched(void)
{
	int i, n = 0;

	for (i = 0; i < GRID_CELLS; i++)
		if (shid.heatmap_touched[i])
			n++;
	return n;
}

/*
 * Rebuild the blob list mshw0231_raw_process_samples() hands to
 * raw_ghost_merge(): every committed blob whose raw (pre-edge-penalty) weight
 * is at least blob_min_weight, sorted by penalised weight descending, capped
 * at HEATMAP_MAX_SLOTS. Only used to call the driver's own merge function
 * again from the outside.
 */
static int build_sorted_list(struct blob_entry *sorted)
{
	int count = 0, i, j;

	for (i = 0; i < HEATMAP_MAX_BLOBS; i++) {
		if (!shid.blob_active[i] || shid.blob_raw_wsum[i] < (u32)blob_min_weight)
			continue;
		sorted[count].gx = shid.blob_x[i];
		sorted[count].gy = shid.blob_y[i];
		sorted[count].w = shid.blob_wsum[i];
		sorted[count].raw_w = shid.blob_raw_wsum[i];
		sorted[count].idx = (u8)i;
		count++;
	}
	for (i = 0; i + 1 < count; i++)
		for (j = i + 1; j < count; j++)
			if (sorted[j].w > sorted[i].w) {
				struct blob_entry t = sorted[i];

				sorted[i] = sorted[j];
				sorted[j] = t;
			}
	if (count > HEATMAP_MAX_SLOTS)
		count = HEATMAP_MAX_SLOTS;
	return count;
}

/* The merge radius raw_ghost_merge() derives from the finger count it sees
 * (mirror of those eight lines, so the detail line can quote the value that
 * was actually compared against). */
static int ghost_radius_cells(int dist, int active_slots)
{
	int gd = dist;

	if (active_slots == 1)
		gd = dist * GHOST_RADIUS_1_FINGER / 10;
	else if (active_slots == 3)
		gd = dist * GHOST_RADIUS_3_FINGERS / 10;
	else if (active_slots == 4)
		gd = dist * GHOST_RADIUS_4_FINGERS / 10;
	else if (active_slots >= 5)
		gd = dist * GHOST_RADIUS_5_FINGERS / 10;
	return gd < 1 ? 1 : gd;
}

/*
 * Per-component statistics from the label raster the driver's own flood fill
 * produced (the driver's units: weighted sums, bbox, max rise), plus the
 * verdict of each CCL gate in the order raw_ccl_flood_fill() applies them.
 */
static void analyse_components(u8 npeaks)
{
	int max_label = 0;
	int i, label;

	memset(ff.comp, 0, sizeof(ff.comp));
	ff.n_components = 0;
	ff.truncated = 0;

	for (i = 0; i < GRID_CELLS; i++)
		if (ff.label_raster[i] > max_label)
			max_label = ff.label_raster[i];

	for (label = 1; label <= max_label; label++) {
		struct component *c;
		long sx = 0, sy = 0, sw = 0;
		int pixels = 0, max_rise = 0;
		int min_r = GRID_ROWS, max_r = -1, min_c = GRID_COLS, max_c = -1;

		if (ff.n_components >= MAX_COMPONENTS) {
			ff.truncated++;
			break;
		}
		for (i = 0; i < GRID_CELLS; i++) {
			int w, row, col;

			if (ff.label_raster[i] != label)
				continue;
			w = shid.heatmap_signal[i];
			if (w <= 0)
				continue;
			row = i / GRID_COLS;
			col = i % GRID_COLS;
			pixels++;
			if (w > max_rise)
				max_rise = w;
			sx += (long)col * w;
			sy += (long)row * w;
			sw += w;
			if (row < min_r) min_r = row;
			if (row > max_r) max_r = row;
			if (col < min_c) min_c = col;
			if (col > max_c) max_c = col;
		}
		if (!pixels)
			continue;

		c = &ff.comp[ff.n_components++];
		c->label = label;
		c->pixels = pixels;
		c->max_rise = max_rise;
		c->sw = (int)sw;
		c->min_r = min_r;
		c->max_r = max_r;
		c->min_c = min_c;
		c->max_c = max_c;
		c->gx100 = (int)(sx * 100 / sw);
		c->gy100 = (int)(sy * 100 / sw);
		c->blob_idx = -1;
		c->merge_loser = -1;

		/* The CCL gates, in the driver's own order and units. */
		if (pixels < HEATMAP_MIN_BLOB_PIXELS) {
			c->drop = "blob builder: pixel count";
			snprintf(c->why, sizeof(c->why),
				 "pixel_count %d < HEATMAP_MIN_BLOB_PIXELS %d",
				 pixels, HEATMAP_MIN_BLOB_PIXELS);
			continue;
		}
		if (max_rise < HEATMAP_TOUCH_MIN_RISE) {
			c->drop = "blob builder: peak signal";
			snprintf(c->why, sizeof(c->why),
				 "max_rise %d < HEATMAP_TOUCH_MIN_RISE %d",
				 max_rise, HEATMAP_TOUCH_MIN_RISE);
			continue;
		}
		if (sw < (long)blob_min_weight) {
			c->drop = "blob builder: total weight";
			snprintf(c->why, sizeof(c->why),
				 "weight %ld < blob_min_weight %d", sw, blob_min_weight);
			continue;
		}
		{
			int gx = (int)(sx / sw), gy = (int)(sy / sw);
			int p, best = 1 << 20;

			for (p = 0; p < (int)npeaks; p++) {
				int dx = gx - (int)ff.peaks_col[p];
				int dy = gy - (int)ff.peaks_row[p];
				int cheb;

				if (dx < 0)
					dx = -dx;
				if (dy < 0)
					dy = -dy;
				cheb = dx > dy ? dx : dy;
				if (cheb < best)
					best = cheb;
			}
			if (best > HEATMAP_VELOCITY_REJECT_RADIUS) {
				c->drop = "velocity rejection (peak proximity)";
				snprintf(c->why, sizeof(c->why),
					 "centroid (%d,%d) is %d cells (Chebyshev) from the nearest of %d peak(s), > HEATMAP_VELOCITY_REJECT_RADIUS %d",
					 gx, gy, best, (int)npeaks,
					 HEATMAP_VELOCITY_REJECT_RADIUS);
				continue;
			}
		}
		c->drop = NULL;
	}
}

/*
 * Attach the driver's committed blob index to every component that cleared
 * the CCL gates: the driver commits in raster order of each component's first
 * cell, which is ascending label order here.
 */
static void assign_blob_indices(void)
{
	int i, next = 0;

	for (i = 0; i < ff.n_components; i++)
		if (!ff.comp[i].drop)
			ff.comp[i].blob_idx = next++;
	if (next != ff.ccl_blobs)
		fprintf(stderr,
			"note: %d component(s) cleared the CCL gates but raw_ccl_flood_fill() committed %d blob(s) — blob splitting is active in this frame, per-component attribution is approximate\n",
			next, ff.ccl_blobs);
}

static void analyse_frame(void)
{
	static struct spi_hid ccl_state;
	static struct spi_hid merge_state;
	u16 nlabels = 0;
	u8 post_count;
	int i, j;

	ff.touched_cells = count_touched();
	ff.peaks = raw_detect_peaks(&shid, GRID_CELLS, GRID_COLS, GRID_ROWS,
				    ff.peaks_col, ff.peaks_row);

	memcpy(&ccl_state, &shid, sizeof(ccl_state));
	memset(ccl_state.heatmap_label, 0, GRID_CELLS * sizeof(ccl_state.heatmap_label[0]));
	ff.ccl_blobs = 0;
	ff.ccl_touched = 0;
	/* raw_ccl_flood_fill() is the driver's own function on the driver's own
	 * touched/signal arrays — and, as in mshw0231_raw_process_samples(), it
	 * only runs at all when the peak detector found something. */
	if (ff.peaks > 0)
		ff.ccl_blobs = raw_ccl_flood_fill(&ccl_state, GRID_CELLS, GRID_COLS, GRID_ROWS,
						 &nlabels, &ff.ccl_touched, (u8)ff.peaks,
						 ff.peaks_col, ff.peaks_row);
	memcpy(ff.label_raster, ccl_state.heatmap_label, sizeof(ff.label_raster));
	analyse_components((u8)ff.peaks);
	assign_blob_indices();

	/* Ghost merge: the driver's own function, on the list its caller builds,
	 * with the slot state from before this frame as the radius selector (it
	 * runs before raw_update_slots(), exactly as in the driver). */
	ff.active_slots_before = 0;
	for (i = 0; i < HEATMAP_MAX_SLOTS; i++)
		if (pre_state.blob_slot_state[i] >= 2)
			ff.active_slots_before++;
	ff.ghost_radius = ghost_radius_cells(ghost_dist, ff.active_slots_before);

	ff.n_pre = build_sorted_list(ff.pre);
	memcpy(ff.post, ff.pre, sizeof(ff.post));
	post_count = (u8)ff.n_pre;
	ff.n_post = ff.n_pre;
	ff.n_merges = 0;
	if (ff.n_pre > 1) {
		memcpy(&merge_state, &pre_state, sizeof(merge_state));
		raw_ghost_merge(&merge_state, ff.post, &post_count, (u32)ghost_dist);
		ff.n_post = post_count;
		/* The merge zeroes a loser's weight and the caller's compaction then
		 * drops it. Loser and winner are identified by blob index, which is
		 * unique, so the compaction cannot confuse them. */
		for (i = 0; i < ff.n_pre; i++) {
			int found = 0, best = -1, best_d = 1 << 30;

			if (ff.pre[i].w == 0)
				continue;
			for (j = 0; j < ff.n_post; j++)
				if (ff.post[j].w > 0 && ff.post[j].idx == ff.pre[i].idx) {
					found = 1;
					break;
				}
			if (found)
				continue;
			for (j = 0; j < ff.n_post; j++) {
				int dx, dy, d;

				if (ff.post[j].w == 0)
					continue;
				dx = (int)ff.pre[i].gx - (int)ff.post[j].gx;
				dy = (int)ff.pre[i].gy - (int)ff.post[j].gy;
				d = dx * dx + dy * dy;
				if (d < best_d) {
					best_d = d;
					best = j;
				}
			}
			if (ff.n_merges < HEATMAP_MAX_BLOBS) {
				ff.merge_loser[ff.n_merges] = i;
				ff.merge_winner[ff.n_merges] = best;
				ff.n_merges++;
			}
			for (j = 0; j < ff.n_components; j++)
				if (ff.comp[j].blob_idx == (int)ff.pre[i].idx)
					ff.comp[j].merge_loser = i;
		}
	}
	/*
	 * The driver's own matcher, re-run on the same inputs (the post-merge list
	 * and the slot state from before the frame), so a blob that never reached
	 * a slot can be traced to the assignment and the radius that rejected it.
	 * raw_hungarian_match() only reads blob_slot_* and writes cost[][], so a
	 * copy of the pre-frame state is enough.
	 */
	for (i = 0; i < HEATMAP_MAX_BLOBS; i++)
		ff.row_of_blob[i] = -1;
	for (i = 0; i < ff.n_post; i++)
		if (ff.post[i].w > 0)
			ff.row_of_blob[ff.post[i].idx] = i;
	memcpy(&merge_state, &pre_state, sizeof(merge_state));
	for (i = 0; i < HEATMAP_MAX_BLOBS; i++)
		ff.assigned[i] = 0xFF;
	ff.bmd = 0;
	if (ff.n_post > 0)
		ff.bmd = raw_hungarian_match(&merge_state, ff.post, (u8)ff.n_post,
					     ff.assigned, (u32)blob_max_distance);
	ff.published = mt_record_active_count();
}

/* ── Reporting ───────────────────────────────────────────────────────── */

static void print_frame_line(int idx)
{
	printf("f%03d ref=%d touch=%4d peaks=%2d ccl=%d sorted=%d merged=%d pub=%d%s\n",
	       idx, frames[idx].ref_count, ff.touched_cells, ff.peaks, ff.ccl_blobs,
	       ff.n_pre, ff.n_post, ff.published,
	       ff.truncated ? " (components truncated)" : "");
	if (getenv("SL4A_DEBUG_SLOTS")) {
		int s;
		for (s = 0; s < HEATMAP_MAX_SLOTS; s++)
			if (shid.blob_slot_state[s])
				printf("    slot %d: state %u dur %u gx %u gy %u missed %u\n", s,
				       shid.blob_slot_state[s], shid.blob_slot_duration[s],
				       shid.blob_slot_gx[s], shid.blob_slot_gy[s],
				       shid.blob_slot_missed[s]);
		for (s = 0; s < HEATMAP_MAX_BLOBS; s++)
			if (shid.blob_active[s])
				printf("    blob %d: active x %u y %u wsum %u raw %u\n", s,
				       shid.blob_x[s], shid.blob_y[s], shid.blob_wsum[s],
				       shid.blob_raw_wsum[s]);
	}
}

/*
 * Which slot did the driver put this blob in? The decided assignment comes
 * from raw_hungarian_match() re-run on the same inputs; whether the state
 * machine then ACCEPTED it is decided by the same jump guard raw_update_slots()
 * applies before its state switch (only claimed slots are guarded). Matching
 * positions instead would be unreliable: the slot's stored position is
 * EMA-smoothed and deadband-frozen, so it legitimately lags the blob.
 */
static const char *slot_verdict_for_blob(int blob_idx, char *why, size_t why_len)
{
	int row = ff.row_of_blob[blob_idx];
	int as = row >= 0 ? (int)ff.assigned[row] : 0xFF;
	u32 jmax = ff.bmd + HUNGARIAN_JUMP_REJECT_MARGIN;
	s32 jdx, jdy;

	if (as == 0xFF || as >= HEATMAP_MAX_SLOTS) {
		snprintf(why, why_len,
			 "raw_hungarian_match() left this blob unassigned (assigned_slot 0xff) for blob %d at grid (%d,%d) weight %u",
			 blob_idx, (int)shid.blob_x[blob_idx] / 100,
			 (int)shid.blob_y[blob_idx] / 100, shid.blob_raw_wsum[blob_idx]);
		return "slot assignment (unassigned)";
	}

	jdx = (s32)shid.blob_x[blob_idx] - (s32)pre_state.blob_slot_gx[as];
	jdy = (s32)shid.blob_y[blob_idx] - (s32)pre_state.blob_slot_gy[as];
	if (jdx < 0)
		jdx = -jdx;
	if (jdy < 0)
		jdy = -jdy;

	if (pre_state.blob_slot_state[as] >= 2 && ((u32)jdx > jmax || (u32)jdy > jmax)) {
		snprintf(why, why_len,
			 "the matcher assigned this blob (grid (%d,%d) x100) to claimed slot %d at grid (%d,%d) x100, and raw_update_slots()'s jump guard rejected the update: |jdx| %d or |jdy| %d > bmd %u + HUNGARIAN_JUMP_REJECT_MARGIN %d = %u (grid cells x100), so the blob is dropped for this frame and slot %d goes to state %u",
			 (int)shid.blob_x[blob_idx], (int)shid.blob_y[blob_idx], as,
			 (int)pre_state.blob_slot_gx[as], (int)pre_state.blob_slot_gy[as],
			 (int)jdx, (int)jdy, ff.bmd, HUNGARIAN_JUMP_REJECT_MARGIN, jmax,
			 as, shid.blob_slot_state[as]);
		return "slot assignment (jump guard)";
	}

	if (shid.blob_slot_state[as] == 1) {
		snprintf(why, why_len,
			 "slot %d is state 1 (new), duration %u < blob_debounce %d, so it is not published yet (claim: |jdx| %d, |jdy| %d, bmd %u + %d = %u)",
			 as, shid.blob_slot_duration[as], blob_debounce, (int)jdx,
			 (int)jdy, ff.bmd, HUNGARIAN_JUMP_REJECT_MARGIN, jmax);
		return "slot state machine (debounce)";
	}
	snprintf(why, why_len, "slot %d, state %d, duration %u (claim: |jdx| %d, |jdy| %d)",
		 as, shid.blob_slot_state[as], shid.blob_slot_duration[as], (int)jdx, (int)jdy);
	return "published";
}

/* The verdict for one reference blob. Returns the stage name. */
static const char *verdict_for_ref_blob(int idx, int r, char *why, size_t why_len)
{
	const struct ref_blob *b = &frames[idx].ref[r];
	int label = ff.label_raster[b->first_cell];
	struct component *c = NULL;
	int i;

	why[0] = '\0';
	for (i = 0; i < ff.n_components; i++)
		if (ff.comp[i].label == label) {
			c = &ff.comp[i];
			break;
		}

	if (ff.peaks == 0) {
		snprintf(why, why_len,
			 "raw_detect_peaks() found no peak anywhere in this frame (no touched cell is a strict local maximum within Chebyshev radius HEATMAP_PEAK_RADIUS %d), so mshw0231_raw_process_samples() skipped raw_ccl_flood_fill() entirely (`if (npeaks > 0)`)",
			 HEATMAP_PEAK_RADIUS);
		return "peak detector (whole frame)";
	}
	if (!c) {
		int row = b->first_cell / GRID_COLS, col = b->first_cell % GRID_COLS;
		int base = shid.c590_lut[shid.heatmap_baseline[b->first_cell]];
		int rise = shid.heatmap_signal[b->first_cell];

		snprintf(why, why_len,
			 "reference cell (r=%d,c=%d) is in no touched component: rise %d vs HEATMAP_TOUCH_MIN_RISE %d, curr %d vs HEATMAP_TOUCH_MIN_ABSOLUTE %d (baseline byte %u -> c590 %d, touched=%d)",
			 row, col, rise, HEATMAP_TOUCH_MIN_RISE, base + rise,
			 HEATMAP_TOUCH_MIN_ABSOLUTE, shid.heatmap_baseline[b->first_cell],
			 base, shid.heatmap_touched[b->first_cell]);
		return "per-cell touched mask";
	}
	if (c->drop) {
		snprintf(why, why_len, "%s: %s", c->drop, c->why);
		return c->drop;
	}
	if (c->merge_loser >= 0) {
		int loser = c->merge_loser;
		int winner = -1;

		for (i = 0; i < ff.n_merges; i++)
			if (ff.merge_loser[i] == loser)
				winner = ff.merge_winner[i];
		if (winner >= 0) {
			int dx = (int)ff.pre[loser].gx - (int)ff.post[winner].gx;
			int dy = (int)ff.pre[loser].gy - (int)ff.post[winner].gy;

			snprintf(why, why_len,
				 "blob at (%d,%d) weight %u: dx %d, dy %d (grid x100) -> dx^2+dy^2 = %d < ghost_dist^2*10000 = %d, so raw_ghost_merge() zeroed it in favour of the blob at (%d,%d) weight %u (ghost_dist %d scaled to %d cell(s) with %d claimed slot(s) before this frame)",
				 (int)ff.pre[loser].gx / 100, (int)ff.pre[loser].gy / 100,
				 ff.pre[loser].w, dx, dy, dx * dx + dy * dy,
				 ff.ghost_radius * ff.ghost_radius * 10000,
				 (int)ff.post[winner].gx / 100,
				 (int)ff.post[winner].gy / 100, ff.post[winner].w,
				 ghost_dist, ff.ghost_radius, ff.active_slots_before);
		} else {
			snprintf(why, why_len, "merged away by raw_ghost_merge()");
		}
		return "ghost merge";
	}
	if (c->blob_idx < 0) {
		snprintf(why, why_len, "component cleared every gate but has no committed blob index");
		return "blob builder (unattributed)";
	}
	return slot_verdict_for_blob(c->blob_idx, why, why_len);
}

struct tally {
	const char *stage;
	int count;
};

static struct tally lost_tally[32];
static int n_tally;

static void tally_lost(const char *stage)
{
	int i;

	for (i = 0; i < n_tally; i++)
		if (!strcmp(lost_tally[i].stage, stage)) {
			lost_tally[i].count++;
			return;
		}
	if (n_tally < 32) {
		lost_tally[n_tally].stage = stage;
		lost_tally[n_tally].count = 1;
		n_tally++;
	}
}

static int n_ref_all, n_pub_all;
static int n_lost_frames, n_lost_all, n_extra_all;
static int n_ge3_frames, n_short_ge3, lost_ge3;
static int n_onecell_frames, n_peak_zero_frames, n_onecell_blobs;
static int n_extra_frames;
static int lost_frame_idx[512], lost_frame_deficit[512];

/*
 * Published positions per frame (grid cells), so a contact lost to the
 * debounce can be checked against the next few frames: the debounce only
 * delays a contact, it does not remove it, and a loss that never turns into a
 * publication is a different finding from one that shows up two frames later.
 */
static u16 pub_gx[MAX_FRAMES][8], pub_gy[MAX_FRAMES][8];
static u8 pub_n[MAX_FRAMES];
struct pending_lost {
	int frame;
	int gx, gy;
	bool delayed;
};
static struct pending_lost pending[1024];
static int n_pending;
static int n_delayed, n_never;

/* Detail block for one replayed frame, printed while its analysis is live. */
static int report_frame(int idx)
{
	char why[400];
	int r, s, deficit = frames[idx].ref_count - ff.published;

	pub_n[idx] = 0;
	for (s = 0; s < HEATMAP_MAX_SLOTS; s++)
		if (shid.blob_slot_state[s] >= 2 && pub_n[idx] < 8) {
			pub_gx[idx][pub_n[idx]] = (u16)(shid.blob_slot_gx[s] / 100);
			pub_gy[idx][pub_n[idx]] = (u16)(shid.blob_slot_gy[s] / 100);
			pub_n[idx]++;
		}

	if (deficit <= 0) {
		if (deficit < 0) {
			n_extra_all += -deficit;
			n_extra_frames++;
			printf("  F%03d reference has %d blob(s), we publish %d -> %d extra contact(s)\n",
			       idx, frames[idx].ref_count, ff.published, -deficit);
		}
		return 0;
	}

	if (n_lost_frames < 512) {
		lost_frame_idx[n_lost_frames] = idx;
		lost_frame_deficit[n_lost_frames] = deficit;
		n_lost_frames++;
	}
	n_lost_all += deficit;
	if (frames[idx].ref_count >= 3) {
		n_short_ge3++;
		lost_ge3 += deficit;
	}

	printf("  F%03d reference has %d blob(s), we publish %d -> %d contact(s) short\n",
	       idx, frames[idx].ref_count, ff.published, deficit);
	printf("    masks: %d touched cell(s), %d peak(s), %d CCL blob(s) committed, %d blob(s) enter the merge, %d leave it\n",
	       ff.touched_cells, ff.peaks, ff.ccl_blobs, ff.n_pre, ff.n_post);
	if (ff.n_components && ff.n_components <= 12) {
		int i;

		for (i = 0; i < ff.n_components; i++) {
			const struct component *c = &ff.comp[i];

			printf("    mask component %d: %d cell(s) weight %d max_rise %d bbox r%d-%d c%d-%d centroid (%d,%d) -> %s\n",
			       c->label, c->pixels, c->sw, c->max_rise, c->min_r, c->max_r,
			       c->min_c, c->max_c, c->gx100 / 100, c->gy100 / 100,
			       c->drop ? c->why : "committed (no CCL gate rejected it)");
		}
	}
	for (r = 0; r < frames[idx].ref_count; r++) {
		const struct ref_blob *b = &frames[idx].ref[r];
		const char *stage;

		stage = verdict_for_ref_blob(idx, r, why, sizeof(why));
		printf("    reference blob %d: %d cell(s) at centroid (%d,%d) weight %d min byte %d\n",
		       r, b->size, b->gx100 / 100, b->gy100 / 100, b->weight, b->min_byte);
		printf("      %s -> %s\n", stage, why);
		if (strcmp(stage, "published")) {
			tally_lost(stage);
			if (!strcmp(stage, "slot state machine (debounce)") && n_pending < 1024) {
				pending[n_pending].frame = idx;
				pending[n_pending].gx = b->gx100 / 100;
				pending[n_pending].gy = b->gy100 / 100;
				n_pending++;
			}
		}
	}
	return deficit;
}

/*
 * Did a debounce loss come back? The debounce delays a contact three frames; a
 * loss whose blob position never publishes in the next three frames is a
 * contact the pipeline lost outright, not a delayed one. Runs once, after the
 * replay, over the pending list built during it.
 */
static void resolve_pending(int n_frames)
{
	int i;

	for (i = 0; i < n_pending; i++) {
		int f, found = 0;

		for (f = pending[i].frame + 1; f <= pending[i].frame + 3 && f < n_frames && !found; f++)
			for (int p = 0; p < pub_n[f]; p++) {
				int dx = (int)pub_gx[f][p] - pending[i].gx;
				int dy = (int)pub_gy[f][p] - pending[i].gy;

				if (dx < 0) dx = -dx;
				if (dy < 0) dy = -dy;
				if (dx <= 3 && dy <= 3)
					found = 1;
			}
		pending[i].delayed = found;
		if (found)
			n_delayed++;
		else
			n_never++;
	}
}

/* ── Main ────────────────────────────────────────────────────────────── */

int main(int argc, char **argv)
{
	const char *path = argc > 1 ? argv[1] : NULL;
	int bad_lines = 0, n_frames, i, r, ref_hist[16] = { 0 };

	if (!path) {
		printf("SKIP: no frames file given (usage: real_frame_replay_test <frames.txt> [--trace])\n");
		return 0;
	}
	if (argc > 2 && !strcmp(argv[2], "--trace")) {
		/* Verbose early (so warnings during baseline priming are visible),
		 * but the driver's own seq_dbg() trace only for the replay frames —
		 * one trace line group per replay frame, in frame order. */
		sl4a_stub_verbose = 1;
	}

	printf("real_frame_replay_test: replaying %s through the driver's own decoder + raw pipeline\n",
	       path);
	n_frames = load_frames(path, &bad_lines);
	if (n_frames < 0) {
		printf("SKIP: cannot read %s (this corpus is Microsoft-derived capture data and is not part of the repository)\n",
		       path);
		return 0;
	}
	if (bad_lines)
		printf("note: %d unreadable line(s) ignored\n", bad_lines);
	if (n_frames == 0) {
		printf("SKIP: %s has no frames\n", path);
		return 0;
	}
	printf("corpus: %d frame(s) of %d bytes (5 bytes SPB padding + %d-byte V0 body)\n",
	       n_frames, BODY_BYTES + PAD_BYTES, BODY_BYTES);

	setup_device();
	CHECK(mt_init_slots_requested == HEATMAP_MAX_SLOTS,
	      "input_mt_init_slots() called with %u slots, expected %u",
	      mt_init_slots_requested, (unsigned)HEATMAP_MAX_SLOTS);

	/*
	 * Reference pass: the driver's own decoder on every body, then an
	 * independent 4-connected CCL at the Windows per-cell threshold.
	 * mshw0231_raw_consume_v0() is not used here because it would advance the
	 * pipeline; spi_hid_capimg_decode_v0() is the decoder it calls.
	 */
	for (i = 0; i < n_frames; i++) {
		struct spi_hid_capimg_raster raster;
		int ret = spi_hid_capimg_decode_v0(frames[i].body, BODY_BYTES, GRID_CELLS, &raster);

		if (ret) {
			fprintf(stderr, "FATAL: the driver's decoder rejected frame %d (%d)\n", i, ret);
			return 1;
		}
		if (raster.samples != frames[i].body + 28) {
			fprintf(stderr, "FATAL: frame %d: decoded raster is not body[28:28+3456]\n", i);
			return 1;
		}
		frames[i].ref_count = ref_blobs_at_threshold(raster.samples, shid.c590_lut,
							     frames[i].ref, MAX_REF_BLOBS);
		for (r = 0; r < frames[i].ref_count; r++)
			if (frames[i].ref[r].size == 1) {
				n_onecell_blobs++;
				n_onecell_frames++;
			}
		ref_hist[frames[i].ref_count < 8 ? frames[i].ref_count : 8]++;
	}
	CHECK(n_frames > 0, "at least one frame decoded by the driver's decoder");

	/*
	 * Baseline: this capture has no 30-frame resting run and the baseline is
	 * per-cell max tracking, so the session's own blob-free frames are fed
	 * first as the probe-time resting window (order does not matter to a max
	 * tracker). Without a resting window the pipeline would be asked to
	 * acquire its baseline while a finger is already down, which is a
	 * different experiment.
	 */
	for (i = 0; i < n_frames; i++)
		if (frames[i].ref_count == 0)
			feed_body(frames[i].body);
	CHECK(shid.heatmap_have_baseline, "baseline established from the session's resting frames");
	CHECK(mt_record_active_count() == 0,
	      "no contacts published while priming from resting frames, got %d",
	      mt_record_active_count());

	printf("baseline primed from the session's blob-free frames; replaying all %d frames in order\n\n",
	       n_frames);
	if (sl4a_stub_verbose)
		sl4a_debug_level = 2;   /* --trace: the driver's own per-frame trace */
	printf("%-6s %-4s %-6s %-5s %-4s %-7s %-7s %s\n",
	       "frame", "ref", "touch", "peaks", "ccl", "sorted", "merged", "pub");

	for (i = 0; i < n_frames; i++) {
		memcpy(&pre_state, &shid, sizeof(pre_state));
		feed_body(frames[i].body);
		analyse_frame();
		print_frame_line(i);
		n_ref_all += frames[i].ref_count;
		n_pub_all += ff.published;
		if (frames[i].ref_count >= 3)
			n_ge3_frames++;
		if (ff.peaks == 0 && frames[i].ref_count > 0)
			n_peak_zero_frames++;
		report_frame(i);
	}

	printf("\n-- summary --\n");
	resolve_pending(n_frames);
	printf("frames                                  %d\n", n_frames);
	printf("reference blobs at 0.1 (total)          %d\n", n_ref_all);
	printf("published contacts (total)              %d\n", n_pub_all);
	printf("reference blob-count histogram          ");
	for (i = 0; i < 9; i++)
		printf("%d:%d%s", i, ref_hist[i], i == 8 ? "\n" : " ");
	printf("frames with reference >= 3              %d\n", n_ge3_frames);
	printf("  of those, publishing fewer contacts   %d (contacts lost in them: %d)\n",
	       n_short_ge3, lost_ge3);
	printf("frames publishing fewer contacts (any)  %d (contacts lost in them: %d)\n",
	       n_lost_frames, n_lost_all);
	printf("frames publishing MORE than the reference count %d (extra contacts: %d)\n",
	       n_extra_frames, n_extra_all);
	printf("reference blobs of a single cell          %d (in %d frames)\n",
	       n_onecell_blobs, n_onecell_frames);
	printf("frames where the peak detector found nothing at all %d\n", n_peak_zero_frames);
	printf("first stage that dropped a lost contact:\n");
	for (i = 0; i < n_tally; i++)
		printf("  %-42s %d\n", lost_tally[i].stage, lost_tally[i].count);
	printf("  of the debounce cases: %d publish within 3 frames (delayed), %d never do",
	       n_delayed, n_never);
	for (i = 0; i < n_pending; i++)
		if (!pending[i].delayed)
			printf(" f%03d(%d,%d)", pending[i].frame, pending[i].gx, pending[i].gy);
	printf("\n");
	printf("frames with a deficit (ref > published):\n");
	for (i = 0; i < n_lost_frames; i++)
		printf("  f%03d: ref=%d pub=%d (%d short)%s\n", lost_frame_idx[i],
		       frames[lost_frame_idx[i]].ref_count,
		       frames[lost_frame_idx[i]].ref_count - lost_frame_deficit[i],
		       lost_frame_deficit[i],
		       frames[lost_frame_idx[i]].ref_count >= 3 ? "  <-- reference >= 3" : "");

	printf("\nreal_frame_replay_test: %d assertions passed, %d failures\n", passed, failed);
	return failed != 0;
}
