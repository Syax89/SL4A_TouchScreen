/* SPDX-License-Identifier: GPL-2.0 */
#ifndef RAW_PIPELINE_MT_RECORD_H
#define RAW_PIPELINE_MT_RECORD_H

/*
 * Test-facing recording surface for the linux/input.h + linux/input/mt.h
 * stubs (implemented in stubs.c). mshw0231-raw.c's raw_emit_mt() calls
 * input_mt_slot()/input_mt_report_slot_state()/input_report_abs() for
 * every slot on every processed frame; the stub implementations mirror
 * those calls into mt_slots[] below so a replay test can inspect, after
 * calling mshw0231_raw_consume_samples(), exactly what the real pipeline
 * decided to report — active slot count, positions, touch ellipse — the
 * same information a real evdev listener would see.
 */

#include <stdbool.h>

#define MT_RECORD_MAX_SLOTS 64

struct mt_slot_record {
	bool active;
	int x, y;               /* last ABS_MT_POSITION_X/Y reported for this slot */
	int major, minor, ori;  /* last ABS_MT_TOUCH_MAJOR/MINOR/ORIENTATION reported */
};

extern struct mt_slot_record mt_slots[MT_RECORD_MAX_SLOTS];
extern int mt_btn_touch;             /* last value passed to input_report_key(BTN_TOUCH) */
extern unsigned long mt_sync_count;  /* number of input_sync() calls seen so far */
extern unsigned int mt_init_slots_requested; /* num_slots passed to input_mt_init_slots() */

/* Count of slots currently recorded active — the number of concurrently
 * tracked touches the real pipeline is reporting right now. */
int mt_record_active_count(void);

/* Not required between frames (recordings are naturally overwritten/
 * accumulated the same way real evdev slot state persists across
 * frames), but handy for isolating scenarios inside one test binary. */
void mt_record_reset(void);

#endif
