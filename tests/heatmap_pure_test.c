/* SPDX-License-Identifier: GPL-2.0 */
/*
 * heatmap_pure_test — userspace heatmap regression test on the REAL
 * driver/mshw0231-raw.c + driver/spi-hid-capimg.c pipeline, compiled and
 * linked for host via tests/linux-compat/raw-pipeline-stubs/ (see
 * tests/Makefile's heatmap_pure_test rule, which mirrors
 * raw_pipeline_replay_test's staged-copy pattern).
 *
 * Unlike raw_pipeline_replay_test.c, which replays committed fixture
 * files, every raster here is generated in-code (pure synthetic), and
 * the capture-shape half reads the tracked V0 bodies straight from
 * captures/id5-20260718/frames/:
 *
 * Case A (synthetic): 30 baseline frames flip heatmap_have_baseline,
 * then 1-5 finger rasters report exactly that many MT slots.
 * Case B (capture shape): each raw-slot-*.v0 (4304B V0 0x0c) decodes to
 * a 3456-cell raster via spi_hid_capimg_decode_v0(), feeds through the
 * pipeline without crashing (ASan/UBSan clean), and tail bytes past
 * cell 3456 do not change the reported slot count.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spi-hid-core.h"
#include "mshw0231-raw.h"
#include "mshw0231-raw-constants.h"
#include "spi-hid-capimg.h"
#include <linux/input.h>
#include <linux/input/mt.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include "mt_record.h"

#define GRID_COLS 72
#define GRID_ROWS 48
#define FRAME_BYTES (GRID_COLS * GRID_ROWS)
#define BASELINE_BYTE 0xB4
#define V0_BODY_LEN 4304
#define CAPTURE_SLOTS 8
#define TAIL_EXTRA 128

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

/* ── Synthetic raster generation (mirrors
 * tests/fixtures/raw-replay/generate_fixtures.py) ── */

struct blob_cell {
	int dx, dy;
	unsigned char dip;
};

/* CENTER_DIP=53 (0x35): baseline 0xB4 -> centre 0x7F. Dip per offset is
 * round(53 * frac) with the fixture script's fractions (1.00 / 0.75 /
 * 0.55 / 0.25 / 0.12), precomputed as integers so no float is needed. */
static const struct blob_cell BLOB_SHAPE[] = {
	{ 0, 0, 53 },
	{ 1, 0, 40 }, { -1, 0, 40 }, { 0, 1, 40 }, { 0, -1, 40 },
	{ 1, 1, 29 }, { 1, -1, 29 }, { -1, 1, 29 }, { -1, -1, 29 },
	{ 2, 0, 13 }, { -2, 0, 13 }, { 0, 2, 13 }, { 0, -2, 13 },
	{ 2, 1, 6 }, { 2, -1, 6 }, { -2, 1, 6 }, { -2, -1, 6 },
	{ 1, 2, 6 }, { 1, -2, 6 }, { -1, 2, 6 }, { -1, -2, 6 },
};

static void new_baseline_frame(unsigned char *buf)
{
	memset(buf, BASELINE_BYTE, FRAME_BYTES);
}

static void stamp_blob(unsigned char *buf, int cx, int cy)
{
	size_t i;

	for (i = 0; i < sizeof(BLOB_SHAPE) / sizeof(BLOB_SHAPE[0]); i++) {
		int x = cx + BLOB_SHAPE[i].dx;
		int y = cy + BLOB_SHAPE[i].dy;
		unsigned int idx;
		unsigned char val;

		if (x < 0 || x >= GRID_COLS || y < 0 || y >= GRID_ROWS)
			continue;
		idx = (unsigned int)y * GRID_COLS + (unsigned int)x;
		val = (unsigned char)(BASELINE_BYTE - BLOB_SHAPE[i].dip);
		if (val < buf[idx])
			buf[idx] = val;
	}
}

