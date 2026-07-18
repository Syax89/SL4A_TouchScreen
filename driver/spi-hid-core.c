// SPDX-License-Identifier: GPL-2.0
/*
 * HID over SPI protocol implementation
 * spi-hid-core.c
 *
 * Copyright (c) 2020 Microsoft Corporation
 *
 * This code is partly based on "HID over I2C protocol implementation:
 *
 *  Copyright (c) 2012 Benjamin Tissoires <benjamin.tissoires@gmail.com>
 *  Copyright (c) 2012 Ecole Nationale de l'Aviation Civile, France
 *  Copyright (c) 2012 Red Hat, Inc
 *
 *  which in turn is partly based on "USB HID support for Linux":
 *
 *  Copyright (c) 1999 Andreas Gal
 *  Copyright (c) 2000-2005 Vojtech Pavlik <vojtech@suse.cz>
 *  Copyright (c) 2005 Michael Haboustak <mike-@cinci.rr.com> for Concept2, Inc
 *  Copyright (c) 2007-2008 Oliver Neukum
 *  Copyright (c) 2006-2010 Jiri Kosina
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#include <linux/acpi.h>
#include <linux/module.h>
#include <linux/spi/spi.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/wait.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/hid.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/pinctrl/consumer.h>
#include <linux/firmware.h>
#include "spi-amd.h"
#include <linux/regulator/consumer.h>
#include <linux/workqueue.h>
#include <linux/dma-mapping.h>
#include <linux/crc32.h>
#include <linux/kernel.h>
#include <linux/input/mt.h>
#include <linux/math.h>

#include "spi-hid-core.h"
#include "spi-hid-capimg.h"
#include "spi-hid-protocol.h"
#include "spi-hid_trace.h"

#define SPI_HID_MAX_RESET_ATTEMPTS 3
#define RAW_TRANSITION_DELAY_MS 5900
#define RAW_TRANSITION_RESPONSE_TIMEOUT_MS 1000
#define ISOLATED_SET_OBSERVATION_TIMEOUT_MS 45000

static int debug_level;
static int probe_raw_id = 4;  /* Windows content_id for raw-mode handshake */
static int getfeat_delay_ms = 5900;  /* Windows-observed RPT_DESC -> GET_FEATURE settle time */
static bool raw_transition_once;
static bool isolated_set_test;
#define seq_dbg(shid, level, fmt, ...) \
	do { if (debug_level >= (level)) \
		dev_info(&(shid)->spi->dev, "TRACE[hid:%d] " fmt, (level), ##__VA_ARGS__); } while (0)

enum spi_hid_seq_reason {
	SPI_HID_SEQ_PROBE,
	SPI_HID_SEQ_RESET_RESPONSE,
	SPI_HID_SEQ_DEVICE_DESCRIPTOR,
	SPI_HID_SEQ_REPORT_DESCRIPTOR,
	SPI_HID_SEQ_FEATURE_REQUEST,
	SPI_HID_SEQ_FEATURE_RESPONSE,
	SPI_HID_SEQ_DEVICE_RESET,
	SPI_HID_SEQ_WATCHDOG,
	SPI_HID_SEQ_FALLBACK,
};

enum spi_hid_lifecycle_action {
	SPI_HID_LIFECYCLE_PROBE,
	SPI_HID_LIFECYCLE_IRQ_ARMED,
	SPI_HID_LIFECYCLE_REMOVE,
	SPI_HID_LIFECYCLE_RECOVERY,
	SPI_HID_LIFECYCLE_PROBE_FAILED,
};

static void spi_hid_seq_set_state(struct spi_hid *shid, int new_state,
		enum spi_hid_seq_reason reason)
{
	int old_state = shid->seq_state;

	if (old_state == new_state)
		return;

	shid->seq_state = new_state;
	trace_spi_hid_seq_state(shid, old_state, new_state, reason);
	seq_dbg(shid, 1, "SEQ: state %d -> %d (reason=%d)\n",
		old_state, new_state, reason);
}

static struct hid_ll_driver spi_hid_ll_driver;

static void spi_hid_parse_dev_desc(struct spi_hid_device_desc_raw *raw,
		struct spi_hid_device_descriptor *desc)
{
	desc->hid_version = le16_to_cpu(raw->bcdVersion);
	desc->report_descriptor_length = le16_to_cpu(raw->wReportDescLength);
	desc->report_descriptor_register =
		le16_to_cpu(raw->wReportDescRegister);
	desc->input_register = le16_to_cpu(raw->wInputRegister);
	desc->max_input_length = le16_to_cpu(raw->wMaxInputLength);
	desc->output_register = le16_to_cpu(raw->wOutputRegister);
	desc->max_output_length = le16_to_cpu(raw->wMaxOutputLength);
	desc->command_register = le16_to_cpu(raw->wCommandRegister);
	desc->vendor_id = le16_to_cpu(raw->wVendorID);
	desc->product_id = le16_to_cpu(raw->wProductID);
	desc->version_id = le16_to_cpu(raw->wVersionID);
	desc->device_power_support = 0;
	desc->power_response_delay = 0;
}

static void spi_hid_populate_input_header(__u8 *buf,
		struct spi_hid_input_header *header)
{
	struct spi_hid_protocol_header protocol_header;

	spi_hid_protocol_decode_header(buf, &protocol_header);
	header->version = protocol_header.version;
	header->report_type = protocol_header.report_type;
	header->fragment_id = protocol_header.fragment_id;
	header->length_reserved = protocol_header.length_reserved;
	header->report_length = protocol_header.report_length;
	header->sync_const = protocol_header.sync_const;
}

static void spi_hid_populate_input_body(__u8 *buf,
		struct spi_hid_input_body *body)
{
	body->content_length = (buf[0] | (buf[1] << 8)) -
		(sizeof(body->content_length) + sizeof(body->content_id));
	body->content_id = buf[2];
}

static void spi_hid_input_report_prepare(struct spi_hid_input_buf *buf,
		struct spi_hid_input_report *report)
{
	struct spi_hid_input_header header;
	struct spi_hid_input_body body;

	spi_hid_populate_input_header(buf->header, &header);
	spi_hid_populate_input_body(buf->body, &body);
	report->report_type = header.report_type;
	report->content_length = body.content_length;
	report->content_id = body.content_id;
	report->content = buf->content;
}

static void spi_hid_output_header(__u8 *buf,
		u16 output_register, u16 output_report_length)
{
	spi_hid_protocol_encode_output_header(buf, output_register,
			output_report_length);
}

static void spi_hid_read_approval(u32 input_register, u8 *buf)
{
	spi_hid_protocol_encode_read_approval(buf, input_register);
}

static int spi_hid_input_async(struct spi_hid *shid, void *buf, u16 length,
		void (*complete)(void*))
{
	int ret;

	shid->input_transfer[0].tx_buf = shid->read_approval;
	shid->input_transfer[0].len = SPI_HID_READ_APPROVAL_LEN;

	shid->input_transfer[1].rx_buf = buf;
	shid->input_transfer[1].len = length;

	/*
	 * Optimization opportunity: we really do not need the input_register
	 * field in struct spi_hid; we can calculate the read_approval field
	 * with default input_register value during probe and then re-calculate
	 * from spi_hid_parse_dev_desc. And then we can get rid of the below
	 * spi_hid_read_approval call which is run twice per interrupt.
	 *
	 * Long term, for spec v1.0, we'll be using the input_register value
	 * from device tree, not from the device descriptor.
	 */
	spi_hid_read_approval(shid->desc.input_register,
			shid->read_approval);
	spi_message_init_with_transfers(&shid->input_message,
			shid->input_transfer, 2);

	shid->input_message.complete = complete;
	shid->input_message.context = shid;

	trace_spi_hid_input_async(shid,
			shid->input_transfer[0].tx_buf,
			shid->input_transfer[0].len,
			shid->input_transfer[1].rx_buf,
			shid->input_transfer[1].len, 0);

	ret = spi_async(shid->spi, &shid->input_message);
	if (ret) {
		shid->bus_error_count++;
		shid->bus_last_error = ret;
	}

	return ret;
}

static int spi_hid_output(struct spi_hid *shid, void *buf, u16 length)
{
	struct spi_transfer transfer;
	struct spi_message message;
	int ret;
	u8 *tx_buf;

	tx_buf = kmalloc(length + 1, GFP_KERNEL);
	if (!tx_buf)
		return -ENOMEM;

	tx_buf[0] = 0x02; /* Prepend write register opcode 0x02 */
	memcpy(&tx_buf[1], buf, length);

	memset(&transfer, 0, sizeof(transfer));
	transfer.tx_buf = tx_buf;
	transfer.len = length + 1;

	spi_message_init_with_transfers(&message, &transfer, 1);

	/*
	 * REVISIT: Should output be asynchronous?
	 *
	 * According to Documentation/hid/hid-transport.rst, ->output_report()
	 * must be implemented as an asynchronous operation.
	 */
	trace_spi_hid_output_begin(shid, transfer.tx_buf,
			transfer.len, NULL, 0, 0);

	ret = spi_sync(shid->spi, &message);

	trace_spi_hid_output_end(shid, transfer.tx_buf,
			transfer.len, NULL, 0, ret);

	kfree(tx_buf);

	if (ret) {
		shid->bus_error_count++;
		shid->bus_last_error = ret;
	}

	return ret;
}

static const char *spi_hid_power_mode_string(u8 power_state)
{
	switch (power_state) {
	case SPI_HID_POWER_MODE_ACTIVE:
		return "d0";
	case SPI_HID_POWER_MODE_SLEEP:
		return "d2";
	case SPI_HID_POWER_MODE_OFF:
		return "d3";
	case SPI_HID_POWER_MODE_WAKING_SLEEP:
		return "d3*";
	default:
		return "unknown";
	}
}

static int spi_hid_set_power(struct spi_hid *shid, u8 power_mode)
{
	u8 *raw_buf;
	u16 body_len = 3; /* len16 LE = ContentID(1) + SET_POWER(1) + param(1) = 3 */
	u16 total_len = 1 + 3 + 2 + body_len; /* opcode(1) + addr(3) + len(2) + body(3) */
	int ret;

	if (shid->isolated_set_armed || shid->desc.command_register == 0)
		return 0;

	raw_buf = kmalloc(total_len, GFP_KERNEL);
	if (!raw_buf)
		return -ENOMEM;

	raw_buf[0] = 0x02; /* write opcode */
	raw_buf[1] = (shid->desc.command_register >> 16) & 0xff;
	raw_buf[2] = (shid->desc.command_register >> 8) & 0xff;
	raw_buf[3] = (shid->desc.command_register >> 0) & 0xff;
	raw_buf[4] = body_len & 0xff; /* len16 LE */
	raw_buf[5] = (body_len >> 8) & 0xff;
	raw_buf[6] = 0x00; /* ContentID */
	raw_buf[7] = 0x08; /* SET_POWER opcode */
	raw_buf[8] = power_mode;

	mutex_lock(&shid->output_lock);
	{
		struct spi_transfer transfer;
		struct spi_message message;

		memset(&transfer, 0, sizeof(transfer));
		transfer.tx_buf = raw_buf;
		transfer.len = total_len;

		spi_message_init_with_transfers(&message, &transfer, 1);
		ret = spi_sync(shid->spi, &message);
	}
	mutex_unlock(&shid->output_lock);

	kfree(raw_buf);

	return ret;
}

static int spi_hid_power_down(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	int ret;

	if (!shid->powered)
		return 0;

	ret = spi_hid_set_power(shid, SPI_HID_POWER_MODE_SLEEP);
	if (ret)
		dev_err(dev, "failed to set power SLEEP: %d\n", ret);

	if (shid->spi->dev.of_node) {
		pinctrl_select_state(shid->pinctrl, shid->pinctrl_sleep);

		ret = regulator_disable(shid->supply);
		if (ret) {
			dev_err(dev, "failed to disable regulator\n");
			return ret;
		}
	}

	shid->powered = false;

	return 0;
}

static struct hid_device *spi_hid_disconnect_hid(struct spi_hid *shid)
{
	struct hid_device *hid = shid->hid;

	shid->hid = NULL;

	return hid;
}

static void spi_hid_stop_hid(struct spi_hid *shid)
{
	struct hid_device *hid;

	hid = spi_hid_disconnect_hid(shid);
	if (hid) {
		cancel_work_sync(&shid->create_device_work);
		cancel_work_sync(&shid->refresh_device_work);
		hid_destroy_device(hid);
	}
}

/* ACPI has already powered the device before probe. */
static int spi_hid_reset_via_acpi(struct spi_hid *shid)
{
	msleep(300);
	return 0;
}

static int spi_hid_error_handler(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	int ret = 0;
	bool terminal_failure = false;

	mutex_lock(&shid->power_lock);
	if (shid->isolated_set_armed)
		goto out;
	if (shid->power_state == SPI_HID_POWER_MODE_OFF)
		goto out;

	dev_dbg(dev, "error handler entered\n");
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_RECOVERY, 0);

	if (shid->attempts++ >= SPI_HID_MAX_RESET_ATTEMPTS) {
		dev_err(dev, "unresponsive device, aborting.\n");
		shid->ready = false;
		sysfs_notify(&dev->kobj, NULL, "ready");
		ret = -ESHUTDOWN;
		terminal_failure = true;
		goto out;
	}

	/* A reset invalidates the current transport state. Do not advertise a
	 * usable HID device until descriptor discovery has completed again. */
	shid->ready = false;
	sysfs_notify(&dev->kobj, NULL, "ready");

	if (dev->of_node) {
		ret = pinctrl_select_state(shid->pinctrl, shid->pinctrl_reset);
		if (ret) {
			dev_err(dev, "Power Reset failed\n");
			goto out;
		}
	}

	shid->power_state = SPI_HID_POWER_MODE_OFF;
	shid->input_stage = SPI_HID_INPUT_STAGE_IDLE;
	shid->input_transfer_pending = 0;
	cancel_work_sync(&shid->reset_work);

	if (dev->of_node) {
		/* Drive reset for at least 100 ms */
		msleep(100);
	}

	shid->power_state = SPI_HID_POWER_MODE_ACTIVE;

	if (dev->of_node) {
		ret = pinctrl_select_state(shid->pinctrl, shid->pinctrl_active);
		if (ret) {
			dev_err(dev, "Power Restart failed\n");
			goto out;
		}
	} else {
		ret = spi_hid_reset_via_acpi(shid);
		if (ret) {
			dev_err(dev, "Reset failed\n");
			goto out;
		}
	}

out:
	mutex_unlock(&shid->power_lock);
	if (terminal_failure) {
		/* refresh_device_work also needs power_lock. Never wait for it while
		 * holding that lock, otherwise exhausted recovery deadlocks. */
		spi_hid_stop_hid(shid);
		mutex_lock(&shid->power_lock);
		spi_hid_power_down(shid);
		mutex_unlock(&shid->power_lock);
	}
	return ret;
}

/* Forward declarations */
static int spi_hid_seq_write(struct spi_hid *shid, const u8 *buf, int len, u8 *rx, int rx_len);
static void heatmap_process_frame(struct spi_hid *shid, const u8 *data,
				  u32 data_len, u8 content_id);
static void spi_hid_raw_capture_frame(struct spi_hid *shid, const u8 *body,
				      unsigned int body_length);
static void spi_hid_handle_raw_body(struct spi_hid *shid, const u8 *body,
				    unsigned int body_length);
static void seq_handle_reset(struct spi_hid *shid, int type, u16 blen, bool *expect_fast);
static void seq_handle_desc(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_rpt(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_feat(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_vendor(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_data(struct spi_hid *shid, int type, u16 blen);
static void spi_hid_isolated_set_capture(struct spi_hid *shid, int type,
		const u8 *body, u32 body_length, u16 transport_length);

/* Canonical DESCREQ frame used by every state transition that needs to re-request
 * the device descriptor. Opcode 0x02, register 0x000001, content_id=0, len=3. */
static const u8 seq_descreq[] = {
	0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
	0x00, 0x00, 0x03, 0x00, 0x00
};

/* Windows vendor init: SET_POWER (D2→D0) on command_register 0x0004.
 * Sent on every cold boot / D3→D0 transition before DESCREQ; the device
 * streams DATA type=1 immediately afterward (no DESCREQ needed).
 * Wire format (from surface_init.csv TXN#267):
 *   D2: 02 00 00 04 82 00 00 04 00 01 02 0C EE 5B
 *   D0: 02 00 00 04 82 00 00 04 00 01 01 00 00 00
 * Uses seq_write (doubled-opcode + V2 body magic 0x82), not spi_hid_set_power
 * (which uses different internal opcode 0x08 encoding). */
static int spi_hid_vendor_init(struct spi_hid *shid)
{
	static const u8 vd2[] = {
		0x02, 0x02, 0x00, 0x00, 0x04, 0x82, 0x00,
		0x00, 0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00
	};
	static const u8 vd0[] = {
		0x02, 0x02, 0x00, 0x00, 0x04, 0x82, 0x00,
		0x00, 0x04, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00
	};
	int ret;

	ret = spi_hid_seq_write(shid, vd2, sizeof(vd2), NULL, 0);
	if (ret)
		return ret;
	msleep(100);
	ret = spi_hid_seq_write(shid, vd0, sizeof(vd0), NULL, 0);
	msleep(100);
	return ret;
}

static void spi_hid_error_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, error_work);
	struct device *dev = &shid->spi->dev;
	int ret;

	if (READ_ONCE(shid->removing))
		return;
	if (READ_ONCE(shid->isolated_set_armed))
		return;

	ret = spi_hid_error_handler(shid);
	if (ret)
		dev_err(dev, "%s: error handler failed\n", __func__);
}

/**
 * Handle the reset response from the FW by sending a request for the device
 * descriptor.
 * @shid: a pointer to the driver context
 */
static void spi_hid_reset_work(struct work_struct *work)
{
	struct spi_hid *shid =
		container_of(work, struct spi_hid, reset_work);
	struct device *dev = &shid->spi->dev;
	struct spi_hid_output_buf *buf = &shid->output;
	int ret;

	trace_spi_hid_reset_work(shid);

	dev_dbg(dev, "reset handler\n");
	if (READ_ONCE(shid->removing))
		return;
	if (READ_ONCE(shid->isolated_set_armed))
		return;
	if (shid->ready) {
		dev_err(dev, "Spontaneous FW reset!");
		shid->ready = false;
		shid->dir_count++;
		sysfs_notify(&dev->kobj, NULL, "ready");
	}

	if (flush_work(&shid->create_device_work))
		dev_dbg(dev, "reset handler waited for create_device_work\n");

	if (shid->power_state == SPI_HID_POWER_MODE_OFF) {
		return;
	}

	if (flush_work(&shid->refresh_device_work))
		dev_dbg(dev, "reset handler waited for refresh_device_work\n");

	mutex_lock(&shid->output_lock);
	{
		u16 body_len = round_up(sizeof(buf->body) + 2, 4);

		buf->body[0] = 0x00; /* content_type = COMMAND */
		buf->body[1] = 0x04; /* content_length = 4 (len16+ContentID+opcode) */
		buf->body[2] = 0x00;
		buf->body[3] = 0x00; /* content_id = 0 */
		buf->content[0] = 0x01; /* RESET command opcode */
		buf->content[1] = 0x00; /* padding/reserved */
		spi_hid_output_header(buf->header, shid->desc.command_register, body_len);

		ret =  spi_hid_output(shid, buf, sizeof(buf->header) + body_len);
	}
	mutex_unlock(&shid->output_lock);
	if (ret) {
		dev_err(dev, "failed to send device reset request\n");
		schedule_work(&shid->error_work);
		return;
	}
}

static int spi_hid_input_report_handler(struct spi_hid *shid,
		struct spi_hid_input_buf *buf)
{
	struct device *dev = &shid->spi->dev;
	struct spi_hid_input_report r;
	int ret;

	dev_dbg(dev, "input report handler\n");

	trace_spi_hid_input_report_handler(shid);

	if (!shid->ready) {
		dev_dbg(dev, "discarding input report: device not ready\n");
		return 0;
	}

	if (shid->refresh_in_progress) {
		dev_dbg(dev, "discarding input report: refresh in progress\n");
		return 0;
	}

	if (!shid->hid) {
		dev_dbg(dev, "discarding input report: no HID device\n");
		return 0;
	}

	spi_hid_input_report_prepare(buf, &r);

	if (shid->perf_mode &&
			(r.content_id == SPI_HID_RIGHT_SCREEN_TOUCH_HEAT_MAP_REPORT_ID ||
			r.content_id == SPI_HID_LEFT_SCREEN_TOUCH_HEAT_MAP_REPORT_ID)) {
		r.content[1] = shid->touch_signature_index >> 8;
		r.content[0] = shid->touch_signature_index++;
	}

	ret = hid_input_report(shid->hid, HID_INPUT_REPORT,
			r.content - 1,
			r.content_length + 1, 1);

	if (shid->perf_mode &&
			(r.content_id == SPI_HID_HEARTBEAT_REPORT_ID ||
			r.content_id == SPI_HID_RIGHT_SCREEN_TOUCH_HEAT_MAP_REPORT_ID ||
			r.content_id == SPI_HID_LEFT_SCREEN_TOUCH_HEAT_MAP_REPORT_ID)) {
		shid->latencies[shid->latency_index].end_time = ktime_get_ns();
		shid->latencies[shid->latency_index].report_id = r.content_id;
		shid->latencies[shid->latency_index].signature = (r.content[1] << 8) | r.content[0];
		shid->latencies[shid->latency_index].start_time = shid->interrupt_time_stamps[0];

		shid->latency_index = (shid->latency_index + 1) % SPI_HID_MAX_LATENCIES;
	}

	if (ret == -ENODEV || ret == -EBUSY) {
		dev_err(dev, "ignoring report --> %d\n", ret);
		return 0;
	}

	return ret;
}

static int spi_hid_response_handler(struct spi_hid *shid,
		struct spi_hid_input_buf *buf)
{
	trace_spi_hid_response_handler(shid);
	dev_dbg(&shid->spi->dev, "response handler\n");

	/* completion_done returns 0 if there are waiters, otherwise 1 */
	if (completion_done(&shid->output_done))
		dev_err(&shid->spi->dev, "Unexpected response report\n");
	else
		complete(&shid->output_done);

	return 0;
}

static int spi_hid_send_output_report(struct spi_hid *shid, u32 output_register,
		struct spi_hid_output_report *report)
{
	u8 *raw_buf;
	u16 content_len = report->content_length;
	u16 body_len; /* len16 LE = ContentID(1) + content_len */
	size_t total_len;
	int ret;

	if (content_len == U16_MAX)
		return -EMSGSIZE;

	body_len = content_len + 1;
	total_len = 1 + 3 + 2 + body_len; /* opcode(1) + addr(3) + len(2) + body_len */

	raw_buf = kmalloc(total_len, GFP_KERNEL);
	if (!raw_buf)
		return -ENOMEM;

	/* Format the packet in V0 format */
	raw_buf[0] = 0x02; /* write opcode */
	raw_buf[1] = (output_register >> 16) & 0xff;
	raw_buf[2] = (output_register >> 8) & 0xff;
	raw_buf[3] = (output_register >> 0) & 0xff;
	raw_buf[4] = body_len & 0xff; /* len16 LE */
	raw_buf[5] = (body_len >> 8) & 0xff;
	raw_buf[6] = report->content_id;
	if (content_len)
		memcpy(&raw_buf[7], report->content, content_len);

	mutex_lock(&shid->output_lock);
	{
		struct spi_transfer transfer;
		struct spi_message message;

		memset(&transfer, 0, sizeof(transfer));
		transfer.tx_buf = raw_buf;
		transfer.len = total_len;

		spi_message_init_with_transfers(&message, &transfer, 1);
		ret = spi_sync(shid->spi, &message);
	}
	mutex_unlock(&shid->output_lock);

	kfree(raw_buf);

	if (ret)
		dev_err(&shid->spi->dev, "failed output transfer: %d\n", ret);

	return ret;
}

/*
* This function shouldn't be called from the interrupt thread context since it
* waits for completion that gets completed in one of the future runs of the
* interrupt thread.
*/
static int spi_hid_sync_request(struct spi_hid *shid, u16 output_register,
		struct spi_hid_output_report *report)
{
	struct device *dev = &shid->spi->dev;
	int ret = 0;

	/* A completion is single-use: never let a prior response satisfy this request. */
	reinit_completion(&shid->output_done);
	ret = spi_hid_send_output_report(shid, output_register,
			report);
	if (ret) {
		dev_err(dev, "failed to transfer output report\n");
		return ret;
	}

	mutex_unlock(&shid->lock);
	ret = wait_for_completion_interruptible_timeout(&shid->output_done,
			msecs_to_jiffies(1000));
	mutex_lock(&shid->lock);
	if (ret == 0) {
		dev_err(dev, "response timed out\n");
		schedule_work(&shid->error_work);
		return -ETIMEDOUT;
	}
	if (ret < 0)
		return ret;

	return 0;
}

/*
* This function returns the length of the report descriptor, or a negative
* error code if something went wrong.
*/
static int spi_hid_report_descriptor_request(struct spi_hid *shid)
{
	int ret;
	struct device *dev = &shid->spi->dev;
	struct spi_hid_output_report report = {
		.content_type = SPI_HID_CONTENT_TYPE_COMMAND,
		.content_length = 3,
		.content_id = 0,
		.content = NULL,
	};


	ret =  spi_hid_sync_request(shid,
			shid->desc.report_descriptor_register, &report);
	if (ret) {
		dev_err(dev, "Expected report descriptor not received!\n");
		goto out;
	}

	ret = (shid->response.body[0] | (shid->response.body[1] << 8)) - 3;
	if (ret != shid->desc.report_descriptor_length) {
		dev_warn(dev, "report descriptor length differs from device descriptor; using the shorter value\n");
		ret = min_t(unsigned int, ret,
			shid->desc.report_descriptor_length);
	}
out:
	return ret;
}

static int spi_hid_process_input_report(struct spi_hid *shid,
		struct spi_hid_input_buf *buf)
{
	struct spi_hid_input_header header;
	struct spi_hid_input_body body;
	struct device *dev = &shid->spi->dev;
	struct spi_hid_device_desc_raw *raw;
	int ret;

	trace_spi_hid_process_input_report(shid);

	spi_hid_populate_input_header(buf->header, &header);
	spi_hid_populate_input_body(buf->body, &body);

	if (body.content_length > header.report_length) {
		dev_err(dev, "Bad body length %d > %d\n", body.content_length,
							header.report_length);
		return -EINVAL;
	}

	if (body.content_id == SPI_HID_HEARTBEAT_REPORT_ID) {
		dev_warn(dev, "Heartbeat ID 0x%x from device %u\n",
			buf->content[1], buf->content[0]);
	}

	switch (header.report_type) {
	case SPI_HID_REPORT_TYPE_DATA:
		ret = spi_hid_input_report_handler(shid, buf);
		break;
	case SPI_HID_REPORT_TYPE_RESET_RESP:
		schedule_work(&shid->reset_work);
		ret = 0;
		break;
	case SPI_HID_REPORT_TYPE_DEVICE_DESC:
		dev_dbg(dev, "received device descriptor\n");
		/* Reset attempts at every device descriptor fetch */
		shid->attempts = 0;
		raw = (struct spi_hid_device_desc_raw *) buf->content;
		spi_hid_parse_dev_desc(raw, &shid->desc);
		if (!shid->hid) {
			schedule_work(&shid->create_device_work);
		} else {
			schedule_work(&shid->refresh_device_work);
		}
		ret = 0;
		break;
	case SPI_HID_REPORT_TYPE_COMMAND_RESP:
	case SPI_HID_REPORT_TYPE_GET_FEATURE_RESP:
		if (!shid->ready) {
			dev_err(dev,
				"Unexpected response report type while not ready: 0x%x\n",
				header.report_type);
			ret = -EINVAL;
			break;
		}
		fallthrough;
	case SPI_HID_REPORT_TYPE_REPORT_DESC:
		ret = spi_hid_response_handler(shid, buf);
		break;
	default:
		dev_err(dev, "Unknown input report: 0x%x\n", header.report_type);
		ret = -EINVAL;
		break;
	}


	return ret;
}

static int spi_hid_bus_validate_header(struct spi_hid *shid, struct spi_hid_input_header *header)
{
	struct device *dev = &shid->spi->dev;
	size_t body_capacity = sizeof(shid->input.body) + sizeof(shid->input.content);

	if (header->sync_const != SPI_HID_INPUT_HEADER_SYNC_BYTE) {
		dev_err(dev, "Invalid input report sync constant (0x%x)\n",
				header->sync_const);
		return -EINVAL;
	}

	if (header->version != SPI_HID_INPUT_HEADER_VERSION) {
		dev_err(dev, "Unknown input report version (v 0x%x)\n",
				header->version);
		return -EINVAL;
	}

	if (header->length_reserved) {
		dev_err(dev, "Invalid input report length reserved bits (0x%x)\n",
			header->length_reserved);
		return -EINVAL;
	}

	if (shid->desc.max_input_length != 0 && header->report_length > shid->desc.max_input_length) {
		dev_err(dev, "Report body of size %u larger than max expected of %u\n",
				header->report_length, shid->desc.max_input_length);
		return -EMSGSIZE;
	}
	if (header->report_length > body_capacity) {
		dev_err(dev, "Report body of size %u exceeds input buffer capacity %zu\n",
			header->report_length, body_capacity);
		return -EMSGSIZE;
	}

	return 0;
}

static int spi_hid_create_device(struct spi_hid *shid)
{
	struct hid_device *hid;
	struct device *dev = &shid->spi->dev;
	int ret;

	hid = hid_allocate_device();

	if (IS_ERR(hid)) {
		dev_err(dev, "Failed to allocate hid device: %ld\n",
				PTR_ERR(hid));
		ret = PTR_ERR(hid);
		return ret;
	}

	hid->driver_data = shid->spi;
	hid->ll_driver = &spi_hid_ll_driver;
	hid->dev.parent = &shid->spi->dev;
	hid->bus = BUS_SPI;
	hid->version = shid->desc.hid_version;
	hid->vendor = shid->desc.vendor_id;
	hid->product = shid->desc.product_id;

	snprintf(hid->name, sizeof(hid->name), "spi %04hX:%04hX",
			hid->vendor, hid->product);
	strscpy(hid->phys, dev_name(&shid->spi->dev), sizeof(hid->phys));

	shid->hid = hid;

	ret = hid_add_device(hid);
	/* hid_add_device() can succeed without a driver binding the parsed device. */
	if (!ret && !hid->driver) {
		dev_warn(dev, "SEQ: hid_add_device succeeded but no driver bound to it\n");
		ret = -ENODEV;
	}
	if (ret) {
		dev_err(dev, "Failed to add hid device: %d\n", ret);
		/*
		* We likely got here because report descriptor request timed
		* out. Let's disconnect and destroy the hid_device structure.
		*/
		hid = spi_hid_disconnect_hid(shid);
		if (hid)
			hid_destroy_device(hid);

		return ret;
	}

	return 0;
}

static void spi_hid_create_device_work(struct work_struct *work)
{
	struct spi_hid *shid =
		container_of(work, struct spi_hid, create_device_work);
	struct device *dev = &shid->spi->dev;
	int ret;

	trace_spi_hid_create_device_work(shid);
	if (READ_ONCE(shid->removing))
		return;

	dev_dbg(dev, "create device work\n");

	if (shid->desc.hid_version != SPI_HID_SUPPORTED_VERSION) {
		dev_err(dev, "Unsupported device descriptor version %4x\n",
			shid->desc.hid_version);
		schedule_work(&shid->error_work);
		return;
	}

	ret = spi_hid_create_device(shid);
	if (ret) {
		dev_err(dev, "Failed to create hid device\n");
		return;
	}

	shid->attempts = 0;
	shid->power_state = SPI_HID_POWER_MODE_ACTIVE;
}

static void spi_hid_refresh_device_work(struct work_struct *work)
{
	struct spi_hid *shid =
		container_of(work, struct spi_hid, refresh_device_work);
	struct device *dev = &shid->spi->dev;
	struct hid_device *hid;
	int ret;
	u32 new_crc32;

	trace_spi_hid_refresh_device_work(shid);
	if (READ_ONCE(shid->removing))
		return;

	dev_dbg(dev, "refresh device work\n");

	if (shid->desc.hid_version != SPI_HID_SUPPORTED_VERSION) {
		dev_err(dev, "Unsupported device descriptor version %4x\n",
			shid->desc.hid_version);
		schedule_work(&shid->error_work);
		return;
	}

	mutex_lock(&shid->power_lock);

	if (shid->power_state == SPI_HID_POWER_MODE_OFF)
		goto out;

	mutex_lock(&shid->lock);
	ret = spi_hid_report_descriptor_request(shid);
	mutex_unlock(&shid->lock);
	if (ret < 0) {
		dev_err(dev, "Refresh: failed report descriptor request, error %d", ret);
		goto out;
	}

	new_crc32 = crc32_le(0, (unsigned char const *) shid->response.content, (size_t)ret);
	if (new_crc32 == shid->report_descriptor_crc32)
	{
		dev_err(dev, "Refresh device work - returning\n");
		shid->ready = true;
		sysfs_notify(&dev->kobj, NULL, "ready");
		goto out;
	}

	dev_err(dev, "Re-creating the HID device\n");

	shid->report_descriptor_crc32 = new_crc32;
	shid->refresh_in_progress = true;

	hid = spi_hid_disconnect_hid(shid);
	if (hid) {
		hid_destroy_device(hid);
	}

	ret = spi_hid_create_device(shid);
	if (ret) {
		dev_err(dev, "Failed to create hid device\n");
		goto out;
	}

	shid->ready = true;
	sysfs_notify(&dev->kobj, NULL, "ready");

out:
	/* This worker owns the flag for its entire lifetime. In particular, a
	 * failed HID recreation must not leave input delivery permanently gated. */
	shid->refresh_in_progress = false;
	mutex_unlock(&shid->power_lock);
}

static void spi_hid_input_header_complete(void *_shid);

static void spi_hid_input_body_complete(void *_shid)
{
	struct spi_hid *shid = _shid;
	struct device *dev = &shid->spi->dev;
	unsigned long flags;
	int ret;
	struct spi_hid_input_buf *buf;
	struct spi_hid_input_header header;

	spin_lock_irqsave(&shid->input_lock, flags);
	if (!shid->powered)
		goto out;

	trace_spi_hid_input_body_complete(shid,
			shid->input_transfer[0].tx_buf,
			shid->input_transfer[0].len,
			shid->input_transfer[1].rx_buf,
			shid->input_transfer[1].len,
			shid->input_message.status);

	shid->input_stage = SPI_HID_INPUT_STAGE_IDLE;

	if (shid->input_message.status < 0) {
		dev_warn(dev, "error reading body, resetting %d\n",
				shid->input_message.status);
		shid->bus_error_count++;
		shid->bus_last_error = shid->input_message.status;
		schedule_work(&shid->error_work);
		goto out;
	}

	if (shid->power_state == SPI_HID_POWER_MODE_OFF) {
		dev_warn(dev, "input body complete called while device is "
				"off\n");
		goto out;
	}

	spi_hid_populate_input_header(shid->input.header, &header);
	buf = &shid->input;
	if (header.report_type == SPI_HID_REPORT_TYPE_COMMAND_RESP ||
		header.report_type == SPI_HID_REPORT_TYPE_GET_FEATURE_RESP ||
		header.report_type == SPI_HID_REPORT_TYPE_REPORT_DESC) {
			buf = &shid->response;
	}

	ret = spi_hid_process_input_report(shid, buf);
	if (ret) {
		dev_err(dev, "failed input callback: %d\n", ret);
		schedule_work(&shid->error_work);
		goto out;
	}

	if (--shid->input_transfer_pending) {
		buf = &shid->input;

		// On interrupt, the old start value is stored at index 1. This replaces it back to 0 after the interrupt
		shid->interrupt_time_stamps[0] = shid->interrupt_time_stamps[1];

		ret = spi_hid_input_async(shid, buf->header,
				sizeof(buf->header),
				spi_hid_input_header_complete);
		if (ret)
			dev_err(dev, "failed to start header --> %d\n", ret);
	}

out:
	spin_unlock_irqrestore(&shid->input_lock, flags);
}

static void spi_hid_input_header_complete(void *_shid)
{
	struct spi_hid *shid = _shid;
	struct device *dev = &shid->spi->dev;
	struct spi_hid_input_header header;
	struct spi_hid_input_buf *buf;
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&shid->input_lock, flags);
	if (!shid->powered)
		goto out;

	trace_spi_hid_input_header_complete(shid,
			shid->input_transfer[0].tx_buf,
			shid->input_transfer[0].len,
			shid->input_transfer[1].rx_buf,
			shid->input_transfer[1].len,
			shid->input_message.status);

	if (shid->input_message.status < 0) {
		dev_warn(dev, "error reading header, resetting %d\n",
				shid->input_message.status);
		shid->bus_error_count++;
		shid->bus_last_error = shid->input_message.status;
		schedule_work(&shid->error_work);
		goto out;
	}

	if (shid->power_state == SPI_HID_POWER_MODE_OFF) {
		dev_warn(dev, "input header complete called while device is "
				"off\n");
		goto out;
	}

	spi_hid_populate_input_header(shid->input.header, &header);

	seq_dbg(shid, 3, "header: version=0x%02x, report_type=0x%02x, report_length=%u, fragment_id=0x%02x, sync_const=0x%02x\n",
		header.version, header.report_type, header.report_length, header.fragment_id, header.sync_const);

	ret = spi_hid_bus_validate_header(shid, &header);
	if (ret) {
		dev_err(dev, "failed to validate header: %d\n", ret);
		print_hex_dump(KERN_ERR, "spi_hid: header buffer: ",
						DUMP_PREFIX_NONE, 16, 1,
						shid->input.header,
						sizeof(shid->input.header),
						false);
		shid->bus_error_count++;
		shid->bus_last_error = ret;
		goto out;
	}

	buf = &shid->input;
	if (header.report_type == SPI_HID_REPORT_TYPE_COMMAND_RESP ||
		header.report_type == SPI_HID_REPORT_TYPE_GET_FEATURE_RESP ||
		header.report_type == SPI_HID_REPORT_TYPE_REPORT_DESC) {
			buf = &shid->response;
			memcpy(shid->response.header, shid->input.header,
					sizeof(shid->input.header));
	}

	shid->input_stage = SPI_HID_INPUT_STAGE_BODY;

	ret = spi_hid_input_async(shid, buf->body, header.report_length,
			spi_hid_input_body_complete);
	if (ret)
		dev_err(dev, "failed body async transfer: %d\n", ret);

out:
	if (ret)
		shid->input_transfer_pending = 0;

	spin_unlock_irqrestore(&shid->input_lock, flags);
}

static int spi_hid_get_request(struct spi_hid *shid, u8 content_id)
{
	struct spi_hid_output_report report = {
		.content_type = SPI_HID_CONTENT_TYPE_GET_FEATURE,
		.content_length = 3,
		.content_id = content_id,
		.content = NULL,
	};


	return spi_hid_sync_request(shid, shid->desc.output_register,
			&report);
}

static int spi_hid_set_request(struct spi_hid *shid,
		u8 *arg_buf, u16 arg_len, u8 content_id)
{
	if (arg_len > U16_MAX - 4)
		return -EMSGSIZE;

	struct spi_hid_output_report report = {
		.content_type = SPI_HID_CONTENT_TYPE_SET_FEATURE,
		.content_length = arg_len + 3,
		.content_id = content_id,
		.content = arg_buf,
	};


	return spi_hid_send_output_report(shid,
			shid->desc.output_register, &report);
}

static int spi_hid_seq_read_reg(struct spi_hid *shid, u32 reg, u8 *rx, int rx_len)
{
	u8 tx[5]; struct spi_transfer xf[2]; struct spi_message msg;
	int ret;

	if (!rx || rx_len <= 0)
		return -EINVAL;
	lockdep_assert_held(&shid->seq_lock);
	tx[0]=0x0B; tx[1]=(reg>>16)&0xff; tx[2]=(reg>>8)&0xff; tx[3]=reg&0xff;
	tx[4]=0xFF;
	memset(rx,0,rx_len); memset(xf,0,sizeof(xf));
	xf[0].tx_buf=tx; xf[0].len=5; xf[1].rx_buf=rx; xf[1].len=rx_len;
	spi_message_init(&msg); spi_message_add_tail(&xf[0],&msg); spi_message_add_tail(&xf[1],&msg);
	seq_dbg(shid, 2, "read begin reg=0x%06x len=%d state=%d\n",
		reg, rx_len, shid->seq_state);
	ret = spi_sync(shid->spi, &msg);
	seq_dbg(shid, 2, "read complete reg=0x%06x ret=%d\n", reg, ret);
	seq_dbg(shid, 3, "SEQ: read reg=0x%06x len=%d ret=%d raw=[%*ph]\n",
		 reg, rx_len, ret, min(rx_len, 16), rx);
	return ret;
}
static int spi_hid_seq_read(struct spi_hid *shid, u8 *rx, int rx_len)
{ return spi_hid_seq_read_reg(shid, shid->desc.input_register, rx, rx_len); }
static int spi_hid_seq_write_speed(struct spi_hid *shid, const u8 *buf, int len,
				    u8 *rx, int rx_len, u32 speed_hz)
{
	struct spi_transfer xf[2];
	struct spi_message msg;

	if (!buf || len <= 0 || rx_len < 0 || (rx_len && !rx))
		return -EINVAL;
	lockdep_assert_held(&shid->seq_lock);
	seq_dbg(shid, 3, "SEQ: write op=0x%02x len=%d rx=%d raw=[%*ph]\n",
		buf[0], len, rx_len, min(len, 16), buf);

	memset(xf, 0, sizeof(xf));
	xf[0].tx_buf = (void *)buf;
	xf[0].len = len;
	if (speed_hz)
		xf[0].speed_hz = speed_hz;
	spi_message_init(&msg);
	spi_message_add_tail(&xf[0], &msg);

	if (rx && rx_len > 0) {
		xf[1].rx_buf = rx;
		xf[1].len = rx_len;
		if (speed_hz)
			xf[1].speed_hz = speed_hz;
		spi_message_add_tail(&xf[1], &msg);
	}

	seq_dbg(shid, 2, "write begin op=0x%02x len=%d rx=%d speed=%u state=%d\n",
		buf[0], len, rx_len, speed_hz, shid->seq_state);
	{
		int ret = spi_sync(shid->spi, &msg);

		seq_dbg(shid, 2, "write complete op=0x%02x ret=%d\n", buf[0], ret);
		return ret;
	}
}

static int spi_hid_seq_write(struct spi_hid *shid, const u8 *buf, int len, u8 *rx, int rx_len)
{
	return spi_hid_seq_write_speed(shid, buf, len, rx, rx_len, 0);
}
static int spi_hid_seq_hdr_type(const u8 *rx, int len, int *hdr_off)
{
	return spi_hid_protocol_find_header(rx, len, hdr_off);
}

static int spi_hid_seq_restart_discovery(struct spi_hid *shid, int reason)
{
	int ret;

	ret = spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0);
	if (ret) {
		dev_warn(&shid->spi->dev, "SEQ: DESCREQ recovery write failed: %d\n", ret);
		return ret;
	}
	spi_hid_seq_set_state(shid, 1, reason);
	return 0;
}

/* Legacy raw-handshake watchdog. raw_mode does not activate this path. */
#define RAW_HANDSHAKE_TIMEOUT_MS 2000
#define RAW_HANDSHAKE_MAX_RETRIES 3

static void spi_hid_raw_handshake_watchdog(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, raw_handshake_watchdog.work);
	struct device *dev = &shid->spi->dev;
	int ret;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || !shid->raw_mode_active ||
	    shid->raw_handshake_confirmed)
		goto out;

	if (shid->raw_handshake_retries_left <= 0) {
		dev_err(dev, "SEQ: raw_mode handshake failed after %d retries, giving up\n",
			RAW_HANDSHAKE_MAX_RETRIES);
		/* The descriptor was already acquired. Stop the experimental input
		 * path and instantiate standard HID without requiring module reload. */
		shid->raw_mode_active = false;
		shid->poll_active = false;
		shid->stream_watchdog_active = false;
		shid->feat_delay_pending = false;
		if (shid->touch_input) {
			input_unregister_device(shid->touch_input);
			shid->touch_input = NULL;
		}
		if (!shid->hid) {
			ret = spi_hid_create_device(shid);
			if (ret)
				dev_err(dev, "SEQ: standard HID fallback failed: %d\n", ret);
			else
				dev_info(dev, "SEQ: raw handshake failed; using standard HID\n");
		}
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_WATCHDOG);
		shid->raw_handshake_state5_defers = 0;
		goto out;
	}

	/* Don't interrupt an in-progress GET_FEATURE/SET_FEATURE handshake.
	 * Defer up to 3 times (6 seconds total), then force retry. */
	if (shid->seq_state == 5) {
		shid->raw_handshake_state5_defers++;
		seq_dbg(shid, 1, "SEQ: watchdog found state 5 active, defer #%u\n",
			shid->raw_handshake_state5_defers);
		if (shid->raw_handshake_state5_defers <= 3) {
			schedule_delayed_work(&shid->raw_handshake_watchdog,
					      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
			goto out;
		}
		shid->raw_handshake_state5_defers = 0;
	}
	shid->raw_handshake_retries_left--;
	dev_warn(dev, "SEQ: raw_mode handshake watchdog: no heatmap data after %dms, retrying (%d left)\n",
		 RAW_HANDSHAKE_TIMEOUT_MS, shid->raw_handshake_retries_left);
	/* Prevent concurrent delayed feature work during legacy recovery. */
	cancel_delayed_work(&shid->feat_delay_work);
	shid->feat_delay_pending = false;
	if (spi_hid_vendor_init(shid)) {
		dev_warn(dev, "SEQ: raw watchdog vendor recovery failed\n");
		schedule_delayed_work(&shid->raw_handshake_watchdog,
				      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		goto out;
	}
	if (spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0)) {
		dev_warn(dev, "SEQ: raw watchdog DESCREQ retry failed\n");
		schedule_delayed_work(&shid->raw_handshake_watchdog,
				      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		goto out;
	}
	spi_hid_seq_set_state(shid, 1, SPI_HID_SEQ_WATCHDOG);
	schedule_delayed_work(&shid->raw_handshake_watchdog,
			      msecs_to_jiffies(getfeat_delay_ms + RAW_HANDSHAKE_TIMEOUT_MS + 1000));
out:
	mutex_unlock(&shid->seq_lock);
}

/* GET_FEATURE delayed work (Windows: ~5900ms gap between RPT_DESC and GET_FEATURE).
 * The device needs this idle period to stabilise before accepting feature commands.
 * Running as a delayed work avoids holding seq_lock across a long msleep. */
static void spi_hid_feat_delay_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, feat_delay_work.work);
	struct device *dev = &shid->spi->dev;
	u8 selector;
	u8 gf[11] = {
		/* spi-amd consumes byte zero as its opcode; the following bytes are
		 * the ten-byte Windows FIFO payload. */
		0x02, 0x02, 0x00, 0x00, 0x03, 0x42,
		0x00, 0x00, 0x03, 0x00, 0x06
	};

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || !shid->feat_delay_pending)
		goto out;
	shid->feat_delay_pending = false;

	/* The normal HID create work may advance to DONE during the required
	 * 5.9-second one-shot delay. Both WAIT_RPT and DONE have a valid descriptor. */
	if (shid->seq_state != 2 &&
	    !(raw_transition_once && shid->raw_transition_attempted &&
	      shid->seq_state == 4)) {
		if (raw_transition_once && shid->raw_transition_attempted) {
			shid->raw_transition_state_skipped++;
			dev_info(dev,
				 "RAW_TRANSITION: GET skipped because state changed to %d\n",
				 shid->seq_state);
		}
		seq_dbg(shid, 1, "SEQ: feat_delay_work: state changed to %d, skipping\n",
			shid->seq_state);
		goto out;
	}

	selector = raw_transition_once ? 4 : probe_raw_id & 0xFF;
	if (raw_transition_once)
		gf[7] = selector;
	else
		gf[8] = selector;
	seq_dbg(shid, 1, "SEQ: GET_FEATURE(id=%u) after delay, state 5\n", selector);
	if (spi_hid_seq_write(shid, gf, sizeof(gf), NULL, 0)) {
		if (raw_transition_once && shid->raw_transition_attempted)
			shid->raw_transition_get_write_failed++;
		dev_warn(dev, "SEQ: delayed GET_FEATURE write failed\n");
		if (!raw_transition_once)
			mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
				msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		goto out;
	}
	if (raw_transition_once && shid->raw_transition_attempted) {
		shid->raw_transition_get_sent++;
		dev_info(dev, "RAW_TRANSITION: GET selector %u sent\n", selector);
	}
	spi_hid_seq_set_state(shid, 5, SPI_HID_SEQ_FEATURE_REQUEST);
	if (raw_transition_once && shid->raw_transition_attempted)
		schedule_delayed_work(&shid->raw_transition_timeout_work,
			msecs_to_jiffies(RAW_TRANSITION_RESPONSE_TIMEOUT_MS));
	if (!raw_transition_once)
		mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
			msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
