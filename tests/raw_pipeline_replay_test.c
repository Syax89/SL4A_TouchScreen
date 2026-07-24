/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Replays saved synthetic heatmap frames (tests/fixtures/raw-replay/)
 * through the REAL driver/mshw0231-raw.c pipeline, compiled and linked
 * for host via tests/linux-compat/raw-pipeline-stubs/ (see tests/Makefile's
 * raw_pipeline_replay_test rule for exactly how spi-hid-core.h is
 * shadowed). Unlike raw_pipeline_math_test.c, which checks small
 * hand-written mirrors of the pipeline's math in isolation, this test
 * calls the actual mshw0231_raw_init()/mshw0231_raw_consume_samples()
 * entry points and asserts on what the real pipeline reports through
 * the linux/input.h + linux/input/mt.h stubs (see mt_record.h) — the
 * same slot/position/BTN_TOUCH state a real evdev listener would see.
 *
 * Each scenario below gets its own freshly-initialized struct spi_hid
 * and its own freshly-established 30-frame baseline, exactly like a
 * real device would have right after probe — this keeps scenarios
 * independent of each other's slot lift/hold timing instead of
 * chaining raw frame counts across unrelated fixtures.
 *
 * Key question this test answers: given a clean, well-separated
 * synthetic 2-blob signal (bypassing any physical touch-sensing
 * question entirely), does the real pipeline logic detect and track
 * both fingers? Answer: YES for 2 simultaneous well-separated blobs
 * (both the far-apart and close-together fixtures report exactly 2
 * active MT slots at the right positions).
 *
 * FIXED BUG found by this harness, affecting 3+ simultaneous blobs:
 * raw_detect_peaks() used to compare each candidate cell against only
 * 4 fixed points at exactly HEATMAP_PEAK_RADIUS cells away (one probe
 * per axis) instead of scanning its full neighborhood. A real finger
 * blob tapers smoothly across several concentric rings of decreasing
 * signal, so a single-point probe at one exact distance almost always
 * landed either outside the blob (untouched, trivially passed) or on
 * a same-signal ring cell (not *strictly* greater, so also passed) —
 * every touched cell of the blob (not just its true center)
 * independently qualified as a "peak", ~13 per blob on this hardware's
 * ~2-cell-radius fingertip contacts. Since raw_detect_peaks() shares
 * one global HEATMAP_MAX_PEAKS=16 budget across the whole frame and
 * scans in raster/row-major order, 2 blobs already exhausted it
 * partway through the second one, so any 3rd+ blob's row range was
 * never reached by the scan; its CCL component was still found (peak
 * detection doesn't gate CCL directly), but raw_ccl_flood_fill()'s
 * velocity-rejection check requires >=1 recorded peak within
 * HEATMAP_VELOCITY_REJECT_RADIUS=6 of the blob's centroid, so it was
 * silently discarded despite valid pixel count and weight. Fixed by
 * replacing the 4-point probe with a true local-maximum scan over the
 * full neighborhood (see the HEATMAP_PEAK_RADIUS comment in
 * driver/mshw0231-raw-constants.h) — the blob's true center, being
 * strictly higher signal than every other cell in the blob, always
 * falls within radius of any of its own cells, so only the center
 * now passes regardless of taper shape. This was a pipeline-logic
 * bug, fully independent of the physical/sensing question a live
 * 2-finger touch on real hardware raised in this same session: it
 * reproduced with synthetic frames that bypass the sensor entirely.
 * test_n_finger() below asserts normally (CHECK()) that 3/4/5-finger
 * fixtures now report the correct slot count — a regression here
 * would fail `make test`, not be silently tolerated.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spi-hid-core.h"
#include "mshw0231-raw.h"
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include "mt_record.h"

#define FIXTURE_DIR "fixtures/raw-replay/"
#define GRID_COLS 72
#define GRID_ROWS 48
#define FRAME_BYTES (GRID_COLS * GRID_ROWS)

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

/* ── Fixture loading ─────────────────────────────────────────────── */

static void load_fixture(const char *name, unsigned char *buf)
{
	char path[512];
	FILE *f;
	size_t n;

	snprintf(path, sizeof(path), FIXTURE_DIR "%s", name);
	f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "FATAL: cannot open fixture %s\n", path);
		exit(1);
	}
	n = fread(buf, 1, FRAME_BYTES, f);
	fclose(f);
	if (n != FRAME_BYTES) {
		fprintf(stderr, "FATAL: fixture %s is %zu bytes, expected %d\n",
			path, n, FRAME_BYTES);
		exit(1);
	}
}