/* Finger centres per count, same spread as the raw-replay fixtures. */
static const int FINGERS[5][5][2] = {
	{ { 20, 15 }, { 0, 0 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 5, 5 }, { 65, 42 }, { 0, 0 }, { 0, 0 }, { 0, 0 } },
	{ { 15, 10 }, { 36, 24 }, { 57, 38 }, { 0, 0 }, { 0, 0 } },
	{ { 10, 8 }, { 28, 20 }, { 46, 32 }, { 64, 44 }, { 0, 0 } },
	{ { 8, 6 }, { 22, 15 }, { 36, 24 }, { 50, 33 }, { 64, 42 } },
};

static void build_n_finger_frame(unsigned char *buf, int n)
{
	int f;

	new_baseline_frame(buf);
	for (f = 0; f < n; f++)
		stamp_blob(buf, FINGERS[n - 1][f][0], FINGERS[n - 1][f][1]);
}

/* ── Device setup, mirroring raw_pipeline_replay_test.c ── */

static void setup_device(struct spi_hid *shid, struct spi_device *spidev)
{
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
	CHECK(mt_init_slots_requested == HEATMAP_MAX_SLOTS,
	      "input_mt_init_slots() called with %u slots, expected %u",
	      mt_init_slots_requested, (unsigned)HEATMAP_MAX_SLOTS);
}

static void teardown_device(struct spi_hid *shid)
{
	input_free_device(shid->touch_input);
	kfree(shid->heatmap_buf);
}

static void feed_samples(struct spi_hid *shid, const unsigned char *buf,
			 unsigned int len)
{
	jiffies += 10;
	mshw0231_raw_consume_samples(shid, buf, len, 0x0C);
}

static void establish_baseline(struct spi_hid *shid)
{
	unsigned char buf[FRAME_BYTES];
	int i;

	new_baseline_frame(buf);
	for (i = 0; i < 30; i++)
		feed_samples(shid, buf, FRAME_BYTES);
	CHECK(shid->heatmap_have_baseline, "baseline established after 30 frames");
	CHECK(mt_record_active_count() == 0,
	      "no phantom touches from resting frames, got %d",
	      mt_record_active_count());
}

static void new_ready_device(struct spi_hid *shid, struct spi_device *spidev)
{
	mt_record_reset();
	setup_device(shid, spidev);
	establish_baseline(shid);
}

/* ── Case A: baseline gate ── */

static void test_baseline_gate(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	unsigned char buf[FRAME_BYTES];

	mt_record_reset();
	setup_device(&shid, &spidev);
	new_baseline_frame(buf);
	for (int i = 0; i < 29; i++)
		feed_samples(&shid, buf, FRAME_BYTES);
	CHECK(!shid.heatmap_have_baseline, "no baseline after 29 frames");
	feed_samples(&shid, buf, FRAME_BYTES);
	CHECK(shid.heatmap_have_baseline, "baseline established after 30 frames");
	CHECK(mt_record_active_count() == 0,
	      "no phantom touches from resting frames, got %d",
	      mt_record_active_count());
	teardown_device(&shid);
}

/* ── Case A: 1-5 synthetic fingers ── */

static void test_synthetic_n_finger(int n)
{
	struct spi_hid shid;
	struct spi_device spidev;
	unsigned char buf[FRAME_BYTES];
	int count;

	new_ready_device(&shid, &spidev);
	build_n_finger_frame(buf, n);
	for (int i = 0; i < 5; i++)
		feed_samples(&shid, buf, FRAME_BYTES);
	count = mt_record_active_count();
	CHECK(count == n, "%d synthetic finger(s): expected %d active slots, got %d",
	      n, n, count);
	printf("  synthetic %d-finger: %d active slot(s)\n", n, count);
	teardown_device(&shid);
}

/* ── Case B: capture shape ── */

static void load_v0_body(int slot, unsigned char *body)
{
	char path[128];
	FILE *f;
	size_t n;

	snprintf(path, sizeof(path),
		 "../captures/id5-20260718/frames/raw-slot-%d.v0", slot);
	f = fopen(path, "rb");
	if (!f) {
		fprintf(stderr, "FATAL: cannot open capture %s\n", path);
		exit(1);
	}
	n = fread(body, 1, V0_BODY_LEN, f);
	if (n != V0_BODY_LEN) {
		fprintf(stderr, "FATAL: capture %s is %zu bytes, expected %d\n",
			path, n, V0_BODY_LEN);
		fclose(f);
		exit(1);
	}
	if (fgetc(f) != EOF) {
		fprintf(stderr, "FATAL: capture %s has trailing bytes\n", path);
		fclose(f);
		exit(1);
	}
	fclose(f);
}

