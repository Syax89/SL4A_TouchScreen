// SPDX-License-Identifier: GPL-2.0
/*
 * Focused host regression for association-before-coalescing.
 * Includes the staged real driver source so the static policy function is
 * exercised directly.
 */
#include <stdio.h>
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

static int failed;

#define CHECK(cond, msg) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL: %s\n", msg); \
		failed++; \
	} \
} while (0)

static struct blob_entry blob(u32 gx, u32 gy, u32 raw_w)
{
	struct blob_entry b = {
		.gx = gx,
		.gy = gy,
		.w = raw_w,
		.raw_w = raw_w,
		.idx = 0,
	};
	return b;
}

static void two_established_close_survive(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 0, 1 };

	memset(&shid, 0, sizeof(shid));
	shid.blob_slot_state[0] = 2;
	shid.blob_slot_state[1] = 2;
	blobs[0] = blob(1000, 1000, 5000);
	blobs[1] = blob(1490, 1000, 4500); /* 4.9 cells */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 0 && assigned[1] == 1,
	      "two established close tracks must both survive");
}

static void lift_pending_continuity_survives(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 3, 7 };

	memset(&shid, 0, sizeof(shid));
	shid.blob_slot_state[3] = 2;
	shid.blob_slot_state[7] = 3;
	blobs[0] = blob(2000, 2000, 5200);
	blobs[1] = blob(2500, 2000, 5100); /* 5 cells */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 3 && assigned[1] == 7,
	      "state-2 + state-3 continuity must survive proximity suppression");
}

static void established_beats_new_duplicate(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 4, 9 };

	memset(&shid, 0, sizeof(shid));
	shid.blob_slot_state[4] = 2;
	shid.blob_slot_state[9] = 0;
	blobs[0] = blob(3000, 3000, 4000);
	blobs[1] = blob(3450, 3000, 9000); /* heavier new candidate */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 4,
	      "established track must survive a close new candidate");
	CHECK(assigned[1] == 0xff,
	      "close new candidate must not steal established continuity");
}

static void ambiguous_new_pair_uses_raw_weight(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 2, 8 };

	memset(&shid, 0, sizeof(shid));
	blobs[0] = blob(4000, 4000, 7000);
	blobs[1] = blob(4450, 4000, 3000);

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 2,
	      "stronger ambiguous new candidate must survive");
	CHECK(assigned[1] == 0xff,
	      "weaker ambiguous new candidate must be suppressed");
}

static void exact_boundary_is_not_coalesced(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 0, 1 };

	memset(&shid, 0, sizeof(shid));
	blobs[0] = blob(1000, 1000, 5000);
	blobs[1] = blob(1600, 1000, 4000); /* exactly 6 cells */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 0 && assigned[1] == 1,
	      "exact ghost_dist boundary must remain strict");
}


static void winner_at_b_suppresses_loser_at_a(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 2, 8 };

	memset(&shid, 0, sizeof(shid));
	blobs[0] = blob(4000, 4000, 3000);
	blobs[1] = blob(4450, 4000, 7000);

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned,
				3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 0xff && assigned[1] == 8,
	      "winner-at-b / loser-at-a branch must preserve the stronger candidate");
}

static void rejected_assignment_is_ignored(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 0xff, 8 };

	memset(&shid, 0, sizeof(shid));
	blobs[0] = blob(4000, 4000, 9000);
	blobs[1] = blob(4200, 4000, 3000);

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned,
				3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 0xff && assigned[1] == 8,
	      "0xff rejected assignments must stay outside coalescing");
}

static void raw_weight_not_penalized_weight_decides_duplicate(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 2, 8 };

	memset(&shid, 0, sizeof(shid));
	blobs[0] = blob(4000, 4000, 7000);
	blobs[1] = blob(4450, 4000, 3000);
	blobs[0].w = 1000; /* lower edge-penalised weight */
	blobs[1].w = 9000; /* higher edge-penalised weight */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned,
				3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 2 && assigned[1] == 0xff,
	      "duplicate choice must use pre-penalty raw_w, not penalized w");
}

static void three_track_ghost_radius_scaling_is_preserved(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 3, 4 };

	memset(&shid, 0, sizeof(shid));
	/* Three established slots shrink ghost radius from 6 to 4 cells
	 * (6 * GHOST_RADIUS_3_FINGERS / 10 = 4). */
	shid.blob_slot_state[0] = 2;
	shid.blob_slot_state[1] = 2;
	shid.blob_slot_state[2] = 2;
	blobs[0] = blob(1000, 1000, 7000);
	blobs[1] = blob(1450, 1000, 3000); /* 4.5 cells: inside 6, outside scaled 4 */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned,
				3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 3 && assigned[1] == 4,
	      "three-track ghost-radius scaling must remain narrower than base radius");
}

