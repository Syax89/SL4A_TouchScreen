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

#include "spi-hid-core.h"
#include "spi-hid_trace.h"
/* Hardcoded HID Report Descriptor from Windows dump (936 bytes) */
#include "hardcoded_rd.h"

#define SPI_HID_MAX_RESET_ATTEMPTS 3

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
	header->version       = (buf[0] >> 0) & 0xf;
	header->report_type   = (buf[0] >> 4) & 0xf;
	header->fragment_id   = (buf[1] >> 0) & 0xf;
	header->report_length = ((((buf[1] >> 4) & 0xf) << 0) |
			(buf[2] << 4)) * 4;
	header->sync_const    = buf[3];
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
	buf[0] = SPI_HID_OUTPUT_HEADER_OPCODE_WRITE;
	buf[1] = (output_register >> 16) & 0xff;
	buf[2] = (output_register >> 8) & 0xff;
	buf[3] = (output_register >> 0) & 0xff;
	buf[4] = (SPI_HID_OUTPUT_HEADER_VERSION << 0) |
			(((output_report_length >> 0) & 0xf) << 4);
	buf[5] = (output_report_length >> 4) & 0xff;
}

static void spi_hid_output_body(__u8 *buf,
		struct spi_hid_output_report *report)
{
	u16 content_length = report->content_length;

	buf[0] = report->content_type;
	buf[1] = (content_length >> 0) & 0xff;
	buf[2] = (content_length >> 8) & 0xff;
	buf[3] = report->content_id;
}

static void spi_hid_read_approval(u32 input_register, u8 *buf)
{
	buf[0] = SPI_HID_READ_APPROVAL_OPCODE_READ;
	buf[1] = (input_register >> 16) & 0xff;
	buf[2] = (input_register >> 8) & 0xff;
	buf[3] = (input_register >> 0) & 0xff;
	buf[4] = SPI_HID_READ_APPROVAL_CONSTANT;
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

	if (shid->desc.command_register == 0)
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

	mutex_lock(&shid->power_lock);
	if (shid->power_state == SPI_HID_POWER_MODE_OFF)
		goto out;

	dev_err(dev, "Error Handler\n");

	if (shid->attempts++ >= SPI_HID_MAX_RESET_ATTEMPTS) {
		dev_err(dev, "unresponsive device, aborting.\n");
		spi_hid_stop_hid(shid);
		spi_hid_power_down(shid);

		ret = -ESHUTDOWN;
		goto out;
	}

	shid->ready = shid->seq_state >= 4 ? true : false;
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
	return ret;
}

/* Forward declarations */
static int spi_hid_seq_write(struct spi_hid *shid, const u8 *buf, int len, u8 *rx, int rx_len);

static void spi_hid_fw_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, fw_work);
	struct device *dev = &shid->spi->dev;

	dev_info(dev, "SEQ: fw_work: firmware disabled, skipping\n");
	shid->ready = true;
	shid->keep_powered = true;
	dev_info(dev, "SEQ: fw_work complete\n");
}