static void test_capture_shape(void)
{
	unsigned char body[V0_BODY_LEN];

	for (int s = 0; s < CAPTURE_SLOTS; s++) {
		struct spi_hid shid;
		struct spi_device spidev;
		struct spi_hid_capimg_raster raster = { NULL };
		int rc, count;

		load_v0_body(s, body);
		rc = spi_hid_capimg_decode_v0(body, sizeof(body), 3456, &raster);
		CHECK(rc == 0, "raw-slot-%d.v0 decodes with 3456 samples (rc=%d)",
		      s, rc);
		if (rc != 0)
			continue;
		CHECK(raster.samples != NULL, "raw-slot-%d.v0 raster is non-NULL", s);
		rc = spi_hid_capimg_decode_v0(body, sizeof(body), 3455, &raster);
		CHECK(rc != 0, "raw-slot-%d.v0 rejected with wrong count 3455", s);

		/* Pipeline must swallow the decoded frame without crashing;
		 * no slot-count assertion here — the captures are shape
		 * carriers, not known touch counts. */
		new_ready_device(&shid, &spidev);
		for (int i = 0; i < 4; i++) {
			jiffies += 10;
			mshw0231_raw_consume_v0(&shid, body, sizeof(body));
		}
		count = mt_record_active_count();
		printf("  raw-slot-%d.v0: decode ok, %d active slot(s) after feed\n",
		       s, count);
		teardown_device(&shid);
	}
}

/* Tail bytes past cell 3456 must not change what the pipeline reports. */
static void test_tail_bytes_ignored(void)
{
	unsigned char body[V0_BODY_LEN];
	unsigned char exact[FRAME_BYTES];
	unsigned char tailed[FRAME_BYTES + TAIL_EXTRA];

	for (int s = 0; s < CAPTURE_SLOTS; s++) {
		struct spi_hid shid_a, shid_b;
		struct spi_device dev_a, dev_b;
		struct spi_hid_capimg_raster raster = { NULL };
		int count_exact, count_tailed;

		load_v0_body(s, body);
		if (spi_hid_capimg_decode_v0(body, sizeof(body), 3456, &raster) != 0 ||
		    !raster.samples) {
			fprintf(stderr, "FATAL: raw-slot-%d.v0 failed to decode\n", s);
			exit(1);
		}
		memcpy(exact, raster.samples, FRAME_BYTES);
		memcpy(tailed, raster.samples, FRAME_BYTES);
		memset(tailed + FRAME_BYTES, 0xA5, TAIL_EXTRA);

		new_ready_device(&shid_a, &dev_a);
		for (int i = 0; i < 4; i++)
			feed_samples(&shid_a, exact, FRAME_BYTES);
		count_exact = mt_record_active_count();
		teardown_device(&shid_a);

		new_ready_device(&shid_b, &dev_b);
		for (int i = 0; i < 4; i++)
			feed_samples(&shid_b, tailed, FRAME_BYTES + TAIL_EXTRA);
		count_tailed = mt_record_active_count();
		CHECK(count_tailed == count_exact,
		      "raw-slot-%d.v0: tail bytes ignored (exact=%d tailed=%d)",
		      s, count_exact, count_tailed);
		teardown_device(&shid_b);
	}
}

int main(void)
{
	printf("heatmap_pure_test: running (real driver/mshw0231-raw.c)...\n");

	printf("-- baseline gate --\n");
	test_baseline_gate();

	for (int n = 1; n <= 5; n++) {
		printf("-- synthetic %d finger --\n", n);
		test_synthetic_n_finger(n);
	}

	printf("-- capture shape (V0 decodes to 3456) --\n");
	test_capture_shape();

	printf("-- tail bytes past cell 3456 ignored --\n");
	test_tail_bytes_ignored();

	printf("heatmap_pure_test: %d assertions passed, %d failures\n",
	       passed, failed);
	return failed != 0;
}