static void provisional_survivor_keeps_state1_history(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 0, 1 };

	memset(&shid, 0, sizeof(shid));
	/* Weaker A owns the existing provisional slot; stronger B was assigned
	 * to a fresh slot. B is still within the normal 3-cell Hungarian range
	 * of A's previous position, so keeping B must retain slot 0 history. */
	shid.blob_slot_state[0] = 1;
	shid.blob_slot_gx[0] = 2000;
	shid.blob_slot_gy[0] = 2000;
	shid.blob_slot_duration[0] = 1;
	blobs[0] = blob(2100, 2000, 5000);
	blobs[1] = blob(2300, 2000, 6000);

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned,
				3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 0xff && assigned[1] == 0,
	      "strong survivor must inherit valid state-1 provisional assignment");
}

static void provisional_rehome_requires_hungarian_range(void)
{
	struct spi_hid shid;
	struct blob_entry blobs[2];
	u8 assigned[2] = { 0, 1 };

	memset(&shid, 0, sizeof(shid));
	shid.blob_slot_state[0] = 1;
	shid.blob_slot_gx[0] = 1000;
	shid.blob_slot_gy[0] = 1000;
	blobs[0] = blob(1100, 1000, 5000);
	blobs[1] = blob(1450, 1000, 6000); /* 4.5 cells from prior provisional position */

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned,
				3 * HUNGARIAN_COST_SCALE, 6);

	CHECK(assigned[0] == 0xff && assigned[1] == 1,
	      "out-of-range survivor must not steal provisional history");
}

#define GRID_COLS 72
#define GRID_ROWS 48
#define GRID_CELLS (GRID_COLS * GRID_ROWS)

static const struct spi_hid_dev_cfg sl4_cfg = {
	.capimg_raster_samples = GRID_CELLS,
	.heatmap_baseline_needed = 30,
	.heatmap_baseline_alpha = 7,
	.grid_cols = GRID_COLS,
	.grid_rows = GRID_ROWS,
};

static void provisional_full_pipeline_regression(void)
{
	struct spi_hid shid;
	struct spi_device spidev;
	unsigned char frame[GRID_CELLS];
	int f, r, c, active_after_debounce = 0;
	int max_duration = 0;

	memset(&shid, 0, sizeof(shid));
	memset(&spidev, 0, sizeof(spidev));
	shid.spi = &spidev;
	shid.cfg = &sl4_cfg;
	shid.raw_mode_active = true;
	mshw0231_raw_init(&shid);
	CHECK(mshw0231_raw_input_register(&shid) == 0 && shid.touch_input,
	      "full-pipeline setup must register the MT test device");
	if (!shid.touch_input)
		return;

	mt_record_reset();
	memset(frame, 220, sizeof(frame));
	for (f = 0; f < 30; f++) {
		jiffies += 10;
		mshw0231_raw_consume_samples(&shid, frame, GRID_CELLS, 0x0c);
	}
	CHECK(shid.heatmap_have_baseline,
	      "full-pipeline reproduction must establish its 30-frame baseline");

	/* Exact review reproduction: two disconnected 3x5 rectangles, centers
	 * four cells apart, both moving three cells/frame. Strong=40, weak=80. */
	for (f = 0; f < 6; f++) {
		int strong = 20 + 3 * f;
		int weak = 16 + 3 * f;

		memset(frame, 220, sizeof(frame));
		for (r = 18; r <= 22; r++) {
			for (c = weak - 1; c <= weak + 1; c++)
				frame[r * GRID_COLS + c] = 80;
			for (c = strong - 1; c <= strong + 1; c++)
				frame[r * GRID_COLS + c] = 40;
		}

		jiffies += 10;
		mshw0231_raw_consume_samples(&shid, frame, GRID_CELLS, 0x0c);
		if (f >= 2 && mt_record_active_count() == 1)
			active_after_debounce++;
	}

	for (c = 0; c < HEATMAP_MAX_SLOTS; c++)
		if ((int)shid.blob_slot_duration[c] > max_duration)
			max_duration = (int)shid.blob_slot_duration[c];

	CHECK(active_after_debounce == 4,
	      "moving provisional contact must publish from frame 2 through frame 5");
	CHECK(max_duration >= 6,
	      "surviving contact must retain provisional duration instead of restarting debounce");

	input_free_device(shid.touch_input);
	/* Production remove frees this; the isolated test fixture must too. */
	kfree(shid.heatmap_buf);
}

int main(void)
{
	two_established_close_survive();
	lift_pending_continuity_survives();
	established_beats_new_duplicate();
	ambiguous_new_pair_uses_raw_weight();
	exact_boundary_is_not_coalesced();
	winner_at_b_suppresses_loser_at_a();
	rejected_assignment_is_ignored();
	raw_weight_not_penalized_weight_decides_duplicate();
	three_track_ghost_radius_scaling_is_preserved();
	provisional_survivor_keeps_state1_history();
	provisional_rehome_requires_hungarian_range();
	provisional_full_pipeline_regression();

	if (failed)
		return 1;

	puts("tracker association/coalescing host test: PASS");
	return 0;
}