/* Feeds one fixture frame through the real pipeline entry point,
 * advancing the host jiffies clock by 10 (10ms @ ~100Hz, well under
 * the 60ms HEATMAP_MISSED_FRAME_TIMEOUT_MS the driver uses to detect a
 * stream gap and release all slots). */
static void feed_frame(struct spi_hid *shid, const char *fixture_name)
{
	unsigned char buf[FRAME_BYTES];

	load_fixture(fixture_name, buf);
	jiffies += 10;
	mshw0231_raw_consume_samples(shid, buf, FRAME_BYTES, 0x0C);
}

/* ── Device setup, mirroring what the real driver does at probe time ── */

static void setup_device(struct spi_hid *shid, struct spi_device *spidev)
{
	memset(shid, 0, sizeof(*shid));
	memset(spidev, 0, sizeof(*spidev));
	shid->spi = spidev;
	shid->raw_mode_active = true;

	mshw0231_raw_init(shid);
	if (mshw0231_raw_input_register(shid) != 0 || !shid->touch_input) {
		fprintf(stderr, "FATAL: mshw0231_raw_input_register() failed\n");
		exit(1);
	}
	CHECK(mt_init_slots_requested == HEATMAP_MAX_SLOTS,
	      "input_mt_init_slots() called with %u slots, expected %u",
	      mt_init_slots_requested, (unsigned)HEATMAP_MAX_SLOTS);
}

static void establish_baseline(struct spi_hid *shid)
{
	char name[64];

	CHECK(!shid->heatmap_have_baseline, "baseline not yet established before feeding frames");
	for (int i = 0; i < 30; i++) {
		snprintf(name, sizeof(name), "baseline_%02d.bin", i);
		feed_frame(shid, name);
	}
	CHECK(shid->heatmap_have_baseline, "baseline established after 30 baseline frames");
	CHECK(mt_record_active_count() == 0, "no phantom touches reported from resting frames, got %d",
	      mt_record_active_count());
	CHECK(mt_btn_touch == 0, "BTN_TOUCH not set after resting frames");
}

/* Fresh device + fresh baseline + fresh MT recording state, exactly as
 * if this were a brand new probe. Every scenario calls this first so
 * scenarios never depend on each other's slot lift/hold timing. */
static void new_ready_device(struct spi_hid *shid, struct spi_device *spidev)
{
	mt_record_reset();
	setup_device(shid, spidev);
	establish_baseline(shid);
}

/* Mirrors what a real driver unbind/remove would release: the input
 * device from input_allocate_device() and the debug snapshot buffer
 * kmalloc()'d by mshw0231_raw_process_samples(). Each scenario is a
 * fresh stack-local struct spi_hid, so without this its heap
 * allocations are simply lost when the function returns. */
static void teardown_device(struct spi_hid *shid)
{
	input_free_device(shid->touch_input);
	kfree(shid->heatmap_buf);
}

/* ── Scenario: single well-isolated finger ───────────────────────── */