out:
	mutex_unlock(&shid->seq_lock);
}

static void spi_hid_raw_transition_timeout_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid,
					    raw_transition_timeout_work.work);

	mutex_lock(&shid->seq_lock);
	if (!READ_ONCE(shid->removing) && raw_transition_once &&
	    shid->raw_transition_attempted && !shid->raw_transition_get_response &&
	    shid->seq_state == 5) {
		shid->raw_transition_timeout++;
		dev_warn(&shid->spi->dev,
			 "RAW_TRANSITION: GET response timeout; returning to standard HID\n");
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_RESPONSE);
	}
	mutex_unlock(&shid->seq_lock);
}

static void spi_hid_isolated_set_timeout_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid,
					    isolated_set_timeout_work.work);

	mutex_lock(&shid->seq_lock);
	if (!READ_ONCE(shid->removing) &&
	    shid->isolated_set_state == SPI_HID_ISOLATED_SET_OBSERVING) {
		shid->isolated_set_timeout++;
		shid->isolated_set_state = SPI_HID_ISOLATED_SET_COMPLETE;
		/* Observation ends without a recovery, reset, or follow-up write. */
	}
	mutex_unlock(&shid->seq_lock);
}

static void spi_hid_isolated_set_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid,
					    isolated_set_work.work);
	/* spi-amd consumes source byte zero as the opcode; FIFO sees the ten bytes
	 * captured from Windows for GET_FEATURE report ID 6. */
	static const u8 get_id6[11] = {
		0x02, 0x02, 0x00, 0x00, 0x03, 0x42,
		0x00, 0x04, 0x03, 0x00, 0x06,
	};

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) ||
	    shid->isolated_set_state != SPI_HID_ISOLATED_SET_WAIT_GET ||
	    shid->seq_state != 4 || !shid->hid) {
		if (!READ_ONCE(shid->removing)) {
			shid->isolated_set_write_failed++;
			shid->isolated_set_state = SPI_HID_ISOLATED_SET_FAILED;
			dev_warn(&shid->spi->dev,
				 "ISOLATED_SET: HID runtime precondition failed; no write sent\n");
		}
		goto out;
	}
	if (spi_hid_seq_write(shid, get_id6, sizeof(get_id6), NULL, 0)) {
		shid->isolated_set_write_failed++;
		shid->isolated_set_state = SPI_HID_ISOLATED_SET_FAILED;
		goto out;
	}
	shid->isolated_set_get_sent++;
	spi_hid_seq_set_state(shid, 5, SPI_HID_SEQ_FEATURE_REQUEST);
