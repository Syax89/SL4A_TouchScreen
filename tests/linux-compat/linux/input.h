/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_INPUT_H
#define TEST_LINUX_INPUT_H

/*
 * Minimal host stand-in for the Linux input subsystem, covering only
 * what driver/mshw0231-raw.c actually references. Every mutating call
 * (input_report_abs, input_mt_report_slot_state, ...) is implemented
 * in raw-pipeline-stubs/stubs.c and RECORDS what was reported into the
 * globals declared in raw-pipeline-stubs/mt_record.h, so a replay test
 * can assert on the real pipeline's real output instead of return
 * values that don't exist in the real kernel API either.
 */

#include <linux/types.h>
#include <stdbool.h>

/* ── Event types ──────────────────────────────────────────────── */
#define EV_KEY 0x01
#define EV_ABS 0x03

/* ── Key/button codes ────────────────────────────────────────── */
#define BTN_TOUCH 0x14a

/* ── Absolute axis codes (values match linux/input-event-codes.h) ── */
#define ABS_X               0x00
#define ABS_Y               0x01
#define ABS_MT_TOUCH_MAJOR  0x30
#define ABS_MT_TOUCH_MINOR  0x31
#define ABS_MT_ORIENTATION  0x34
#define ABS_MT_POSITION_X   0x35
#define ABS_MT_POSITION_Y   0x36

/* ── Input device properties / bus types ─────────────────────── */
#define INPUT_PROP_DIRECT 0x00
#define BUS_SPI            0x1C

/*
 * Real evbit/keybit/propbit bitmaps are sized to KEY_CNT/ABS_CNT/etc
 * (hundreds of bits). BTN_TOUCH=0x14a=330 alone needs 6 64-bit words;
 * size generously so set_bit() below can never run off the array.
 */
#define TEST_INPUT_BITMAP_LONGS 32

struct device;

struct input_id {
	unsigned short bustype;
	unsigned short vendor;
	unsigned short product;
	unsigned short version;
};

struct input_dev {
	const char *name;
	const char *phys;
	struct input_id id;
	struct {
		struct device *parent;
	} dev;
	unsigned long propbit[TEST_INPUT_BITMAP_LONGS];
	unsigned long evbit[TEST_INPUT_BITMAP_LONGS];
	unsigned long keybit[TEST_INPUT_BITMAP_LONGS];
};

static inline void set_bit(int nr, unsigned long *addr)
{
	unsigned int word = (unsigned int)nr / (8 * sizeof(unsigned long));
	unsigned int bit = (unsigned int)nr % (8 * sizeof(unsigned long));

	addr[word] |= (1UL << bit);
}

struct input_dev *input_allocate_device(void);
void input_free_device(struct input_dev *dev);
int input_register_device(struct input_dev *dev);

void input_set_abs_params(struct input_dev *dev, unsigned int axis,
			   int min, int max, int fuzz, int flat);
void input_abs_set_res(struct input_dev *dev, unsigned int axis, int res);

void input_report_abs(struct input_dev *dev, unsigned int code, int value);
void input_report_key(struct input_dev *dev, unsigned int code, int value);
void input_sync(struct input_dev *dev);

#endif