static void test_single_touch(void)
{
	struct spi_hid shid;
	struct spi_device spidev;

	new_ready_device(&shid, &spidev);

	/* Debounce: blob_debounce=3 frames in state "new" before a slot is
	 * reported as claimed/active (see raw_update_slots() case 0/1).
	 * Feed the same contact a few times to let it debounce in, exactly
	 * like a real held finger would appear across frames. */
	for (int i = 0; i < 4; i++)
		feed_frame(&shid, "touch_1finger.bin");

	int n = mt_record_active_count();
	CHECK(n == 1, "single-touch fixture: expected 1 active MT slot, got %d", n);
	CHECK(mt_btn_touch == 1, "single-touch fixture: BTN_TOUCH should be set");

	/* Grid (20,15) on a 72-wide grid, screen_max=32767, scale_x =
	 * 32767000/(72-1) -> expected screen X centered around
	 * 20/71 * 32767 ~= 9230, Y centered around 15/47 * 32767 ~= 10457.
	 * Generous tolerance: EMA/deadband and blob-shape centroid shift
	 * can move this a bit from the exact grid coordinate. */
	for (int i = 0; i < MT_RECORD_MAX_SLOTS; i++) {
		if (!mt_slots[i].active)
			continue;
		CHECK(mt_slots[i].x > 7000 && mt_slots[i].x < 11500,
		      "single-touch slot %d x=%d out of expected range (~9230)", i, mt_slots[i].x);
		CHECK(mt_slots[i].y > 8500 && mt_slots[i].y < 12500,
		      "single-touch slot %d y=%d out of expected range (~10457)", i, mt_slots[i].y);
	}
	printf("  single-touch: %d active slot(s)\n", n);
	teardown_device(&shid);
}

/* ── Scenario: 2 fingers, far apart ──────────────────────────────── */

/* Note on the KNOWN BUG documented at the top of this file: 2 blobs
 * still exceed the 16-peak budget too (~13 peaks/blob here), but the
 * second blob still gets a partial handful of its own peaks recorded
 * before the scan hits the cap, and any single peak within
 * HEATMAP_VELOCITY_REJECT_RADIUS=6 of its own centroid is enough to
 * save it — trivially satisfied since this blob's own footprint is
 * only ~2 cells in radius. That's why 2 fingers passes while 3+ does
 * not: it isn't that 2-blob input avoids the bug, it's that 2-blob
 * input is small enough to survive it by luck of raster scan order. */
static void test_two_finger_far(void)
{
	struct spi_hid shid;
	struct spi_device spidev;

	new_ready_device(&shid, &spidev);

	for (int i = 0; i < 5; i++)
		feed_frame(&shid, "touch_2finger_far.bin");

	int n = mt_record_active_count();
	CHECK(n == 2, "far-apart 2-finger fixture: expected 2 active MT slots, got %d", n);
	printf("  two-finger (far apart): %d active slot(s)\n", n);
	for (int i = 0; i < MT_RECORD_MAX_SLOTS; i++)
		if (mt_slots[i].active)
			printf("    slot %d: x=%d y=%d major=%d minor=%d ori=%d\n",
			       i, mt_slots[i].x, mt_slots[i].y,
			       mt_slots[i].major, mt_slots[i].minor, mt_slots[i].ori);
	teardown_device(&shid);
}

/* ── Scenario: 2 fingers, close together (~9 cells) ──────────────── */

static void test_two_finger_close(void)
{
	struct spi_hid shid;
	struct spi_device spidev;

	new_ready_device(&shid, &spidev);

	for (int i = 0; i < 5; i++)
		feed_frame(&shid, "touch_2finger_close.bin");

	int n = mt_record_active_count();
	CHECK(n == 2, "close-together 2-finger fixture: expected 2 active MT slots "
	      "(ghost-merge must NOT collapse a realistic adjacent-finger gesture), got %d", n);
	printf("  two-finger (close, ~9 cells): %d active slot(s)\n", n);
	for (int i = 0; i < MT_RECORD_MAX_SLOTS; i++)
		if (mt_slots[i].active)
			printf("    slot %d: x=%d y=%d\n", i, mt_slots[i].x, mt_slots[i].y);
	teardown_device(&shid);
}