out:
	mutex_unlock(&shid->seq_lock);
}

static void spi_hid_seq_descreq_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, descreq_work.work);
	u8 hdr[9];
	int type, hdr_off;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || shid->seq_state != 1)
		goto out;

	seq_dbg(shid, 1, "SEQ: poll-work: reading for DEVICE_DESC...\n");
	if (spi_hid_seq_read(shid, hdr, sizeof(hdr))) {
		seq_dbg(shid, 1, "SEQ: poll-work: read failed, retrying...\n");
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
		goto out;
	}
	type = spi_hid_seq_hdr_type(hdr, sizeof(hdr), &hdr_off);
	if (type >= 0 && hdr_off != 5) {
		seq_dbg(shid, 1, "SEQ: poll-work: header at unexpected offset %d\n", hdr_off);
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
		goto out;
	}
	seq_dbg(shid, 2, "SEQ: poll-work: type=%d raw=[%*ph]\n", type, 9, hdr);
	if (type == 7) {
		shid->stat_device_desc++;
		seq_dbg(shid, 1, "SEQ: poll-work: GOT DEVICE_DESC!\n");
		/* Re-trigger the IRQ thread to handle it */
	} else if (type == 3) {
		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: poll-work: RESET_RSP, restarting discovery\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	} else {
		seq_dbg(shid, 1, "SEQ: poll-work: unexpected type=%d, retrying...\n", type);
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
	}
out:
	mutex_unlock(&shid->seq_lock);
}


/* CapImg raster blob detection. The raster is validated before this code sees
 * it; baseline tracking, connected-component labeling and slot reporting are
 * deliberately local to the beta input device. */

/* ── Module parameters ──────────────────────────────────────────
 * Stable controls: modify probe behaviour or runtime operation.
 * Experimental: device/firmware-specific; may become sysfs or be
 * removed once validated. Screen calibration: needed per-unit. */

/* ── Screen calibration ────────────────────────────────────────── */
static bool debug_coords;
module_param(debug_coords, bool, 0444);
MODULE_PARM_DESC(debug_coords, "(unused, reserved for future calibration debugging)");

static bool invert_x;
module_param(invert_x, bool, 0644);
MODULE_PARM_DESC(invert_x, "Invert X axis");

static bool invert_y;
module_param(invert_y, bool, 0644);
MODULE_PARM_DESC(invert_y, "Invert Y axis");

static bool swap_xy;
module_param(swap_xy, bool, 0644);
MODULE_PARM_DESC(swap_xy, "Swap X and Y axes");

/* ── Operating mode ────────────────────────────────────────────── */
/* Deprecated compatibility knob. It is intentionally inert: switching modes
 * is unsafe, while raw_capture_only observes a frame only if one already arrives. */
static bool raw_mode = true;
module_param(raw_mode, bool, 0644);
MODULE_PARM_DESC(raw_mode,
	"Deprecated and inert: raw mode transitions are unsupported");

static bool raw_capture_only;
module_param(raw_capture_only, bool, 0444);
MODULE_PARM_DESC(raw_capture_only,
	"Passively retain complete 0x0c V0 bodies that already arrive; never sends raw-mode commands");

static bool raw_input_beta;
module_param(raw_input_beta, bool, 0444);
MODULE_PARM_DESC(raw_input_beta,
	"Beta: report multitouch from passively arriving validated 0x0c CapImg frames; requires raw_capture_only=1");

module_param(raw_transition_once, bool, 0444);
MODULE_PARM_DESC(raw_transition_once,
	"DANGEROUS: once per boot, send only the observed GET_FEATURE selector 4; requires raw_capture_only=1");

module_param(isolated_set_test, bool, 0444);
MODULE_PARM_DESC(isolated_set_test,
	"DANGEROUS: boot-time-only GET ID6 then one exact SET observation; requires raw_capture_only=1 and raw_transition_once=0");

/* Standard discovery consumes the ACPI-provided reset response without
 * changing the device power state. */
static bool acpi_probe_power_cycle;
module_param(acpi_probe_power_cycle, bool, 0644);
MODULE_PARM_DESC(acpi_probe_power_cycle,
	"Power-cycle ACPI _PS3->_PS0 at probe (disabled by default; experimental)");

/* Legacy raw-handshake tuning controls. */
static uint setfeat_speed_hz;
module_param(setfeat_speed_hz, uint, 0644);
MODULE_PARM_DESC(setfeat_speed_hz,
	"Override SPI clock speed (Hz) for the SET_FEATURE write only; 0 = bus default (33.33MHz)");

static bool setfeat_no_double;
module_param(setfeat_no_double, bool, 0644);
MODULE_PARM_DESC(setfeat_no_double,
	"Send SET_FEATURE without the leading-opcode-doubling quirk (14 bytes instead of 15)");

static bool skip_getfeat;
module_param(skip_getfeat, bool, 0644);
MODULE_PARM_DESC(skip_getfeat,
	"Skip GET_FEATURE, send SET_FEATURE directly after RPT_DESC");

static int probe_raw_id;
module_param(probe_raw_id, int, 0644);
MODULE_PARM_DESC(probe_raw_id,
	"Legacy raw feature selector (unused while raw_mode is disabled)");

module_param(debug_level, int, 0644);
MODULE_PARM_DESC(debug_level, "Log verbosity: 0=errors, 1=transitions, 2=per-frame, 3=full hex");

/* ── Blob detection tunables (runtime-validated) ────────────────── */

static int blob_min_weight = 1000;
module_param(blob_min_weight, int, 0644);
MODULE_PARM_DESC(blob_min_weight,
	"Minimum c590 signal rise sum across blob to consider it valid");

static int ema_alpha = 3;
module_param(ema_alpha, int, 0644);
MODULE_PARM_DESC(ema_alpha,
	"EMA smoothing factor: (prev*alpha + new) / (alpha+1)");

static int touch_threshold_min = 20;
module_param(touch_threshold_min, int, 0444);
MODULE_PARM_DESC(touch_threshold_min,
	"(unused, reserved for future adaptive filtering)");

static int touch_signal_mode = 2;
module_param(touch_signal_mode, int, 0644);
MODULE_PARM_DESC(touch_signal_mode,
	"Touch detection mode: 0=any change, 1=rise only (touch INCREASES c590), 2=percentage rise");

static int touch_threshold_pct = 10;
module_param(touch_threshold_pct, int, 0644);
MODULE_PARM_DESC(touch_threshold_pct,
	"Percentage of c590 full-scale (4000) used as the touch rise threshold when touch_signal_mode=2. Default 10% (=400); observed touch peaks reach a rise of ~2300");

static int ghost_dist = 15;
module_param(ghost_dist, int, 0644);
MODULE_PARM_DESC(ghost_dist, "Ghost rejection distance threshold (grid cells)");

static int grid_cols = 0;  /* 0 = default 72 */
module_param(grid_cols, int, 0644);
MODULE_PARM_DESC(grid_cols, "Override grid columns (0=default 72)");

static int grid_rows = 0;  /* 0 = default 48 */
module_param(grid_rows, int, 0644);
MODULE_PARM_DESC(grid_rows, "Override grid rows (0=default 48)");

static int calib_scale_x = 0;  /* 0 = use default: SCREEN_MAX/(cols-1) */
module_param(calib_scale_x, int, 0644);
MODULE_PARM_DESC(calib_scale_x, "X scale factor * 1000 (0=auto-calculate from grid dimensions)");

static int calib_scale_y = 0;
module_param(calib_scale_y, int, 0644);
MODULE_PARM_DESC(calib_scale_y, "Y scale factor * 1000 (0=auto-calculate from grid dimensions)");

static int calib_offset_x = 0;  /* screen pixel offset */
module_param(calib_offset_x, int, 0644);
MODULE_PARM_DESC(calib_offset_x, "X screen offset in logical coords (0..32767)");

static int calib_offset_y = 0;
module_param(calib_offset_y, int, 0644);
MODULE_PARM_DESC(calib_offset_y, "Y screen offset in logical coords (0..32767)");

static int blob_debounce = 3;
module_param(blob_debounce, int, 0644);
MODULE_PARM_DESC(blob_debounce, "Frames before claiming a new blob (debounce)");

static int blob_lift_frames = 2;
module_param(blob_lift_frames, int, 0644);
MODULE_PARM_DESC(blob_lift_frames, "Consecutive missed frames before lifting");

static int blob_max_distance = 3;
module_param(blob_max_distance, int, 0644);
MODULE_PARM_DESC(blob_max_distance, "Max grid distance for slot re-assignment");

/* Legacy raw-handshake timing. */
module_param(getfeat_delay_ms, int, 0644);
MODULE_PARM_DESC(getfeat_delay_ms,
	"Delay in ms between RPT_DESC and GET_FEATURE (default 5900; 0=immediate)");

static int stream_watchdog_ms = 0;  /* disabled by default, too aggressive for raw mode */
module_param(stream_watchdog_ms, int, 0644);
MODULE_PARM_DESC(stream_watchdog_ms,
	"Runtime streaming watchdog interval in ms (0=disable, Windows uses 2000)");

static int stream_watchdog_max_retries = 3;
module_param(stream_watchdog_max_retries, int, 0644);
MODULE_PARM_DESC(stream_watchdog_max_retries,
	"Max re-init retries before giving up");

/* ── Runtime recovery ──────────────────────────────────────────── */
static int poll_interval;
module_param(poll_interval, int, 0644);
MODULE_PARM_DESC(poll_interval,
	"Deprecated and ignored: IRQ is the sole input-report consumer");

struct heatmap_blob_candidate {
	u32 gx;
	u32 gy;
	u32 w;
	u8 idx;
};

/* Match real blobs to real active slots globally. Unmatched blobs are left for
 * the caller to allocate; dummy and out-of-radius edges never map to a slot. */
static void heatmap_assign_active_slots(struct spi_hid *shid,
		const struct heatmap_blob_candidate *blobs, u8 blob_count,
		u8 assigned_slot[HEATMAP_MAX_BLOBS])
{
	int cost[HEATMAP_MAX_BLOBS][HEATMAP_MAX_SLOTS + HEATMAP_MAX_BLOBS];
	int u[HEATMAP_MAX_BLOBS + 1] = {};
	int v[HEATMAP_MAX_SLOTS + HEATMAP_MAX_BLOBS + 1] = {};
	int p[HEATMAP_MAX_SLOTS + HEATMAP_MAX_BLOBS + 1] = {};
	int way[HEATMAP_MAX_SLOTS + HEATMAP_MAX_BLOBS + 1] = {};
	u8 active_slots[HEATMAP_MAX_SLOTS];
	int row_to_column[HEATMAP_MAX_BLOBS];
	u32 max_distance_squared = (u32)blob_max_distance * blob_max_distance;
	int active_count = 0;
	int columns;
	u8 i, j;

	for (i = 0; i < HEATMAP_MAX_BLOBS; i++)
		assigned_slot[i] = 0xFF;
	if (!blob_count)
		return;

	for (i = 0; i < HEATMAP_MAX_SLOTS; i++)
		if (shid->blob_slot_state[i] >= 1)
			active_slots[active_count++] = i;
	if (!active_count)
		return;

	columns = active_count + blob_count;
	for (i = 0; i < blob_count; i++) {
		for (j = 0; j < active_count; j++) {
			s32 dx = (s32)(blobs[i].gx >> 8) - (s32)(shid->blob_slot_gx[active_slots[j]] >> 8);
			s32 dy = (s32)(blobs[i].gy >> 8) - (s32)(shid->blob_slot_gy[active_slots[j]] >> 8);
			u32 distance_squared = dx * dx + dy * dy;

			cost[i][j] = distance_squared <= max_distance_squared ?
				int_sqrt(distance_squared) : 1000000;
		}
		for (; j < columns; j++)
			cost[i][j] = 1000;
		row_to_column[i] = -1;
	}

	/* Rectangular Hungarian assignment with rows <= columns. */
	for (i = 1; i <= blob_count; i++) {
		int minv[HEATMAP_MAX_SLOTS + HEATMAP_MAX_BLOBS + 1];
		bool used[HEATMAP_MAX_SLOTS + HEATMAP_MAX_BLOBS + 1] = {};
		int j0 = 0;

		for (j = 1; j <= columns; j++)
			minv[j] = 1000000;
		p[0] = i;
		do {
			int i0 = p[j0];
			int delta = 1000000;
			int j1 = 0;

			used[j0] = true;
			for (j = 1; j <= columns; j++) {
				int cur;

				if (used[j])
					continue;
				cur = cost[i0 - 1][j - 1] - u[i0] - v[j];
				if (cur < minv[j]) {
					minv[j] = cur;
					way[j] = j0;
				}
				if (minv[j] < delta) {
					delta = minv[j];
					j1 = j;
				}
			}
			for (j = 0; j <= columns; j++) {
				if (used[j]) {
					u[p[j]] += delta;
					v[j] -= delta;
				} else if (j) {
					minv[j] -= delta;
				}
			}
			j0 = j1;
		} while (p[j0]);
		do {
			int j1 = way[j0];

			p[j0] = p[j1];
			j0 = j1;
		} while (j0);
	}

	for (j = 1; j <= columns; j++)
		if (p[j])
			row_to_column[p[j] - 1] = j - 1;
	for (i = 0; i < blob_count; i++)
		if (row_to_column[i] >= 0 && row_to_column[i] < active_count &&
		    cost[i][row_to_column[i]] < 1000000)
			assigned_slot[i] = active_slots[row_to_column[i]];
}

