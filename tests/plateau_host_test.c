// SPDX-License-Identifier: GPL-2.0
/*
 * plateau_host_test — equal-signal plateau anchoring and the zero-ellipse
 * clear, executed on the REAL driver code.
 *
 * Two properties of the raw pipeline that no other host test pins:
 *
 *  - a flat-topped plateau contributes exactly ONE peak, and its recorded
 *    position is re-anchored to the centre of the plateau's equal-signal
 *    region. The position matters: with the peak recorded at the region's
 *    raster-first corner, a 15-cell-wide saturated block's blob centroid sat
 *    7 cells from its only peak — more than HEATMAP_VELOCITY_REJECT_RADIUS —
 *    and raw_ccl_flood_fill() silently dropped the whole contact (measured:
 *    0 contacts tracked for a 15x15 block, 1 for every width <= 14; the
 *    complete-loss threshold was the same before the P6 tie-break). A
 *    non-convex region (a plus/cross shape, previously 2 surviving peaks)
 *    must likewise produce exactly one.
 *  - a slot whose blob carries eigmaj <= 0 (a split sub-blob, or a blob
 *    whose second moment came out zero) reports a zero ellipse, so a client
 *    cannot keep a stale ABS_MT_TOUCH_MAJOR/MINOR/ORIENTATION from an
 *    earlier frame — the stale oval that appeared exactly while two fingers
 *    converge and split apart.
 *
 * Includes the staged driver copy (see the Makefile rule) so the driver's
 * static stage functions are callable, like real_frame_replay_test.c.
 * Synthetic frames only — this test needs no corpus and runs in CI.
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

#include "raw-pipeline-stage/mshw0231-raw.c"

#define GRID_COLS 72
#define GRID_ROWS 48
#define FRAME_BYTES (GRID_COLS * GRID_ROWS)
#define GRID_CELLS FRAME_BYTES

static int failed;
static int passed;

#define CHECK(cond, msg, ...) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL: " msg "\n", ##__VA_ARGS__); \
		failed++; \
	} else { \
		passed++; \
	} \
} while (0)

static void setup_device(struct spi_hid *shid, struct spi_device *spidev)
{
	/* SL4 (MSHW0231) tuning: same values raw_pipeline_replay_test uses. */
	static const struct spi_hid_dev_cfg sl4_cfg = {
		.capimg_raster_samples   = 3456,
		.heatmap_baseline_needed = 30,
		.heatmap_baseline_alpha  = 7,
		.grid_cols               = 72,
		.grid_rows               = 48,
	};

	memset(shid, 0, sizeof(*shid));
	memset(spidev, 0, sizeof(*spidev));
	shid->spi = spidev;
	shid->raw_mode_active = true;
	shid->cfg = &sl4_cfg;

	mshw0231_raw_init(shid);
	if (mshw0231_raw_input_register(shid) != 0 || !shid->touch_input) {
		fprintf(stderr, "FATAL: mshw0231_raw_input_register() failed\n");
		exit(1);
	}
}

static void teardown_device(struct spi_hid *shid)
{
	input_free_device(shid->touch_input);
	kfree(shid->heatmap_buf);
}

static void feed_baseline(struct spi_hid *shid)
{
	static unsigned char buf[FRAME_BYTES];

	memset(buf, 200, sizeof(buf));
	for (int i = 0; i < 30; i++) {
		jiffies += 10;
		mshw0231_raw_consume_samples(shid, buf, FRAME_BYTES, 0x0C);
	}
	CHECK(shid->heatmap_have_baseline, "baseline established after 30 frames");
}

/* <frames> identical frames holding a saturated block: byte value 140 against
 * the 200 baseline, i.e. a flat equal-signal region with no taper at all. */
static void feed_block(struct spi_hid *shid, int w, int h, int r0, int c0,
		       int frames)
{
	static unsigned char buf[FRAME_BYTES];

	for (int f = 0; f < frames; f++) {
		memset(buf, 200, sizeof(buf));
		for (int r = r0; r < r0 + h; r++)
			for (int c = c0; c < c0 + w; c++)
				buf[r * GRID_COLS + c] = 140;
		jiffies += 10;
		mshw0231_raw_consume_samples(shid, buf, FRAME_BYTES, 0x0C);
	}
}