/* ── Scenario: 3/4/5 fingers ──────────────────────────────────────── */

/* See the FIXED BUG block comment at the top of this file: 3+
 * simultaneous well-separated finger-sized blobs used to exhaust
 * raw_detect_peaks()'s shared HEATMAP_MAX_PEAKS=16 budget before the
 * scan (which runs in raster/row-major order) ever reached the later
 * blob(s), silently dropping them at the CCL near-peak check. Fixed
 * by the local-maximum neighborhood scan in raw_detect_peaks() — this
 * now asserts normally, so a regression fails `make test`. */
static void test_n_finger(const char *fixture, int expected)
{
	struct spi_hid shid;
	struct spi_device spidev;

	new_ready_device(&shid, &spidev);

	for (int i = 0; i < 5; i++)
		feed_frame(&shid, fixture);

	int n = mt_record_active_count();
	CHECK(n == expected, "%s: expected %d active MT slots, got %d",
	      fixture, expected, n);
	printf("  %s: %d active slot(s) (expected %d)\n", fixture, n, expected);
	teardown_device(&shid);
}

/* ── Scenario: hold-and-move sequence ─────────────────────────────── */

static void test_hold_and_move(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	char name[64];
	int prev_x = -1, prev_y = -1;
	int jumps = 0;
	int saw_active = 0;

	new_ready_device(&shid, &spidev);

	for (int i = 0; i < 16; i++) {
		snprintf(name, sizeof(name), "move_%02d.bin", i);
		feed_frame(&shid, name);

		int n = mt_record_active_count();
		if (n != 1) {
			/* Debounce window: the first blob_debounce-1 frames are
			 * expected to report 0 active slots (state new, not yet
			 * claimed). Anything else past that window is worth
			 * flagging. */
			if (i >= 3)
				CHECK(n == 1, "move sequence frame %d: expected 1 active slot, got %d", i, n);
			continue;
		}
		saw_active = 1;
		for (int s = 0; s < MT_RECORD_MAX_SLOTS; s++) {
			if (!mt_slots[s].active)
				continue;
			int x = mt_slots[s].x, y = mt_slots[s].y;
			if (prev_x >= 0) {
				int dx = x - prev_x, dy = y - prev_y;
				if (dx < 0) dx = -dx;
				if (dy < 0) dy = -dy;
				/* One grid cell of movement per frame is roughly
				 * scale_x/71 ~= 461 screen units in X
				 * (32767000/71/1000). Allow generous slack for EMA
				 * lag but catch a genuine erratic jump (teleporting
				 * across the pad) or a frozen/stuck position. */
				if (dx > 2000 || dy > 2000)
					jumps++;
			}
			prev_x = x;
			prev_y = y;
		}
	}
	CHECK(saw_active, "move sequence: slot became active at some point");
	CHECK(jumps == 0, "move sequence: no erratic position jumps (>2000 units/frame), saw %d", jumps);
	printf("  hold-and-move: final position x=%d y=%d, erratic jumps=%d\n", prev_x, prev_y, jumps);
	teardown_device(&shid);
}

int main(void)
{
	printf("raw_pipeline_replay_test: running (real driver/mshw0231-raw.c)...\n");

	printf("-- single touch --\n");
	test_single_touch();

	printf("-- two finger, far apart (opposite corners) --\n");
	test_two_finger_far();

	printf("-- two finger, close together --\n");
	test_two_finger_close();

	printf("-- three finger --\n");
	test_n_finger("touch_3finger.bin", 3);

	printf("-- four finger --\n");
	test_n_finger("touch_4finger.bin", 4);

	printf("-- five finger --\n");
	test_n_finger("touch_5finger.bin", 5);

	printf("-- hold and move --\n");
	test_hold_and_move();

	printf("raw_pipeline_replay_test: %d assertions passed, %d failures\n",
	       passed, failed);
	return failed != 0;
}