static void release_all_slots(struct input_dev *input, u8 *slot_state,
		unsigned int max_slots)
{
	bool had_touch = false;

	for (unsigned int s = 0; s < max_slots; s++) {
		if (slot_state[s] >= 1) {
			input_mt_slot(input, s);
			input_mt_report_slot_state(input, MT_TOOL_FINGER, false);
			had_touch = true;
		}
	}
	if (had_touch) {
		input_mt_sync_frame(input);
		input_report_key(input, BTN_TOUCH, 0);
		input_sync(input);
	}
}

static void heatmap_reset_baseline(struct spi_hid *shid)
{
	if (shid->touch_input)
		release_all_slots(shid->touch_input, shid->blob_slot_state,
				  HEATMAP_MAX_SLOTS);

	shid->heatmap_have_baseline = false;
	shid->heatmap_baseline_cells = 0;
	shid->heatmap_baseline_frames = 0;
	memset(shid->heatmap_baseline, 0, sizeof(shid->heatmap_baseline));
	memset(shid->heatmap_frame_persistence, 0, sizeof(shid->heatmap_frame_persistence));
	memset(shid->blob_slot_state, 0, sizeof(shid->blob_slot_state));
	memset(shid->blob_slot_duration, 0, sizeof(shid->blob_slot_duration));
	memset(shid->blob_slot_gx, 0, sizeof(shid->blob_slot_gx));
	memset(shid->blob_slot_gy, 0, sizeof(shid->blob_slot_gy));
	memset(shid->blob_slot_weight, 0, sizeof(shid->blob_slot_weight));
	memset(shid->blob_slot_missed, 0, sizeof(shid->blob_slot_missed));
	memset(shid->eigmaj, 0, sizeof(shid->eigmaj));
	memset(shid->eigmin, 0, sizeof(shid->eigmin));
	memset(shid->eigori, 0, sizeof(shid->eigori));
}

/* Beta mapping uses a 72-column by 48-row raster and the logical input range. */
#define GRID_COLS_DEFAULT   72     /* default, overridden by auto-detect or module param */
#define GRID_ROWS_DEFAULT   48
#define GRID_CELLS_DEFAULT  (GRID_COLS_DEFAULT * GRID_ROWS_DEFAULT)  /* 3456 */
#define GRID_CELLS  (288 * 50)     /* max buffer allocation (headroom) */
#define GRID_ROW_STRIDE_DEFAULT GRID_COLS_DEFAULT

/* Signal lookup table (c590[256]). Actual values extracted from DLL .rdata:
 *   c590[i] = 1.0 - (i * 0.00222035 + 0.6)
 * Range: 0.4 (resting, byte=0) down to ~0 (touched, byte=255).
 * We use fixed-point: c590[i] = (s16)((1.0f - (i*0.00222035f + 0.6f)) * 10000.0f)
 * giving values 4000..0 with 4 decimal digits of precision. */
#define C590_BASE   10000   /* 1.0 * 10000 */
#define C590_STEP   22      /* 0.00222035 * 10000 ≈ 22 */
#define C590_OFFSET 6000    /* 0.6 * 10000 */

static void heatmap_init_c590(struct spi_hid *shid)
{
	int i;
	for (i = 0; i < 256; i++) {
		s32 v = 10000 - ((s32)i * 22 + 6000);
		shid->c590_lut[i] = (s16)(v > 0 ? v : 0);
	}
	seq_dbg(shid, 1, "HEATMAP: c590 lookup table initialized (range %d..%d)\n",
		(int)shid->c590_lut[0], (int)shid->c590_lut[255]);
}

/* Fixed-point atan2 approximation. Returns angle in degrees * 100.
 * Uses rational approximation: atan2(y,x) ≈ (y * 18000) / (PI * (abs(x) + abs(y) + 1))
 * with quadrant correction. Range: [-18000, 18000] (i.e., [-180.00°, 180.00°]). */
static s32 atan2_approx(s32 y, s32 x)
{
	s32 ax = x < 0 ? -x : x;
	s32 ay = y < 0 ? -y : y;
	s32 angle;

	if (ax + ay == 0)
		return 0;

	angle = (s32)(((s64)ay * 9000) / (ax + ay));

	if (x < 0)
		angle = 18000 - angle;
	if (y < 0)
		angle = -angle;

	return angle;
}

static void spi_hid_stream_watchdog_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(to_delayed_work(work), struct spi_hid, stream_watchdog);
	struct device *dev = &shid->spi->dev;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || !shid->stream_watchdog_active)
		goto out;
	if (shid->seq_state != 4)
		goto out;

	if (shid->stat_data != shid->stream_watchdog_data) {
		shid->stream_watchdog_data = shid->stat_data;
		shid->stream_watchdog_misses = 0;
		goto resched;
	}

	shid->stream_watchdog_misses++;
	dev_warn(dev, "SEQ: stream watchdog: no data for %d interval(s), miss %d/%d\n",
		 shid->stream_watchdog_misses,
		 shid->stream_watchdog_misses, 3);

	if (shid->stream_watchdog_misses >= 3) {
		if (shid->stream_watchdog_reinits < stream_watchdog_max_retries) {
			dev_warn(dev, "SEQ: stream watchdog: triggering re-init %d/%d\n",
				 shid->stream_watchdog_reinits + 1,
				 stream_watchdog_max_retries);
			shid->stream_watchdog_reinits++;
			shid->stream_watchdog_misses = 0;

			shid->raw_handshake_confirmed = false;
			shid->raw_handshake_retries_left = 3;
			spi_hid_seq_set_state(shid, 0, SPI_HID_SEQ_WATCHDOG);
			shid->seq_enabled = true;
			heatmap_reset_baseline(shid);

			{
			spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0);
			spi_hid_seq_set_state(shid, 1, SPI_HID_SEQ_WATCHDOG);
		}
		} else {
			dev_err(dev, "SEQ: stream watchdog: max retries reached, giving up\n");
			shid->stream_watchdog_active = false;
			goto out;
		}
	}

resched:
	if (shid->stream_watchdog_active)
		schedule_delayed_work(&shid->stream_watchdog,
				      msecs_to_jiffies(stream_watchdog_ms));
out:
	mutex_unlock(&shid->seq_lock);
}

static void spi_hid_poll_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(to_delayed_work(work), struct spi_hid, poll_work);
	u8 hdr[9];
	int type, ret, hdr_off;
	u16 blen;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing))
		goto out;
	if (!shid->poll_active || shid->seq_state != 4)
		goto resched;
	if (!shid->raw_handshake_confirmed)
		goto resched;

	ret = spi_hid_seq_read(shid, hdr, sizeof(hdr));
	if (ret)
		goto resched;

	type = spi_hid_seq_hdr_type(hdr, sizeof(hdr), &hdr_off);
	if (type >= 0 && hdr_off != 5) {
		seq_dbg(shid, 1, "SEQ: poller header at unexpected offset %d\n", hdr_off);
		shid->poll_missed++;
		goto resched;
	}
	if (type == 1 && (shid->hid ||
			(shid->raw_mode_active && shid->touch_input))) {
		u32 cap = shid->desc.max_input_length ?
			  shid->desc.max_input_length : 0x1000;

		blen = (((hdr[6] >> 4) & 0xF) << 0) | (hdr[7] << 4);
		blen *= 4;

		{
			u32 rblen = (blen + 5) < cap ? (blen + 5) : cap;
			u32 avail;
			u16 rl;
			struct spi_hid_protocol_content content;

			rblen = min_t(u32, rblen, shid->data_buf_len);
			avail = (rblen > 8) ? (rblen - 8) : 0;

			ret = spi_hid_seq_read(shid, shid->data_buf, rblen);
			if (ret)
				goto resched;

			if (rblen < 9)
				goto resched;
			rl = shid->data_buf[5] | (shid->data_buf[6] << 8);

			shid->stat_data++;
			seq_dbg(shid, 2, "SEQ: poller cid=0x%02x len=%u\n",
				 shid->data_buf[7], rl);

			if (raw_capture_only &&
			    !spi_hid_protocol_parse_content(shid->data_buf + 5,
							     rblen - 5, &content) &&
			    content.content_id == SPI_HID_RAW_CAPTURE_CONTENT_ID) {
				spi_hid_handle_raw_body(shid, shid->data_buf + 5, rblen - 5);
			} else if (shid->raw_mode_active && shid->touch_input &&
			    !spi_hid_protocol_parse_content(shid->data_buf + 5,
							     rblen - 5, &content) &&
			    content.content_id == 0x0C) {
				heatmap_process_frame(shid, content.data,
						      content.data_length, content.content_id);
			} else if (rl > 2 && rl - 2 <= avail) {
				if (shid->hid) {
					int hret = hid_input_report(shid->hid,
						HID_INPUT_REPORT,
						&shid->data_buf[8],
						rl - 2, 1);
					if (hret)
						seq_dbg(shid, 1,
							"SEQ: poller hid_input_report failed: %d\n",
							hret);
				}
			}
			shid->poll_missed = 0;
		}
	} else if (type == 3) {
		seq_dbg(shid, 1, "SEQ: poller detected RESET_RSP, stopping poll loop\n");
		shid->poll_active = false;
		goto out;
	} else {
		shid->poll_missed++;
	}

resched:
	if (shid->poll_active)
		schedule_delayed_work(&shid->poll_work,
			      msecs_to_jiffies(shid->poll_interval_ms));
out:
	mutex_unlock(&shid->seq_lock);
}

static void heatmap_process_frame(struct spi_hid *shid, const u8 *data,
				  u32 data_len, u8 content_id)
{
	struct device *dev = &shid->spi->dev;
	u32 i, cell_count, ncols, nrows;
	int data_offset;
	int configured_cols, configured_rows;
	int touched_count = 0;

	/* The CapImg decoder supplies only the validated raster. */
	data_offset = 0;
	configured_cols = READ_ONCE(grid_cols);
	configured_rows = READ_ONCE(grid_rows);
	if (data_len != SPI_HID_CAPIMG_RASTER_SAMPLES)
		return;

	/* Clamp unsafe module parameters to prevent division by zero
	 * and undefined behavior. Values outside these ranges produce
	 * garbage output or kernel panics. */
	if (blob_min_weight < 1) blob_min_weight = 1;
	if (ema_alpha < 0 || ema_alpha > 10000) ema_alpha = 3;
	if (blob_debounce < 1) blob_debounce = 3;
	if (blob_lift_frames < 1) blob_lift_frames = 2;
	if (blob_max_distance < 1 || blob_max_distance > 1000) blob_max_distance = 3;
	if (ghost_dist < 1) ghost_dist = 15;
	if (touch_threshold_pct < 0) touch_threshold_pct = 0;
	if (touch_threshold_pct > 100) touch_threshold_pct = 100;
	/* calib_offset_x/y may be negative: final clamp to [0,SCREEN_MAX]
	 * is applied after swap/invert, so a negative shift is valid. */

	/* data contains precisely the 72 by 48 decoder-validated samples. */
	cell_count = data_len;

	/* Use the decoder-established geometry unless explicitly overridden. */
	if (!shid->heatmap_grid_cols || !shid->heatmap_grid_rows) {
		if (configured_cols > 1)
			ncols = configured_cols;
		else
			ncols = GRID_COLS_DEFAULT;
		if (configured_rows > 1)
			nrows = configured_rows;
		else if (configured_cols > 1)
			nrows = cell_count / ncols;   /* custom cols: derive rows */
		else
			nrows = GRID_ROWS_DEFAULT;
		if (ncols < 2) {
			dev_warn(dev, "HEATMAP: invalid grid columns %u, using default\n", ncols);
			ncols = GRID_COLS_DEFAULT;
		}
		if (nrows < 2) {
			dev_warn(dev, "HEATMAP: invalid grid rows %u, using default\n", nrows);
			nrows = GRID_ROWS_DEFAULT;
		}
		shid->heatmap_grid_cols = ncols;
		shid->heatmap_grid_rows = nrows;
		seq_dbg(shid, 1, "HEATMAP: grid %u cols × %u rows (offset %d, frame avail %u cells)\n",
			 ncols, nrows, data_offset, cell_count);
	}
	ncols = shid->heatmap_grid_cols;
	nrows = shid->heatmap_grid_rows;

	/* Clamp cell_count to the actual grid so footer/metadata bytes beyond
	 * the 72×48 field are never treated as sensor cells. */
	if (!ncols || nrows > HEATMAP_MAX_CELLS / ncols) {
		dev_warn(dev, "HEATMAP: invalid grid %ux%u\n", ncols, nrows);
		return;
	}
	if (cell_count > ncols * nrows)
		cell_count = ncols * nrows;

	if (cell_count > HEATMAP_MAX_CELLS) {
		dev_warn(dev, "HEATMAP: frame too large (%u cells > %u max)\n", cell_count, HEATMAP_MAX_CELLS);
		return;
	}

	/* Store raw frame for sysfs debug */
	if (!shid->heatmap_buf || shid->heatmap_len < data_len) {
		kfree(shid->heatmap_buf);
		shid->heatmap_buf = kmalloc(data_len, GFP_KERNEL);
	}
	if (shid->heatmap_buf) {
		memcpy(shid->heatmap_buf, data, data_len);
		shid->heatmap_len = data_len;
		shid->heatmap_content_id = content_id;
	}

	/* Max-tracking baseline: track the per-cell maximum raw byte index
	 * over ~30 frames to establish the resting (no-touch) reference.
	 * A higher raw byte index maps to a lower c590 signal.
	 *
	 * After baseline is established, use Exponential Moving Average (EMA)
	 * to slowly track thermal drift: baseline[i] = (baseline[i]*7 + raw)/8.
	 * This prevents single-count thermal shifts from being detected as
	 * false touches (the 60% default threshold would catch a 1-count shift
	 * at 62 vs baseline threshold 64, but EMA further smooths this). */
	if (!shid->heatmap_have_baseline) {
		if (content_id == 0x0C && cell_count >= 1000) {
			shid->heatmap_baseline_frames++;
			for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
				u8 raw = data[data_offset + i];
				if (shid->heatmap_baseline_frames == 1 || raw > shid->heatmap_baseline[i])
					shid->heatmap_baseline[i] = raw;
			}
			if (shid->heatmap_baseline_frames >= 30) {
				shid->heatmap_baseline_cells = cell_count;
				shid->heatmap_have_baseline = true;
				seq_dbg(shid, 1, "HEATMAP: baseline stabilized after %u frames (%u cells)\n",
					 shid->heatmap_baseline_frames, cell_count);
			}
		}
		return;
	}
	/* Asymmetric baseline tracking. Resting raw is the per-cell maximum
	 * (a touch only *lowers* the raw byte). So:
	 *  - raw >= baseline: recover toward resting quickly (EMA 12.5%), which
	 *    also absorbs slow upward thermal drift;
	 *  - raw <  baseline: this is (probably) an active touch — do NOT let the
	 *    baseline chase it down, otherwise a held finger fades into the
	 *    baseline within a few frames and stops being detected. Allow only a
	 *    very slow decay (~1 count / many frames) to track genuine downward
	 *    thermal drift without swallowing touches. */
	for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
		u8 base = shid->heatmap_baseline[i];
		u8 raw = data[data_offset + i];

		if (raw >= base) {
			u16 cur = (u16)base * 7 + (u16)raw;
			shid->heatmap_baseline[i] = (u8)(cur / 8);
		} else if (shid->heatmap_frame_persistence[i] == 0 && base > 0) {
			/* untouched-but-lower: rare; nudge down 1 count slowly */
			shid->heatmap_baseline[i] = base - 1;
		}
	}

	if (!shid->touch_input) return;

	/* Surface frame-gap reset: if >5 frames skipped, release all slots.
	 * FUN_180600ad0 + FUN_1805ffd00:107-112. */
	{
		u32 frame_id = shid->heatmap_frame_id + 1;
		s32 gap = (s32)frame_id - (s32)shid->heatmap_frame_id;
		if (shid->heatmap_frame_id && gap > 5) {
			for (i = 0; i < HEATMAP_MAX_SLOTS; i++)
				shid->blob_slot_state[i] = 0;
			memset(shid->blob_slot_duration, 0, sizeof(shid->blob_slot_duration));
			memset(shid->blob_slot_missed, 0, sizeof(shid->blob_slot_missed));
			memset(shid->blob_slot_hpos, 0, sizeof(shid->blob_slot_hpos));
		}
		shid->heatmap_frame_id = frame_id;
	}

