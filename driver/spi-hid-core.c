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
#include "spi-hid-protocol.h"
#include "spi-hid-capimg.h"
#include "spi-hid_trace.h"
/* Hardcoded HID Report Descriptor from Windows dump (936 bytes) */
#include "hardcoded_rd.h"

_Static_assert(sizeof(hardcoded_report_descriptor) == HARDCODED_RD_SIZE,
	       "hardcoded RD size mismatch");

#define SPI_HID_MAX_RESET_ATTEMPTS 3
/* V0 has a 12-bit body field, but spi-amd only guarantees one atomic 70-byte
 * PIO segment. The six-byte V0 header leaves 64 bytes for its padded body. */
#define SPI_HID_V0_MAX_OUTPUT_BODY 64

static int debug_level;
static int getfeat_delay_ms;  /* RPT_DESC → GET_FEATURE settle time (0 = immediate, safe default) */
static bool skip_getfeat = true;
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

static int spi_hid_validate_dev_desc(const struct spi_hid_device_desc_raw *raw,
		size_t available)
{
	u16 desc_len = le16_to_cpu(raw->wDeviceDescLength);

	if (desc_len < sizeof(*raw) || desc_len > available)
		return -EPROTO;
	if (!le16_to_cpu(raw->wReportDescLength) ||
	    le16_to_cpu(raw->wReportDescLength) > SZ_8K ||
	    !le16_to_cpu(raw->wMaxInputLength))
		return -EPROTO;

	return 0;
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

static int spi_hid_output_header(__u8 *buf,
		u16 output_register, u16 output_report_length)
{
	return spi_hid_protocol_encode_output_header(buf, output_register,
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

	/*
	 * NOTE: This prepends a second 0x02 opcode byte. The caller's header
	 * already encodes the output opcode. This doubled-opcode behavior
	 * matches the observed Linux workaround but differs from Windows
	 * canonical 10-byte DESCREQ. Review for v1.0 protocol compliance.
	 */
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
	u8 raw_buf[14] = {
		0x02, 0x00, 0x00, 0x00, 0x82, 0x00, 0x00, 0x04,
		SPI_HID_CONTENT_TYPE_COMMAND, SPI_HID_COMMAND_SET_POWER,
		0x00, 0x00, 0x00, 0x00,
	};
	int ret;

	if (shid->desc.command_register == 0)
		return 0;

	raw_buf[1] = (shid->desc.command_register >> 16) & 0xff;
	raw_buf[2] = (shid->desc.command_register >> 8) & 0xff;
	raw_buf[3] = shid->desc.command_register & 0xff;
	raw_buf[10] = power_mode;

	mutex_lock(&shid->output_lock);
	{
		struct spi_transfer transfer;
		struct spi_message message;

		memset(&transfer, 0, sizeof(transfer));
		transfer.tx_buf = raw_buf;
		transfer.len = sizeof(raw_buf);

		spi_message_init_with_transfers(&message, &transfer, 1);
		ret = spi_sync(shid->spi, &message);
	}
	mutex_unlock(&shid->output_lock);

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

/* _RST calls M010 which DESTROYS the device (2026-07-07).
 * On ACPI systems, _INI has already powered the device. Just wait. */
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
static void heatmap_process_frame(struct spi_hid *shid, const u8 *data, u32 data_len, u8 content_id);
static void seq_handle_reset(struct spi_hid *shid, int type, u16 blen, bool *expect_fast);
static void seq_handle_desc(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_rpt(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_feat(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_vendor(struct spi_hid *shid, int type, u16 blen);
static void seq_handle_data(struct spi_hid *shid, int type, u16 blen);

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
		ret = spi_hid_output_header(buf->header, shid->desc.command_register, body_len);
		if (!ret)
			ret = spi_hid_output(shid, buf, sizeof(buf->header) + body_len);
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

	/*
	 * content points into buf->content which is offset within a larger
	 * struct — the -1 pointer arithmetic to prepend report ID is valid.
	 */
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
		u8 report_type, u8 content_id)
{
	trace_spi_hid_response_handler(shid);
	dev_dbg(&shid->spi->dev, "response handler\n");

	if (!READ_ONCE(shid->output_pending) ||
	    READ_ONCE(shid->expected_response_type) != report_type ||
	    READ_ONCE(shid->expected_response_id) != content_id) {
		dev_warn(&shid->spi->dev, "Unexpected response type 0x%x ID 0x%x\n",
			report_type, content_id);
		return -EPROTO;
	}
	complete(&shid->output_done);

	return 0;
}

static int spi_hid_send_output_report(struct spi_hid *shid, u32 output_register,
		struct spi_hid_output_report *report)
{
	u8 *raw_buf;
	u16 payload_len = report->content_length;
	u16 total_content_len;
	u16 body_len;
	u32 body_len_u32;
	size_t total_len;
	int ret;

	if (payload_len && !report->content)
		return -EINVAL;
	if (payload_len > U16_MAX - 3)
		return -EMSGSIZE;

	/* V0 body: type + total_content_len + content ID + payload, padded to 4. */
	total_content_len = payload_len + 3;
	body_len_u32 = round_up((u32)payload_len + 4, 4);
	if (body_len_u32 > SPI_HID_V0_MAX_OUTPUT_BODY ||
	    !spi_hid_protocol_output_length_valid(body_len_u32))
		return -EMSGSIZE;
	body_len = body_len_u32;
	total_len = SPI_HID_OUTPUT_HEADER_LEN + body_len;

	raw_buf = kzalloc(total_len, GFP_KERNEL);
	if (!raw_buf)
		return -ENOMEM;

	ret = spi_hid_protocol_encode_output_header(raw_buf, output_register, body_len);
	if (ret) {
		kfree(raw_buf);
		return ret;
	}
	raw_buf[6] = report->content_type;
	raw_buf[7] = total_content_len & 0xff;
	raw_buf[8] = total_content_len >> 8;
	raw_buf[9] = report->content_id;
	if (payload_len)
		memcpy(&raw_buf[10], report->content, payload_len);

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
		struct spi_hid_output_report *report, u8 expected_response_type)
{
	struct device *dev = &shid->spi->dev;
	int ret = 0;

	/* A completion is single-use: never let a prior response satisfy this request. */
	reinit_completion(&shid->output_done);
	WRITE_ONCE(shid->expected_response_type, expected_response_type);
	WRITE_ONCE(shid->expected_response_id, report->content_id);
	WRITE_ONCE(shid->output_pending, true);
	ret = spi_hid_send_output_report(shid, output_register,
			report);
	if (ret) {
		WRITE_ONCE(shid->output_pending, false);
		dev_err(dev, "failed to transfer output report\n");
		return ret;
	}

	/*
	 * Release shid->lock before blocking on completion to allow the
	 * IRQ thread to process the response. The caller (ll_raw_request)
	 * expects this release/reacquire pattern.
	 */
	mutex_unlock(&shid->lock);
	ret = wait_for_completion_interruptible_timeout(&shid->output_done,
			msecs_to_jiffies(1000));
	mutex_lock(&shid->lock);
	WRITE_ONCE(shid->output_pending, false);
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
		.content_length = 0,
		.content_id = 0,
		.content = NULL,
	};


	ret =  spi_hid_sync_request(shid,
			shid->desc.report_descriptor_register, &report,
			SPI_HID_REPORT_TYPE_REPORT_DESC);
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

	{
		u16 total_content_len = buf->body[0] | (buf->body[1] << 8);

		if (total_content_len < SPI_HID_INPUT_BODY_LEN ||
		    total_content_len > header.report_length) {
			dev_err(dev, "Bad body length %u for report body %u\n",
				total_content_len, header.report_length);
			return -EINVAL;
		}
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
		if (body.content_length < sizeof(*raw)) {
			dev_err(dev, "device descriptor too short: %u\n", body.content_length);
			return -EPROTO;
		}
		/* Reset attempts at every device descriptor fetch */
		shid->attempts = 0;
		raw = (struct spi_hid_device_desc_raw *) buf->content;
		ret = spi_hid_validate_dev_desc(raw, body.content_length);
		if (ret) {
			dev_err(dev, "invalid device descriptor: %d\n", ret);
			return ret;
		}
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
		ret = spi_hid_response_handler(shid, header.report_type, body.content_id);
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
	if (header->report_length < SPI_HID_INPUT_BODY_LEN) {
		dev_err(dev, "Input report body too short: %u\n", header->report_length);
		return -EMSGSIZE;
	}

	if (!shid->raw_mode_active && shid->desc.max_input_length != 0 &&
	    header->report_length > shid->desc.max_input_length) {
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
	/* 2026-07-08: hid_add_device() only reports whether the device was
	 * registered on the bus, not whether any driver actually bound to it
	 * — a corrupted wire-read descriptor can make hid_parse_report()
	 * return success (its own per-item validation is lenient, e.g.
	 * "unexpected long global item" is only a warning there) while the
	 * matched driver's own probe (hid-generic in our case) fails
	 * synchronously inside this same call, leaving hid->driver NULL.
	 * That's the actual reliable "this descriptor wasn't good enough"
	 * signal (see docs/NEXT_STEPS.md §C), not hid_parse_report()'s return
	 * code or hid_add_device()'s return code. */
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

		if (shid->wire_report_descriptor_len > 0 &&
		    !shid->wire_report_descriptor_rejected) {
			/*
			 * Only one retry with hardcoded descriptor. If
			 * hardcoded descriptor also fails, the device is left
			 * without HID driver. Consider additional fallback
			 * strategies.
			 */
			dev_warn(dev, "SEQ: forcing hardcoded report descriptor fallback and retrying once\n");
			shid->wire_report_descriptor_rejected = true;
			return spi_hid_create_device(shid);
		}
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
		.content_length = 0,
		.content_id = content_id,
		.content = NULL,
	};


	return spi_hid_sync_request(shid, shid->desc.output_register,
			&report, SPI_HID_REPORT_TYPE_GET_FEATURE_RESP);
}

static int spi_hid_set_request(struct spi_hid *shid,
		u8 *arg_buf, u16 arg_len, u8 content_id)
{
	if (arg_len > U16_MAX - 4)
		return -EMSGSIZE;

	struct spi_hid_output_report report = {
		.content_type = SPI_HID_CONTENT_TYPE_SET_FEATURE,
		.content_length = arg_len,
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

/* raw_mode handshake watchdog (2026-07-08, docs/GROUND_TRUTH.md §18.7): SET_FEATURE
 * occasionally makes the device go completely silent (no further IRQ at all, not even
 * a RESET_RSP), so the existing IRQ-triggered retry in spi_hid_seq_thread() never gets
 * a chance to run. Decompiling the real HidSpiCx.sys showed Windows's own SmFx state
 * machine hits the same intermittent failure and papers over it with a bounded,
 * timer-based retry — CompleteTransferIfDoneOrStartResponseTimer arms a 2000ms response
 * timer, and CheckingResetRetryCountEntry retries up to 3 times before giving up. This
 * mirrors those exact parameters. */
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
	/* Windows-style recovery: SET_POWER(D2→D0) instead of _PS3→_PS0.
	 * The D2→D0 cycle is a "soft" reset that doesn't cut physical power,
	 * matching how Windows recovers from a failed feature handshake.
	 *
	 * 2026-07-12: cancel any pending feat_delay_work (otherwise
	 * feat_delay_work's 5900ms timer and our 2000ms watchdog timer race —
	 * the watchdog always fires first, creating an infinite reset loop).
	 * Re-arm the watchdog with a timeout longer than getfeat_delay_ms so
	 * the next feat_delay_work always has time to fire before us. */
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
	u8 gf[11] = {
		0x02, 0x02, 0x00, 0x00, 0x03, 0x42,
		0x00, 0x04, 0x03, 0x00, 0x06
	};

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || !shid->feat_delay_pending)
		goto out;
	shid->feat_delay_pending = false;

	if (shid->seq_state != 2) {
		seq_dbg(shid, 1, "SEQ: feat_delay_work: state changed to %d, skipping\n",
			shid->seq_state);
		goto out;
	}
	if (skip_getfeat) {
		static const u8 vendor_init[] = {
			0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00,
			0x03, 0x0A, 0x00, 0x56, 0xBD, 0x0C, 0xEE,
			0x5B, 0x44, 0x4C, 0x00, 0x00
		};
		u8 sf[15] = {
			0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
			0x03, 0x04, 0x00, 0x05, 0x01,
			0x00, 0x00, 0x00
		};

		seq_dbg(shid, 1, "SEQ: delayed vendor init + SET_FEATURE\n");
		if (spi_hid_seq_write(shid, vendor_init, sizeof(vendor_init), NULL, 0))
			goto retry_watchdog;
		usleep_range(36000, 39000);
		if (spi_hid_seq_write(shid, sf, sizeof(sf), NULL, 0))
			goto retry_watchdog;
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_REQUEST);
		mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
			msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		goto out;
	}

	seq_dbg(shid, 1, "SEQ: raw_mode=1 -> GET_FEATURE after delay, state 5\n");
	if (spi_hid_seq_write(shid, gf, sizeof(gf), NULL, 0)) {
		goto retry_watchdog;
	}
	spi_hid_seq_set_state(shid, 5, SPI_HID_SEQ_FEATURE_REQUEST);
	mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
			 msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
	goto out;

retry_watchdog:
	dev_warn(dev, "SEQ: delayed raw handshake write failed\n");
	mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
		msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
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
	seq_dbg(shid, 2, "SEQ: poll-work: type=%d raw=[%*ph]\n", type, 10, hdr);
	if (type == 7) {
		shid->stat_device_desc++;
		seq_dbg(shid, 1, "SEQ: poll-work: GOT DEVICE_DESC!\n");
		/* Re-trigger the IRQ thread to handle it */
	} else if (type == 3) {
		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: poll-work: still RESET_RSP, DESCREQ failed\n");
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FALLBACK);
		shid->ready = true;
		shid->keep_powered = true;
		dev_warn(&shid->spi->dev, "SEQ: poll-work: DESCREQ failed, using hardcoded fallback descriptors\n");
		/* Hardcode and create device */
		shid->desc.hid_version = 0x0100;
		shid->desc.report_descriptor_length = 936;
		shid->desc.report_descriptor_register = 0x0002;
		shid->desc.input_register = 0x0000;
		shid->desc.max_input_length = 0x1000;
		shid->desc.output_register = 0x0003;
		shid->desc.max_output_length = 0x0100;
		shid->desc.command_register = 0x0004;
		shid->desc.vendor_id = 0x045E;
		shid->desc.product_id = 0x0C19;
		shid->desc.version_id = 0x0100;
		if (!shid->hid) spi_hid_create_device(shid);
	} else {
		seq_dbg(shid, 1, "SEQ: poll-work: unexpected type=%d, retrying...\n", type);
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
	}
out:
	mutex_unlock(&shid->seq_lock);
}


/* Heatmap blob detection (2026-07-08 D).
 * The raw content_id=0x0C frame is a capacitive sensor heatmap:
 * 2 bytes SurfaceSwitch (timestamp/scan ID), then cell data.
 * Each byte indexes the c590 lookup table for actual signal level.
 * Baseline is tracked per cell via dynamic max-tracking (~30 frames).
 * Detect significant signal changes, cluster into contiguous blobs via
 * connected-component labeling, compute weighted centroids, and emit
 * multitouch events.
 */
/* DFT antenna frame layout (content_id=0x0C, ~4302 bytes):
 *   byte[0]      content_id (0x0C)
 *   byte[1..2]   SurfaceSwitch (16-bit timestamp)
 *   byte[3..25]  frame metadata (23 bytes)
 *   byte[26..]   capacitive node magnitudes (288 columns, row-major, 1 byte each)
 * Each byte is an index into the c590 signal lookup table.
 * Row count is auto-detected from the payload size. */
#define HEATMAP_DFT_META_LEN  23
#define HEATMAP_DATA_OFFSET   (3 + HEATMAP_DFT_META_LEN)   /* = 26 */

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
static bool raw_mode;
module_param(raw_mode, bool, 0444);
MODULE_PARM_DESC(raw_mode,
	"0 = standard HID mode (single-touch, Report ID 0x40); "
	"1 = raw DFT heatmap mode (send GET_FEATURE/SET_FEATURE, multi-touch blob detection)");

static bool raw_input_beta = true;
module_param(raw_input_beta, bool, 0444);
MODULE_PARM_DESC(raw_input_beta,
	"Publish beta multitouch input device from decoded CapImg heatmap frames");

/* Kept enabled for compatibility with the currently validated boot path.
 * Disable only for a controlled trace comparison against ACPI _INI startup. */
static bool acpi_probe_power_cycle = false;
module_param(acpi_probe_power_cycle, bool, 0444);
MODULE_PARM_DESC(acpi_probe_power_cycle,
	"Power-cycle ACPI _PS3->_PS0 at probe (legacy default; experimental to disable)");

/* 2026-07-08 (handshake reliability experiments, GROUND_TRUTH.md §18.7): SET_FEATURE
 * always writes fine at the driver level but the device silently stops responding
 * afterward most of the time. Two testable hypotheses, toggled independently so results
 * aren't conflated: (1) the write happens too fast/at the wrong SPI clock speed for the
 * touch chip to sample correctly; (2) our seq_write() opcode-doubling quirk (needed for
 * DESCREQ/GET_FEATURE) doesn't apply the same way to this specific, longer write. */
static uint setfeat_speed_hz;
module_param(setfeat_speed_hz, uint, 0444);
MODULE_PARM_DESC(setfeat_speed_hz,
	"Override SPI clock speed (Hz) for the SET_FEATURE write only; 0 = bus default (33.33MHz)");

static bool setfeat_no_double;
module_param(setfeat_no_double, bool, 0444);
MODULE_PARM_DESC(setfeat_no_double,
	"Send SET_FEATURE without the leading-opcode-doubling quirk (14 bytes instead of 15)");

module_param(skip_getfeat, bool, 0444);
MODULE_PARM_DESC(skip_getfeat,
	"Skip GET_FEATURE, send SET_FEATURE directly after RPT_DESC");

module_param(debug_level, int, 0444);
MODULE_PARM_DESC(debug_level, "Log verbosity: 0=errors, 1=transitions, 2=per-frame, 3=full hex");

/* ── Blob detection tunables (runtime-validated) ────────────────── */

static int blob_min_weight = 1000;
module_param(blob_min_weight, int, 0644);
MODULE_PARM_DESC(blob_min_weight,
	"Minimum c590 signal rise sum across blob to consider it valid");

static int ema_alpha = 10;
module_param(ema_alpha, int, 0644);
MODULE_PARM_DESC(ema_alpha,
	"EMA smoothing: (prev*alpha + new)/(alpha+1). Higher = smoother. Default 10 (weight ~1/11)");

static int dfa_data_offset;
module_param(dfa_data_offset, int, 0444);
MODULE_PARM_DESC(dfa_data_offset,
	"DFT antenna frame data offset in bytes (0 = decoded raster, no offset)");

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

static int ghost_dist = 6;
module_param(ghost_dist, int, 0644);
MODULE_PARM_DESC(ghost_dist,
	"Coalescence radius in grid cells (Surface: distance < 6 cells)");

static int grid_cols = 0;  /* 0 = default 72 */
module_param(grid_cols, int, 0444);
MODULE_PARM_DESC(grid_cols, "Override grid columns (0=default 72)");

static int grid_rows = 0;  /* 0 = default 48 */
module_param(grid_rows, int, 0444);
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

static int blob_lift_frames = 3;
module_param(blob_lift_frames, int, 0644);
MODULE_PARM_DESC(blob_lift_frames, "Consecutive missed frames before lifting");

static int blob_max_distance = 3;
module_param(blob_max_distance, int, 0644);
MODULE_PARM_DESC(blob_max_distance,
	"Max grid distance for slot re-assignment (Surface: 0.545 cells ≈ 3)");

/* ── Raw-mode handshake timing (EXPERIMENTAL) ───────────────────── */
module_param(getfeat_delay_ms, int, 0444);
MODULE_PARM_DESC(getfeat_delay_ms,
	"Delay in ms between RPT_DESC and GET_FEATURE (default 5900; 0=immediate)");
/* Default matches the Windows-observed ~5.9s RPT_DESC->GET_FEATURE gap. */

static int stream_watchdog_ms = 0;  /* disabled by default, too aggressive for raw mode */
module_param(stream_watchdog_ms, int, 0444);
MODULE_PARM_DESC(stream_watchdog_ms,
	"Runtime streaming watchdog interval in ms (0=disable, Windows uses 2000)");

static int stream_watchdog_max_retries = 3;
module_param(stream_watchdog_max_retries, int, 0444);
MODULE_PARM_DESC(stream_watchdog_max_retries,
	"Max re-init retries before giving up");

/* ── Runtime recovery ──────────────────────────────────────────── */
#if 0 /* deprecated: IRQ is the sole input-report consumer */
static int poll_interval;
module_param(poll_interval, int, 0444);
MODULE_PARM_DESC(poll_interval,
	"Deprecated and ignored: IRQ is the sole input-report consumer");
#endif

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
	memset(shid->heatmap_touched, 0, sizeof(shid->heatmap_touched));
	memset(shid->heatmap_expanded, 0, sizeof(shid->heatmap_expanded));
	memset(shid->heatmap_label, 0, sizeof(shid->heatmap_label));
	memset(shid->blob_slot_hx, 0, sizeof(shid->blob_slot_hx));
	memset(shid->blob_slot_hy, 0, sizeof(shid->blob_slot_hy));
	memset(shid->blob_slot_hpos, 0, sizeof(shid->blob_slot_hpos));
	memset(shid->blob_slot_hcount, 0, sizeof(shid->blob_slot_hcount));
	memset(shid->blob_slot_stationary, 0, sizeof(shid->blob_slot_stationary));
	memset(shid->blob_slot_blob, 0, sizeof(shid->blob_slot_blob));
	memset(shid->blob_x, 0, sizeof(shid->blob_x));
	memset(shid->blob_y, 0, sizeof(shid->blob_y));
	memset(shid->blob_wsum, 0, sizeof(shid->blob_wsum));
	memset(shid->blob_xsum, 0, sizeof(shid->blob_xsum));
	memset(shid->blob_ysum, 0, sizeof(shid->blob_ysum));
	memset(shid->blob_active, 0, sizeof(shid->blob_active));
	memset(shid->label_equiv, 0, sizeof(shid->label_equiv));
	shid->heatmap_frame_id = 0;
	shid->heatmap_last_consecutive = 0;
}

/* Screen mapping: logical range 0..32767 for both X and Y.
 * Reverse-engineered from TouchPenProcessor0C19.dll (GROUND_TRUTH.md §22.3):
 * raw frame is 288 columns (stride 0x120), rows auto-detected from data size.
 * Each byte indexes a float[256] lookup table (c590) for actual signal level.
 * Signal = c590[raw_byte] = max_signal - (index * step + offset).
 *
 * Grid calibration formula (§22.8):
 *   screen_x = grid_x * scale_x  where scale_x = (phys_x * SCALE) / (grid_w - 1)
 *   screen_y = grid_y * scale_y  where scale_y = (phys_y * SCALE) / (grid_h - 1)
 *
 * Using max range 0..32767 with aspect ratio from HID descriptor (2934×1652).
 *
 * Grid geometry (empirically derived 2026-07-13 from surface_touch.csv raw
 * frames): the content_id=0x0C report carries a contiguous 3456-byte cell
 * field starting at dfa_data_offset=26, laid out as 72 columns × 48 rows
 * (row-major, 72-wide fast axis). This is a clean 3:2 grid matching the
 * landscape display — confirmed by connected-component / compactness analysis
 * across 150 touch frames (W=72 gave a single compact blob per finger; other
 * widths fragmented the blobs). The earlier 288×~14 assumption stretched Y
 * onto ~14 rows and scrambled the X/Y mapping. The trailing ~820 bytes of the
 * frame past cell 3456 are footer/metadata and must NOT be treated as cells.
 */
#define GRID_COLS_DEFAULT   72     /* default, overridden by auto-detect or module param */
#define GRID_ROWS_DEFAULT   48
#define GRID_CELLS_DEFAULT  (GRID_COLS_DEFAULT * GRID_ROWS_DEFAULT)  /* 3456 */
#define GRID_CELLS  (288 * 50)     /* max buffer allocation (headroom) */
#define GRID_ROW_STRIDE_DEFAULT GRID_COLS_DEFAULT

/* Signal lookup table (c590[256]). Extracted from TouchPenProcessor0C19.dll .rdata:
 *   c590[i] = 1.0 - (i * 0.00222035428 + 0.600000024)
 * Range: 0.4 (resting) down to ~-0.166 (byte 255).
 * Fixed-point: c590[i] = (10000 - (i * 22204 / 1000 + 6000)), clamped >= 0.
 * Scaled to [0, 4000] with 4 decimal digits of precision. */
#define C590_BASE   10000
#define C590_STEP_NUM 22204   /* 0.00222035428 * 10000000 / 1000 */
#define C590_STEP_DEN 1000
#define C590_OFFSET 6000      /* 0.600000024 * 10000 ≈ 6000 */

static void heatmap_init_c590(struct spi_hid *shid)
{
	int i;
	for (i = 0; i < 256; i++) {
		s32 v = 10000 - ((s32)i * C590_STEP_NUM / C590_STEP_DEN + C590_OFFSET);
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
			if (spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0)) {
				dev_warn(dev, "SEQ: stream watchdog DESCREQ failed\n");
			} else {
				spi_hid_seq_set_state(shid, 1, SPI_HID_SEQ_WATCHDOG);
			}
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
	struct device *dev = &shid->spi->dev;
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
		u32 cap = shid->raw_mode_active ? shid->data_buf_len :
			  (shid->desc.max_input_length ? shid->desc.max_input_length : 0x1000);

		blen = (((hdr[6] >> 4) & 0xF) << 0) | (hdr[7] << 4);
		blen *= 4;

		{
			u32 wanted = blen + 5;
			u32 rblen = min_t(u32, wanted, cap);
			u32 avail;
			u16 rl;

			if (shid->raw_mode_active && wanted > shid->data_buf_len) {
				dev_warn_ratelimited(dev,
					"SEQ: raw DATA body %u exceeds buffer %u, dropped\n",
					wanted, shid->data_buf_len);
				shid->stat_frames_dropped++;
				goto resched;
			}

			rblen = min_t(u32, rblen, shid->data_buf_len);
			avail = (rblen > 8) ? (rblen - 8) : 0;

			ret = spi_hid_seq_read(shid, shid->data_buf, rblen);
			if (ret)
				goto resched;

			if (rblen < 9)
				goto resched;
			rl = shid->data_buf[6] | (shid->data_buf[7] << 8);

			shid->stat_data++;
			seq_dbg(shid, 2, "SEQ: poller cid=0x%02x len=%u\n",
				 shid->data_buf[8], rl);

			if (rl >= 3 && rl - 3 > avail) {
				dev_warn_ratelimited(dev,
					"SEQ: poller DATA report len=%u exceeds buffer (avail=%u), dropped\n",
					rl, avail);
				shid->stat_frames_dropped++;
				goto resched;
			}

			if (shid->raw_mode_active && shid->data_buf[8] == 0x0C &&
			    shid->touch_input) {
				u32 clen = (rl > 2) ? (rl - 2) : 0;

				heatmap_process_frame(shid, &shid->data_buf[8],
						      clen, shid->data_buf[8]);
		} else if (rl > 3 && rl - 3 <= avail) {
			if (shid->hid) {
				int hret = hid_input_report(shid->hid,
					HID_INPUT_REPORT,
					&shid->data_buf[8],
					rl - 3, 1);
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

static void heatmap_process_frame(struct spi_hid *shid, const u8 *data, u32 data_len, u8 content_id)
{
	struct device *dev = &shid->spi->dev;
	u32 i, cell_count, ncols, nrows;
	u16 nlabels;
	int data_offset;
	int configured_cols, configured_rows;
	int touched_count = 0;

	/* Parameters are fixed at module load; validate the frame before caching geometry. */
	data_offset = READ_ONCE(dfa_data_offset);
	configured_cols = READ_ONCE(grid_cols);
	configured_rows = READ_ONCE(grid_rows);
	if (data_offset < 0 || data_offset >= data_len)
		return;

	/* Clamp unsafe parameters to prevent division by zero
	 * and undefined behavior. Use local variables to avoid
	 * overwriting user-set sysfs values every frame. */
	{
		s32 val;
		val = READ_ONCE(blob_min_weight); if (val < 1) val = 1;
		blob_min_weight = val;  /* one-time clamp only */
		val = READ_ONCE(ema_alpha); if (val < 0 || val > 10000) val = 3;
		ema_alpha = val;
		val = READ_ONCE(blob_debounce); if (val < 1) val = 3;
		blob_debounce = val;
		val = READ_ONCE(blob_lift_frames); if (val < 1) val = 3;
		blob_lift_frames = val;
		val = READ_ONCE(blob_max_distance); if (val < 1) val = 3;
		blob_max_distance = val;
		val = READ_ONCE(ghost_dist); if (val < 1) val = 6;
		ghost_dist = val;
		val = READ_ONCE(touch_threshold_pct); if (val < 0) val = 0;
		if (val > 100) val = 100;
		touch_threshold_pct = val;
	}

	/* The candidate cell field begins after metadata. Its geometry is not yet
	 * proven, so never infer cells from a short or malformed frame. */
	{
		u32 avail = data_len - data_offset;
		cell_count = avail;
	}

	/* The 72×48 default is an unvalidated experimental candidate. It remains
	 * configurable only for controlled capture comparison, never calibration. */
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
		if (nrows > HEATMAP_MAX_CELLS / ncols || ncols * nrows > cell_count) {
			dev_warn(dev, "HEATMAP: frame has %u bytes, insufficient for %ux%u grid\n",
				 cell_count, ncols, nrows);
			return;
		}
		shid->heatmap_grid_cols = ncols;
		shid->heatmap_grid_rows = nrows;
		seq_dbg(shid, 1, "HEATMAP: grid %u cols × %u rows (offset %d, frame avail %u cells)\n",
			 ncols, nrows, data_offset, cell_count);
	}
	ncols = shid->heatmap_grid_cols;
	nrows = shid->heatmap_grid_rows;

	/* Clamp to the candidate grid so trailing bytes cannot become cells. */
	if (!ncols || nrows > HEATMAP_MAX_CELLS / ncols) {
		dev_warn(dev, "HEATMAP: invalid grid %ux%u\n", ncols, nrows);
		return;
	}
	if (cell_count > ncols * nrows)
		cell_count = ncols * nrows;

	/* Frame-gap detection (Windows: >5 frames → release all contacts).
	 * heatmap_frame_id is a monotonic counter; we detect gaps by
	 * tracking the last consecutive value. */
	shid->heatmap_frame_id++;
	if (shid->heatmap_frame_id > 0 &&
	    shid->heatmap_frame_id - shid->heatmap_last_consecutive > 6) {
		release_all_slots(shid->touch_input, shid->blob_slot_state,
				  HEATMAP_MAX_SLOTS);
		memset(shid->blob_slot_stationary, 0, sizeof(shid->blob_slot_stationary));
		memset(shid->blob_slot_hcount, 0, sizeof(shid->blob_slot_hcount));
		memset(shid->blob_slot_hpos, 0, sizeof(shid->blob_slot_hpos));
	}
	shid->heatmap_last_consecutive = shid->heatmap_frame_id;

	if (cell_count > HEATMAP_MAX_CELLS) {
		dev_warn(dev, "HEATMAP: frame too large (%u cells > %u max)\n", cell_count, HEATMAP_MAX_CELLS);
		return;
	}

	/* Store raw frame for sysfs debug */
	if (!shid->heatmap_buf || shid->heatmap_len < data_len) {
		u8 *old_buf = shid->heatmap_buf;
		shid->heatmap_buf = kmalloc(data_len, GFP_KERNEL);
		if (shid->heatmap_buf) {
			kfree(old_buf);
			shid->heatmap_len = data_len;
		}
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

	/* Step 1: compute signal rise per cell. */
	memset(shid->heatmap_touched, 0, cell_count);
	for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
		s16 base = shid->c590_lut[shid->heatmap_baseline[i]];
		s16 curr = shid->c590_lut[data[data_offset + i]];
		s16 rise = curr - base;
		shid->heatmap_touched[i] = (rise >= 400) ? 1 : 0;
	}

	/* Step 2: peak detection — cross-shaped 5-cell check
	 * (matching Python oracle / Windows FUN_1805fba00).
	 * A cell is a peak if no neighbor at ±5 in N/S/E/W has a
	 * HIGHER signal rise. Min rise = 500. Max 16 peaks. */
	memset(shid->blob_xsum, 0, sizeof(shid->blob_xsum));
	memset(shid->blob_ysum, 0, sizeof(shid->blob_ysum));
	memset(shid->blob_wsum, 0, sizeof(shid->blob_wsum));
	memset(shid->blob_active, 0, sizeof(shid->blob_active));
	nlabels = 0;

	{
		struct { u16 col; u16 row; u16 rise; } peaks[16];
		s16 npeaks = 0, pi, ci;

		for (i = 0; i < cell_count && npeaks < 16; i++) {
			u16 col, row;
			s16 rise;

			if (!shid->heatmap_touched[i]) continue;
			col = i % ncols; row = i / ncols;
			if (row >= nrows) break;
			{
				s16 base = shid->c590_lut[shid->heatmap_baseline[i]];
				s16 curr = shid->c590_lut[data[data_offset + i]];
				rise = curr - base;
			}
			if (rise < 500) continue;

			/* Cross-shaped: check ±5 in N,S,E,W directions only */
			{
				int k; bool ok = true;
				s16 nb[4] = { -9999, -9999, -9999, -9999 };
				if (col >= 5 && shid->heatmap_touched[i - 5])
					nb[0] = (s16)shid->c590_lut[data[data_offset + i - 5]] -
						shid->c590_lut[shid->heatmap_baseline[i - 5]];
				if (col + 5 < ncols && shid->heatmap_touched[i + 5])
					nb[1] = (s16)shid->c590_lut[data[data_offset + i + 5]] -
						shid->c590_lut[shid->heatmap_baseline[i + 5]];
				if (row >= 5 && shid->heatmap_touched[i - 5 * ncols])
					nb[2] = (s16)shid->c590_lut[data[data_offset + i - 5 * ncols]] -
						shid->c590_lut[shid->heatmap_baseline[i - 5 * ncols]];
				if (row + 5 < nrows && shid->heatmap_touched[i + 5 * ncols])
					nb[3] = (s16)shid->c590_lut[data[data_offset + i + 5 * ncols]] -
						shid->c590_lut[shid->heatmap_baseline[i + 5 * ncols]];
				for (k = 0; k < 4; k++) {
					if (nb[k] > rise) { ok = false; break; }
				}
				if (!ok) continue;
			}

			peaks[npeaks].col = col;
			peaks[npeaks].row = row;
			peaks[npeaks].rise = rise;
			npeaks++;
		}

		/* Sort by signal strength (strongest first). */
		for (pi = 0; pi + 1 < npeaks; pi++)
			for (ci = pi + 1; ci < npeaks; ci++)
				if (peaks[ci].rise > peaks[pi].rise) {
					typeof(peaks[0]) t = peaks[pi];
					peaks[pi] = peaks[ci]; peaks[ci] = t;
				}

		/* Step 3: 5×5 local centroid around each peak
		 * (matching Python oracle _centroid function).
		 * Only cells within ±2 of the peak contribute to
		 * the weighted centroid. */
		for (pi = 0; pi < npeaks; pi++) {
			s64 sx = 0, sy = 0, sw = 0;
			s32 pr, pc, r, c;

			pc = peaks[pi].col;
			pr = peaks[pi].row;
			for (r = max(0, pr - 2); r <= min((s32)nrows - 1, pr + 2); r++) {
				for (c = max(0, pc - 2); c <= min((s32)ncols - 1, pc + 2); c++) {
					u32 idx = (u32)r * ncols + (u32)c;
					s16 w;

					if (idx >= cell_count || !shid->heatmap_touched[idx]) continue;
					{
						s16 base = shid->c590_lut[shid->heatmap_baseline[idx]];
						s16 curr = shid->c590_lut[data[data_offset + idx]];
						w = curr - base;
					}
					if (w <= 0) continue;
					sx += (s64)c * w;
					sy += (s64)r * w;
					sw += w;
				}
			}
			if (sw >= blob_min_weight) {
				shid->blob_x[pi] = (u16)(sx / sw);
				shid->blob_y[pi] = (u16)(sy / sw);
				shid->blob_wsum[pi] = (u32)sw;
				shid->blob_active[pi] = true;
				nlabels++;
				touched_count++;
			}
		}
	}

	/* Eigenvalue decomposition for the heaviest blob (GROUND_TRUTH.md §22.6) */
	{
		u32 best_idx = 0;
		u32 best_w = 0;
		s64 sum_xx = 0, sum_yy = 0, sum_xy = 0;

		for (i = 0; i < HEATMAP_MAX_BLOBS; i++) {
			if (shid->blob_active[i] && shid->blob_wsum[i] > best_w) {
				best_w = shid->blob_wsum[i];
				best_idx = i;
			}
		}
		if (best_w >= (u32)blob_min_weight) {
			s32 cx = (s32)shid->blob_x[best_idx];
			s32 cy = (s32)shid->blob_y[best_idx];

			for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
				u16 label;
				s32 dx, dy, w;
				s16 base, curr;

				if (!shid->heatmap_touched[i])
					continue;
				label = shid->heatmap_label[i];
				while (label < 256 && shid->label_equiv[label])
					label = shid->label_equiv[label];
				if (label != best_idx)
					continue;

				base = shid->c590_lut[shid->heatmap_baseline[i]];
				curr = shid->c590_lut[data[data_offset + i]];
				w = (s32)curr - (s32)base;
				if (w <= 0)
					continue;

				dx = (s32)(i % ncols) - cx;
				dy = (s32)(i / ncols) - cy;
				sum_xx += dx * dx * w;
				sum_yy += dy * dy * w;
				sum_xy += dx * dy * w;
			}

			{
				s64 trace = (s64)sum_xx + (s64)sum_yy;
				s64 det = (s64)sum_xx * (s64)sum_yy -
					  (s64)sum_xy * (s64)sum_xy;
				s64 disc = trace * trace - 4 * det;
				s32 sq;

				if (disc < 0)
					disc = 0;
				sq = (s32)int_sqrt((u64)disc);

				shid->eigmaj[0] = (s32)((trace + sq) / 2);
				shid->eigmin[0] = (s32)((trace - sq) / 2);

				{
					s32 num = 2 * sum_xy;
					s32 den = sum_yy - sum_xx;
					s32 deg;

					if (den != 0 || num != 0) {
						deg = atan2_approx(num, den) / 2;
						/* Signed range [-90.00°, +89.99°] */
						if (deg >= 18000)
							deg -= 18000;
						else if (deg <= -18000)
							deg += 18000;
						shid->eigori[0] = deg;
					} else {
						shid->eigori[0] = 0;
					}
				}
			}
		}
	}

	/* Step 5: emit multitouch events with EMA smoothing and slot tracking.
	 * Grid → screen mapping per GROUND_TRUTH.md §22.8.  Calibration
	 * parameters describe the final screen axes, so choose the source grid
	 * extent after swap_xy has selected which grid axis feeds each output. */
	{
		struct input_dev *input = shid->touch_input;
		bool any_touch = false;
		struct { u16 gx; u16 gy; u32 w; u8 idx; } sorted[HEATMAP_MAX_BLOBS];
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
					typeof(sorted[0]) t = sorted[i];
					sorted[i] = sorted[j]; sorted[j] = t;
				}
		if (sorted_count > HEATMAP_MAX_SLOTS)
			sorted_count = HEATMAP_MAX_SLOTS;

		for (i = 0; i < sorted_count; i++) {
			u16 screen_gx = swap_xy ? sorted[i].gy : sorted[i].gx;
			u16 screen_gy = swap_xy ? sorted[i].gx : sorted[i].gy;

			seq_dbg(shid, 2, "CALIB: blob[%u] grid=(%u,%u) screen=(%u,%u) weight=%u scale=(%ux%u)\n",
				 i, sorted[i].gx, sorted[i].gy,
				 (screen_gx * scale_x + 500) / 1000,
				 (screen_gy * scale_y + 500) / 1000,
				 sorted[i].w, scale_x, scale_y);
		}
		if (sorted_count || touched_count)
			seq_dbg(shid, 2, "CALIB: blobs=%u cells_touched=%d\n",
				 sorted_count, touched_count);

		/* Ghost rejection (GROUND_TRUTH.md §22.7 step 3):
		 * Merge blobs closer than ghost_dist threshold (grid cells).
		 * Windows uses Euclidean squared distance (dx²+dy² < ghost_dist²),
		 * not axis-aligned box. If two blobs are within radius, keep the
		 * heavier one, discard the lighter. */
		{
			u8 a, b, j;
			u32 gdsq = (u32)ghost_dist * (u32)ghost_dist;

			for (a = 0; a < sorted_count; a++) {
				if (sorted[a].w == 0)
					continue;
				for (b = a + 1; b < sorted_count; b++) {
					s16 dx, dy;

					if (sorted[b].w == 0)
						continue;
					dx = (s16)sorted[a].gx - (s16)sorted[b].gx;
					dy = (s16)sorted[a].gy - (s16)sorted[b].gy;
					if ((u32)(dx * dx) + (u32)(dy * dy) <= gdsq) {
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

		/* Blob splitting disabled — the eigenvalue heuristic split
		 * single-finger blobs (centroid jitter → temporarily elongated
		 * shape). The real Windows pipeline uses per-pixel peak detection
		 * (FUN_180602e60); re-enable after that is implemented. */
#if 0
		/* Blob splitting (GROUND_TRUTH.md §22.7 step 4):
		 * If the heaviest blob is elongated (ratio > 2:1) and heavy enough,
		 * it likely contains two touching fingers. Split along the major
		 * eigenvalue axis into two sub-blobs at 30% of the semi-axis.
		 * Each sub-blob gets half the weight and centroid shifted by
		 * the split offset. This handles the most common merged-finger
		 * case without per-pixel peak detection. */
		{
			u8 split_idx = 0;
			u32 best_ws = 0;

			for (i = 0; i < sorted_count; i++) {
				if (sorted[i].w > best_ws) {
					best_ws = sorted[i].w;
					split_idx = i;
				}
			}
			if (best_ws >= (u32)blob_min_weight && sorted_count < HEATMAP_MAX_SLOTS - 1) {
				s32 maj = shid->eigmaj[0];
				s32 min = shid->eigmin[0];
				s32 ori = shid->eigori[0];

				if (maj > min * 2 + 10 && maj >= 50 && best_ws > (u32)blob_min_weight * 3) {
					s32 split_dist = (s32)int_sqrt((u64)maj) * 3 / 10;
					s32 dx = 0, dy = 0;
					u8 idx = sorted[split_idx].idx;

					if (split_dist < 1)
						split_dist = 1;
					if (split_dist > 5)
						split_dist = 5;

					/* Approximate direction from orientation (degrees*100):
					 * - near 0° or ±180° → split horizontally
					 * - near ±90° → split vertically
					 * - other → split diagonally */
					if (ori > 18000) ori -= 18000;
					if (ori < -18000) ori += 18000;

					if (ori >= -6000 && ori <= 6000) {
						dx = split_dist;
					} else if ((ori >= 8000 && ori <= 10000) ||
						   (ori >= -10000 && ori <= -8000)) {
						dy = split_dist;
					} else {
						dx = split_dist * 7 / 10;
						dy = split_dist * 7 / 10;
					}

					sorted[split_idx].gx = (s16)shid->blob_x[idx] - dx;
					sorted[split_idx].gy = (s16)shid->blob_y[idx] - dy;
					sorted[split_idx].w = best_ws / 2;

					if (sorted_count < HEATMAP_MAX_SLOTS) {
						sorted[sorted_count].gx = (s16)shid->blob_x[idx] + dx;
						sorted[sorted_count].gy = (s16)shid->blob_y[idx] + dy;
						sorted[sorted_count].w = best_ws / 2;
						sorted[sorted_count].idx = HEATMAP_MAX_BLOBS;
						sorted_count++;
					}
				}
			}
		}
#endif
		/* Hungarian global assignment (matching Windows TouchPenProcessor0C19).
		 * Replaces the old greedy nearest-neighbor with minimum-cost bipartite
		 * matching.  Each blob is assigned to exactly one slot, minimizing
		 * Euclidean squared distance. Empty slots receive a uniform penalty
		 * so that new slots are only created when no claimed slot is nearby. */
		{
			u8 assigned_slot[HEATMAP_MAX_BLOBS];
			u16 new_gx[HEATMAP_MAX_SLOTS], new_gy[HEATMAP_MAX_SLOTS];
			bool new_active[HEATMAP_MAX_SLOTS];
			u16 cost[HEATMAP_MAX_BLOBS][HEATMAP_MAX_SLOTS];
			u8 row_match[HEATMAP_MAX_BLOBS];
			u8 col_match[HEATMAP_MAX_SLOTS];
			u16 row_val[HEATMAP_MAX_BLOBS], col_val[HEATMAP_MAX_SLOTS];
			u8 row_cover[HEATMAP_MAX_BLOBS], col_cover[HEATMAP_MAX_SLOTS];
			u16 max_dist_sq;
			s16 dx, dy;
			u8 n_blobs = sorted_count;
			u8 round, row, col;
			u16 min_val;

			memset(new_active, 0, sizeof(new_active));
			memset(new_gx, 0, sizeof(new_gx));
			memset(new_gy, 0, sizeof(new_gy));
			memset(row_match, 0xFF, sizeof(row_match));
			memset(col_match, 0xFF, sizeof(col_match));
			memset(row_val, 0, sizeof(row_val));
			memset(col_val, 0, sizeof(col_val));
			max_dist_sq = (u16)blob_max_distance * (u16)blob_max_distance;

			/* Build cost matrix and initial row minima. */
			for (row = 0; row < n_blobs; row++) {
				row_val[row] = U16_MAX;
				for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
					if (shid->blob_slot_state[col] >= 1) {
						dx = (s16)sorted[row].gx - (s16)shid->blob_slot_gx[col];
						dy = (s16)sorted[row].gy - (s16)shid->blob_slot_gy[col];
						if (dx < 0) dx = -dx;
						if (dy < 0) dy = -dy;
						if ((u16)dx <= (u16)blob_max_distance &&
						    (u16)dy <= (u16)blob_max_distance) {
							cost[row][col] = (u16)dx * (u16)dx + (u16)dy * (u16)dy;
						} else {
							cost[row][col] = max_dist_sq * 8 + 1;
						}
					} else {
						cost[row][col] = max_dist_sq * 4;
					}
					if (cost[row][col] < row_val[row])
						row_val[row] = cost[row][col];
				}
				/* Subtract row minimum. */
				for (col = 0; col < HEATMAP_MAX_SLOTS; col++)
					cost[row][col] -= row_val[row];
			}

			/* Hungarian algorithm iterations. */
			memset(row_cover, 0, sizeof(row_cover));
			memset(col_cover, 0, sizeof(col_cover));
			for (round = 0; round < n_blobs; round++) {
				memset(row_cover, 0, sizeof(row_cover));
				memset(col_cover, 0, sizeof(col_cover));

				for (;;) {
					u8 changed = 0;

					/* Select uncovered row with zero in an uncovered column. */
					for (row = 0; row < n_blobs; row++) {
						if (row_cover[row]) continue;
						for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
							if (col_cover[col]) continue;
							if (cost[row][col] == 0) {
								row_match[row] = col;
								col_match[col] = row;
								row_cover[row] = 1;
								col_cover[col] = 1;
								changed = 1;
								break;
							}
						}
					}
					if (changed) continue;

					/* All uncovered costs > 0: find min and update. */
					min_val = U16_MAX;
					for (row = 0; row < n_blobs; row++) {
						if (row_cover[row]) continue;
						for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
							if (col_cover[col]) continue;
							if (cost[row][col] < min_val)
								min_val = cost[row][col];
						}
					}
					if (min_val == U16_MAX)
						break;

					for (row = 0; row < n_blobs; row++) {
						for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
							if (row_cover[row] && col_cover[col])
								cost[row][col] += min_val;
							else if (!row_cover[row] && !col_cover[col])
								cost[row][col] -= min_val;
						}
					}
				}
			}

			/* Build final assignments from the matching.
			 * Only keep matches with cost below the empty-slot penalty. */
			for (row = 0; row < n_blobs; row++)
				assigned_slot[row] = 0xFF;
			for (col = 0; col < HEATMAP_MAX_SLOTS; col++) {
				row = col_match[col];
				if (row >= n_blobs) continue;
				if (shid->blob_slot_state[col] >= 1) {
					dx = (s16)sorted[row].gx - (s16)shid->blob_slot_gx[col];
					dy = (s16)sorted[row].gy - (s16)shid->blob_slot_gy[col];
					if (dx < 0) dx = -dx;
					if (dy < 0) dy = -dy;
					if ((u16)dx <= (u16)blob_max_distance &&
					    (u16)dy <= (u16)blob_max_distance)
						assigned_slot[row] = col;
				} else {
					assigned_slot[row] = col;
				}
			}

			/* Process slot state transitions (GROUND_TRUTH §22.4):
			 * 0=empty, 1=new, 2=claimed, 3=lift */
			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				u8 bi = 0xFF;

				for (i = 0; i < sorted_count; i++) {
					if (assigned_slot[i] == s) { bi = i; break; }
				}

				if (bi != 0xFF) {
					u16 gx = sorted[bi].gx;
					u16 gy = sorted[bi].gy;
					u32 w  = sorted[bi].w;
					bool was_claimed = (shid->blob_slot_state[s] >= 2);

					switch (shid->blob_slot_state[s]) {
					case 0:
						shid->blob_slot_state[s] = 1;
						shid->blob_slot_duration[s] = 1;
						break;
					case 1:
						shid->blob_slot_duration[s]++;
						if (shid->blob_slot_duration[s] >= (u32)blob_debounce)
							shid->blob_slot_state[s] = 2;
						break;
					case 2:
						shid->blob_slot_duration[s]++;
						break;
					case 3:
						shid->blob_slot_state[s] = 1;
						shid->blob_slot_duration[s] = 1;
						break;
					}
					u16 old_gx = shid->blob_slot_gx[s];
					u16 old_gy = shid->blob_slot_gy[s];

					shid->blob_slot_missed[s] = 0;
					shid->blob_slot_gx[s] = gx;
					shid->blob_slot_gy[s] = gy;
					/* EMA on blob weight (matching Windows: weight_smoothed = (old*7+new)/8) */
					if (was_claimed && shid->blob_slot_state[s] == 2)
						shid->blob_slot_weight[s] = (shid->blob_slot_weight[s] * 7 + w) / 8;
					else
						shid->blob_slot_weight[s] = w;

					new_active[s] = (shid->blob_slot_state[s] >= 2);

					if (new_active[s]) {
						if (was_claimed && shid->blob_slot_state[s] == 2) {
							/* Track stationary counter for diagnostics only.
							 * We do NOT freeze the position (that causes
							 * stick-slip jumping during slow drags). The
							 * 3-sample MA + EMA are enough for smoothness. */
							s16 raw_dx = (s16)gx - (s16)shid->blob_slot_gx[s];
							s16 raw_dy = (s16)gy - (s16)shid->blob_slot_gy[s];

							if (raw_dx >= -1 && raw_dx <= 1 &&
							    raw_dy >= -1 && raw_dy <= 1) {
								shid->blob_slot_stationary[s]++;
							} else {
								shid->blob_slot_stationary[s] = 0;
							}

							new_gx[s] = (old_gx * ema_alpha + gx) / (ema_alpha + 1);
							new_gy[s] = (old_gy * ema_alpha + gy) / (ema_alpha + 1);
						} else {
							new_gx[s] = gx;
							new_gy[s] = gy;
							shid->blob_slot_stationary[s] = 0;
						}
						shid->blob_slot_gx[s] = new_gx[s];
						shid->blob_slot_gy[s] = new_gy[s];

						/* History ring: push current position for 5-sample MA. */
						{
							u8 hp = shid->blob_slot_hpos[s];
							shid->blob_slot_hx[s][hp] = new_gx[s];
							shid->blob_slot_hy[s][hp] = new_gy[s];
							shid->blob_slot_hpos[s] = (hp + 1) % SLOT_HISTORY_DEPTH;
							if (shid->blob_slot_hcount[s] < SLOT_HISTORY_DEPTH)
								shid->blob_slot_hcount[s]++;
						}
					}
				} else {
					switch (shid->blob_slot_state[s]) {
					case 1:
						shid->blob_slot_state[s] = 0;
						shid->blob_slot_duration[s] = 0;
						break;
					case 2:
						shid->blob_slot_missed[s]++;
						if (shid->blob_slot_missed[s] >= (u32)blob_lift_frames) {
							shid->blob_slot_state[s] = 3;
							shid->blob_slot_missed[s] = 0;
						}
						break;
					case 3:
						shid->blob_slot_missed[s]++;
						if (shid->blob_slot_missed[s] >= 2) {
							shid->blob_slot_state[s] = 0;
							shid->blob_slot_missed[s] = 0;
						}
						break;
					case 0:
						break;
					}

				new_active[s] = false;
		}
	}

			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				input_mt_slot(input, s);
				input_mt_report_slot_state(input, MT_TOOL_FINGER, new_active[s]);
				if (new_active[s]) {
					s64 fx, fy, tmp;
					u16 screen_gx, screen_gy;

					/* 3-sample moving average from history ring
					 * for smooth cursor movement. */
					{
						u8 hc = shid->blob_slot_hcount[s];
						u8 hp = shid->blob_slot_hpos[s];
						u8 n = (hc < 3) ? hc : 3;
						u32 sumx = 0, sumy = 0;

						for (u8 k = 0; k < n; k++) {
							u8 idx = (hp + SLOT_HISTORY_DEPTH - n + k) % SLOT_HISTORY_DEPTH;
							sumx += shid->blob_slot_hx[s][idx];
							sumy += shid->blob_slot_hy[s][idx];
						}
						screen_gx = (u16)(sumx / n);
						screen_gy = (u16)(sumy / n);
					}
					/* Orient first: scales and offsets always address final X/Y. */
					if (swap_xy) {
						tmp = screen_gx;
						screen_gx = screen_gy;
						screen_gy = tmp;
					}
					fx = ((s64)screen_gx * scale_x + 500) / 1000;
					fy = ((s64)screen_gy * scale_y + 500) / 1000;
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

					/* Emit orientation and touch ellipse for slot 0 (heaviest blob) */
					if (s == 0 && shid->eigmaj[0] > 0) {
						u32 major, minor;
						s32 ori;

						/* Anisotropic ellipse: eigenvalues are in
						 * grid-cell² units; project the std-dev onto
						 * screen units using the per-axis scale so the
						 * major/minor reflect the real X/Y pitch. */
						major = ((u32)int_sqrt(shid->eigmaj[0]) * scale_x + 500) / 1000;
						minor = ((u32)int_sqrt(shid->eigmin[0]) * scale_y + 500) / 1000;
						ori = shid->eigori[0] / 100;
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

		/* Bug fix (2026-07-08 review): only HEATMAP_MAX_SLOTS slots were
		 * ever allocated via input_mt_init_slots() — the loop above
		 * already reports the correct active/inactive state for all of
		 * them. Calling input_mt_slot() with an out-of-range index here
		 * was a no-op in the input core (slot left unchanged at the last
		 * valid one, HEATMAP_MAX_SLOTS-1), so this loop was re-clearing
		 * that last real slot immediately after it was just reported
		 * active, silently deactivating the second finger every frame. */
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
		snapshot_len - 2,
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

	if (shid->seq_storm_count > 100) {
		unsigned long delta = jiffies - shid->seq_last_valid_jiffies;
		if (delta < HZ) {
			WRITE_ONCE(shid->seq_enabled, false);
			shid->seq_storm_count = 0;
			return IRQ_HANDLED;
		}
		shid->seq_storm_count = 0;
	}

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || !READ_ONCE(shid->seq_enabled)) {
		result = IRQ_NONE;
		goto out;
	}

	shid->stat_irq_count++;

	/* Bug fix (2026-07-08 review): this used to compare seq_state (int,
	 * values 0-5) against seq_enabled (bool), which collapses to
	 * "seq_state != 1" and fired on every IRQ while parked in any other
	 * state — including state 4, the steady-state loop that handles every
	 * touch report — defeating the point of only logging on transitions. */
	if (shid->seq_state != shid->seq_dbg_last_state) {
		seq_dbg(shid, 1, "SEQ: thread seq_state=%d\n", shid->seq_state);
		shid->seq_dbg_last_state = shid->seq_state;
	}

	dbg_dt_us = shid->seq_dbg_last_irq ?
		ktime_us_delta(ktime_get(), shid->seq_dbg_last_irq) : -1;
	shid->seq_dbg_last_irq = ktime_get();

	/* Windows reads response header first: 5 sync bytes + 4-byte header = 9 bytes.
	 * AMD controller with TX_COUNT=3 sees 6 sync bytes; try 10 to capture full header. */
	if (spi_hid_seq_read(shid, hdr, sizeof(hdr))) {
		dev_dbg(dev, "sequencer header read failed\n");
		shid->seq_storm_count++;
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
			"SEQ: malformed input header at offset %d, dropping frame\n", hdr_off);
		shid->stat_frames_dropped++;
		shid->seq_storm_count++;
		goto out;
	}

	shid->seq_last_valid_jiffies = jiffies;
	shid->seq_storm_count = 0;

	blen = (((hdr[6] >> 4) & 0xF) << 0) | (hdr[7] << 4);
	blen *= 4;
	if (blen > sizeof(shid->input.content))
		blen = sizeof(shid->input.content);

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
			u32 required = sizeof(raw);

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
				u32 k;

				memcpy(shid->wire_report_descriptor, body + off, len);
				for (k = 55; k < len; k += 64) {
					if (shid->wire_report_descriptor[k] == 0xFF &&
					    k < HARDCODED_RD_SIZE &&
					    hardcoded_report_descriptor[k] != 0xFF) {
						seq_dbg(shid, 1, "SEQ: patching known-corrupt wire descriptor byte at offset %u (0xff -> 0x%02x)\n",
							 k, hardcoded_report_descriptor[k]);
						shid->wire_report_descriptor[k] =
							hardcoded_report_descriptor[k];
					}
				}
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
		if (shid->raw_mode_active) {
			if (skip_getfeat) {
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
					if (spi_hid_seq_write(shid, vendor_init, sizeof(vendor_init), NULL, 0)) {
						dev_warn(&shid->spi->dev, "SEQ: vendor init write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					usleep_range(36000, 39000);
					{
						u8 sf[15] = {
							0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
							0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
						};
						seq_dbg(shid, 1, "SEQ: SET_FEATURE -> state 4\n");
						if (spi_hid_seq_write(shid, sf, sizeof(sf), NULL, 0)) {
							dev_warn(&shid->spi->dev, "SEQ: SET_FEATURE write failed\n");
							schedule_delayed_work(&shid->raw_handshake_watchdog,
								msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
							return;
						}
					}
					spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_REPORT_DESCRIPTOR);
				}
			} else {
				static const u8 vendor_init[] = {
					0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00,
					0x03, 0x0A, 0x00,
					0x56, 0xBD, 0x0C, 0xEE, 0x5B, 0x44, 0x4C, 0x00, 0x00
				};
				u8 gf[11] = {
					0x02, 0x02, 0x00, 0x00, 0x03, 0x42,
					0x00, 0x04, 0x03, 0x00, 0x06
				};
				usleep_range(1400, 1800);
				if (getfeat_delay_ms > 0) {
					seq_dbg(shid, 1, "SEQ: scheduling vendor init + GET_FEATURE after %dms...\n",
						getfeat_delay_ms);
					shid->feat_delay_pending = true;
					schedule_delayed_work(&shid->feat_delay_work,
							      msecs_to_jiffies(getfeat_delay_ms));
				} else {
					seq_dbg(shid, 1, "SEQ: vendor init + GET_FEATURE...\n");
					usleep_range(68000, 72000);
					if (spi_hid_seq_write(shid, vendor_init, sizeof(vendor_init), NULL, 0)) {
						dev_warn(&shid->spi->dev, "SEQ: vendor init write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					usleep_range(36000, 39000);
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
		{
			u8 sf[15] = {
				0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
				0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
			};
			u8 sf_nd[14] = {
				0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
				0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
			};
			usleep_range(4500, 5500);
			seq_dbg(shid, 1, "SEQ: sending SET_FEATURE speed=%u no_double=%d\n",
				 setfeat_speed_hz, setfeat_no_double);
			int ret;

			if (setfeat_no_double)
				ret = spi_hid_seq_write_speed(shid, sf_nd, sizeof(sf_nd),
								  NULL, 0, setfeat_speed_hz);
			else
				ret = spi_hid_seq_write_speed(shid, sf, sizeof(sf),
								  NULL, 0, setfeat_speed_hz);
			if (ret) {
				dev_warn(&shid->spi->dev, "SEQ: SET_FEATURE write failed: %d\n", ret);
				return;
			}
		}
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_RESPONSE);
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in state 5, sending DESCREQ directly\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	}
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
			schedule_work(&shid->create_device_work);
		seq_handle_data(shid, type, blen);
	} else if (type == 3) {
		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: VENDOR_INIT: got RESET_RSP, vendor init ignored. Hardcoding descriptors...\n");
		shid->desc.hid_version = 0x0100;
		shid->desc.report_descriptor_length = 936;
		shid->desc.report_descriptor_register = 0x0002;
		shid->desc.input_register = 0x0000;
		shid->desc.max_input_length = 0x1000;
		shid->desc.output_register = 0x0003;
		shid->desc.max_output_length = 0x0100;
		shid->desc.command_register = 0x0004;
		shid->desc.vendor_id = 0x045E;
		shid->desc.product_id = 0x0C19;
		shid->desc.version_id = 0x0100;
		spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FALLBACK);
		shid->ready = true;
		shid->keep_powered = true;
		if (!shid->hid) {
			seq_dbg(shid, 1, "SEQ: creating HID device with hardcoded descriptors...\n");
			schedule_work(&shid->create_device_work);
		}
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
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_DEVICE_RESET);
		return;
	}
	if (type != 1)
		return;
	if (!shid->hid && !(shid->raw_mode_active && shid->touch_input))
		return;

	shid->stat_data++;
	{
		u32 cap = shid->raw_mode_active ? shid->data_buf_len :
			  (shid->desc.max_input_length ? shid->desc.max_input_length : 0x1000);
		u32 rblen = min_t(u32, blen + 5, cap);
		u32 avail;
		u16 rl;
		u8 *body;

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
		rl = body[5] | (body[6] << 8);
		seq_dbg(shid, 2, "SEQ: state4 cid=0x%02x len=%u\n", body[7], rl);
		if (rl >= 3 && rl - 3 > avail) {
			dev_warn_ratelimited(dev,
				"SEQ: DATA report len=%u exceeds buffer (avail=%u), dropped\n",
				rl, avail);
			shid->stat_frames_dropped++;
			return;
		}

		if (shid->raw_mode_active && raw_input_beta && body[7] == 0x0C && shid->touch_input) {
			struct spi_hid_capimg_raster raster;
			int cret;

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
			}
			cret = spi_hid_capimg_decode_v0(&body[5], rblen - 5, &raster);
			if (cret) {
				dev_warn(dev, "SEQ: CapImg decode failed: %d (rblen=%u)\n", cret, rblen);
				shid->stat_frames_dropped++;
				return;
			}
			heatmap_process_frame(shid, raster.samples, SPI_HID_CAPIMG_RASTER_SAMPLES, 0x0C);
		} else if (rl >= 3 && rl - 3 <= avail) {
			if (shid->raw_mode_active && body[7] == 0x40 && rl - 2 >= 6) {
				u16 hx = body[8] | (body[10] << 8);
				u16 hy = body[10] | (body[12] << 8);
				seq_dbg(shid, 2, "CALIB_REF: hid=(%u,%u)\n", hx, hy);
			}
			if (shid->hid) {
				int hret = hid_input_report(shid->hid, HID_INPUT_REPORT,
							    &body[7], rl - 2, 1);
				if (hret)
					dev_warn(dev, "SEQ: hid_input_report failed: %d (content_id=0x%02x)\n",
						 hret, body[7]);
			}
		} else if (rl < 3) {
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

	if (!shid->desc.max_input_length) {
		dev_err(&shid->spi->dev, "device descriptor has no input length\n");
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

	/* 2026-07-08: prefer the descriptor actually read off the wire
	 * (docs/NEXT_STEPS.md §C) — a real driver shouldn't permanently rely on
	 * a descriptor hardcoded from one past capture, which can't track
	 * firmware revisions or other SKUs of the same VID/PID. Trust
	 * hid_parse_report()'s own structural validation as the sanity check:
	 * if the wire-read bytes don't parse, fall back to the hardcoded copy
	 * exactly as before. */
	if (shid->wire_report_descriptor_len > 0 && !shid->wire_report_descriptor_rejected) {
		seq_dbg(shid, 1, "SEQ: ll_parse — trying device-read report descriptor (%u bytes)\n",
			 shid->wire_report_descriptor_len);
		ret = hid_parse_report(hid, shid->wire_report_descriptor,
					shid->wire_report_descriptor_len);
		if (!ret) {
			shid->report_descriptor_crc32 = crc32_le(0,
				shid->wire_report_descriptor,
				shid->wire_report_descriptor_len);
			mutex_unlock(&shid->lock);
			return 0;
		}
		dev_warn(dev, "SEQ: device-read report descriptor failed to parse (%d), falling back to hardcoded\n",
			 ret);
	}

	seq_dbg(shid, 1, "SEQ: ll_parse — using HARDCODED report descriptor (%d bytes)\n",
		 HARDCODED_RD_SIZE);

	/* Copy hardcoded descriptor into response buffer */
	memcpy(shid->response.content, hardcoded_report_descriptor, HARDCODED_RD_SIZE);

	ret = hid_parse_report(hid, (__u8 *) shid->response.content, HARDCODED_RD_SIZE);
	if (ret)
		dev_err(dev, "failed parsing report: %d\n", ret);
	else
		shid->report_descriptor_crc32 = crc32_le(0,
			(unsigned char const *) shid->response.content,
			HARDCODED_RD_SIZE);

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
			/*
			 * NOTE: Assumes the response was populated by the
			 * IRQ thread before ll_raw_request reads it. The
			 * spi_hid_get_request path ensures completion via
			 * wait_for_completion, but no explicit response_valid
			 * flag is checked here.
			 */
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

	report.content_type = SPI_HID_CONTENT_TYPE_OUTPUT_REPORT;
	report.content_length = len - 1;
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

	u8 perf_mode;

	if (kstrtou8(buf, 10, &perf_mode))
		return -EINVAL;

	spin_lock_irqsave(&shid->input_lock, flags);
	shid->perf_mode = perf_mode;

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

	dev_info(dev, "TRACE[hid] probe begin irq=%d raw_mode=%u acpi_power_cycle=%u\n",
		 spi->irq, raw_mode, acpi_probe_power_cycle);

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
	spin_lock_init(&shid->input_lock);
	spi_set_drvdata(spi, shid);

	ret = sysfs_create_files(&dev->kobj, spi_hid_attributes);
	if (ret) {
		dev_err(dev, "Unable to create sysfs attributes\n");
		goto err0;
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

	INIT_WORK(&shid->reset_work, spi_hid_reset_work);
	INIT_WORK(&shid->create_device_work, spi_hid_create_device_work);
	INIT_WORK(&shid->refresh_device_work, spi_hid_refresh_device_work);
	INIT_WORK(&shid->error_work, spi_hid_error_work);
	INIT_DELAYED_WORK(&shid->descreq_work, spi_hid_seq_descreq_work);
	INIT_DELAYED_WORK(&shid->raw_handshake_watchdog, spi_hid_raw_handshake_watchdog);
	INIT_DELAYED_WORK(&shid->feat_delay_work, spi_hid_feat_delay_work);
	shid->raw_handshake_confirmed = false;
	shid->raw_handshake_retries_left = RAW_HANDSHAKE_MAX_RETRIES;
	shid->raw_mode_active = raw_mode; /* false → heatmap/raw code dead; infrastructure stays zeroed for mode switch */
	shid->seq_dbg_last_state = -1;

	INIT_DELAYED_WORK(&shid->stream_watchdog, spi_hid_stream_watchdog_work);
	shid->stream_watchdog_active = false;

	INIT_DELAYED_WORK(&shid->poll_work, spi_hid_poll_work);
	shid->poll_active = false;
	shid->poll_interval_ms = 0;
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

	/* This is a device-specific legacy experiment, not generic SPI-HID power
	 * management. Keep it controllable until cold-boot A/B traces prove it is
	 * required. Never wait as if a failed AML transition had succeeded. */
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

	/* TEST: wait for device to stabilize after ACPI _INI power-on.
	 * _INI is called by the ACPI subsystem before probe() and handles
	 * GPIO power sequencing. The device is already powered and sending
	 * RESET_RSP. Do NOT call _RST/M009/M010 — power cycle kills the
	 * device until reboot (verified 2026-07-07, see GROUND_TRUTH §10.7). */
	seq_dbg(shid, 1, "probe settling delay begin\n");
	msleep(300);
	seq_dbg(shid, 1, "probe settling delay complete\n");
	shid->desc.input_register = 0x000000;

	/* 2026-07-12: vendor init disabled — the working cold boot did not
	 * have it. D2->D0 may confuse the controller after _PS3->_PS0. */
	if (0 && raw_mode) {
		dev_info(dev, "SEQ: sending vendor init (SET_POWER D2->D0)...\n");
		spi_hid_vendor_init(shid);
	}

	dev_info(dev, "SEQ: device powered by ACPI _INI, arming IRQ\n");

	/* Create multitouch input device for heatmap-to-touch pipeline.
	 * Bug fix (2026-07-08 review): this used to be created unconditionally,
	 * even in the default raw_mode=0 configuration where it never receives
	 * a single event (only heatmap_process_frame() feeds it, and that's
	 * only called when raw_mode is set) — exposing a second, permanently
	 * dead "MSHW0231 Touchscreen" input device alongside the real
	 * hid-core one and risking userspace picking the wrong one. Switching
	 * raw_mode 0->1 always goes through a fresh probe (module
	 * reload/rebind), so gating creation on raw_mode here loses no
	 * capability. */
	if (shid->raw_mode_active) {
		shid->touch_input = input_allocate_device();
		if (shid->touch_input) {
			shid->touch_input->name = "MSHW0231 Touchscreen";
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
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);
	dev_info(dev, "TRACE[hid] remove complete\n");
}

static const struct spi_device_id spi_hid_id_table[] = {
	{ "hid", 0 },
	{ "hid-over-spi", 0 },
	{ },
};
MODULE_DEVICE_TABLE(spi, spi_hid_id_table);

static int spi_hid_suspend(struct device *dev)
{
	struct spi_device *spi = to_spi_device(dev);
	struct spi_hid *shid = spi_get_drvdata(spi);

	seq_dbg(shid, 1, "PM: suspend\n");
	mutex_lock(&shid->seq_lock);
	WRITE_ONCE(shid->seq_enabled, false);
	shid->poll_active = false;
	shid->raw_handshake_confirmed = false;
	shid->feat_delay_pending = false;
	mutex_unlock(&shid->seq_lock);

	cancel_delayed_work_sync(&shid->feat_delay_work);
	cancel_delayed_work_sync(&shid->raw_handshake_watchdog);

	if (shid->irq_enabled) {
		disable_irq(shid->irq);
		shid->irq_enabled = false;
	}
	return 0;
}

static int spi_hid_resume(struct device *dev)
{
	struct spi_device *spi = to_spi_device(dev);
	struct spi_hid *shid = spi_get_drvdata(spi);

	seq_dbg(shid, 1, "PM: resume\n");
	mutex_lock(&shid->seq_lock);
	shid->ready = false;
	shid->keep_powered = false;
	shid->raw_handshake_confirmed = false;
	shid->raw_handshake_retries_left = RAW_HANDSHAKE_MAX_RETRIES;
	shid->feat_delay_pending = false;
	heatmap_reset_baseline(shid);
	WRITE_ONCE(shid->seq_enabled, true);
	shid->seq_state = 0;
	mutex_unlock(&shid->seq_lock);

	if (!shid->irq_enabled) {
		enable_irq(shid->irq);
		shid->irq_enabled = true;
	}
	return 0;
}

static const struct dev_pm_ops spi_hid_pm_ops = {
	.suspend = spi_hid_suspend,
	.resume = spi_hid_resume,
};

static struct spi_driver spi_hid_driver = {
	.driver = {
		.name	= "spi_hid",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(spi_hid_of_match),
		.acpi_match_table = ACPI_PTR(spi_hid_acpi_match),
		.pm	= &spi_hid_pm_ops,
	},
	.probe		= spi_hid_probe,
	.remove		= spi_hid_remove,
	.id_table	= spi_hid_id_table,
};

module_spi_driver(spi_hid_driver);

MODULE_DESCRIPTION("HID over SPI transport driver");
MODULE_LICENSE("GPL");