static void spi_hid_error_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, error_work);
	struct device *dev = &shid->spi->dev;
	int ret;

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

	dev_err(dev, "Reset Handler\n");
	if (shid->ready) {
		dev_err(dev, "Spontaneous FW reset!");
		shid->ready = shid->seq_state >= 4 ? true : false;
		shid->dir_count++;
		sysfs_notify(&dev->kobj, NULL, "ready");
	}

	if (flush_work(&shid->create_device_work))
		dev_err(dev, "Reset handler waited for create_device_work");

	if (shid->power_state == SPI_HID_POWER_MODE_OFF) {
		return;
	}

	if (flush_work(&shid->refresh_device_work))
		dev_err(dev, "Reset handler waited for refresh_device_work");

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

	dev_err(dev, "Input Report Handler\n");

	trace_spi_hid_input_report_handler(shid);

	if (!shid->ready) {
		dev_err(dev, "discarding input report, not ready!\n");
		return 0;
	}

	if (shid->refresh_in_progress) {
		dev_err(dev, "discarding input report, refresh in progress!\n");
		return 0;
	}

	if (!shid->hid) {
		dev_err(dev, "discarding input report, no HID device!\n");
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
	dev_err(&shid->spi->dev, "Response Handler\n");

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
	u16 total_len;
	int ret;

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
		dev_err(dev, "Received report descriptor length doesn't match device descriptor field, using min of the two\n");
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
		dev_err(dev, "Received device descriptor\n");
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

	if (shid->desc.max_input_length != 0 && header->report_length > shid->desc.max_input_length) {
		dev_err(dev, "Report body of size %u larger than max expected of %u\n",
				header->report_length, shid->desc.max_input_length);
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
	dev_err(dev, "Create device work\n");

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
	dev_err(dev, "Refresh device work\n");

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

	shid->refresh_in_progress = false;
	shid->ready = true;
	sysfs_notify(&dev->kobj, NULL, "ready");

out:
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

	dev_err(dev, "read header: version=0x%02x, report_type=0x%02x, report_length=%u, fragment_id=0x%02x, sync_const=0x%02x\n",
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

static int spi_hid_bus_input_report(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	int ret;

	trace_spi_hid_bus_input_report(shid);
	if (shid->input_transfer_pending++)
		return 0;

	ret = spi_hid_input_async(shid, shid->input.header,
			sizeof(shid->input.header),
			spi_hid_input_header_complete);
	if (ret) {
		dev_err(dev, "Failed to receive header: %d\n", ret);
		return ret;
	}

	return 0;
}

static int spi_hid_assert_reset(struct spi_hid *shid)
{
	int ret;

	if (!shid->spi->dev.of_node)
		return 0;

	ret = pinctrl_select_state(shid->pinctrl, shid->pinctrl_reset);
	if (ret)
		return ret;

	/* Let VREG_TS_5V0 stabilize */
	usleep_range(10000, 11000);

	return 0;
}

static int spi_hid_deassert_reset(struct spi_hid *shid)
{
	int ret;

	if (!shid->spi->dev.of_node)
		return spi_hid_reset_via_acpi(shid);

	ret = pinctrl_select_state(shid->pinctrl, shid->pinctrl_active);
	if (ret)
		return ret;

	/* Let VREG_S10B_1P8V stabilize */
	usleep_range(5000, 6000);

	return 0;
}

static int spi_hid_power_up(struct spi_hid *shid)
{
	int ret;

	if (shid->powered)
		return 0;

	shid->input_transfer_pending = 0;
	shid->powered = true;

	if (shid->spi->dev.of_node) {
		ret = regulator_enable(shid->supply);
		if (ret) {
			shid->regulator_error_count++;
			shid->regulator_last_error = ret;
			goto err0;
		}

		/* Let VREG_S10B_1P8V stabilize */
		usleep_range(5000, 6000);
	}

	ret = spi_hid_set_power(shid, SPI_HID_POWER_MODE_ACTIVE);
	if (ret)
		dev_err(&shid->spi->dev, "failed to set power ACTIVE: %d\n", ret);

	return 0;

err0:
	shid->powered = false;

	return ret;
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
	tx[0]=0x0B; tx[1]=(reg>>16)&0xff; tx[2]=(reg>>8)&0xff; tx[3]=reg&0xff;
	tx[4]=0xFF;
	memset(rx,0,rx_len); memset(xf,0,sizeof(xf));
	xf[0].tx_buf=tx; xf[0].len=5; xf[1].rx_buf=rx; xf[1].len=rx_len;
	spi_message_init(&msg); spi_message_add_tail(&xf[0],&msg); spi_message_add_tail(&xf[1],&msg);
	ret = spi_sync(shid->spi, &msg);
	dev_info(&shid->spi->dev, "SEQ: read reg=0x%06x len=%d ret=%d raw=[%*ph]\n",
		 reg, rx_len, ret, min(rx_len, 16), rx);
	return ret;
}
static int spi_hid_seq_read(struct spi_hid *shid, u8 *rx, int rx_len)
{ return spi_hid_seq_read_reg(shid, shid->desc.input_register, rx, rx_len); }
static int spi_hid_seq_write(struct spi_hid *shid, const u8 *buf, int len, u8 *rx, int rx_len)
{
	struct spi_transfer xf[2];
	struct spi_message msg;

	dev_err(&shid->spi->dev, "SEQ: write op=0x%02x len=%d rx=%d raw=[%*ph]\n",
		buf[0], len, rx_len, min(len, 16), buf);

	memset(xf, 0, sizeof(xf));
	xf[0].tx_buf = (void *)buf;
	xf[0].len = len;
	spi_message_init(&msg);
	spi_message_add_tail(&xf[0], &msg);

	if (rx && rx_len > 0) {
		xf[1].rx_buf = rx;
		xf[1].len = rx_len;
		spi_message_add_tail(&xf[1], &msg);
	}

	return spi_sync(shid->spi, &msg);
}
static int spi_hid_seq_hdr_type(const u8 *rx, int len, int *hdr_off)
{
	int i;
	for (i = 3; i < len; i++) {
		if (rx[i] == 0x5A && (rx[i-3] & 0x0F) == 2) {
			if (hdr_off) *hdr_off = i - 3;
			return (rx[i-3] >> 4) & 0xF;
		}
	}
	return -1;
}

static void spi_hid_seq_descreq_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, descreq_work.work);
	struct device *dev = &shid->spi->dev;
	u8 hdr[10];
	int type;

	if (shid->seq_state != 1) return;

	dev_info(dev, "SEQ: poll-work: reading for DEVICE_DESC...\n");
	if (spi_hid_seq_read(shid, hdr, sizeof(hdr))) {
		dev_info(dev, "SEQ: poll-work: read failed, retrying...\n");
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
		return;
	}
	type = spi_hid_seq_hdr_type(&hdr[6], sizeof(hdr) - 6, NULL);
	dev_info(dev, "SEQ: poll-work: type=%d raw=[%*ph]\n", type, 10, hdr);
	if (type == 7) {
		dev_info(dev, "SEQ: poll-work: GOT DEVICE_DESC!\n");
		/* Re-trigger the IRQ thread to handle it */
	} else if (type == 3) {
		dev_info(dev, "SEQ: poll-work: still RESET_RSP, DESCREQ failed\n");
		shid->seq_state = 4;
		shid->ready = true;
		shid->keep_powered = true;
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
		dev_info(dev, "SEQ: poll-work: unexpected type=%d, retrying...\n", type);
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
	}
}


/* Heatmap blob detection (2026-07-08 D).
 * The raw content_id=0x0C frame is a capacitive sensor heatmap:
 * 2 bytes SurfaceSwitch (timestamp/scan ID), then 4297 bytes of cell data.
 * Each byte represents one capacitive cell's deviation from baseline (~0xB4).
 * Low values = touched area (finger absorbs charge).
 * We track a baseline per cell, detect significant drops, cluster into
 * contiguous blobs via connected-component labeling, compute weighted
 * centroids, and emit multitouch events.
 */
#define HEATMAP_MAX_CELLS   4300
#define HEATMAP_TOUCH_THRESHOLD  25
#define HEATMAP_MAX_BLOBS   10
#define HEATMAP_BLOB_MIN_WEIGHT  150    /* total deviation sum across blob */
#define HEATMAP_MAX_SLOTS   2            /* max fingers (Surface Laptop 4 = 2) */
#define HEATMAP_EMA_ALPHA   3            /* smoothing: new_weight = 1, old_weight = 3 */

static bool debug_coords;
module_param(debug_coords, bool, 0644);
MODULE_PARM_DESC(debug_coords, "Log raw blob grid coordinates for calibration");

static bool invert_x;
module_param(invert_x, bool, 0644);
MODULE_PARM_DESC(invert_x, "Invert X axis");

static bool invert_y;
module_param(invert_y, bool, 0644);
MODULE_PARM_DESC(invert_y, "Invert Y axis");

static bool swap_xy;
module_param(swap_xy, bool, 0644);
MODULE_PARM_DESC(swap_xy, "Swap X and Y axes");

struct touch_point { u16 x; u16 y; bool active; };

static u8  heatmap_baseline[HEATMAP_MAX_CELLS];
static u32 heatmap_baseline_cells;
static bool heatmap_have_baseline;
static u8  heatmap_touched[HEATMAP_MAX_CELLS];
static u16 heatmap_label[HEATMAP_MAX_CELLS];

/* Screen mapping: logical range 0..32767 for both X and Y.
 * Physical dims from HID descriptor: 2934 x 1652 (0.1mm) = 293.4mm x 165.2mm.
 * Aspect ratio 2934/1652 ≈ 1.78. 4300 cells / grid: 86 cols × 50 rows = 4300 cells,
 * aspect 86/50 = 1.72 (close match). */
#define GRID_COLS 86
#define GRID_ROWS 50
#define GRID_CELLS (GRID_COLS * GRID_ROWS)

static void heatmap_process_frame(struct spi_hid *shid, u8 *data, u32 data_len, u8 content_id)
{
	struct device *dev = &shid->spi->dev;
	u32 i, cell_count, ncols, nrows;
	static u16 blob_x[HEATMAP_MAX_BLOBS], blob_y[HEATMAP_MAX_BLOBS];
	static u32 blob_wsum[HEATMAP_MAX_BLOBS], blob_xsum[HEATMAP_MAX_BLOBS];
	static u32 blob_ysum[HEATMAP_MAX_BLOBS];
	static bool blob_active[HEATMAP_MAX_BLOBS];
	static u16 label_equiv[256];
	u16 nlabels;
	int touched_count = 0;

	if (data_len < 5) return;
	/* cell_data starts after content_id (1 byte) + SurfaceSwitch (2 bytes).
	 * The HID descriptor defines Report ID 0x0C as: 16-bit SurfaceSwitch + 4297 bytes DeviceIndex constant.
	 * 4302 (content_length) - 1 (content_id) - 2 (SurfaceSwitch) = 4299. */
	cell_count = data_len - 3;

	if (!shid->heatmap_grid_cols || !shid->heatmap_grid_rows) {
		ncols = GRID_COLS; nrows = GRID_ROWS;
		shid->heatmap_grid_cols = ncols;
		shid->heatmap_grid_rows = nrows;
		dev_info(dev, "HEATMAP: grid %u cols x %u rows (up to %u cells, actual %u)\n",
			 ncols, nrows, ncols * nrows, cell_count);
	}
	ncols = shid->heatmap_grid_cols;
	nrows = shid->heatmap_grid_rows;

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

	/* Continuous max-tracking baseline: touches always reduce capacitance,
	 * so the maximum value seen per cell is the resting (no-touch) state.
	 * Update every frame — lifts are immediately reflected. */
	if (!heatmap_have_baseline) {
		static u32 baseline_frames;
		if (content_id == 0x0C && cell_count >= 1000) {
			baseline_frames++;
			for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++)
				if (baseline_frames == 1 || data[3 + i] > heatmap_baseline[i])
					heatmap_baseline[i] = data[3 + i];
			if (baseline_frames >= 30) {
				heatmap_baseline_cells = cell_count;
				heatmap_have_baseline = true;
				dev_info(dev, "HEATMAP: baseline stabilized after %u frames (%u cells, first val=0x%02x)\n",
					 baseline_frames, cell_count, heatmap_baseline[0]);
			}
		}
		return;
	}
	/* Update baseline max on every frame (lifts are immediately captured) */
	for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++)
		if (data[3 + i] > heatmap_baseline[i])
			heatmap_baseline[i] = data[3 + i];

	if (!shid->touch_input) return;

	/* Step 1: mark touched cells.
	 * Only consider cells with an established baseline (resting value > 0x20)
	 * to avoid edge/bezel cells where calibration is not uniform. */
	memset(heatmap_touched, 0, cell_count);
	for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
		u8 cell = data[3 + i];
		s16 diff = (s16)heatmap_baseline[i] - (s16)cell;
		if (diff > HEATMAP_TOUCH_THRESHOLD && heatmap_baseline[i] >= 0x20)
			heatmap_touched[i] = 1;
	}

	/* Morphological dilation: expand touched region by 1 cell to merge
	 * nearby clusters into a single blob. */
	{
		u8 expanded[HEATMAP_MAX_CELLS];
		memcpy(expanded, heatmap_touched, cell_count);
		for (i = 0; i < cell_count && i < HEATMAP_MAX_CELLS; i++) {
			if (!heatmap_touched[i]) continue;
			u16 col = i % ncols, row = i / ncols;
			if (col > 0 && i > 0) expanded[i - 1] = 1;
			if (col + 1 < ncols && i + 1 < cell_count && i + 1 < HEATMAP_MAX_CELLS) expanded[i + 1] = 1;
			if (row > 0 && i >= ncols) {
				expanded[i - ncols] = 1;
				if (col > 0) expanded[i - ncols - 1] = 1;
				if (col + 1 < ncols && i - ncols + 1 < cell_count) expanded[i - ncols + 1] = 1;
			}
			if (row + 1 < nrows) {
				u32 idx = i + ncols;
				if (idx < cell_count && idx < HEATMAP_MAX_CELLS) {
					expanded[idx] = 1;
					if (col > 0 && idx > 0) expanded[idx - 1] = 1;
					if (col + 1 < ncols && idx + 1 < cell_count) expanded[idx + 1] = 1;
				}
			}
		}
		memcpy(heatmap_touched, expanded, cell_count);
	}

	/* Step 2: two-pass connected-component labeling (8-way) */
	memset(heatmap_label, 0xFF, cell_count * 2);
	memset(label_equiv, 0, sizeof(label_equiv));
	nlabels = 0;

	for (i = 0; i < cell_count; i++) {
		u16 col, row, new_label, up;

		if (!heatmap_touched[i]) continue;
		col = i % ncols;
		row = i / ncols;
		if (row >= nrows) break;

		new_label = 0xFFFF;
		if (col > 0 && heatmap_touched[i - 1])
			new_label = heatmap_label[i - 1];
		up = 0;
		if (row > 0) {
			if (col > 0 && heatmap_touched[i - ncols - 1])
				up = heatmap_label[i - ncols - 1];
			if (heatmap_touched[i - ncols])
				up = up ?: heatmap_label[i - ncols];
			if (col + 1 < ncols && heatmap_touched[i - ncols + 1])
				up = up ?: heatmap_label[i - ncols + 1];
		}
		if (up && new_label != 0xFFFF && new_label != up) {
			u16 a = min(new_label, up), b = max(new_label, up);
			if (a < 256 && b < 256) {
				while (label_equiv[a]) a = label_equiv[a];
				while (label_equiv[b]) b = label_equiv[b];
				if (a != b) label_equiv[b] = a;
			}
		}
		if (up) new_label = new_label != 0xFFFF ? new_label : up;
		if (new_label == 0xFFFF) {
			new_label = nlabels++;
			if (nlabels >= 256) new_label = 255;
		}
		heatmap_label[i] = new_label;
	}

	/* Resolve label equivalence */
	for (i = 0; i < nlabels && i < 256; i++) {
		u16 r = label_equiv[i];
		while (r && r < 256 && label_equiv[r]) r = label_equiv[r];
		label_equiv[i] = r;
	}

	/* Step 3: accumulate blob sums (weighted by deviation from baseline) */
	memset(blob_xsum, 0, sizeof(blob_xsum));
	memset(blob_ysum, 0, sizeof(blob_ysum));
	memset(blob_wsum, 0, sizeof(blob_wsum));
	memset(blob_active, 0, sizeof(blob_active));

	for (i = 0; i < cell_count; i++) {
		u16 label, col, row;
		s16 diff;

		if (!heatmap_touched[i]) continue;
		label = heatmap_label[i];
		while (label < 256 && label_equiv[label]) label = label_equiv[label];
		if (label >= HEATMAP_MAX_BLOBS) continue;

		col = i % ncols;
		row = i / ncols;
		if (row >= nrows) break;

		diff = (s16)heatmap_baseline[i] - (s16)data[3 + i];
		blob_xsum[label] += (u32)col * (u32)diff;
		blob_ysum[label] += (u32)row * (u32)diff;
		blob_wsum[label] += (u32)diff;
		blob_active[label] = true;
		touched_count++;
	}

	/* Step 4: compute centroids and emit */
	for (i = 0; i < HEATMAP_MAX_BLOBS; i++) {
		if (!blob_active[i] || blob_wsum[i] < HEATMAP_BLOB_MIN_WEIGHT)
			continue;
		blob_x[i] = (u16)(blob_xsum[i] / blob_wsum[i]);
		blob_y[i] = (u16)(blob_ysum[i] / blob_wsum[i]);
	}

	/* Step 5: emit multitouch events with EMA smoothing and slot tracking.
	 * Map grid coords to logical: x_screen = (col * 32767) / (ncols - 1),
	 * y_screen = (row * 32767) / (nrows - 1). */
	{
		struct input_dev *input = shid->touch_input;
		static u16 prev_gx[HEATMAP_MAX_SLOTS] = {0xFFFF, 0xFFFF};
		static u16 prev_gy[HEATMAP_MAX_SLOTS] = {0xFFFF, 0xFFFF};
		bool any_touch = false;
		u8 valid_count = 0;
		struct { u16 gx; u16 gy; u32 w; u8 idx; } sorted[HEATMAP_MAX_BLOBS];
		u8 sorted_count = 0;

		for (i = 0; i < HEATMAP_MAX_BLOBS; i++) {
			if (!blob_active[i] || blob_wsum[i] < HEATMAP_BLOB_MIN_WEIGHT)
				continue;
			sorted[sorted_count].gx = blob_x[i];
			sorted[sorted_count].gy = blob_y[i];
			sorted[sorted_count].w = blob_wsum[i];
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

		if (debug_coords) {
			for (i = 0; i < sorted_count; i++)
				dev_info(dev, "CALIB: blob[%u] grid=(%u,%u) weight=%u screen=(%u,%u)\n",
					 i, sorted[i].gx, sorted[i].gy, sorted[i].w,
					 (sorted[i].gx * 32767) / (ncols - 1),
					 (sorted[i].gy * 32767) / (nrows - 1));
			if (sorted_count || touched_count)
				dev_info(dev, "CALIB: blobs=%u cells_touched=%d\n",
					 sorted_count, touched_count);
		}

		{
			u8 assigned[HEATMAP_MAX_SLOTS] = {0xFF, 0xFF};
			u16 new_gx[HEATMAP_MAX_SLOTS], new_gy[HEATMAP_MAX_SLOTS];
			bool new_active[HEATMAP_MAX_SLOTS] = {false, false};

			for (i = 0; i < sorted_count; i++) {
				u16 best_dist = 0xFFFF;
				u8  best_slot = HEATMAP_MAX_SLOTS;

				for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
					if (assigned[s] != 0xFF) continue;
					if (prev_gx[s] == 0xFFFF) {
						if (best_slot == HEATMAP_MAX_SLOTS) best_slot = s;
						continue;
					}
					u16 dx = abs((s16)sorted[i].gx - (s16)prev_gx[s]);
					u16 dy = abs((s16)sorted[i].gy - (s16)prev_gy[s]);
					u16 dist = dx + dy;
					if (dist < best_dist || best_slot == HEATMAP_MAX_SLOTS) {
						best_dist = dist;
						best_slot = s;
					}
				}
				if (best_slot < HEATMAP_MAX_SLOTS) assigned[best_slot] = i;
			}

			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				if (assigned[s] != 0xFF) {
					u8 bi = assigned[s];
					if (prev_gx[s] != 0xFFFF) {
						new_gx[s] = (prev_gx[s] * HEATMAP_EMA_ALPHA +
							     sorted[bi].gx) / (HEATMAP_EMA_ALPHA + 1);
						new_gy[s] = (prev_gy[s] * HEATMAP_EMA_ALPHA +
							     sorted[bi].gy) / (HEATMAP_EMA_ALPHA + 1);
					} else {
						new_gx[s] = sorted[bi].gx;
						new_gy[s] = sorted[bi].gy;
					}
					new_active[s] = true;
				} else {
					new_gx[s] = prev_gx[s];
					new_gy[s] = prev_gy[s];
					new_active[s] = false;
				}
			}

			for (u8 s = 0; s < HEATMAP_MAX_SLOTS; s++) {
				input_mt_slot(input, s);
				input_mt_report_slot_state(input, MT_TOOL_FINGER, new_active[s]);
				if (new_active[s]) {
					u16 sx, sy, tmp;
					sx = (new_gx[s] * 32767) / (ncols - 1);
					sy = (new_gy[s] * 32767) / (nrows - 1);
					if (swap_xy) { tmp = sx; sx = sy; sy = tmp; }
					if (invert_x)  sx = 32767 - sx;
					if (invert_y)  sy = 32767 - sy;
					input_report_abs(input, ABS_MT_POSITION_X, sx);
					input_report_abs(input, ABS_MT_POSITION_Y, sy);
					any_touch = true;
				}
				prev_gx[s] = new_gx[s];
				prev_gy[s] = new_gy[s];
			}
		}

		for (u8 s = (u8)HEATMAP_MAX_SLOTS; s < 10; s++) {
			input_mt_slot(input, s);
			input_mt_report_slot_state(input, MT_TOOL_FINGER, false);
		}
		input_report_key(input, BTN_TOUCH, any_touch ? 1 : 0);
		input_mt_sync_frame(input);
		input_sync(input);
	}
}