/* Surface peak detection (FUN_1805fba00): find local maxima with 5-cell
 * neighbour radius, using c590 fixed-point signal rise.
 * Minimum peak rise threshold 500 filters noise (c590 ~22-326),
 * keeping genuine touch signals (c590 ~1448-1912). */
	#define PEAK_MAX_CANDIDATES 16
	#define PEAK_MIN_RISE 500
	{
		struct { u32 cell; u32 rise; } peaks[PEAK_MAX_CANDIDATES];
		u32 peak_count = 0;
		u32 pi;

		for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
			u8 raw = data[data_offset + i];
			s16 curr = shid->c590_lut[raw];
			s16 base = shid->c590_lut[shid->heatmap_baseline[i]];
			s32 rise = (s32)curr - (s32)base;
			u32 col, row;
			bool is_peak;

			if (rise < PEAK_MIN_RISE)
				continue;
			col = i % ncols;
			row = i / ncols;

			/* 5-cell neighbour comparison */
			is_peak = true;
			if (col >= 5) {
				u32 ni = i - 5;
				s16 nc = shid->c590_lut[data[data_offset + ni]];
				s16 nb = shid->c590_lut[shid->heatmap_baseline[ni]];
				if ((s32)nc - (s32)nb > rise)
					is_peak = false;
			}
			if (is_peak && col + 5 < ncols) {
				u32 ni = i + 5;
				s16 nc = shid->c590_lut[data[data_offset + ni]];
				s16 nb = shid->c590_lut[shid->heatmap_baseline[ni]];
				if ((s32)nc - (s32)nb > rise)
					is_peak = false;
			}
			if (is_peak && row >= 5) {
				u32 ni = i - 5 * ncols;
				s16 nc = shid->c590_lut[data[data_offset + ni]];
				s16 nb = shid->c590_lut[shid->heatmap_baseline[ni]];
				if ((s32)nc - (s32)nb > rise)
					is_peak = false;
			}
			if (is_peak && row + 5 < nrows) {
				u32 ni = i + 5 * ncols;
				s16 nc = shid->c590_lut[data[data_offset + ni]];
				s16 nb = shid->c590_lut[shid->heatmap_baseline[ni]];
				if ((s32)nc - (s32)nb > rise)
					is_peak = false;
			}

			if (is_peak) {
				/* Insertion sort by rise descending */
				u32 pos;
				for (pos = peak_count; pos > 0 && peaks[pos-1].rise < (u32)rise; pos--)
					peaks[pos] = peaks[pos-1];
				peaks[pos].cell = i;
				peaks[pos].rise = (u32)rise;
				if (peak_count < PEAK_MAX_CANDIDATES)
					peak_count++;
			}
		} /* end peak scan */

		/* Surface Stage 9e centroid (FUN_180602e60): signal-weighted
		 * over full connected component via flood-fill from each peak.
		 * Noise-floor subtraction (baseline c590), per-blob eigenellipsis. */
		memset(shid->blob_active, 0, sizeof(shid->blob_active));
		memset(shid->heatmap_label, 0, cell_count * sizeof(shid->heatmap_label[0]));
		memset(shid->eigmaj, 0, sizeof(shid->eigmaj));
		memset(shid->eigmin, 0, sizeof(shid->eigmin));
		memset(shid->eigori, 0, sizeof(shid->eigori));
		for (pi = 0; pi < peak_count; pi++) {
			u32 pcell = peaks[pi].cell;
			u32 queue[HEATMAP_MAX_CELLS];
			u32 qhead = 0, qtail = 0;
			u64 fxsum = 0, fysum = 0, fwsum = 0;
			u16 blob_label = (u16)(pi + 1);
			s32 blob_cx_cell, blob_cy_cell;
			s64 sum_xx = 0, sum_yy = 0, sum_xy = 0;
			s32 blob_pixel_count = 0;

			if (shid->heatmap_label[pcell] != 0)
				continue;

			queue[qtail++] = pcell;
			shid->heatmap_label[pcell] = blob_label;

			while (qhead < qtail) {
				u32 ci = queue[qhead++];
				s32 c = (s32)(ci % ncols);
				s32 r = (s32)(ci / ncols);
				s16 curr = shid->c590_lut[data[data_offset + ci]];
				s16 base = shid->c590_lut[shid->heatmap_baseline[ci]];
				s32 rise = (s32)curr - (s32)base;

				if (rise > 0) {
					fxsum += (u64)c * (u64)rise * 256;
					fysum += (u64)r * (u64)rise * 256;
					fwsum += (u64)rise;
					blob_pixel_count++;

					if (c > 0) { u32 ni = ci - 1; if (!shid->heatmap_label[ni]) { shid->heatmap_label[ni] = blob_label; queue[qtail++] = ni; } }
					if (c + 1 < (s32)ncols) { u32 ni = ci + 1; if (!shid->heatmap_label[ni]) { shid->heatmap_label[ni] = blob_label; queue[qtail++] = ni; } }
					if (r > 0) { u32 ni = ci - ncols; if (!shid->heatmap_label[ni]) { shid->heatmap_label[ni] = blob_label; queue[qtail++] = ni; } }
					if (r + 1 < (s32)nrows) { u32 ni = ci + ncols; if (!shid->heatmap_label[ni]) { shid->heatmap_label[ni] = blob_label; queue[qtail++] = ni; } }
				}
			}

			if (fwsum == 0 || fwsum < (u32)blob_min_weight)
				continue;

			shid->blob_x[pi] = (u32)(fxsum / fwsum);
			shid->blob_y[pi] = (u32)(fysum / fwsum);
			shid->blob_wsum[pi] = (u32)fwsum;
			shid->blob_active[pi] = true;

			if (blob_pixel_count < 2)
				continue;

			blob_cx_cell = (s32)(shid->blob_x[pi] >> 8);
			blob_cy_cell = (s32)(shid->blob_y[pi] >> 8);

			for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
				s32 rise, dx, dy;
				s16 base, curr;

				if (shid->heatmap_label[i] != blob_label)
					continue;
				base = shid->c590_lut[shid->heatmap_baseline[i]];
				curr = shid->c590_lut[data[data_offset + i]];
				rise = (s32)curr - (s32)base;
				if (rise <= 0)
					continue;

				dx = (s32)(i % (u32)ncols) - blob_cx_cell;
				dy = (s32)(i / (u32)ncols) - blob_cy_cell;
				sum_xx += (s64)dx * (s64)dx * (s64)rise;
				sum_yy += (s64)dy * (s64)dy * (s64)rise;
				sum_xy += (s64)dx * (s64)dy * (s64)rise;
			}

			{
				s64 trace = sum_xx + sum_yy;
				s64 det = sum_xx * sum_yy - sum_xy * sum_xy;
				s64 disc = trace * trace - 4 * det;
				s32 sq;

				if (disc < 0)
					disc = 0;
				sq = (s32)int_sqrt((u64)disc);

				shid->eigmaj[pi] = (s32)((trace + sq) / 2);
				shid->eigmin[pi] = (s32)((trace - sq) / 2);

				{
					s32 num = 2 * sum_xy;
					s32 den = sum_yy - sum_xx;
					s32 deg;

					if (den != 0 || num != 0) {
						deg = atan2_approx(num, den) / 2;
						if (deg >= 18000)
							deg -= 18000;
						else if (deg <= -18000)
							deg += 18000;
						shid->eigori[pi] = deg;
					} else {
						shid->eigori[pi] = 0;
					}
				}
			}
		}
		touched_count = peak_count;
	}

	/* Emit multitouch events with smoothing and slot tracking. */
	{
		struct input_dev *input = shid->touch_input;
		bool any_touch = false;
		struct heatmap_blob_candidate sorted[HEATMAP_MAX_BLOBS];
		u8 sorted_count = 0;
		const u32 SCREEN_MAX = 32767;
		u32 scale_x, scale_y;
		u32 screen_x_cells = swap_xy ? nrows : ncols;
		u32 screen_y_cells = swap_xy ? ncols : nrows;

		if (calib_scale_x > 0)
			scale_x = (u32)calib_scale_x;
		else
			scale_x = (SCREEN_MAX * 1000) / (screen_x_cells - 1);

		if (calib_scale_y > 0)
			scale_y = (u32)calib_scale_y;
		else
			scale_y = (SCREEN_MAX * 1000) / (screen_y_cells - 1);

		for (i = 0; i < HEATMAP_MAX_BLOBS; i++) {
			if (!shid->blob_active[i] || shid->blob_wsum[i] < blob_min_weight)
				continue;
			sorted[sorted_count].gx = shid->blob_x[i];
			sorted[sorted_count].gy = shid->blob_y[i];
			sorted[sorted_count].w = shid->blob_wsum[i];
			sorted[sorted_count].idx = i;
			sorted_count++;
		}
		for (i = 0; i + 1 < sorted_count; i++)
			for (u8 j = i + 1; j < sorted_count; j++)
				if (sorted[j].w > sorted[i].w) {
					struct heatmap_blob_candidate t = sorted[i];
					sorted[i] = sorted[j]; sorted[j] = t;
				}
		if (sorted_count > HEATMAP_MAX_SLOTS)
			sorted_count = HEATMAP_MAX_SLOTS;

		for (i = 0; i < sorted_count; i++) {
			u32 screen_gx = swap_xy ? sorted[i].gy >> 8 : sorted[i].gx >> 8;
			u32 screen_gy = swap_xy ? sorted[i].gx >> 8 : sorted[i].gy >> 8;

			seq_dbg(shid, 2, "CALIB: blob[%u] grid=(%u,%u) screen=(%u,%u) weight=%u scale=(%ux%u)\n",
				 i, sorted[i].gx >> 8, sorted[i].gy >> 8,
				 (screen_gx * scale_x + 500) / 1000,
				 (screen_gy * scale_y + 500) / 1000,
				 sorted[i].w, scale_x, scale_y);
		}
		if (sorted_count || touched_count)
			seq_dbg(shid, 2, "CALIB: blobs=%u cells_touched=%d\n",
				 sorted_count, touched_count);

		/* Pre-assignment blob merging: Surface coalesces within 6 cells. */
		{
			u8 a, b, j;

			for (a = 0; a < sorted_count; a++) {
				if (sorted[a].w == 0)
					continue;
				for (b = a + 1; b < sorted_count; b++) {
					s32 dx, dy;

					if (sorted[b].w == 0)
						continue;
					dx = (s32)(sorted[a].gx >> 8) - (s32)(sorted[b].gx >> 8);
					dy = (s32)(sorted[a].gy >> 8) - (s32)(sorted[b].gy >> 8);
					/* Surface coalesces within 6 grid cells (dx^2+dy^2 < 36). */
					if (dx * dx + dy * dy < 36) {
						if (sorted[b].w > sorted[a].w) {
							sorted[a].w = 0;
							break;
						} else {
							sorted[b].w = 0;
					}
					}
				}
			}
			j = 0;
			for (a = 0; a < sorted_count; a++) {
				if (sorted[a].w > 0) {
					if (a != j)
						sorted[j] = sorted[a];
					j++;
				}
			}
			sorted_count = j;
		}

		/* Large merged blobs are not split in the beta path. */

		{
			u8 assigned_slot[HEATMAP_MAX_BLOBS];
			u32 new_gx[HEATMAP_MAX_SLOTS], new_gy[HEATMAP_MAX_SLOTS];
			bool new_active[HEATMAP_MAX_SLOTS];

			memset(new_active, 0, sizeof(new_active));
			memset(new_gx, 0, sizeof(new_gx));
			memset(new_gy, 0, sizeof(new_gy));
			for (i = 0; i < sorted_count; i++)
				assigned_slot[i] = 0xFF;

			heatmap_assign_active_slots(shid, sorted, sorted_count, assigned_slot);
			for (i = 0; i < sorted_count; i++) {
				if (assigned_slot[i] != 0xFF)
					continue;
				for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
					bool taken = false;
					u8 k;

					if (shid->blob_slot_state[s] != 0)
						continue;
					for (k = 0; k < i; k++)
						if (assigned_slot[k] == s) {
							taken = true;
							break;
						}
					if (!taken) {
						assigned_slot[i] = s;
						break;
					}
				}
			}

			/* States: empty, new, claimed, lift. */
			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				u8 bi = 0xFF;

				for (i = 0; i < sorted_count; i++) {
					if (assigned_slot[i] == s) { bi = i; break; }
				}

				if (bi != 0xFF) {
					u32 gx = sorted[bi].gx;
					u32 gy = sorted[bi].gy;
					u32 w  = sorted[bi].w;
					bool was_claimed = (shid->blob_slot_state[s] >= 2);

					switch (shid->blob_slot_state[s]) {
					case 0:
						shid->blob_slot_state[s] = 1;
						shid->blob_slot_duration[s] = 1;
						shid->blob_slot_hpos[s] = 0;
						shid->blob_slot_hcount[s] = 0;
						memset(shid->blob_slot_hx[s], 0, sizeof(shid->blob_slot_hx[s]));
						memset(shid->blob_slot_hy[s], 0, sizeof(shid->blob_slot_hy[s]));
						break;
					case 1:
						shid->blob_slot_duration[s]++;
						if (shid->blob_slot_duration[s] >= (u32)blob_debounce) {
							u32 hmin = gx, hmax = gx;
							u8 hc = shid->blob_slot_hcount[s];
							u8 hp;
							for (hp = 0; hp < hc; hp++) {
								u32 hx = shid->blob_slot_hx[s][hp];
								if (hx && hx < hmin) hmin = hx;
								if (hx > hmax) hmax = hx;
							}
							if (hmax - hmin <= 768)
								shid->blob_slot_state[s] = 2;
							else
								shid->blob_slot_duration[s] = 1;
						}
						break;
					case 2:
						shid->blob_slot_duration[s]++;
						break;
					case 3:
						shid->blob_slot_state[s] = 1;
						shid->blob_slot_duration[s] = 1;
						break;
					}
					shid->blob_slot_blob[s] = sorted[bi].idx;
					/* Push to history ring (10-entry circular buffer). */
				{
						u8 hp = shid->blob_slot_hpos[s];
						shid->blob_slot_hx[s][hp] = gx;
						shid->blob_slot_hy[s][hp] = gy;
						shid->blob_slot_hpos[s] = (hp + 1) % SLOT_HISTORY_DEPTH;
						if (shid->blob_slot_hcount[s] < SLOT_HISTORY_DEPTH)
							shid->blob_slot_hcount[s]++;
					}
					shid->blob_slot_missed[s] = 0;
					/* Surface stationary detection (FUN_180608000):
					 * lock position if movement < 1 cell for 5+ frames. */
					{
						u32 ox = shid->blob_slot_gx[s];
						u32 oy = shid->blob_slot_gy[s];
						u32 dx = gx > ox ? gx - ox : ox - gx;
						u32 dy = gy > oy ? gy - oy : oy - gy;
						if (dx <= 256 && dy <= 256 &&
						    shid->blob_slot_state[s] == 2)
							shid->blob_slot_stationary[s]++;
						else
							shid->blob_slot_stationary[s] = 0;
					}
					/* Surface EMA weight filter (FUN_180608000):
					 * weight[s] = (1-α) * old + α * new, α=1/8. */
					shid->blob_slot_weight[s] = (shid->blob_slot_weight[s] * 7 + w) / 8;
					shid->blob_slot_gx[s] = gx;
					shid->blob_slot_gy[s] = gy;

					new_active[s] = (shid->blob_slot_state[s] >= 2);

					if (new_active[s]) {
						if (was_claimed && shid->blob_slot_state[s] == 2) {
							u8 hc = shid->blob_slot_hcount[s];
							u8 n = hc > 3 ? 3 : (hc > 0 ? hc : 1);
							u8 hp = shid->blob_slot_hpos[s];
							u64 sx = 0, sy = 0;
							u8 k;
							for (k = 0; k < n; k++) {
								u8 idx = (hp + SLOT_HISTORY_DEPTH - 1 - k) % SLOT_HISTORY_DEPTH;
								sx += shid->blob_slot_hx[s][idx];
								sy += shid->blob_slot_hy[s][idx];
							}
							new_gx[s] = (u32)(sx / n);
							new_gy[s] = (u32)(sy / n);
						} else {
							new_gx[s] = gx;
							new_gy[s] = gy;
						}
						/* Lock position when stationary (Surface still detection). */
						if (shid->blob_slot_stationary[s] >= 5) {
							new_gx[s] = shid->blob_slot_gx[s];
							new_gy[s] = shid->blob_slot_gy[s];
						}
						shid->blob_slot_gx[s] = new_gx[s];
						shid->blob_slot_gy[s] = new_gy[s];
					}
				} else {
					switch (shid->blob_slot_state[s]) {
					case 1:
						shid->blob_slot_state[s] = 0;
						shid->blob_slot_duration[s] = 0;
						new_active[s] = false;
						break;
					case 2:
						shid->blob_slot_missed[s]++;
						if (shid->blob_slot_missed[s] >= (u32)blob_lift_frames) {
							shid->blob_slot_state[s] = 3;
							shid->blob_slot_missed[s] = 0;
						}
						new_active[s] = true;
						new_gx[s] = shid->blob_slot_gx[s];
						new_gy[s] = shid->blob_slot_gy[s];
						break;
				case 3:
					shid->blob_slot_missed[s]++;
					/* Surface lift history lookback (FUN_180601dd0):
					 * emit lift at position from N frames ago. */
				{
						u8 hc = shid->blob_slot_hcount[s];
						u8 hp = shid->blob_slot_hpos[s];
							u8 lb = shid->blob_slot_missed[s];
							if (lb > hc || lb < 1)
								lb = (hc > 0) ? 1 : 0;
							if (lb > 0) {
								u8 idx = (hp + SLOT_HISTORY_DEPTH - lb) % SLOT_HISTORY_DEPTH;
								new_gx[s] = shid->blob_slot_hx[s][idx];
								new_gy[s] = shid->blob_slot_hy[s][idx];
							} else {
								new_gx[s] = shid->blob_slot_gx[s];
								new_gy[s] = shid->blob_slot_gy[s];
							}
						}
						new_active[s] = true;
						if (shid->blob_slot_missed[s] >= 2) {
							shid->blob_slot_state[s] = 0;
							shid->blob_slot_missed[s] = 0;
							new_active[s] = false;
						}
						break;
					case 0:
						new_active[s] = false;
						break;
					}
		}
	}

	/* Surface post-emission coalescence (FUN_1806025c0, Stage 10):
		 * suppress weaker contacts within 6 cells (squared dist < 36). */
		{
			u8 sa, sb;
			for (sa = 0; sa < HEATMAP_MAX_SLOTS; sa++) {
				if (!new_active[sa])
					continue;
				for (sb = sa + 1; sb < HEATMAP_MAX_SLOTS; sb++) {
					s32 dx, dy;
					if (!new_active[sb])
						continue;
					dx = (s32)(new_gx[sa] >> 8) - (s32)(new_gx[sb] >> 8);
					dy = (s32)(new_gy[sa] >> 8) - (s32)(new_gy[sb] >> 8);
					if (dx * dx + dy * dy < 36) {
						if (shid->blob_slot_weight[sa] <
						    shid->blob_slot_weight[sb])
							new_active[sa] = false;
						else
							new_active[sb] = false;
					}
				}
			}
		}

			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				input_mt_slot(input, s);
				input_mt_report_slot_state(input, MT_TOOL_FINGER, new_active[s]);
				if (new_active[s]) {
					s64 fx, fy;
					u32 raw_gx = swap_xy ? new_gy[s] : new_gx[s];
					u32 raw_gy = swap_xy ? new_gx[s] : new_gy[s];
					u32 cell_x = raw_gx >> 8, frac_x = raw_gx & 0xFF;
					u32 cell_y = raw_gy >> 8, frac_y = raw_gy & 0xFF;

					fx = ((s64)cell_x * scale_x + 500) / 1000;
					fx += ((s64)frac_x * scale_x) / (1000 * 256);
					fy = ((s64)cell_y * scale_y + 500) / 1000;
					fy += ((s64)frac_y * scale_y) / (1000 * 256);
					fx += calib_offset_x;
					fy += calib_offset_y;
					if (invert_x) fx = (s64)SCREEN_MAX - fx;
					if (invert_y) fy = (s64)SCREEN_MAX - fy;
					if (fx < 0) fx = 0;
					if (fy < 0) fy = 0;
					if (fx > SCREEN_MAX) fx = SCREEN_MAX;
					if (fy > SCREEN_MAX) fy = SCREEN_MAX;
					input_report_abs(input, ABS_MT_POSITION_X, (u16)fx);
					input_report_abs(input, ABS_MT_POSITION_Y, (u16)fy);
					any_touch = true;

				/* Emit per-slot eigenellipsis from flood-fill blob. */
				{
					u8 bi = shid->blob_slot_blob[s];
					if (shid->blob_active[bi] && shid->eigmaj[bi] > 0) {
						u32 major, minor;
						s32 ori;

						/* Anisotropic ellipse: eigenvalues are in
						 * grid-cell² units; project the std-dev onto
						 * screen units using the per-axis scale so the
						 * major/minor reflect the real X/Y pitch. */
						major = ((u32)int_sqrt(shid->eigmaj[bi]) * scale_x + 500) / 1000;
						minor = ((u32)int_sqrt(shid->eigmin[bi]) * scale_y + 500) / 1000;
						ori = shid->eigori[bi] / 100;
						if (swap_xy) {
							u32 t = major; major = minor; minor = t;
							ori = -ori;
						}
						if (major < 1)
							major = 1;
						if (minor < 1)
							minor = 1;
						if (major > SCREEN_MAX)
							major = SCREEN_MAX;
						if (minor > SCREEN_MAX)
							minor = SCREEN_MAX;
						if (ori > 89) ori = 89;
						if (ori < -89) ori = -89;
						input_report_abs(input, ABS_MT_TOUCH_MAJOR, major);
						input_report_abs(input, ABS_MT_TOUCH_MINOR, minor);
						input_report_abs(input, ABS_MT_ORIENTATION, ori);
					}
				}
			}
			}
		}

		input_mt_sync_frame(input);
		input_report_key(input, BTN_TOUCH, any_touch ? 1 : 0);
		input_sync(input);
	}
}

static ssize_t heatmap_debug_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	u32 off = 0, i;
	u32 snapshot_len;
	u32 snapshot_cid;
	u16 snapshot_cols, snapshot_rows;

	mutex_lock(&shid->seq_lock);
	if (!shid->heatmap_buf || !shid->heatmap_len) {
		mutex_unlock(&shid->seq_lock);
		return sysfs_emit(buf, "no frame captured\n");
	}
	snapshot_len = shid->heatmap_len;
	snapshot_cid = shid->heatmap_content_id;
	snapshot_cols = shid->heatmap_grid_cols;
	snapshot_rows = shid->heatmap_grid_rows;
	off += sysfs_emit_at(buf, off,
		"content_id=0x%02x len=%u cells=%u grid=%ux%u\n",
		snapshot_cid, snapshot_len,
		snapshot_len,
		snapshot_cols, snapshot_rows);
	for (i = 0; i < snapshot_len && off < PAGE_SIZE - 4; i += 32) {
		u32 chunk = min_t(u32, 32, snapshot_len - i);
		off += sysfs_emit_at(buf, off,
			"%04x: %*ph\n", i, chunk, shid->heatmap_buf + i);
	}
	mutex_unlock(&shid->seq_lock);
	return off;
}
static DEVICE_ATTR_RO(heatmap_debug);

static void spi_hid_raw_capture_frame(struct spi_hid *shid, const u8 *body,
		unsigned int body_length)
{
	struct spi_hid_protocol_content content;
	u32 slot;

	if (spi_hid_protocol_validate_raw_capture(body, body_length, &content)) {
		mutex_lock(&shid->raw_capture_lock);
		shid->raw_capture_invalid++;
		mutex_unlock(&shid->raw_capture_lock);
		return;
	}

	mutex_lock(&shid->raw_capture_lock);
	slot = shid->raw_capture_next;
	memcpy(shid->raw_capture_frames[slot], body, body_length);
	shid->raw_capture_count++;
	shid->raw_capture_sequence[slot] = shid->raw_capture_count;
	shid->raw_capture_timestamp_ns[slot] = ktime_get_ns();
	shid->raw_capture_next = (slot + 1) % SPI_HID_RAW_CAPTURE_SLOTS;
	mutex_unlock(&shid->raw_capture_lock);
}