/* <frames> identical frames holding a plus/cross: horizontal bar on row r,
 * vertical bar on column c, both extended by `half` cells each way. */
static void feed_cross(struct spi_hid *shid, int r, int c, int half, int frames)
{
	static unsigned char buf[FRAME_BYTES];

	for (int f = 0; f < frames; f++) {
		memset(buf, 200, sizeof(buf));
		for (int i = c - half; i <= c + half; i++)
			buf[r * GRID_COLS + i] = 140;
		for (int i = r - half; i <= r + half; i++)
			buf[i * GRID_COLS + c] = 140;
		jiffies += 10;
		mshw0231_raw_consume_samples(shid, buf, FRAME_BYTES, 0x0C);
	}
}

/* Call the driver's peak detector on the heatmap the last frame left behind.
 * The pipeline's own CCL left labels in heatmap_label[], which
 * raw_detect_peaks() uses as scratch — zero it exactly like
 * mshw0231_raw_process_samples() does before its own call. */
static u8 detect_now(struct spi_hid *shid, u16 *pc, u16 *pr)
{
	memset(shid->heatmap_label, 0, GRID_CELLS * sizeof(shid->heatmap_label[0]));
	return raw_detect_peaks(shid, GRID_CELLS, GRID_COLS, GRID_ROWS, pc, pr);
}

static int cheb(int c1, int r1, int c2, int r2)
{
	int dc = c1 - c2;
	int dr = r1 - r2;

	if (dc < 0)
		dc = -dc;
	if (dr < 0)
		dr = -dr;
	return dc > dr ? dc : dr;
}

/* ── a wide saturated plateau is tracked, with its peak near the centre ── */

static void test_wide_plateau_tracked(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	u16 pc[HEATMAP_MAX_PEAKS], pr[HEATMAP_MAX_PEAKS];
	u8 np;

	setup_device(&shid, &spidev);
	feed_baseline(&shid);
	mt_record_reset();

	/* 15x15 saturated block at rows 10..24, cols 20..34; region centre
	 * (col 27, row 17). Before the re-anchor the only peak sat at the
	 * corner (20,10), Chebyshev distance 7 from the centroid — one cell
	 * over the reject radius — and this contact was dropped entirely. */
	feed_block(&shid, 15, 15, 10, 20, 6);

	CHECK(mt_record_active_count() == 1,
	      "15x15 plateau: tracked as 1 contact, got %d",
	      mt_record_active_count());

	np = detect_now(&shid, pc, pr);
	CHECK(np == 1, "15x15 plateau: exactly 1 peak, got %u", np);
	if (np >= 1) {
		CHECK(cheb(pc[0], pr[0], 27, 17) <= 1,
		      "15x15 plateau: peak (%u,%u) near the region centre (27,17)",
		      pc[0], pr[0]);
	}

	teardown_device(&shid);
}

/* ── a non-convex equal-signal region is still exactly one peak ── */

static void test_cross_one_peak(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	u16 pc[HEATMAP_MAX_PEAKS], pr[HEATMAP_MAX_PEAKS];
	u8 np;

	setup_device(&shid, &spidev);
	feed_baseline(&shid);
	mt_record_reset();

	/* Plus shape: crossing at (col 30, row 24); arms 5 cells each way.
	 * The raster tie-break alone left TWO survivors here (the far ends of
	 * the two bars), which the region claim must collapse into one. */
	feed_cross(&shid, 24, 30, 5, 6);

	np = detect_now(&shid, pc, pr);
	CHECK(np == 1, "cross plateau: exactly 1 peak, got %u", np);
	if (np >= 1) {
		CHECK(cheb(pc[0], pr[0], 30, 24) <= 1,
		      "cross plateau: peak (%u,%u) near the crossing (30,24)",
		      pc[0], pr[0]);
	}
	CHECK(mt_record_active_count() == 1,
	      "cross plateau: tracked as 1 contact, got %d",
	      mt_record_active_count());

	teardown_device(&shid);
}