static void heatmap_report_touch(struct spi_hid *shid, struct touch_point *pts, int count)
{
	struct input_dev *input = shid->touch_input;
	int i, active = 0;

	for (i = 0; i < count && active < 10; i++) {
		if (!pts[i].active) continue;
		input_mt_slot(input, active);
		input_mt_report_slot_state(input, MT_TOOL_FINGER, true);
		input_report_abs(input, ABS_MT_POSITION_X, pts[i].x);
		input_report_abs(input, ABS_MT_POSITION_Y, pts[i].y);
		active++;
	}
	for (i = active; i < 10; i++) {
		input_mt_slot(input, i);
		input_mt_report_slot_state(input, MT_TOOL_FINGER, false);
	}
	input_report_key(input, BTN_TOUCH, active > 0 ? 1 : 0);
	input_mt_sync_frame(input);
	input_sync(input);
}

static ssize_t heatmap_debug_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);
	u32 off = 0, i;

	if (!shid->heatmap_buf || !shid->heatmap_len)
		return snprintf(buf, PAGE_SIZE, "no frame captured\n");
	off += snprintf(buf + off, PAGE_SIZE - off,
		"content_id=0x%02x len=%u cells=%u grid=%ux%u\n",
		shid->heatmap_content_id, shid->heatmap_len,
		shid->heatmap_len - 2,
		shid->heatmap_grid_cols, shid->heatmap_grid_rows);
	for (i = 0; i < shid->heatmap_len && off < PAGE_SIZE - 4; i += 32) {
		u32 chunk = min_t(u32, 32, shid->heatmap_len - i);
		off += snprintf(buf + off, PAGE_SIZE - off,
			"%04x: %*ph\n", i, chunk, shid->heatmap_buf + i);
	}
	return off;
}
static DEVICE_ATTR_RO(heatmap_debug);