static void spi_hid_handle_raw_body(struct spi_hid *shid, const u8 *body,
		unsigned int body_length)
{
	struct spi_hid_capimg_raster raster;

	spi_hid_raw_capture_frame(shid, body, body_length);
	if (!raw_input_beta || !shid->touch_input)
		return;
	if (spi_hid_capimg_decode_v0(body, body_length, &raster)) {
		mutex_lock(&shid->raw_capture_lock);
		shid->raw_input_invalid++;
		mutex_unlock(&shid->raw_capture_lock);
		return;
	}

	heatmap_process_frame(shid, raster.samples, SPI_HID_CAPIMG_RASTER_SAMPLES,
			      SPI_HID_RAW_CAPTURE_CONTENT_ID);
}

static void spi_hid_isolated_set_capture(struct spi_hid *shid, int type,
		const u8 *body, u32 body_length, u16 transport_length)
{
	struct spi_hid_isolated_set_frame *frame;
	u32 copied;

	if (shid->isolated_set_state != SPI_HID_ISOLATED_SET_OBSERVING)
		return;
	if (shid->isolated_set_count >= SPI_HID_ISOLATED_SET_RING_SLOTS)
		shid->isolated_set_overflow++;

	frame = &shid->isolated_set_ring[shid->isolated_set_next];
	frame->timestamp_ns = ktime_get_ns();
	frame->transport_length = transport_length;
	frame->body_length = min_t(u32, body_length,
		SPI_HID_ISOLATED_SET_BODY_LENGTH);
	frame->type = type;
	frame->truncated = body_length > SPI_HID_ISOLATED_SET_BODY_LENGTH;
	memcpy(frame->header, shid->isolated_set_header, sizeof(frame->header));
	copied = frame->body_length;
	if (copied)
		memcpy(frame->body, body, copied);
	if (copied < sizeof(frame->body))
		memset(frame->body + copied, 0, sizeof(frame->body) - copied);
	shid->isolated_set_count++;
	shid->isolated_set_next = (shid->isolated_set_next + 1) %
		SPI_HID_ISOLATED_SET_RING_SLOTS;
}

/* The binary file is a fixed eight-slot ring of complete 4304-byte V0 bodies.
 * Consult raw_capture_status for slot sequence numbers and timestamps. */
static ssize_t raw_capture_read(struct file *filp, struct kobject *kobj,
		const struct bin_attribute *attr, char *buf, loff_t off, size_t count)
{
	struct device *dev = kobj_to_dev(kobj);
	struct spi_hid *shid = dev_get_drvdata(dev);
	const size_t total = SPI_HID_RAW_CAPTURE_SLOTS * SPI_HID_RAW_CAPTURE_BODY_LENGTH;
	size_t copied = 0;

	if (off < 0 || off >= total)
		return 0;
	count = min(count, total - (size_t)off);

	mutex_lock(&shid->raw_capture_lock);
	while (copied < count) {
		size_t position = (size_t)off + copied;
		u32 slot = position / SPI_HID_RAW_CAPTURE_BODY_LENGTH;
		size_t slot_offset = position % SPI_HID_RAW_CAPTURE_BODY_LENGTH;
		size_t chunk = min(count - copied,
			SPI_HID_RAW_CAPTURE_BODY_LENGTH - slot_offset);

		memcpy(buf + copied, shid->raw_capture_frames[slot] + slot_offset, chunk);
		copied += chunk;
	}
	mutex_unlock(&shid->raw_capture_lock);

	return copied;
}

static struct bin_attribute raw_capture_bin_attr = {
	.attr = {
		.name = "raw_capture",
		.mode = 0400,
	},
	.size = SPI_HID_RAW_CAPTURE_SLOTS * SPI_HID_RAW_CAPTURE_BODY_LENGTH,
	.read = raw_capture_read,
};

static ssize_t isolated_set_ring_read(struct file *filp, struct kobject *kobj,
		const struct bin_attribute *attr, char *buf, loff_t off, size_t count)
{
	struct device *dev = kobj_to_dev(kobj);
	struct spi_hid *shid = dev_get_drvdata(dev);
	const size_t total = sizeof(shid->isolated_set_ring);

	if (off < 0 || off >= total)
		return 0;
	count = min(count, total - (size_t)off);
	mutex_lock(&shid->seq_lock);
	memcpy(buf, (u8 *)shid->isolated_set_ring + off, count);
	mutex_unlock(&shid->seq_lock);
	return count;
}

static struct bin_attribute isolated_set_ring_bin_attr = {
	.attr = {
		.name = "isolated_set_ring",
		.mode = 0400,
	},
	.size = SPI_HID_ISOLATED_SET_RING_SLOTS *
		sizeof(struct spi_hid_isolated_set_frame),
	.read = isolated_set_ring_read,
};

static ssize_t raw_capture_status_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	u32 i;
	ssize_t len = 0;

	mutex_lock(&shid->raw_capture_lock);
	len += sysfs_emit_at(buf, len,
		"enabled=%u\nraw_input_beta=%u\nframe_body_length=%u\nslots=%u\ncaptured=%llu\ninvalid=%u\nraw_input_invalid=%u\nnext_slot=%u\n",
		raw_capture_only, raw_input_beta, SPI_HID_RAW_CAPTURE_BODY_LENGTH,
		SPI_HID_RAW_CAPTURE_SLOTS, shid->raw_capture_count,
		shid->raw_capture_invalid, shid->raw_input_invalid, shid->raw_capture_next);
	for (i = 0; i < SPI_HID_RAW_CAPTURE_SLOTS; i++)
		len += sysfs_emit_at(buf, len, "slot%u_sequence=%llu\nslot%u_timestamp_ns=%llu\n",
			i, shid->raw_capture_sequence[i], i,
			shid->raw_capture_timestamp_ns[i]);
	mutex_unlock(&shid->raw_capture_lock);

	return len;
}
static DEVICE_ATTR_RO(raw_capture_status);

static irqreturn_t spi_hid_seq_thread(int irq, void *_shid)
{
	struct spi_hid *shid = _shid;
	struct device *dev = &shid->spi->dev;
	u8 hdr[9]; int type; u16 blen = 0;
	int hdr_off;
	s64 dbg_dt_us;
	irqreturn_t result = IRQ_HANDLED;

	if (READ_ONCE(shid->removing) || !READ_ONCE(shid->seq_enabled))
		return IRQ_NONE;
	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || !READ_ONCE(shid->seq_enabled)) {
		result = IRQ_NONE;
		goto out;
	}

	shid->stat_irq_count++;

	if (shid->seq_state != shid->seq_dbg_last_state) {
		seq_dbg(shid, 1, "SEQ: thread seq_state=%d\n", shid->seq_state);
		shid->seq_dbg_last_state = shid->seq_state;
	}

	dbg_dt_us = shid->seq_dbg_last_irq ?
		ktime_us_delta(ktime_get(), shid->seq_dbg_last_irq) : -1;
	shid->seq_dbg_last_irq = ktime_get();

	/* Windows reads response header first: 5 sync bytes plus a 4-byte header. */
	if (spi_hid_seq_read(shid, hdr, sizeof(hdr))) {
		dev_dbg(dev, "sequencer header read failed\n");
		goto out;
	}
	type = spi_hid_seq_hdr_type(hdr, sizeof(hdr), &hdr_off);
	seq_dbg(shid, 2, "SEQ[state=%d] type=%d hdr=[%*ph] dt=%lld us%s\n",
		 shid->seq_state, type, 4, &hdr[5], dbg_dt_us,
		 shid->seq_dbg_expect_fast ? (dbg_dt_us >= 0 && dbg_dt_us < 5000 ?
		 " <<< FAST IRQ AFTER DESCREQ: WRITE REACHED DEVICE" :
		 " <<< slow IRQ: DESCREQ ignored (device just re-reset)") : "");
	shid->seq_dbg_expect_fast = false;
	if (type < 0) {
		seq_dbg(shid, 1, "SEQ: no header found\n");
		/* In state 0, drain any body data and send DESCREQ anyway */
		if (shid->seq_state == 0) {
			u8 body_drain[64];
			spi_hid_seq_read(shid, body_drain, sizeof(body_drain));
			seq_dbg(shid, 1, "SEQ: body drain done, forcing DESCREQ@0x000001...\n");
			spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0);
			spi_hid_seq_set_state(shid, 1, SPI_HID_SEQ_FALLBACK);
		} else {
			goto out;
		}
		goto out;
	}
	if (hdr_off != 5) {
		dev_warn_ratelimited(dev,
			"SEQ: input header at unexpected offset %d, dropping frame\n", hdr_off);
		shid->stat_frames_dropped++;
		goto out;
	}

	blen = (((hdr[6] >> 4) & 0xF) << 0) | (hdr[7] << 4);
	blen *= 4;
	if (blen > sizeof(shid->input.content))
		blen = sizeof(shid->input.content);
	memcpy(shid->isolated_set_header, &hdr[5],
	       sizeof(shid->isolated_set_header));
	if (shid->isolated_set_armed && type == SPI_HID_REPORT_TYPE_RESET_RESP) {
		u32 rblen = min_t(u32, blen + 5, shid->data_buf_len);

		if (!spi_hid_seq_read(shid, shid->data_buf, rblen))
			spi_hid_isolated_set_capture(shid, type, shid->data_buf, rblen,
						     blen);
		shid->isolated_set_reset++;
		shid->isolated_set_state = SPI_HID_ISOLATED_SET_RESET;
		cancel_delayed_work(&shid->isolated_set_timeout_work);
		cancel_delayed_work(&shid->isolated_set_work);
		/* Reset is terminal after the harness arms: never recover or retry. */
		goto out;
	}
	if (shid->isolated_set_state == SPI_HID_ISOLATED_SET_OBSERVING &&
	    (type == SPI_HID_REPORT_TYPE_COMMAND_RESP ||
	     type == SPI_HID_REPORT_TYPE_GET_FEATURE_RESP)) {
		u32 rblen = min_t(u32, blen + 5, shid->data_buf_len);

		if (!spi_hid_seq_read(shid, shid->data_buf, rblen))
			spi_hid_isolated_set_capture(shid, type, shid->data_buf, rblen,
						     blen);
		goto out;
	}

	switch (shid->seq_state) {
	case 0: seq_handle_reset(shid, type, blen, &shid->seq_dbg_expect_fast); break;
	case 1: seq_handle_desc(shid, type, blen);   break;
	case 2: seq_handle_rpt(shid, type, blen);    break;
	case 3: seq_handle_vendor(shid, type, blen); break;
	case 4: seq_handle_data(shid, type, blen);   break;
	case 5: seq_handle_feat(shid, type, blen);   break;
	}
out:
	mutex_unlock(&shid->seq_lock);
	return result;
}

/* ── State handler: WAIT_RESET (state 0) ─────────────────────────── */
static void seq_handle_reset(struct spi_hid *shid, int type, u16 blen, bool *expect_fast)
{
	if (type == 3) {
		u8 body[20];

		shid->stat_reset_rsp++;
		if (spi_hid_seq_read(shid, body, sizeof(body)))
			return;
		seq_dbg(shid, 3, "SEQ[state0]: RESET_RSP body-drain=[%*ph], sending DESCREQ\n",
			 20, body);
		if (spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE))
			return;
		seq_dbg(shid, 1, "SEQ[state0]: DESCREQ sent, waiting for DEVICE_DESC IRQ\n");
		*expect_fast = true;
	} else if (type == 7) {
		seq_dbg(shid, 1, "SEQ[state0]: DEVICE_DESC without RESET_RSP, handing to desc handler\n");
		seq_handle_desc(shid, type, blen);
	} else if (type == 8) {
		seq_dbg(shid, 1, "SEQ[state0]: RPT_DESC without RESET_RSP, handing to rpt handler\n");
		seq_handle_rpt(shid, type, blen);
	} else {
		seq_dbg(shid, 1, "SEQ[state0]: unexpected type=%d in state 0, forcing DESCREQ\n",
			type);
		{
			u8 body_drain[64];
			spi_hid_seq_read(shid, body_drain, sizeof(body_drain));
		}
		spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0);
		spi_hid_seq_set_state(shid, 1, SPI_HID_SEQ_FALLBACK);
	}
}

/* ── State handler: WAIT_DESC (state 1) ──────────────────────────── */
static void seq_handle_desc(struct spi_hid *shid, int type, u16 blen)
{
	if (type == 7) {
		u8 body[64] = {};
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_device_desc++;
		seq_dbg(shid, 1, "SEQ: DEVICE_DESC! reading body (%u bytes)...\n", blen);
		if (rblen < 3 || spi_hid_seq_read(shid, body, rblen)) {
			dev_warn(&shid->spi->dev, "SEQ: DEVICE_DESC read failed or was truncated\n");
			return;
		}
		seq_dbg(shid, 3, "DIFFCHECK: DEVICE_DESC full body=[%*ph]\n", rblen, body);
		{
			struct spi_hid_device_desc_raw raw = {};
			u32 off = 0;
			u32 required = sizeof(raw) - sizeof(raw.reserved);

			while (off + 3 < rblen && body[off] == 0xFF)
				off++;
			/* Reserved bytes are not consumed by spi_hid_parse_dev_desc().
			 * Some V0 replies end immediately after wFlags. */
			if (off + 3 + required > rblen) {
				dev_warn(&shid->spi->dev, "SEQ: DEVICE_DESC body too short (%u bytes)\n",
					 rblen);
				return;
			}
			off += 3;
			seq_dbg(shid, 2, "SEQ: parsing at rx+%u\n", off);
			memcpy(&raw, body + off,
			       min_t(u32, sizeof(raw), rblen > off ? rblen - off : 0));
			spi_hid_parse_dev_desc(&raw, &shid->desc);
			seq_dbg(shid, 2, "SEQ: vid=0x%04X pid=0x%04X ver=0x%04X inp=0x%04X out=0x%04X cmd=0x%04X rpt_len=%u max_in=%u max_out=%u\n",
				shid->desc.vendor_id, shid->desc.product_id,
				shid->desc.version_id, shid->desc.input_register,
				shid->desc.output_register,
				shid->desc.command_register,
				shid->desc.report_descriptor_length,
				shid->desc.max_input_length,
				shid->desc.max_output_length);
		}
		{
			u8 dr2[11] = {
				0x02, 0x02, 0x00, 0x00, 0x02, 0x42,
				0x00, 0x00, 0x03, 0x00, 0x00
			};
			dr2[2] = (shid->desc.report_descriptor_register >> 16) & 0xFF;
			dr2[3] = (shid->desc.report_descriptor_register >> 8) & 0xFF;
			dr2[4] = shid->desc.report_descriptor_register & 0xFF;
			if (spi_hid_seq_write(shid, dr2, sizeof(dr2), NULL, 0)) {
				dev_warn(&shid->spi->dev, "SEQ: RPT_DESC request write failed\n");
				return;
			}
		}
		spi_hid_seq_set_state(shid, 2, SPI_HID_SEQ_DEVICE_DESCRIPTOR);
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in state 1, sending DESCREQ directly\n");
		if (spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE))
			return;
		seq_dbg(shid, 1, "SEQ: DESCREQ sent synchronously, waiting for next IRQ\n");
	}
}

/* ── State handler: WAIT_RPT (state 2) ───────────────────────────── */
static void seq_handle_rpt(struct spi_hid *shid, int type, u16 blen)
{
	if (type == 8) {
		u8 body[1024] = {};
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_rpt_desc++;
		seq_dbg(shid, 1, "SEQ: RPT_DESC! reading body (%u bytes)...\n", blen);
		if (rblen < 3 || spi_hid_seq_read(shid, body, rblen)) {
			dev_warn(&shid->spi->dev, "SEQ: RPT_DESC read failed or was truncated\n");
			return;
		}
		{
			u32 off;
			for (off = 0; off < rblen; off += 64) {
				u32 chunk = min_t(u32, 64, rblen - off);
				seq_dbg(shid, 3, "DIFFCHECK: RPT_DESC+%u=[%*ph]\n", off, chunk, body + off);
			}
		}
		{
			u32 off = 0, len;

			while (off + 3 < rblen && body[off] == 0xFF)
				off++;
			if (off + 3 > rblen) {
				dev_warn(&shid->spi->dev, "SEQ: RPT_DESC body has no content header\n");
				return;
			}
			off += 3;
			len = min_t(u32, shid->desc.report_descriptor_length,
				    sizeof(shid->wire_report_descriptor));
			if (off < rblen && len > 0 && off + len <= rblen) {
				memcpy(shid->wire_report_descriptor, body + off, len);
				shid->wire_report_descriptor_len = len;
			} else {
				shid->wire_report_descriptor_len = 0;
			}
		}
		seq_dbg(shid, 1, "SEQ: report descriptor received, shid->hid=%p, scheduling create_device_work...\n", shid->hid);
		shid->ready = true;
		shid->keep_powered = true;
		if (!shid->hid && !shid->raw_mode_active) {
			bool queued = schedule_work(&shid->create_device_work);
			seq_dbg(shid, 1, "SEQ: scheduled create_device_work, queued=%d\n", queued);
		}
		if (isolated_set_test && !shid->isolated_set_attempted) {
			shid->isolated_set_attempted = true;
			shid->isolated_set_armed = true;
			shid->isolated_set_state = SPI_HID_ISOLATED_SET_WAIT_GET;
			/* Keep normal input dispatch live while the isolated worker waits
			 * for both HID creation and the Windows-observed settle interval. */
			spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_REPORT_DESCRIPTOR);
			dev_info(&shid->spi->dev,
				 "ISOLATED_SET: GET ID6 scheduled; recovery is disabled for this test\n");
			schedule_delayed_work(&shid->isolated_set_work,
				msecs_to_jiffies(RAW_TRANSITION_DELAY_MS));
		} else if (raw_transition_once && !shid->raw_transition_attempted) {
			/* One exact, delayed Windows-observed request. There is no retry if
			 * the device resets or remains silent after the later SET_FEATURE. */
			shid->raw_transition_attempted = true;
			shid->raw_transition_scheduled++;
			shid->feat_delay_pending = true;
			dev_info(&shid->spi->dev,
				 "RAW_TRANSITION: GET selector 4 scheduled after %dms\n",
				 RAW_TRANSITION_DELAY_MS);
			seq_dbg(shid, 1,
				"SEQ: scheduling one-shot GET_FEATURE(id=4) after %dms\n",
				RAW_TRANSITION_DELAY_MS);
			schedule_delayed_work(&shid->feat_delay_work,
				msecs_to_jiffies(RAW_TRANSITION_DELAY_MS));
		} else if (shid->raw_mode_active) {
			if (probe_raw_id < 0) {
				seq_dbg(shid, 1, "SEQ: raw_mode=1 probe_raw_id=%d -> skipping SET_FEATURE, staying standard\n",
					 probe_raw_id);
				spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_REPORT_DESCRIPTOR);
			} else if (skip_getfeat) {
				static const u8 vendor_init[] = {
					0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00,
					0x03, 0x0A, 0x00,
					0x56, 0xBD, 0x0C, 0xEE, 0x5B, 0x44, 0x4C, 0x00, 0x00
				};
				if (getfeat_delay_ms > 0) {
					/* Defer vendor init path too — same delayed-work
					 * pattern: waits the configured delay then sends
					 * vendor-init + SET_FEATURE inline. */
					seq_dbg(shid, 1, "SEQ: scheduling vendor init after %dms...\n",
						getfeat_delay_ms);
					shid->feat_delay_pending = true;
					schedule_delayed_work(&shid->feat_delay_work,
							      msecs_to_jiffies(getfeat_delay_ms));
				} else {
					seq_dbg(shid, 1, "SEQ: vendor init (18B, TXN#267) +70ms delay...\n");
					usleep_range(68000, 72000);
					spi_hid_seq_write(shid, vendor_init, sizeof(vendor_init), NULL, 0);
					usleep_range(36000, 39000);
					{
						u8 sf[15] = {
							0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
							0x03, probe_raw_id & 0xFF, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
						};
						seq_dbg(shid, 1, "SEQ: probe_id=%d SET_FEATURE -> state 4\n", probe_raw_id);
						spi_hid_seq_write(shid, sf, sizeof(sf), NULL, 0);
					}
					spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_REPORT_DESCRIPTOR);
				}
			} else {
				u8 gf[11] = {
					0x02, 0x02, 0x00, 0x00, 0x03, 0x42,
					0x00, probe_raw_id & 0xFF, 0x03, 0x00, 0x06
				};
				usleep_range(1400, 1800);
				if (getfeat_delay_ms > 0) {
					seq_dbg(shid, 1, "SEQ: scheduling GET_FEATURE after %dms delay (Windows: ~5900ms)...\n",
						getfeat_delay_ms);
					shid->feat_delay_pending = true;
					schedule_delayed_work(&shid->feat_delay_work,
							      msecs_to_jiffies(getfeat_delay_ms));
					/* Stay in state 2 until the delayed work fires */
				} else {
					seq_dbg(shid, 1, "SEQ: raw_mode=1 -> GET_FEATURE(id=%d), state 5\n", probe_raw_id);
					if (spi_hid_seq_write(shid, gf, sizeof(gf), NULL, 0)) {
						dev_warn(&shid->spi->dev, "SEQ: GET_FEATURE write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					spi_hid_seq_set_state(shid, 5, SPI_HID_SEQ_FEATURE_REQUEST);
					schedule_delayed_work(&shid->raw_handshake_watchdog,
							      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
				}
			}
		} else {
			spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_REPORT_DESCRIPTOR);
		}
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in state 2, sending DESCREQ directly\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	}
}

