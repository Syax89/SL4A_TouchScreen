/* SPDX-License-Identifier: GPL-2.0 */
#ifndef SPI_HID_CORE_H
#define SPI_HID_CORE_H

/*
 * Host-only stand-in for driver/spi-hid-core.h, used ONLY when building
 * driver/mshw0231-raw.c for the host replay test (tests/raw_pipeline_replay_test.c).
 *
 * driver/mshw0231-raw.c reaches this file via `#include "spi-hid-core.h"`
 * (quote form). The C standard's quote-include search order always
 * checks the directory of the *including* file first; that's why the
 * Makefile stages a copy of mshw0231-raw.c into a build directory that
 * does NOT contain the real spi-hid-core.h, then puts THIS directory
 * (raw-pipeline-stubs/) ahead of ../driver on the -I search path. The
 * real spi-hid-core.h drags in <linux/spi/spi.h>, <linux/pinctrl/consumer.h>,
 * <linux/completion.h> etc., none of which are needed for the touch
 * pipeline math this test exercises.
 *
 * struct spi_hid here defines EXACTLY the fields mshw0231-raw.c touches
 * (grep shid-> in driver/mshw0231-raw.c is the authoritative source),
 * copied with identical types/sizes from the real driver/spi-hid-core.h
 * so struct layout assumptions in the pipeline code hold on host too.
 * The HEATMAP_MAX_ and SLOT_HISTORY_DEPTH values below are copied from
 * the real header for the same reason: they are protocol-matching
 * sizing constants, not driver logic, and driver/ is never modified.
 */

#include <linux/types.h>
#include <stdbool.h>
#include <stdio.h>

#define HEATMAP_MAX_CELLS   4300
#define HEATMAP_MAX_BLOBS   20
#define HEATMAP_MAX_SLOTS   47   /* match Windows DLL blob slot count */
#define SLOT_HISTORY_DEPTH  10

/* Minimal device/spi_device stand-ins: mshw0231-raw.c only ever takes
 * &shid->spi->dev to hand to dev_warn()/dev_info(), both stubbed below
 * to no-ops (unless SL4A_TEST_VERBOSE_DEV is set), so no real fields
 * are needed. */
struct device {
	int _unused;
};

struct spi_device {
	struct device dev;
};

struct input_dev; /* real definition in linux/input.h */

/*
 * Verbosity flag for the dev_*() stubs below, defined in stubs.c.
 * Defaults to 0 (silent) so a passing test run isn't flooded with the
 * driver's own dev_info()/dev_warn() trace output; a replay test can
 * set it to 1 to see what the driver logged while debugging a failure.
 */
extern int sl4a_stub_verbose;

#define dev_warn(dev, fmt, ...) \
	do { (void)(dev); if (sl4a_stub_verbose) \
		fprintf(stderr, "[dev_warn] " fmt, ##__VA_ARGS__); } while (0)
#define dev_info(dev, fmt, ...) \
	do { (void)(dev); if (sl4a_stub_verbose) \
		fprintf(stderr, "[dev_info] " fmt, ##__VA_ARGS__); } while (0)
#define dev_dbg(dev, fmt, ...) \
	do { (void)(dev); if (sl4a_stub_verbose) \
		fprintf(stderr, "[dev_dbg] " fmt, ##__VA_ARGS__); } while (0)
#define dev_err(dev, fmt, ...) \
	do { (void)(dev); if (sl4a_stub_verbose) \
		fprintf(stderr, "[dev_err] " fmt, ##__VA_ARGS__); } while (0)

struct spi_hid {
	struct spi_device *spi;

	/* Multi-touch input device created for raw heatmap mode. */
	struct input_dev *touch_input;
	bool raw_mode_active;

	u8 *heatmap_buf;
	u32 heatmap_len;
	u32 heatmap_capacity;
	u32 heatmap_content_id;
	u16 heatmap_grid_cols;
	u16 heatmap_grid_rows;

	/* Per-device blob detection buffers. */
	u8  heatmap_baseline[HEATMAP_MAX_CELLS];
	bool heatmap_have_baseline;
	u32 heatmap_baseline_frames;
	u8  heatmap_touched[HEATMAP_MAX_CELLS];
	s16 heatmap_signal[HEATMAP_MAX_CELLS];
	u16 heatmap_label[HEATMAP_MAX_CELLS];
	u32 heatmap_queue[HEATMAP_MAX_CELLS];

	/* Blob state. Coordinates are fixed-point grid x100. */
	u32 blob_x[HEATMAP_MAX_BLOBS];
	u32 blob_y[HEATMAP_MAX_BLOBS];
	u32 blob_wsum[HEATMAP_MAX_BLOBS];
	bool blob_active[HEATMAP_MAX_BLOBS];
	s32 blob_eigmaj[HEATMAP_MAX_BLOBS];
	s32 blob_eigmin[HEATMAP_MAX_BLOBS];
	s32 blob_eigori[HEATMAP_MAX_BLOBS];
	u16 cost[HEATMAP_MAX_BLOBS][HEATMAP_MAX_SLOTS];

	/* Slot state, duration, and coordinate history. */
	u8 blob_slot_state[HEATMAP_MAX_SLOTS];
	u32 blob_slot_duration[HEATMAP_MAX_SLOTS];
	u32 blob_slot_gx[HEATMAP_MAX_SLOTS];
	u32 blob_slot_gy[HEATMAP_MAX_SLOTS];
	u32 blob_slot_weight[HEATMAP_MAX_SLOTS];
	u32 blob_slot_missed[HEATMAP_MAX_SLOTS];
	u8 blob_slot_stationary[HEATMAP_MAX_SLOTS];

	u32 blob_slot_hx[HEATMAP_MAX_SLOTS][SLOT_HISTORY_DEPTH];
	u32 blob_slot_hy[HEATMAP_MAX_SLOTS][SLOT_HISTORY_DEPTH];
	u8 blob_slot_hpos[HEATMAP_MAX_SLOTS];
	u8 blob_slot_hcount[HEATMAP_MAX_SLOTS];

	unsigned long heatmap_last_frame_jiffies;

	/* Eigenvalue/ellipsis tracking */
	s32 eigmaj[HEATMAP_MAX_SLOTS];
	s32 eigmin[HEATMAP_MAX_SLOTS];
	s32 eigori[HEATMAP_MAX_SLOTS];

	s16 c590_lut[256];
};

#endif