static irqreturn_t spi_hid_seq_thread(int irq, void *_shid)
{
	struct spi_hid *shid = _shid;
	struct device *dev = &shid->spi->dev;
	u8 hdr[10]; int type; u16 blen = 0;
	static ktime_t dbg_last_irq;
	static bool dbg_expect_fast;
	s64 dbg_dt_us;

	if (!shid->seq_enabled) return IRQ_HANDLED;

	dev_info(dev, "SEQ: thread entry seq_state=%d seq_enabled=%d\n", shid->seq_state, shid->seq_enabled);

	dbg_dt_us = dbg_last_irq ? ktime_us_delta(ktime_get(), dbg_last_irq) : -1;
	dbg_last_irq = ktime_get();

	/* Windows reads response header first: 5 sync bytes + 4-byte header = 9 bytes.
	 * AMD controller with TX_COUNT=3 sees 6 sync bytes; try 10 to capture full header. */
	if (spi_hid_seq_read(shid, hdr, sizeof(hdr))) {
		dev_err(dev, "SEQ: header read fail\n");
		return IRQ_HANDLED;
	}
	type = spi_hid_seq_hdr_type(&hdr[6], sizeof(hdr) - 6, NULL);
	dev_info(dev, "SEQ[state=%d] type=%d hdr=[%*ph] dt=%lld us%s\n",
		 shid->seq_state, type, 4, &hdr[6], dbg_dt_us,
		 dbg_expect_fast ? (dbg_dt_us >= 0 && dbg_dt_us < 5000 ?
			" <<< FAST IRQ AFTER DESCREQ: WRITE REACHED DEVICE" :
			" <<< slow IRQ: DESCREQ ignored (device just re-reset)") : "");
	dbg_expect_fast = false;
	if (type < 0) {
		dev_info(dev, "SEQ: no header found\n");
		/* In state 0, drain any body data and send DESCREQ anyway */
		if (shid->seq_state == 0) {
			u8 body_drain[64];
			spi_hid_seq_read(shid, body_drain, sizeof(body_drain));
			dev_info(dev, "SEQ: body drain done, forcing DESCREQ@0x000001...\n");
			{
				static const u8 dr[] = {
					0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
					0x00, 0x00, 0x03, 0x00, 0x00
				};
				spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
			}
			shid->seq_state = 1;
		} else {
			return IRQ_HANDLED;
		}
		return IRQ_HANDLED;
	}

	blen = (((hdr[7] >> 4) & 0xF) << 0) | (hdr[8] << 4);
	blen *= 4;
	if (blen > sizeof(shid->input.content))
		blen = sizeof(shid->input.content);

	switch (shid->seq_state) {
	case 0: /* WAIT_RESET — PURE WINDOWS FLOW: drain RESET_RSP body, then DESCREQ.
		 * NO Himax vendor init (Windows never sends it; it may corrupt the
		 * device so later DESCREQs fail). Go to state 1 and wait for DEVICE_DESC. */
		if (type == 3) {
			u8 body[20];
			static const u8 dr[] = {
				0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
				0x00, 0x00, 0x03, 0x00, 0x00
			};
			u8 dr_rx[11];

			spi_hid_seq_read(shid, body, sizeof(body));
			dev_info(dev, "SEQ[state0-clean]: RESET_RSP body-drain=[%*ph], sending TX-ONLY DESCREQ (CS pulse = 10 bytes exactly, like Windows)\n",
				 20, body);
			(void)dr_rx;
			/* TX-ONLY: rx_len=0 so spi-amd asserts CS for exactly opcode+9 bytes
			 * then deasserts. A non-zero rx_len holds CS and clocks extra bytes,
			 * which the device counts as part of the command → malformed DESCREQ. */
			spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
			dev_info(dev, "SEQ[state0-clean]: TX-only DESCREQ sent, waiting for DEVICE_DESC IRQ\n");
			dbg_expect_fast = true;
			shid->seq_state = 1;
		}
		break;

	case 1: /* WAIT_DESC — poll for DEVICE_DESC even without IRQ */
		if (type == 7) {
			u8 body[64];
			u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);
			dev_info(dev, "SEQ: DEVICE_DESC! reading body (%u bytes)...\n", blen);
			spi_hid_seq_read(shid, body, rblen);
			dev_info(dev, "DIFFCHECK: DEVICE_DESC full body=[%*ph]\n", rblen, body);
			{
				struct spi_hid_device_desc_raw raw;
				/* body[] from spi_hid_seq_read contains raw MISO:
				 * [sync bytes (5 or 6 x 0xFF)][body_prefix (3B)][descriptor (28B)]
				 * Skip sync bytes, then skip the 3-byte body prefix. */
				u32 off = 0;

				while (off < rblen - 3 && body[off] == 0xFF)
					off++;
				off += 3; /* skip body prefix [len16 LE][ContentID] */
				dev_info(dev, "SEQ: parsing at rx+%u\n", off);
				memcpy(&raw, body + off,
				       min_t(u32, sizeof(raw), rblen > off ? rblen - off : 0));
				spi_hid_parse_dev_desc(&raw, &shid->desc);
				dev_info(dev, "SEQ: vid=0x%04X pid=0x%04X ver=0x%04X inp=0x%04X out=0x%04X cmd=0x%04X rpt_len=%u max_in=%u max_out=%u\n",
					shid->desc.vendor_id, shid->desc.product_id,
					shid->desc.version_id, shid->desc.input_register,
					shid->desc.output_register,
					shid->desc.command_register,
					shid->desc.report_descriptor_length,
					shid->desc.max_input_length,
					shid->desc.max_output_length);
			}
			dev_info(dev, "SEQ: sending DESCREQ2 to reg=0x%04X...\n",
				 shid->desc.report_descriptor_register);
			{
				u8 dr2[11] = {
					0x02, 0x02, 0x00, 0x00, 0x02, 0x42,
					0x00, 0x00, 0x03, 0x00, 0x00  /* byte6 fixed 2026-07-08: was 0x02
					 * ("content_type=0x02 per MS spec"), but the real Windows wire
					 * trace (surface_boot_auto.csv, clock 134276452171762154) shows
					 * this byte as 0x00 for its DESCREQ2 -- verified byte-for-byte
					 * against DESCREQ (byte6=0x00 there too, confirmed matching). */
				};
				dr2[2] = (shid->desc.report_descriptor_register >> 16) & 0xFF;
				dr2[3] = (shid->desc.report_descriptor_register >> 8) & 0xFF;
				dr2[4] = shid->desc.report_descriptor_register & 0xFF;
				spi_hid_seq_write(shid, dr2, sizeof(dr2), NULL, 0);
			}
			shid->seq_state = 2;
		} else if (type == 3) {
			u8 body[16];
			u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);

			if (rblen)
				spi_hid_seq_read(shid, body, rblen);
			dev_info(dev, "SEQ: RESET_RSP in state 1, sending DESCREQ directly (no _RST, no workqueue)\n");
			{
				static const u8 dr[] = {
					0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
					0x00, 0x00, 0x03, 0x00, 0x00
				};
				spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
			}
			dev_info(dev, "SEQ: DESCREQ sent synchronously, waiting for next IRQ\n");
			shid->seq_state = 1;
		}
		break;

	case 2: /* WAIT_RPT → read RPT_DESC body, then replicate Windows's GET_FEATURE/
		 * SET_FEATURE exchange on the output register before going DONE. This exact
		 * exchange (content_id=4) was found in traces/surface_boot_auto.csv (TXN#220-223,
		 * via tools/parse_spi.py) sitting ~10-16ms after RPT_DESC and ~1.6ms before the
		 * first real DATA report — never replicated before. Device has a ~507-508ms
		 * internal reset timer after RPT_DESC if nothing further happens (measured
		 * empirically); this exchange is the candidate for what it's waiting for. */
		if (type == 8) {
			u8 body[1024];
			u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);
			dev_info(dev, "SEQ: RPT_DESC! reading body (%u bytes)...\n", blen);
			spi_hid_seq_read(shid, body, rblen);
			{
				u32 off;
				for (off = 0; off < rblen; off += 64) {
					u32 chunk = min_t(u32, 64, rblen - off);
					dev_info(dev, "DIFFCHECK: RPT_DESC+%u=[%*ph]\n", off, chunk, body + off);
				}
			}
			{
				/* 2026-07-08: stash the wire-read descriptor for
				 * spi_hid_ll_parse() to prefer over the hardcoded fallback
				 * (docs/NEXT_STEPS.md §C). Same prefix-skip convention as
				 * the DEVICE_DESC parse above: variable-length leading
				 * 0xFF dummy bytes, then a fixed 3-byte [len16 LE][ContentID]
				 * body prefix before the real descriptor bytes begin. */
				u32 off = 0;
				u32 len;

				while (off < rblen - 3 && body[off] == 0xFF)
					off++;
				off += 3;
				len = min_t(u32, shid->desc.report_descriptor_length,
					    sizeof(shid->wire_report_descriptor));
				if (off < rblen && len > 0 && off + len <= rblen) {
					u32 k;

					memcpy(shid->wire_report_descriptor, body + off, len);

					/* 2026-07-08 (docs/GROUND_TRUTH.md §18.6): a
					 * characterized, clock-speed-independent,
					 * chunk-boundary-independent hardware defect reads
					 * back a spurious 0xFF at every content offset
					 * n*64+58 (confirmed: tied to the device's own
					 * 64-byte page structure, not to our fetch
					 * chunking — verified by deliberately misaligning
					 * fetch chunks and observing the corruption stay
					 * anchored to these same absolute offsets, just
					 * widening). This is a targeted, minimal patch of
					 * only those specific known-bad bytes using
					 * hardcoded_report_descriptor (same physical touch
					 * chip) as ground truth — not a blanket fallback;
					 * every other byte still comes from the live wire
					 * read, so a genuinely different firmware/SKU would
					 * still be picked up everywhere except these 14
					 * positions.
					 *
					 * Phase note: offset 58 was measured against a
					 * buffer that still included the 3-byte
					 * [len16 LE][ContentID] prefix (`ours_c` in the
					 * diagnostic diff script); `wire_report_descriptor`
					 * here starts right at the real descriptor bytes
					 * (after `off`, which already skips that prefix),
					 * so the equivalent phase in *this* buffer's own
					 * indexing is 58-3=55. */
					for (k = 55; k < len; k += 64) {
						if (shid->wire_report_descriptor[k] == 0xFF &&
						    k < HARDCODED_RD_SIZE &&
						    hardcoded_report_descriptor[k] != 0xFF) {
							dev_info(dev, "SEQ: patching known-corrupt wire descriptor byte at offset %u (0xff -> 0x%02x)\n",
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
			dev_info(dev, "SEQ: report descriptor received, shid->hid=%p, scheduling create_device_work...\n", shid->hid);
			/* Standard HID mode (no GET_FEATURE/SET_FEATURE):
			 * device sends Report ID 0x40 (TouchScreen) with
			 * pre-computed X, Y, TipSwitch — single-touch.
			 * Multi-touch requires raw heatmap mode (send
			 * GET_FEATURE+SEND_FEATURE) with grid blob detection
			 * which needs reverse-engineering of the DFT antenna
			 * layout (see docs/NEXT_STEPS.md §D). */
			shid->seq_state = 4;
			shid->ready = true;
			shid->keep_powered = true;
			if (!shid->hid) {
				bool queued = schedule_work(&shid->create_device_work);
				dev_info(dev, "SEQ: scheduled create_device_work, queued=%d\n", queued);
			}
		} else if (type == 3) {
			u8 body[16];
			u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);

			if (rblen)
				spi_hid_seq_read(shid, body, rblen);
			dev_info(dev, "SEQ: RESET_RSP in state 2, sending DESCREQ directly\n");
			{
				static const u8 dr[] = {
					0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
					0x00, 0x00, 0x03, 0x00, 0x00
				};
				spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
			}
			shid->seq_state = 1;
		}
		break;

	case 5: /* WAIT_FEAT_RESP -- GET_FEATURE(id=4) sent, then SET_FEATURE, then DONE.
		 * Mirrors surface_boot_auto.csv TXN#221-223 (GET_FEAT_RESP read, then the
		 * SET_FEATURE write) which Windows does right before real DATA reports start. */
		if (type == 5) {
			u8 body[256];
			u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);
			dev_info(dev, "SEQ: GET_FEAT_RESP! reading body (%u bytes)...\n", blen);
			spi_hid_seq_read(shid, body, rblen);
			dev_info(dev, "DIFFCHECK: GET_FEAT_RESP full body=[%*ph]\n", rblen, body);
			{
				/* Windows TXN#223: 02 00 00 03 82 00 03 04 00 05 01 00 00 00 --
				 * opcode+reg0x0003+len16=0x82+body[cmd=03,id=04,00,05,01,00,00,00].
				 * Doubled leading 0x02, same quirk as GET_FEATURE/DESCREQ above.
				 * Confirmed byte-identical to Windows's real wire frame
				 * (traces/surface_boot_auto.csv, clock 134276452230827844) —
				 * 2026-07-08: bisected by skipping this write entirely, which
				 * reverted the post-SET_FEATURE reset from ~109ms back to the
				 * old ~508ms idle timeout. So SET_FEATURE's *content* is not
				 * the problem; sending it (correctly) is what triggers a fast
				 * ~109ms reset instead of the passive ~508ms one — meaning the
				 * device is actively rejecting something else about our state
				 * at that point (see docs/NEXT_STEPS.md §A). */
				static const u8 setfeat[] = {
					0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
					0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
				};
				dev_info(dev, "SEQ: sending SET_FEATURE(id=4), replicating surface_boot_auto.csv TXN#223...\n");
				spi_hid_seq_write(shid, setfeat, sizeof(setfeat), NULL, 0);
			}
			shid->seq_state = 4;
		} else if (type == 3) {
			u8 body[16];
			u32 rblen = (blen + 6) < sizeof(body) ? (blen + 6) : sizeof(body);

			if (rblen)
				spi_hid_seq_read(shid, body, rblen);
			dev_info(dev, "SEQ: RESET_RSP in state 5, sending DESCREQ directly\n");
			{
				static const u8 dr[] = {
					0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
					0x00, 0x00, 0x03, 0x00, 0x00
				};
				spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
			}
			shid->seq_state = 1;
		}
		break;

	case 3: /* VENDOR_INIT — wait for DATA (type=1) or RESET_RSP (type=3) */
		if (type == 1) {
			dev_info(dev, "SEQ: VENDOR_INIT: got DATA! Creating HID device...\n");
			shid->seq_state = 4;
			shid->ready = true;
			shid->keep_powered = true;
			if (!shid->hid) {
				spi_hid_create_device(shid);
			}
			/* Also handle this first DATA report */
			goto handle_data;
		} else if (type == 3) {
			dev_info(dev, "SEQ: VENDOR_INIT: got RESET_RSP, vendor init ignored. Hardcoding descriptors...\n");
			/* Vendor init didn't work. Hardcode and go to READY anyway. */
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
			shid->seq_state = 4;
			shid->ready = true;
			shid->keep_powered = true;
			if (!shid->hid) {
				dev_info(dev, "SEQ: creating HID device with hardcoded descriptors...\n");
				spi_hid_create_device(shid);
			}
		}
		break;

	case 4: /* DONE — forward input reports (type==1 DATA) */
handle_data:
		if (type == 3) {
			dev_info(dev, "SEQ: Device reset detected in state 4. Re-initializing sequencer...\n");
			static const u8 dr[] = {
				0x02, 0x02, 0x00, 0x00, 0x01, 0x42,
				0x00, 0x00, 0x03, 0x00, 0x00
			};
			u8 body[20];
			spi_hid_seq_read(shid, body, sizeof(body));
			spi_hid_seq_write(shid, dr, sizeof(dr), NULL, 0);
			shid->seq_state = 1;
			break;
		}
		if (type == 1 && shid->hid) {
			/* 2026-07-08: DEVICE_DESC reports max_input_length=8192 — this
			 * device legitimately sends reports far larger than a small
			 * fixed stack buffer (confirmed live: a real, non-touch report
			 * repeating every ~625ms regardless of touch state has
			 * content-length 4302, consistent with e.g. the heat-map report
			 * IDs already known to this driver, SPI_HID_*_HEAT_MAP_REPORT_ID
			 * in spi-hid-core.h). The previous fixed `body[512]` silently
			 * under-read every such report (rblen clamped to 512 regardless
			 * of the device's true length) and separately risked a stack
			 * buffer over-read if `rl` ever exceeded what was actually read
			 * (the bug that crashed the kernel the first time a real report
			 * reached this path). Fixed properly: heap-allocate a buffer
			 * sized to the report's own claimed length (capped at
			 * max_input_length, with a hard sanity ceiling regardless of
			 * what the device claims), and bound `rl` against what was
			 * actually read into it. */
			u32 cap = shid->desc.max_input_length ?
				  shid->desc.max_input_length : 0x1000;
			u32 rblen = (blen + 6) < cap ? (blen + 6) : cap;
			u32 avail;
			u16 rl;
			u8 *body;

			rblen = min_t(u32, rblen, 8200);
			body = kmalloc(rblen, GFP_KERNEL);
			if (!body)
				break;
			avail = (rblen > 8) ? (rblen - 8) : 0;

			if (spi_hid_seq_read(shid, body, rblen)) {
				kfree(body);
				break;
			}
			rl = body[6] | (body[7] << 8);
			dev_info(dev, "SEQ: DATA report type=1 len=%u content_id=0x%02x\n",
				 rl, body[8]);
			if (rl > 2 && rl - 2 <= avail) {
				int hret = hid_input_report(shid->hid, HID_INPUT_REPORT,
						 &body[8], rl - 2, 1);
				if (hret)
					dev_warn(dev, "SEQ: hid_input_report failed: %d (content_id=0x%02x)\n",
						 hret, body[8]);
			} else if (rl > 2 && rl - 2 > avail)
				dev_warn(dev, "SEQ: DATA report len=%u exceeds buffer (avail=%u), dropped\n",
					 rl, avail);
			kfree(body);
		}
		break;
	}
	return IRQ_HANDLED;
}

