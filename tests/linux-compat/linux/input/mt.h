/* SPDX-License-Identifier: GPL-2.0 */
#ifndef TEST_LINUX_INPUT_MT_H
#define TEST_LINUX_INPUT_MT_H

#include <linux/input.h>
#include <stdbool.h>

#define INPUT_MT_DIRECT 0x1

#define MT_TOOL_FINGER 0

int input_mt_init_slots(struct input_dev *dev, unsigned int num_slots,
			 unsigned int flags);
void input_mt_slot(struct input_dev *dev, int slot);
void input_mt_report_slot_state(struct input_dev *dev, unsigned int tool_type,
				 bool active);
void input_mt_sync_frame(struct input_dev *dev);

#endif