/* ── control: a small plateau keeps its centre peak and stays tracked ── */

static void test_small_plateau_control(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	u16 pc[HEATMAP_MAX_PEAKS], pr[HEATMAP_MAX_PEAKS];
	u8 np;

	setup_device(&shid, &spidev);
	feed_baseline(&shid);
	mt_record_reset();

	/* 6x6 block at rows 10..15, cols 20..25; centre (col 22, row 12). */
	feed_block(&shid, 6, 6, 10, 20, 6);

	CHECK(mt_record_active_count() == 1,
	      "6x6 plateau: tracked as 1 contact, got %d",
	      mt_record_active_count());

	np = detect_now(&shid, pc, pr);
	CHECK(np == 1, "6x6 plateau: exactly 1 peak, got %u", np);
	if (np >= 1) {
		CHECK(cheb(pc[0], pr[0], 22, 12) <= 1,
		      "6x6 plateau: peak (%u,%u) near the region centre (22,12)",
		      pc[0], pr[0]);
	}

	teardown_device(&shid);
}

/* ── a zero-ellipse slot clears the client's ABS instead of leaving it ── */

static void test_zero_ellipse_clears(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	u32 gx[HEATMAP_MAX_SLOTS] = { 0 }, gy[HEATMAP_MAX_SLOTS] = { 0 };
	bool act[HEATMAP_MAX_SLOTS] = { false };

	setup_device(&shid, &spidev);
	feed_baseline(&shid);
	mt_record_reset();

	act[0] = true;
	gx[0] = 2000;
	gy[0] = 3000;

	/* A slot with a real ellipse: reported. */
	shid.eigmaj[0] = 10000;
	shid.eigmin[0] = 4000;
	shid.eigori[0] = 4500;
	raw_emit_mt(&shid, shid.touch_input, act, gx, gy,
		    461507, 697170, 32767, 0, 0, false, false, false);
	CHECK(mt_slots[0].major > 0 && mt_slots[0].minor > 0,
	      "ellipse frame: reported (major=%d minor=%d)",
	      mt_slots[0].major, mt_slots[0].minor);

	/* Same slot, zero ellipse (what a split sub-blob carries): the zero
	 * must reach the client. Before the else-branch, this frame skipped
	 * the reports and the client kept 32767/32767/45. */
	shid.eigmaj[0] = 0;
	shid.eigmin[0] = 0;
	shid.eigori[0] = 0;
	raw_emit_mt(&shid, shid.touch_input, act, gx, gy,
		    461507, 697170, 32767, 0, 0, false, false, false);
	CHECK(mt_slots[0].major == 0 && mt_slots[0].minor == 0 && mt_slots[0].ori == 0,
	      "zero-ellipse frame: cleared (major=%d minor=%d ori=%d)",
	      mt_slots[0].major, mt_slots[0].minor, mt_slots[0].ori);

	/* And a real ellipse again: not a latch. */
	shid.eigmaj[0] = 10000;
	shid.eigmin[0] = 4000;
	shid.eigori[0] = 4500;
	raw_emit_mt(&shid, shid.touch_input, act, gx, gy,
		    461507, 697170, 32767, 0, 0, false, false, false);
	CHECK(mt_slots[0].major > 0,
	      "ellipse frame after the clear: reported again (major=%d)",
	      mt_slots[0].major);

	teardown_device(&shid);
}

int main(void)
{
	printf("plateau_host_test: running (real driver/mshw0231-raw.c)...\n");

	printf("-- wide saturated plateau is tracked --\n");
	test_wide_plateau_tracked();

	printf("-- cross-shaped equal-signal region: one peak --\n");
	test_cross_one_peak();

	printf("-- small plateau control --\n");
	test_small_plateau_control();

	printf("-- zero-ellipse slot clears the client --\n");
	test_zero_ellipse_clears();

	printf("plateau_host_test: %d assertions passed, %d failures\n",
	       passed, failed);
	return failed != 0;
}
