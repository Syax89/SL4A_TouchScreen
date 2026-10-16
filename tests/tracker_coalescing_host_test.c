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

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 6);

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

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 6);

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

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 6);

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

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 6);

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

	raw_post_assoc_coalesce(&shid, blobs, 2, assigned, 6);

	CHECK(assigned[0] == 0 && assigned[1] == 1,
	      "exact ghost_dist boundary must remain strict");
}

int main(void)
{
	two_established_close_survive();
	lift_pending_continuity_survives();
	established_beats_new_duplicate();
	ambiguous_new_pair_uses_raw_weight();
	exact_boundary_is_not_coalesced();

	if (failed)
		return 1;

	puts("tracker association/coalescing host test: PASS");
	return 0;
}