/* ── State handler: WAIT_FEAT_RESP (state 5) ─────────────────────── */
static void seq_handle_feat(struct spi_hid *shid, int type, u16 blen)
{
	if (type == 5) {
		u8 body[256] = {};
		u32 rblen = min_t(u32, blen + 5, sizeof(body));
		u32 off;

		shid->stat_getfeat_resp++;
		if (raw_transition_once && shid->raw_transition_attempted) {
			shid->raw_transition_get_response++;
			dev_info(&shid->spi->dev, "RAW_TRANSITION: GET response received\n");
		}
		seq_dbg(shid, 1, "SEQ: GET_FEAT_RESP! reading body (%u bytes)...\n", blen);
		if (rblen < 3 || spi_hid_seq_read(shid, body, rblen)) {
			dev_warn(&shid->spi->dev, "SEQ: GET_FEATURE response read failed or was truncated\n");
			return;
		}
		for (off = 0; off < rblen; off += 32) {
			u32 chunk = min_t(u32, 32, rblen - off);
			seq_dbg(shid, 3, "  GET_FEAT_RESP[%u..%u]: %*ph\n",
				 off, off + chunk - 1, chunk, body + off);
		}
		if (isolated_set_test &&
		    shid->isolated_set_state == SPI_HID_ISOLATED_SET_WAIT_GET) {
			/* This is the sole SET in the harness. The controller consumes the
			 * first byte, leaving the exact 14-byte Windows FIFO vector. */
			static const u8 set_vector[15] = {
				0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
				0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00,
			};

			shid->isolated_set_state = SPI_HID_ISOLATED_SET_WAIT_SET;
			usleep_range(4500, 5500);
			if (spi_hid_seq_write(shid, set_vector, sizeof(set_vector), NULL, 0)) {
				shid->isolated_set_write_failed++;
				shid->isolated_set_state = SPI_HID_ISOLATED_SET_FAILED;
				goto out_done;
			}
			shid->isolated_set_set_sent++;
			shid->isolated_set_state = SPI_HID_ISOLATED_SET_OBSERVING;
			spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_RESPONSE);
			schedule_delayed_work(&shid->isolated_set_timeout_work,
				msecs_to_jiffies(ISOLATED_SET_OBSERVATION_TIMEOUT_MS));
			return;
		}
		if (raw_transition_once && shid->raw_transition_attempted) {
			/* A GET response proves only that GET reached the device. SET has
			 * no verified completion contract and must remain a separate test. */
			memcpy(shid->raw_transition_get_body, body, rblen);
			shid->raw_transition_get_body_len = rblen;
			cancel_delayed_work(&shid->raw_transition_timeout_work);
			dev_info(&shid->spi->dev,
				 "RAW_TRANSITION: GET-only transaction complete; SET is disabled\n");
			spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_RESPONSE);
			return;
		}
		{
			u8 selector = raw_transition_once ? 4 : probe_raw_id & 0xff;
			u8 sf[15] = {
				0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
				0x03, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
			};
			u8 sf_nd[14] = {
				0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
				0x03, 0x00, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
			};
			sf[8] = selector;
			sf_nd[7] = selector;
			usleep_range(4500, 5500);
			seq_dbg(shid, 1, "SEQ: sending SET_FEATURE(id=%d) speed=%u no_double=%d\n",
					 selector, setfeat_speed_hz, setfeat_no_double);
			int ret;

			if (raw_transition_once)
				ret = spi_hid_seq_write_speed(shid, sf_nd, sizeof(sf_nd),
							  NULL, 0, 0);
			else if (setfeat_no_double)
				ret = spi_hid_seq_write_speed(shid, sf_nd, sizeof(sf_nd),
								  NULL, 0, setfeat_speed_hz);
			else
				ret = spi_hid_seq_write_speed(shid, sf, sizeof(sf),
								  NULL, 0, setfeat_speed_hz);
			if (ret) {
				if (raw_transition_once && shid->raw_transition_attempted)
					shid->raw_transition_set_write_failed++;
				dev_warn(&shid->spi->dev, "SEQ: SET_FEATURE write failed: %d\n", ret);
				return;
			}
			if (raw_transition_once && shid->raw_transition_attempted) {
				shid->raw_transition_set_sent++;
				dev_info(&shid->spi->dev,
					 "RAW_TRANSITION: SET selector %u sent\n", selector);
			}
		}
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_RESPONSE);
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (isolated_set_test && shid->isolated_set_armed) {
			if (rblen)
				spi_hid_seq_read(shid, body, rblen);
			shid->isolated_set_reset++;
			shid->isolated_set_state = SPI_HID_ISOLATED_SET_RESET;
			cancel_delayed_work(&shid->isolated_set_timeout_work);
			/* A reset is terminal for this harness: do not send recovery. */
			return;
		}
		if (raw_transition_once && shid->raw_transition_attempted &&
		    !shid->raw_transition_get_response)
			shid->raw_transition_reset_before_response++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in state 5, sending DESCREQ directly\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	}

out_done:
	return;
}

/* ── State handler: VENDOR_INIT (state 3) ────────────────────────── */
static void seq_handle_vendor(struct spi_hid *shid, int type, u16 blen)
{
	if (type == 1) {
		seq_dbg(shid, 1, "SEQ: VENDOR_INIT: got DATA! Creating HID device...\n");
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FALLBACK);
		shid->ready = true;
		shid->keep_powered = true;
		if (!shid->hid)
			spi_hid_create_device(shid);
		seq_handle_data(shid, type, blen);
	} else if (type == 3) {
		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: VENDOR_INIT: RESET_RSP, restarting discovery\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	}
}

/* ── State handler: DONE (state 4) ───────────────────────────────── */
static void seq_handle_data(struct spi_hid *shid, int type, u16 blen)
{
	struct device *dev = &shid->spi->dev;

	if (type == 3) {
		u8 body[20];

		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: Device reset detected in state 4. Re-initializing sequencer...\n");
		if (spi_hid_seq_read(shid, body, sizeof(body)))
			return;
		spi_hid_isolated_set_capture(shid, type, body, sizeof(body), blen);
		if (shid->isolated_set_state == SPI_HID_ISOLATED_SET_OBSERVING) {
			shid->isolated_set_reset++;
			shid->isolated_set_state = SPI_HID_ISOLATED_SET_RESET;
			cancel_delayed_work(&shid->isolated_set_timeout_work);
			return;
		}
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_DEVICE_RESET);
		return;
	}
	if (type != 1)
		return;
	if (!shid->hid && !(shid->raw_mode_active && shid->touch_input) &&
	    shid->isolated_set_state != SPI_HID_ISOLATED_SET_OBSERVING)
		return;

	shid->stat_data++;
	{
		u32 cap = shid->desc.max_input_length ?
			  shid->desc.max_input_length : 0x1000;
		u32 rblen = min_t(u32, blen + 5, cap);
		u32 avail;
		u16 rl;
		u8 *body;
		struct spi_hid_protocol_content content;

		rblen = min_t(u32, rblen, shid->data_buf_len);
		body = shid->data_buf;
		avail = (rblen > 8) ? (rblen - 8) : 0;

		if (spi_hid_seq_read(shid, body, rblen)) {
			shid->stat_frames_dropped++;
			return;
		}
		if (rblen < 9) {
			dev_warn(dev, "SEQ: DATA report too short for a header (rblen=%u), dropped\n", rblen);
			shid->stat_frames_dropped++;
			return;
		}
		spi_hid_isolated_set_capture(shid, type, body, rblen, blen);
		rl = body[5] | (body[6] << 8);
		seq_dbg(shid, 2, "SEQ: state4 cid=0x%02x len=%u\n", body[7], rl);

		if (raw_capture_only &&
		    !spi_hid_protocol_parse_content(body + 5, rblen - 5, &content) &&
		    content.content_id == SPI_HID_RAW_CAPTURE_CONTENT_ID) {
			spi_hid_handle_raw_body(shid, body + 5, rblen - 5);
			return;
		} else if (shid->raw_mode_active && shid->touch_input &&
		    !spi_hid_protocol_parse_content(body + 5, rblen - 5, &content) &&
		    content.content_id == 0x0C) {
			if (!shid->raw_handshake_confirmed) {
				shid->raw_handshake_confirmed = true;
				cancel_delayed_work(&shid->raw_handshake_watchdog);
				seq_dbg(shid, 1, "SEQ: raw_mode handshake confirmed (first heatmap frame received)\n");
				if (stream_watchdog_ms > 0 && !shid->stream_watchdog_active) {
					shid->stream_watchdog_active = true;
					shid->stream_watchdog_data = shid->stat_data;
					shid->stream_watchdog_misses = 0;
					shid->stream_watchdog_reinits = 0;
					schedule_delayed_work(&shid->stream_watchdog,
							      msecs_to_jiffies(stream_watchdog_ms));
				}
				/* The IRQ thread is the sole reader of input_register. Polling it
				 * concurrently can consume a report before its IRQ is serviced. */
				if (poll_interval > 0)
					dev_warn_once(dev,
						"SEQ: poll_interval is ignored; IRQ owns input reads\n");
			}
			heatmap_process_frame(shid, content.data, content.data_length,
					      content.content_id);
		} else if (rl >= 3 && rl - 3 <= avail) {
			if (shid->raw_mode_active && body[8] == 0x40 && rl - 3 >= 6) {
				u16 hx = body[9] | (body[10] << 8);
				u16 hy = body[11] | (body[12] << 8);
				seq_dbg(shid, 2, "CALIB_REF: hid=(%u,%u)\n", hx, hy);
			}
			if (shid->hid) {
				int hret = hid_input_report(shid->hid, HID_INPUT_REPORT,
						    &body[7], rl - 2, 1);
				if (hret)
					dev_warn(dev, "SEQ: hid_input_report failed: %d (content_id=0x%02x)\n",
						 hret, body[8]);
			}
		} else if (rl >= 3 && rl - 3 > avail) {
			dev_warn(dev, "SEQ: DATA report len=%u exceeds buffer (avail=%u), dropped\n",
				 rl, avail);
		} else {
			dev_warn(dev, "SEQ: DATA report too short to contain a report ID (len=%u), dropped\n",
					 rl);
		}
	}
}

static irqreturn_t spi_hid_dev_irq(int irq, void *_shid)
{
	struct spi_hid *shid = _shid;

	if (READ_ONCE(shid->removing) || !READ_ONCE(shid->seq_enabled))
		return IRQ_NONE;

	return IRQ_WAKE_THREAD;
}

/* hid_ll_driver interface functions */

static int spi_hid_ll_start(struct hid_device *hid)
{
	struct spi_device *spi = hid->driver_data;
	struct spi_hid *shid = spi_get_drvdata(spi);

	if (shid->desc.max_input_length < HID_MIN_BUFFER_SIZE) {
		dev_err(&shid->spi->dev, "HID_MIN_BUFFER_SIZE > max_input_length (%d)\n",
				shid->desc.max_input_length);
		return -EINVAL;
	}

	return 0;
}

static void spi_hid_ll_stop(struct hid_device *hid)
{
	hid->claimed = 0;
}

static int spi_hid_ll_open(struct hid_device *hid)
{
	return 0;
}

static void spi_hid_ll_close(struct hid_device *hid)
{
}

static int spi_hid_ll_power(struct hid_device *hid, int level)
{
	struct spi_device *spi = hid->driver_data;
	struct spi_hid *shid = spi_get_drvdata(spi);
	int ret = 0;

	mutex_lock(&shid->lock);
	if (!shid->hid)
		ret = -ENODEV;
	mutex_unlock(&shid->lock);

	return ret;
}

static int spi_hid_ll_parse(struct hid_device *hid)
{
	struct spi_device *spi = hid->driver_data;
	struct spi_hid *shid = spi_get_drvdata(spi);
	struct device *dev = &spi->dev;
	int ret;

	mutex_lock(&shid->lock);

	if (!shid->wire_report_descriptor_len) {
		ret = -EINVAL;
		goto out;
	}

	seq_dbg(shid, 1, "SEQ: ll_parse: device-read report descriptor (%u bytes)\n",
		 shid->wire_report_descriptor_len);
	ret = hid_parse_report(hid, shid->wire_report_descriptor,
				shid->wire_report_descriptor_len);
	if (ret)
		dev_err(dev, "device-read report descriptor failed to parse: %d\n", ret);
	else
		shid->report_descriptor_crc32 = crc32_le(0,
			shid->wire_report_descriptor,
			shid->wire_report_descriptor_len);

out:
	mutex_unlock(&shid->lock);
	return ret;
}

static int spi_hid_ll_raw_request(struct hid_device *hid,
		unsigned char reportnum, __u8 *buf, size_t len,
		unsigned char rtype, int reqtype)
{
	struct spi_device *spi = hid->driver_data;
	struct spi_hid *shid = spi_get_drvdata(spi);
	struct device *dev = &spi->dev;
	int ret;

	if (!len)
		return -EINVAL;
	if (READ_ONCE(shid->isolated_set_state) == SPI_HID_ISOLATED_SET_OBSERVING)
		return -EBUSY;

	if (!shid->ready) {
		dev_err(&shid->spi->dev, "%s called in unready state\n", __func__);
		return -ENODEV;
	}

	mutex_lock(&shid->lock);

	switch (reqtype) {
	case HID_REQ_SET_REPORT:
		if (len > U16_MAX - 3) {
			ret = -EMSGSIZE;
			break;
		}
		if (buf[0] != reportnum) {
			dev_err(dev, "report id mismatch\n");
			ret = -EINVAL;
			break;
		}

		ret = spi_hid_set_request(shid, &buf[1], len-1,
				reportnum);
		if (ret) {
			dev_err(dev, "failed to set report\n");
			break;
		}

		ret = len;
		break;
	case HID_REQ_GET_REPORT:
		ret = spi_hid_get_request(shid, reportnum);
		if (ret) {
			dev_err(dev, "failed to get report\n");
			break;
		}

		{
			u16 response_len = shid->response.body[0] |
				(shid->response.body[1] << 8);

			if (response_len < 3) {
				ret = -EPROTO;
				break;
			}
			ret = min_t(size_t, len, response_len - 3);
		}
		memcpy(buf, &shid->response.content, ret);
		break;
	default:
		dev_err(dev, "invalid request type\n");
		ret = -EIO;
	}

	mutex_unlock(&shid->lock);

	return ret;
}

static int spi_hid_ll_output_report(struct hid_device *hid,
		__u8 *buf, size_t len)
{
	int ret;
	struct spi_device *spi = hid->driver_data;
	struct spi_hid *shid = spi_get_drvdata(spi);
	struct device *dev = &spi->dev;
	struct spi_hid_output_report report;

	if (!len || len > U16_MAX - 3)
		return -EMSGSIZE;
	if (READ_ONCE(shid->isolated_set_state) == SPI_HID_ISOLATED_SET_OBSERVING)
		return -EBUSY;

	report.content_type = SPI_HID_CONTENT_TYPE_OUTPUT_REPORT;
	report.content_length = len + 2;
	report.content_id = buf[0];
	report.content = &buf[1];

	mutex_lock(&shid->lock);
	if (!shid->ready) {
		dev_err(dev, "%s called in unready state\n", __func__);
		ret = -ENODEV;
		goto out;
	}

	ret = spi_hid_send_output_report(shid, shid->desc.output_register, &report);
	if (ret)
		dev_err(dev, "failed to send output report\n");

out:
	mutex_unlock(&shid->lock);

	if (ret > 0)
		return -ret;

	if (ret < 0)
		return ret;

	return len;
}

static struct hid_ll_driver spi_hid_ll_driver = {
	.start = spi_hid_ll_start,
	.stop = spi_hid_ll_stop,
	.open = spi_hid_ll_open,
	.close = spi_hid_ll_close,
	.power = spi_hid_ll_power,
	.parse = spi_hid_ll_parse,
	.output_report = spi_hid_ll_output_report,
	.raw_request = spi_hid_ll_raw_request,
};

static const struct of_device_id spi_hid_of_match[] = {
	{ .compatible = "hid-over-spi" },
	{},
};
MODULE_DEVICE_TABLE(of, spi_hid_of_match);

static const struct acpi_device_id spi_hid_acpi_match[] = {
	{ "MSHW0134", 0 },	/* Surface Pro X (SQ1) */
	{ "MSHW0162", 0 },	/* Surface Laptop 3 (AMD) */
	{ "MSHW0235", 0 },	/* Surface Pro X (SQ2) */
	{ "PNP0C51",  0 },	/* Generic HID-over-SPI */
	{},
};
MODULE_DEVICE_TABLE(acpi, spi_hid_acpi_match);

static ssize_t ready_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%s\n",
			shid->ready ? "ready" : "not ready");
}
static DEVICE_ATTR_RO(ready);

static ssize_t bus_error_count_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d (%d)\n",
			shid->bus_error_count, shid->bus_last_error);
}
static DEVICE_ATTR_RO(bus_error_count);


static ssize_t device_initiated_reset_count_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", shid->dir_count);
}
static DEVICE_ATTR_RO(device_initiated_reset_count);


static ssize_t
spi_hid_latency_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	int count = 0;
	int i = 0;

	for (i = 0; i < SPI_HID_MAX_LATENCIES; i++) {
		if (shid->latencies[i].report_id == 0)
			break;

		count += snprintf(buf + count, PAGE_SIZE, "%u %u %llu %llu|",
					shid->latencies[i].report_id,
					shid->latencies[i].signature,
					shid->latencies[i].start_time,
					shid->latencies[i].end_time);
	}

	return count;
}
static DEVICE_ATTR_RO(spi_hid_latency);

static ssize_t
spi_hid_perf_mode_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	int count = 0;

	count += snprintf(buf, PAGE_SIZE, "%d", shid->perf_mode);

	return count;
}

static ssize_t
spi_hid_perf_mode_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf,
			size_t size)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	unsigned long flags;

	spin_lock_irqsave(&shid->input_lock, flags);
	if (kstrtou8(buf, 10, &shid->perf_mode))
		return -EINVAL;

	// Reset the log
	if (shid->perf_mode) {
		memset(shid->latencies, 0, sizeof(shid->latencies));
		shid->latency_index = 0;
	}

	spin_unlock_irqrestore(&shid->input_lock, flags);

	return size;
}

static DEVICE_ATTR_RW(spi_hid_perf_mode);

static ssize_t seq_state_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", shid->seq_state);
}
static DEVICE_ATTR_RO(seq_state);

static ssize_t protocol_stats_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf, "reset_rsp=%u\ndevice_desc=%u\nrpt_desc=%u\ndata=%u\ngetfeat_resp=%u\nframes_dropped=%u\nirq_count=%u\n",
		shid->stat_reset_rsp, shid->stat_device_desc, shid->stat_rpt_desc,
		shid->stat_data, shid->stat_getfeat_resp,
		shid->stat_frames_dropped, shid->stat_irq_count);
}
static DEVICE_ATTR_RO(protocol_stats);

static ssize_t raw_transition_status_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	ssize_t len;

	mutex_lock(&shid->seq_lock);
	len = sysfs_emit(buf,
		"enabled=%u\nattempted=%u\ncurrent_state=%d\nscheduled=%u\nget_sent=%u\nget_write_failed=%u\nget_response=%u\ntimeout=%u\nset_sent=%u\nset_write_failed=%u\nstate_skipped=%u\nreset_before_response=%u\n",
		raw_transition_once, shid->raw_transition_attempted, shid->seq_state,
		shid->raw_transition_scheduled, shid->raw_transition_get_sent,
		shid->raw_transition_get_write_failed,
		shid->raw_transition_get_response, shid->raw_transition_timeout,
		shid->raw_transition_set_sent,
		shid->raw_transition_set_write_failed,
		shid->raw_transition_state_skipped,
		shid->raw_transition_reset_before_response);
	mutex_unlock(&shid->seq_lock);

	return len;
}
static DEVICE_ATTR_RO(raw_transition_status);

static ssize_t raw_transition_response_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	ssize_t len;

	mutex_lock(&shid->seq_lock);
	len = sysfs_emit(buf, "length=%u\nbody=%*ph\n",
		shid->raw_transition_get_body_len,
		(int)shid->raw_transition_get_body_len,
		shid->raw_transition_get_body);
	mutex_unlock(&shid->seq_lock);

	return len;
}
static DEVICE_ATTR_RO(raw_transition_response);

static ssize_t isolated_set_status_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	mutex_lock(&shid->seq_lock);
	{
		ssize_t len = sysfs_emit(buf,
			"enabled=%u\narmed=%u\nattempted=%u\nstate=%u\nget_sent=%u\nset_sent=%u\nwrite_failed=%u\ntimeout=%u\nreset=%u\ncaptured=%u\noverflow=%u\nnext_slot=%u\nring_record_size=%zu\n",
			isolated_set_test, shid->isolated_set_armed,
			shid->isolated_set_attempted, shid->isolated_set_state,
			shid->isolated_set_get_sent, shid->isolated_set_set_sent,
			shid->isolated_set_write_failed, shid->isolated_set_timeout,
			shid->isolated_set_reset, shid->isolated_set_count,
			shid->isolated_set_overflow,
			shid->isolated_set_next,
			sizeof(struct spi_hid_isolated_set_frame));

		mutex_unlock(&shid->seq_lock);
		return len;
	}
}
static DEVICE_ATTR_RO(isolated_set_status);

