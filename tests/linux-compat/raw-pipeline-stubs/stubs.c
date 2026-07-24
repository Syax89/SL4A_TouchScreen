/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Implementations backing the linux/input.h + linux/input/mt.h stub
 * declarations, plus the few plain globals driver/mshw0231-raw.c
 * expects to find with external linkage (sl4a_debug_level, jiffies).
 *
 * These are the ONLY pieces of the harness that fake device behaviour;
 * everything else here just records what the real pipeline decided to
 * report, for tests/raw_pipeline_replay_test.c to inspect afterward.
 */
#include <linux/input.h>
#include <linux/input/mt.h>
#include <stdlib.h>
#include <string.h>

#include "mt_record.h"

/* extern in driver/mshw0231-raw.c: `extern int sl4a_debug_level;` used
 * by its seq_dbg() macro. Not module-loadable here, so just a plain
 * global a test can flip to 1+ to see the driver's own trace output
 * (routed through dev_info(), which is gated by sl4a_stub_verbose,
 * declared in the stub spi-hid-core.h). */
int sl4a_debug_level;

/* Gates the dev_warn()/dev_info()/dev_dbg()/dev_err() stub macros in
 * raw-pipeline-stubs/spi-hid-core.h. Off by default. */
int sl4a_stub_verbose;

/* Host stub clock backing linux/jiffies.h; 1 jiffy == 1 ms. Starts at 1
 * (not 0) so mshw0231_raw_process_samples()'s
 * `if (shid->heatmap_last_frame_jiffies && ...)` guard — which treats 0
 * as "no frame processed yet" — is never confused by a real elapsed-time
 * value that happens to be 0. */
unsigned long jiffies = 1;

/* ── MT slot recording ───────────────────────────────────────────── */

struct mt_slot_record mt_slots[MT_RECORD_MAX_SLOTS];
int mt_btn_touch;
unsigned long mt_sync_count;
unsigned int mt_init_slots_requested;

static int current_slot = -1;

int mt_record_active_count(void)
{
	int n = 0;

	for (int i = 0; i < MT_RECORD_MAX_SLOTS; i++)
		if (mt_slots[i].active)
			n++;
	return n;
}

void mt_record_reset(void)
{
	memset(mt_slots, 0, sizeof(mt_slots));
	mt_btn_touch = 0;
	mt_sync_count = 0;
	current_slot = -1;
}

/* ── linux/input.h ────────────────────────────────────────────────── */

struct input_dev *input_allocate_device(void)
{
	return calloc(1, sizeof(struct input_dev));
}

void input_free_device(struct input_dev *dev)
{
	free(dev);
}

int input_register_device(struct input_dev *dev)
{
	(void)dev;
	return 0;
}

void input_set_abs_params(struct input_dev *dev, unsigned int axis,
			   int min, int max, int fuzz, int flat)
{
	(void)dev; (void)axis; (void)min; (void)max; (void)fuzz; (void)flat;
}

void input_abs_set_res(struct input_dev *dev, unsigned int axis, int res)
{
	(void)dev; (void)axis; (void)res;
}

void input_report_abs(struct input_dev *dev, unsigned int code, int value)
{
	(void)dev;
	if (current_slot < 0 || current_slot >= MT_RECORD_MAX_SLOTS)
		return;

	struct mt_slot_record *r = &mt_slots[current_slot];

	switch (code) {
	case ABS_MT_POSITION_X:
		r->x = value;
		break;
	case ABS_MT_POSITION_Y:
		r->y = value;
		break;
	case ABS_MT_TOUCH_MAJOR:
		r->major = value;
		break;
	case ABS_MT_TOUCH_MINOR:
		r->minor = value;
		break;
	case ABS_MT_ORIENTATION:
		r->ori = value;
		break;
	default:
		break;
	}
}

void input_report_key(struct input_dev *dev, unsigned int code, int value)
{
	(void)dev;
	if (code == BTN_TOUCH)
		mt_btn_touch = value;
}

void input_sync(struct input_dev *dev)
{
	(void)dev;
	mt_sync_count++;
}

/* ── linux/input/mt.h ─────────────────────────────────────────────── */

int input_mt_init_slots(struct input_dev *dev, unsigned int num_slots,
			 unsigned int flags)
{
	(void)dev; (void)flags;
	mt_init_slots_requested = num_slots;
	return 0;
}

void input_mt_slot(struct input_dev *dev, int slot)
{
	(void)dev;
	current_slot = slot;
}

void input_mt_report_slot_state(struct input_dev *dev, unsigned int tool_type,
				 bool active)
{
	(void)dev; (void)tool_type;
	if (current_slot < 0 || current_slot >= MT_RECORD_MAX_SLOTS)
		return;
	mt_slots[current_slot].active = active;
}

void input_mt_sync_frame(struct input_dev *dev)
{
	(void)dev;
}