static irqreturn_t spi_hid_dev_irq(int irq, void *_shid)
{
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

	/* 2026-07-08: prefer the descriptor actually read off the wire
	 * (docs/NEXT_STEPS.md §C) — a real driver shouldn't permanently rely on
	 * a descriptor hardcoded from one past capture, which can't track
	 * firmware revisions or other SKUs of the same VID/PID. Trust
	 * hid_parse_report()'s own structural validation as the sanity check:
	 * if the wire-read bytes don't parse, fall back to the hardcoded copy
	 * exactly as before. */
	if (shid->wire_report_descriptor_len > 0 && !shid->wire_report_descriptor_rejected) {
		dev_info(dev, "SEQ: ll_parse — trying device-read report descriptor (%u bytes)\n",
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

	dev_info(dev, "SEQ: ll_parse — using HARDCODED report descriptor (%d bytes)\n",
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

	if (!shid->ready) {
		dev_err(&shid->spi->dev, "%s called in unready state\n", __func__);
		return -ENODEV;
	}

	mutex_lock(&shid->lock);

	switch (reqtype) {
	case HID_REQ_SET_REPORT:
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

		ret = min_t(size_t, len,
			(shid->response.body[0] | (shid->response.body[1] << 8)) - 3);
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
	struct spi_hid_output_report report = {
		.content_type = SPI_HID_CONTENT_TYPE_OUTPUT_REPORT,
		.content_length = len - 1 + 3,
		.content_id = buf[0],
		.content = &buf[1],
	};

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

static ssize_t regulator_error_count_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d (%d)\n",
			shid->regulator_error_count,
			shid->regulator_last_error);
}
static DEVICE_ATTR_RO(regulator_error_count);

static ssize_t device_initiated_reset_count_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d\n", shid->dir_count);
}
static DEVICE_ATTR_RO(device_initiated_reset_count);

static ssize_t logic_error_count_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return snprintf(buf, PAGE_SIZE, "%d (%d)\n",
			shid->logic_error_count, shid->logic_last_error);
}
static DEVICE_ATTR_RO(logic_error_count);

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

static const struct attribute *const spi_hid_attributes[] = {
	&dev_attr_ready.attr,
	&dev_attr_bus_error_count.attr,
	&dev_attr_regulator_error_count.attr,
	&dev_attr_device_initiated_reset_count.attr,
	&dev_attr_logic_error_count.attr,
	&dev_attr_spi_hid_latency.attr,
	&dev_attr_spi_hid_perf_mode.attr,
	&dev_attr_heatmap_debug.attr,
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

static int spi_hid_probe(struct spi_device *spi)
{
	struct device *dev = &spi->dev;
	struct spi_hid *shid;
	struct gpio_desc *gpiod;
	unsigned long irqflags;
	int ret;

	dev_err(dev, "PROBE: ENTER\n");

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

	dev_err(dev, "PROBE: after kzalloc\n");	shid->spi = spi;
	shid->power_state = SPI_HID_POWER_MODE_ACTIVE;
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

	/*
	* input_register is used for read approval. Set to default value here.
	* It will be overwritten later with value from device descriptor
	*/
	shid->desc.input_register = SPI_HID_DEFAULT_INPUT_REGISTER;

	mutex_init(&shid->lock);
	mutex_init(&shid->power_lock);
	mutex_init(&shid->output_lock);
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
	INIT_WORK(&shid->fw_work, spi_hid_fw_work);
	INIT_DELAYED_WORK(&shid->descreq_work, spi_hid_seq_descreq_work);

	dev_err(dev, "PROBE: before GPIO\n");
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

		/* Mask IRQ */
		irq_set_irqchip_state(irq, IRQCHIP_STATE_MASKED, 1);

		/* Reconfigure trigger type */
		irq_set_irq_type(irq, irqflags & IRQF_TRIGGER_MASK);

		/* Clear pending interrupt */
		if (id && id->chip && id->chip->irq_ack)
			id->chip->irq_ack(id);
		irq_set_irqchip_state(irq, IRQCHIP_STATE_PENDING, 0);

		dev_info(dev, "GPIO dance: mask→reconf→clear done\n");
	}

	/* Force a clean electrical reset via ACPI _PS3 -> _PS0 power cycle */
	{
		acpi_handle h = ACPI_HANDLE(dev);
		if (h) {
			dev_info(dev, "SEQ: Power cycling device via ACPI _PS3 -> _PS0...\n");
			acpi_evaluate_object(h, "_PS3", NULL, NULL);
			msleep(50);
			acpi_evaluate_object(h, "_PS0", NULL, NULL);
			msleep(100);
		}
	}

	shid->seq_enabled = true;
	shid->seq_state = 0;
	shid->ready = shid->seq_state >= 4 ? true : false;
	shid->keep_powered = true;

	/* TEST: wait for device to stabilize after ACPI _INI power-on.
	 * _INI is called by the ACPI subsystem before probe() and handles
	 * GPIO power sequencing. The device is already powered and sending
	 * RESET_RSP. Do NOT call _RST/M009/M010 — power cycle kills the
	 * device until reboot (verified 2026-07-07, see GROUND_TRUTH §10.7). */
	msleep(300);
	shid->desc.input_register = 0x000000;
	dev_info(dev, "SEQ: device powered by ACPI _INI, arming IRQ\n");

	/* Create multitouch input device for heatmap-to-touch pipeline */
	shid->touch_input = input_allocate_device();
	if (shid->touch_input) {
		shid->touch_input->name = "MSHW0231 Touchscreen";
		shid->touch_input->phys = "spi-hid/input1";
		shid->touch_input->id.bustype = BUS_SPI;
		shid->touch_input->id.vendor = 0x045E;
		shid->touch_input->id.product = 0x0C19;
		set_bit(EV_ABS, shid->touch_input->evbit);
		set_bit(EV_KEY, shid->touch_input->evbit);
		set_bit(BTN_TOUCH, shid->touch_input->keybit);
		input_set_abs_params(shid->touch_input, ABS_MT_POSITION_X, 0, 32767, 0, 0);
		input_set_abs_params(shid->touch_input, ABS_MT_POSITION_Y, 0, 32767, 0, 0);
		input_mt_init_slots(shid->touch_input, HEATMAP_MAX_SLOTS, 0);
		if (input_register_device(shid->touch_input)) {
			dev_warn(dev, "HEATMAP: failed to register touch input device\n");
			input_free_device(shid->touch_input);
			shid->touch_input = NULL;
		} else {
			dev_info(dev, "HEATMAP: multitouch input device registered\n");
		}
	}
	shid->heatmap_active = true;
	heatmap_have_baseline = false;
	heatmap_baseline_cells = 0;

	ret = request_threaded_irq(shid->irq, spi_hid_dev_irq, spi_hid_seq_thread,
				   irqflags, dev_name(&spi->dev), shid);
	if (ret)
		goto err1;
	shid->irq_enabled = true;
	dev_info(dev, "SEQ: IRQ armed (state=WAIT_RESET, zero touch)\n");
	dev_err(dev, "%s: d3 -> %s\n", __func__,
			spi_hid_power_mode_string(shid->power_state));
	return 0;

err1:
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);

err0:
	return ret;
}

static void spi_hid_remove(struct spi_device *spi)
{
	struct spi_hid *shid = spi_get_drvdata(spi);
	struct device *dev = &spi->dev;

	dev_info(dev, "%s: REMOVING\n", __func__);

	spi_hid_power_down(shid);
	free_irq(shid->irq, shid);
	if (shid->touch_input) {
		input_unregister_device(shid->touch_input);
		shid->touch_input = NULL;
	}
	kfree(shid->heatmap_buf);
	if (shid->gpiod)
		gpiod_put(shid->gpiod);
	shid->irq_enabled = false;
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);
	spi_hid_stop_hid(shid);
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