static ssize_t baseline_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf, "frame_count=%u\nrows=%u\ncols=%u\n",
		shid->heatmap_baseline_frames, shid->heatmap_grid_rows, shid->heatmap_grid_cols);
}
static DEVICE_ATTR_RO(baseline_status);

static ssize_t build_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "spi-hid v1.0\n");
}
static DEVICE_ATTR_RO(build_info);

static ssize_t lifecycle_status_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf,
		"removing=%u\nseq_enabled=%u\nirq_enabled=%u\nworks_initialized=%u\n",
		READ_ONCE(shid->removing), READ_ONCE(shid->seq_enabled),
		shid->irq_enabled, shid->works_initialized);
}
static DEVICE_ATTR_RO(lifecycle_status);

static const struct attribute *const spi_hid_attributes[] = {
	&dev_attr_ready.attr,
	&dev_attr_bus_error_count.attr,
	&dev_attr_device_initiated_reset_count.attr,
	&dev_attr_spi_hid_latency.attr,
	&dev_attr_spi_hid_perf_mode.attr,
	&dev_attr_heatmap_debug.attr,
	&dev_attr_raw_capture_status.attr,
	&dev_attr_raw_transition_status.attr,
	&dev_attr_raw_transition_response.attr,
	&dev_attr_isolated_set_status.attr,
	&dev_attr_seq_state.attr,
	&dev_attr_protocol_stats.attr,
	&dev_attr_baseline_status.attr,
	&dev_attr_build_info.attr,
	&dev_attr_lifecycle_status.attr,
	NULL	/* Terminator */
};

/* 6e2ac436-0fcf-41af-a265-b32a220dcfab */
static const guid_t SPI_HID_DSM_GUID =
	GUID_INIT(0x6e2ac436, 0x0fcf, 0x41af,
		  0xa2, 0x65, 0xb3, 0x2a, 0x22, 0x0d, 0xcf, 0xab);

#define SPI_HID_DSM_REVISION	1

enum spi_hid_dsm_fn {
	SPI_HID_DSM_FN_REG_ADDR = 1,
};

static int spi_hid_get_descriptor_reg_acpi(struct device *dev, u32 *reg)
{
	acpi_handle handle = ACPI_HANDLE(dev);
	union acpi_object *obj;
	u64 val;

	obj = acpi_evaluate_dsm_typed(handle, &SPI_HID_DSM_GUID, SPI_HID_DSM_REVISION,
				      SPI_HID_DSM_FN_REG_ADDR, NULL, ACPI_TYPE_INTEGER);
	if (!obj)
		return -EIO;

	val = obj->integer.value;
	ACPI_FREE(obj);

	if (val > U32_MAX)
		return -ERANGE;

	*reg = val;
	return 0;
}

static int spi_hid_get_descriptor_reg(struct device *dev, u32 *reg)
{
	if (dev->of_node)
		return device_property_read_u32(dev, "hid-descr-addr", reg);
	else
		return spi_hid_get_descriptor_reg_acpi(dev, reg);
}

/* Stop every asynchronous path before tearing down the SPI device.  Several
 * workers can issue SPI transfers or ACPI calls, so leaving one queued makes a
 * following probe observe controller state from the previous driver instance. */
static void spi_hid_cancel_workers(struct spi_hid *shid)
{
	if (!shid->works_initialized)
		return;

	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	cancel_delayed_work_sync(&shid->descreq_work);
	cancel_delayed_work_sync(&shid->poll_work);
	cancel_delayed_work_sync(&shid->raw_handshake_watchdog);
	cancel_delayed_work_sync(&shid->feat_delay_work);
	cancel_delayed_work_sync(&shid->raw_transition_timeout_work);
	cancel_delayed_work_sync(&shid->isolated_set_timeout_work);
	cancel_delayed_work_sync(&shid->isolated_set_work);
	cancel_delayed_work_sync(&shid->stream_watchdog);
	cancel_work_sync(&shid->reset_work);
	cancel_work_sync(&shid->create_device_work);
	cancel_work_sync(&shid->refresh_device_work);
	cancel_work_sync(&shid->error_work);
}

static int spi_hid_probe(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	struct spi_hid *shid;
	unsigned long irqflags;
	int ret;

	dev_info(dev, "TRACE[hid] probe begin irq=%d raw_capture_only=%u raw_transition_once=%u isolated_set_test=%u acpi_power_cycle=%u\n",
		 spi->irq, raw_capture_only, raw_transition_once, isolated_set_test,
		 acpi_probe_power_cycle);
	if (raw_mode)
		dev_warn(dev, "raw_mode=1 is deprecated and inert; no mode-switch command will be sent\n");
	if ((raw_transition_once || raw_input_beta) && !raw_capture_only) {
		dev_err(dev, "raw_transition_once and raw_input_beta require raw_capture_only=1\n");
		return -EINVAL;
	}
	if (isolated_set_test && (!raw_capture_only || raw_transition_once)) {
		dev_err(dev, "isolated_set_test requires raw_capture_only=1 and raw_transition_once=0\n");
		return -EINVAL;
	}

	if (dev->of_node && spi->irq <= 0) {
		dev_err(dev, "Missing IRQ\n");
		ret = spi->irq ?: -EINVAL;
		goto err0;
	}

	shid = devm_kzalloc(dev, sizeof(struct spi_hid), GFP_KERNEL);
	if (!shid) {
		ret = -ENOMEM;
		goto err0;
	}

	shid->spi = spi;
	shid->power_state = SPI_HID_POWER_MODE_ACTIVE;
	mutex_init(&shid->lock);
	mutex_init(&shid->seq_lock);
	mutex_init(&shid->power_lock);
	mutex_init(&shid->output_lock);
	mutex_init(&shid->raw_capture_lock);
	spi_set_drvdata(spi, shid);

	ret = sysfs_create_files(&dev->kobj, spi_hid_attributes);
	if (ret) {
		dev_err(dev, "Unable to create sysfs attributes\n");
		goto err0;
	}
	ret = sysfs_create_bin_file(&dev->kobj, &raw_capture_bin_attr);
	if (ret) {
		dev_err(dev, "Unable to create raw capture file\n");
		goto err1;
	}
	ret = sysfs_create_bin_file(&dev->kobj, &isolated_set_ring_bin_attr);
	if (ret) {
		dev_err(dev, "Unable to create isolated set ring file\n");
		goto err1;
	}

	ret = spi_hid_get_descriptor_reg(dev, &shid->device_descriptor_register);
	if (ret) {
		dev_err(dev, "failed to get HID descriptor register address\n");
		ret = -ENODEV;
		goto err1;
	}
	dev_info(dev, "HID desc reg = 0x%08x\n", shid->device_descriptor_register);
	seq_dbg(shid, 1, "probe descriptor address obtained\n");

	/*
	* input_register is used for read approval. Set to default value here.
	* It will be overwritten later with value from device descriptor
	*/
	shid->desc.input_register = SPI_HID_DEFAULT_INPUT_REGISTER;

	init_completion(&shid->output_done);

	if (dev->of_node) {
		shid->supply = devm_regulator_get(dev, "vdd");
		if (IS_ERR(shid->supply)) {
			if (PTR_ERR(shid->supply) != -EPROBE_DEFER)
				dev_err(dev, "Failed to get regulator: %ld\n",
						PTR_ERR(shid->supply));
			ret = PTR_ERR(shid->supply);
			goto err1;
		}

		shid->pinctrl = devm_pinctrl_get(dev);
		if (IS_ERR(shid->pinctrl)) {
			dev_err(dev, "Could not get pinctrl handle: %ld\n",
					PTR_ERR(shid->pinctrl));
			ret = PTR_ERR(shid->pinctrl);
			goto err1;
		}

		shid->pinctrl_reset = pinctrl_lookup_state(shid->pinctrl, "reset");
		if (IS_ERR(shid->pinctrl_reset)) {
			dev_err(dev, "Could not get pinctrl reset: %ld\n",
					PTR_ERR(shid->pinctrl_reset));
			ret = PTR_ERR(shid->pinctrl_reset);
			goto err1;
		}

		shid->pinctrl_active = pinctrl_lookup_state(shid->pinctrl, "active");
		if (IS_ERR(shid->pinctrl_active)) {
			dev_err(dev, "Could not get pinctrl active: %ld\n",
					PTR_ERR(shid->pinctrl_active));
			 ret = PTR_ERR(shid->pinctrl_active);
			 goto err1;
		}

		shid->pinctrl_sleep = pinctrl_lookup_state(shid->pinctrl, "sleep");
		if (IS_ERR(shid->pinctrl_sleep)) {
			dev_err(dev, "Could not get pinctrl sleep: %ld\n",
					PTR_ERR(shid->pinctrl_sleep));
			ret = PTR_ERR(shid->pinctrl_sleep);
			goto err1;
		}

		ret = pinctrl_select_state(shid->pinctrl, shid->pinctrl_sleep);
		if (ret) {
			dev_err(dev, "Could not select sleep state\n");
			goto err1;
		}

		msleep(100);
	}

	shid->hid_desc_addr = shid->device_descriptor_register;

	spin_lock_init(&shid->input_lock);
	INIT_WORK(&shid->reset_work, spi_hid_reset_work);
	INIT_WORK(&shid->create_device_work, spi_hid_create_device_work);
	INIT_WORK(&shid->refresh_device_work, spi_hid_refresh_device_work);
	INIT_WORK(&shid->error_work, spi_hid_error_work);
	INIT_DELAYED_WORK(&shid->descreq_work, spi_hid_seq_descreq_work);
	INIT_DELAYED_WORK(&shid->raw_handshake_watchdog, spi_hid_raw_handshake_watchdog);
	INIT_DELAYED_WORK(&shid->feat_delay_work, spi_hid_feat_delay_work);
	INIT_DELAYED_WORK(&shid->raw_transition_timeout_work,
			  spi_hid_raw_transition_timeout_work);
	INIT_DELAYED_WORK(&shid->isolated_set_timeout_work,
			  spi_hid_isolated_set_timeout_work);
	INIT_DELAYED_WORK(&shid->isolated_set_work, spi_hid_isolated_set_work);
	shid->raw_handshake_confirmed = false;
	shid->raw_handshake_retries_left = RAW_HANDSHAKE_MAX_RETRIES;
	/* raw_mode is disabled. */
	shid->raw_mode_active = false;
	shid->seq_dbg_last_state = -1;

	INIT_DELAYED_WORK(&shid->stream_watchdog, spi_hid_stream_watchdog_work);
	shid->stream_watchdog_active = false;

	INIT_DELAYED_WORK(&shid->poll_work, spi_hid_poll_work);
	shid->poll_active = false;
	shid->poll_interval_ms = poll_interval;
	shid->poll_missed = 0;
	shid->works_initialized = true;

	seq_dbg(shid, 1, "probe configuring IRQ\n");
	if (dev->of_node) {
		shid->irq = spi->irq;
	} else {
		/* Use SPI core's IRQ directly — skip gpiod_get to avoid EBUSY */
		shid->irq = spi->irq;
		if (shid->irq <= 0) {
			dev_err(dev, "No IRQ from SPI core\n");
			ret = -ENODEV;
			goto err1;
		}
		dev_info(dev, "GPIO: using irq=%d from SPI core\n", shid->irq);
	}

	irqflags = irq_get_trigger_type(shid->irq) | IRQF_ONESHOT;

	/* Windows-style GPIO dance BEFORE arming IRQ:
	 * ClearActiveInterrupts → ReconfigureInterrupt → UnmaskInterrupt
	 */
	if (!dev->of_node) {
		int irq = shid->irq;
		struct irq_data *id = irq_get_irq_data(irq);

		dev_info(dev, "GPIO dance: irq=%d\n", irq);
		seq_dbg(shid, 1, "GPIO dance begin\n");

		/* Mask IRQ */
		irq_set_irqchip_state(irq, IRQCHIP_STATE_MASKED, 1);

		/* Reconfigure trigger type */
		irq_set_irq_type(irq, irqflags & IRQF_TRIGGER_MASK);

		/* Clear pending interrupt */
		if (id && id->chip && id->chip->irq_ack)
			id->chip->irq_ack(id);
		irq_set_irqchip_state(irq, IRQCHIP_STATE_PENDING, 0);

		dev_info(dev, "GPIO dance: mask→reconf→clear done\n");
		seq_dbg(shid, 1, "GPIO dance complete\n");
	}

	/* Optional ACPI power-cycle experiment. */
	if (acpi_probe_power_cycle) {
		acpi_handle h = ACPI_HANDLE(dev);
		if (h) {
			acpi_status status;

			dev_info(dev, "SEQ: Power cycling device via ACPI _PS3 -> _PS0...\n");
			seq_dbg(shid, 1, "ACPI _PS3 begin\n");
			status = acpi_evaluate_object(h, "_PS3", NULL, NULL);
			if (ACPI_FAILURE(status)) {
				dev_warn(dev, "SEQ: ACPI _PS3 failed: %s\n",
					 acpi_format_exception(status));
			} else {
				seq_dbg(shid, 1, "ACPI _PS3 complete\n");
				msleep(50);
				seq_dbg(shid, 1, "ACPI _PS0 begin\n");
				status = acpi_evaluate_object(h, "_PS0", NULL, NULL);
				if (ACPI_FAILURE(status))
					dev_warn(dev, "SEQ: ACPI _PS0 failed: %s\n",
						 acpi_format_exception(status));
				else {
					seq_dbg(shid, 1, "ACPI _PS0 complete\n");
					msleep(100);
				}
			}
		}
	}

	shid->seq_enabled = true;
	spi_hid_seq_set_state(shid, 0, SPI_HID_SEQ_PROBE);
	shid->ready = shid->seq_state >= 4 ? true : false;
	shid->keep_powered = true;

	/* Allow ACPI initialization to complete before arming the IRQ. */
	seq_dbg(shid, 1, "probe settling delay begin\n");
	msleep(300);
	seq_dbg(shid, 1, "probe settling delay complete\n");
	shid->desc.input_register = 0x000000;

	if (0 && raw_mode) {
		dev_info(dev, "SEQ: sending vendor init (SET_POWER D2->D0)...\n");
		spi_hid_vendor_init(shid);
	}

	dev_info(dev, "SEQ: device powered by ACPI _INI, arming IRQ\n");

	/* Register the optional input device only for the passive beta path. */
	if (raw_input_beta && raw_capture_only) {
		shid->touch_input = input_allocate_device();
		if (shid->touch_input) {
			shid->touch_input->name = "Surface Touchscreen";
			shid->touch_input->phys = "spi-hid/input1";
			shid->touch_input->id.bustype = BUS_SPI;
			shid->touch_input->id.vendor = 0x045E;
			shid->touch_input->id.product = 0x0C19;
			shid->touch_input->dev.parent = &shid->spi->dev; /* attach to physical SPI dev */
			set_bit(INPUT_PROP_DIRECT, shid->touch_input->propbit);
			set_bit(EV_ABS, shid->touch_input->evbit);
			set_bit(EV_KEY, shid->touch_input->evbit);
			set_bit(BTN_TOUCH, shid->touch_input->keybit);
			input_set_abs_params(shid->touch_input, ABS_MT_POSITION_X, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_MT_POSITION_Y, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_X, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_Y, 0, 32767, 0, 0);
			input_abs_set_res(shid->touch_input, ABS_MT_POSITION_X, 112);
			input_abs_set_res(shid->touch_input, ABS_MT_POSITION_Y, 198);
			input_abs_set_res(shid->touch_input, ABS_X, 112);
			input_abs_set_res(shid->touch_input, ABS_Y, 198);
			input_set_abs_params(shid->touch_input, ABS_MT_TOUCH_MAJOR, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_MT_TOUCH_MINOR, 0, 32767, 0, 0);
			input_set_abs_params(shid->touch_input, ABS_MT_ORIENTATION, -89, 89, 0, 0);
			ret = input_mt_init_slots(shid->touch_input, HEATMAP_MAX_SLOTS,
						  INPUT_MT_DIRECT);
			if (ret) {
				dev_warn(dev, "HEATMAP: failed to init MT slots (%d)\n", ret);
				input_free_device(shid->touch_input);
				shid->touch_input = NULL;
				goto err1_touch;
			}
			if (input_register_device(shid->touch_input)) {
				dev_warn(dev, "HEATMAP: failed to register touch input device\n");
				input_free_device(shid->touch_input);
				shid->touch_input = NULL;
			} else {
				dev_info(dev, "HEATMAP: multitouch input device registered\n");
			}
		}
	}
	heatmap_init_c590(shid);
	heatmap_reset_baseline(shid);

	shid->data_buf = devm_kmalloc(dev, 8200, GFP_KERNEL);
	shid->data_buf_len = 8200;
	if (!shid->data_buf) {
		ret = -ENOMEM;
		goto err1;
	}

	seq_dbg(shid, 1, "request IRQ begin flags=0x%lx\n", irqflags);
	ret = request_threaded_irq(shid->irq, spi_hid_dev_irq, spi_hid_seq_thread,
				   irqflags, dev_name(&spi->dev), shid);
	if (ret) {
		dev_err(dev, "TRACE[hid] request IRQ failed: %d\n", ret);
		goto err1;
	}
	shid->irq_enabled = true;
	dev_info(dev, "SEQ: IRQ armed (state=WAIT_RESET, zero touch)\n");
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_IRQ_ARMED, 0);
	dev_info(dev, "TRACE[hid] probe complete: d3 -> %s\n",
		spi_hid_power_mode_string(shid->power_state));
	return 0;

	err1_touch:
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_PROBE_FAILED, ret);
	if (shid->touch_input) {
		input_free_device(shid->touch_input);
		shid->touch_input = NULL;
	}
	sysfs_remove_bin_file(&dev->kobj, &isolated_set_ring_bin_attr);
	sysfs_remove_bin_file(&dev->kobj, &raw_capture_bin_attr);
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);
	return ret;

err1:
	dev_err(dev, "TRACE[hid] probe failed ret=%d\n", ret);
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_PROBE_FAILED, ret);
	mutex_lock(&shid->seq_lock);
	WRITE_ONCE(shid->removing, true);
	WRITE_ONCE(shid->seq_enabled, false);
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	mutex_unlock(&shid->seq_lock);
	if (shid->irq_enabled) {
		disable_irq(shid->irq);
		spi_hid_cancel_workers(shid);
		free_irq(shid->irq, shid);
		shid->irq_enabled = false;
	} else {
		spi_hid_cancel_workers(shid);
	}
	spi_hid_stop_hid(shid);
	if (shid->touch_input) {
		input_unregister_device(shid->touch_input);
		shid->touch_input = NULL;
	}
	kfree(shid->heatmap_buf);
	shid->heatmap_buf = NULL;
	sysfs_remove_bin_file(&dev->kobj, &isolated_set_ring_bin_attr);
	sysfs_remove_bin_file(&dev->kobj, &raw_capture_bin_attr);
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);

err0:
	return ret;
}

static void spi_hid_remove(struct spi_device *spi)
{
	struct spi_hid *shid = spi_get_drvdata(spi);
	struct device *dev = &spi->dev;

	dev_info(dev, "removing driver instance\n");
	seq_dbg(shid, 1, "remove begin irq_enabled=%u ready=%u state=%d\n",
		shid->irq_enabled, shid->ready, shid->seq_state);
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_REMOVE, 0);
	mutex_lock(&shid->seq_lock);
	WRITE_ONCE(shid->removing, true);
	WRITE_ONCE(shid->seq_enabled, false);
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	mutex_unlock(&shid->seq_lock);
	if (shid->irq_enabled)
		disable_irq(shid->irq);
	spi_hid_cancel_workers(shid);
	if (shid->irq_enabled) {
		free_irq(shid->irq, shid);
		shid->irq_enabled = false;
	}
	spi_hid_power_down(shid);
	spi_hid_stop_hid(shid);
	if (shid->touch_input) {
		input_unregister_device(shid->touch_input);
		shid->touch_input = NULL;
	}
	kfree(shid->heatmap_buf);
	if (shid->gpiod)
		gpiod_put(shid->gpiod);
	sysfs_remove_bin_file(&dev->kobj, &isolated_set_ring_bin_attr);
	sysfs_remove_bin_file(&dev->kobj, &raw_capture_bin_attr);
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);
	dev_info(dev, "TRACE[hid] remove complete\n");
}

static const struct spi_device_id spi_hid_id_table[] = {
	{ "hid", 0 },
	{ "hid-over-spi", 0 },
	{ },
};
MODULE_DEVICE_TABLE(spi, spi_hid_id_table);

static struct spi_driver spi_hid_driver = {
	.driver = {
		.name	= "spi_hid",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(spi_hid_of_match),
		.acpi_match_table = ACPI_PTR(spi_hid_acpi_match),
	},
	.probe		= spi_hid_probe,
	.remove		= spi_hid_remove,
	.id_table	= spi_hid_id_table,
};

module_spi_driver(spi_hid_driver);

MODULE_DESCRIPTION("HID over SPI transport driver");
MODULE_LICENSE("GPL");
