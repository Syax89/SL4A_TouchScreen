// SPDX-License-Identifier: GPL-2.0
/*
 * HID over SPI protocol implementation
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
#include <linux/input/mt.h>
#include <linux/math.h>

#include "spi-hid-core.h"
#include "spi-hid-protocol.h"
#include "mshw0231-raw.h"
#include "spi-hid_trace.h"
/* Hardcoded HID Report Descriptor from Windows dump (936 bytes) */
#include "hardcoded_rd.h"

_Static_assert(sizeof(hardcoded_report_descriptor) == HARDCODED_RD_SIZE,
	       "hardcoded RD size mismatch");

#define SPI_HID_MAX_RESET_ATTEMPTS 3
/* V0 has a 12-bit body field, but spi-amd only guarantees one atomic 70-byte
 * PIO segment. The six-byte V0 header leaves 64 bytes for its padded body. */
#define SPI_HID_V0_MAX_OUTPUT_BODY 64

int sl4a_debug_level;
static int getfeat_delay_ms;  /* RPT_DESC → GET_FEATURE settle time (0 = immediate, safe default) */
static bool skip_getfeat = true;
#define seq_dbg(shid, level, fmt, ...) \
	do { if (sl4a_debug_level >= (level)) \
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

static const char *spi_hid_seq_reason_name(enum spi_hid_seq_reason reason)
{
	switch (reason) {
	case SPI_HID_SEQ_PROBE:
		return "PROBE";
	case SPI_HID_SEQ_RESET_RESPONSE:
		return "RESET_RESPONSE";
	case SPI_HID_SEQ_DEVICE_DESCRIPTOR:
		return "DEVICE_DESCRIPTOR";
	case SPI_HID_SEQ_REPORT_DESCRIPTOR:
		return "REPORT_DESCRIPTOR";
	case SPI_HID_SEQ_FEATURE_REQUEST:
		return "FEATURE_REQUEST";
	case SPI_HID_SEQ_FEATURE_RESPONSE:
		return "FEATURE_RESPONSE";
	case SPI_HID_SEQ_DEVICE_RESET:
		return "DEVICE_RESET";
	case SPI_HID_SEQ_WATCHDOG:
		return "WATCHDOG";
	case SPI_HID_SEQ_FALLBACK:
		return "FALLBACK";
	default:
		return "UNKNOWN";
	}
}

static const char *spi_hid_seq_state_name(enum spi_hid_seq_state state)
{
	switch (state) {
	case SPI_HID_SEQ_WAIT_RESET:
		return "WAIT_RESET";
	case SPI_HID_SEQ_WAIT_DESC:
		return "WAIT_DESC";
	case SPI_HID_SEQ_WAIT_RPT:
		return "WAIT_RPT";
	case SPI_HID_SEQ_VENDOR_INIT:
		return "VENDOR_INIT";
	case SPI_HID_SEQ_DONE:
		return "DONE";
	case SPI_HID_SEQ_WAIT_FEATURE:
		return "WAIT_FEATURE";
	case SPI_HID_SEQ_INVALID:
	default:
		return "INVALID";
	}
}

/* Set the sequencer state machine to a new state. */
static void spi_hid_seq_set_state(struct spi_hid *shid,
		enum spi_hid_seq_state new_state, enum spi_hid_seq_reason reason)
{
	enum spi_hid_seq_state old_state = shid->seq_state;

	if (old_state == new_state)
		return;

	shid->seq_state = new_state;
	trace_spi_hid_seq_state(shid, old_state, new_state, reason);
	seq_dbg(shid, 1, "SEQ: state %s(%d) -> %s(%d) reason=%s(%d)\n",
		spi_hid_seq_state_name(old_state), old_state,
		spi_hid_seq_state_name(new_state), new_state,
		spi_hid_seq_reason_name(reason), reason);

	if (new_state == SPI_HID_SEQ_WAIT_DESC)
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
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

static int spi_hid_output_header(__u8 *buf,
		u16 output_register, u16 output_report_length)
{
	return spi_hid_protocol_encode_output_header(buf, output_register,
			output_report_length);
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
	 * output_report() is implemented with spi_sync() rather than
	 * spi_async(), intentionally deviating from the requirement in
	 * Documentation/hid/hid-transport.rst that ->output_report() be
	 * asynchronous.  This is safe for the following reasons:
	 *
	 * 1) The SPI bus framework serializes all transfers so only one
	 *    message can be in-flight at a time.
	 * 2) The sequencer state machine (seq_lock) already enforces
	 *    ordering — input reads and output writes never race.
	 * 3) The standard HID profile for this device only sends
	 *    infrequent single-touch and pen reports that complete in
	 *    well under 2 ms, so blocking the calling thread has no
	 *    measurable latency impact.
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

/*
 * Set device power state via SET_POWER command.
 * raw_buf[14] = power mode (ACTIVE/SLEEP/OFF).
 * Called under power_lock mutex.
 */
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

	lockdep_assert_held(&shid->power_lock);

	if (!shid->powered)
		return 0;

	ret = spi_hid_set_power(shid, SPI_HID_POWER_MODE_SLEEP);
	if (ret) {
		dev_err(dev, "failed to set power SLEEP: %d\n", ret);
		return ret;
	}

	if (shid->spi->dev.of_node) {
		ret = pinctrl_select_state(shid->pinctrl, shid->pinctrl_sleep);
		if (ret) {
			dev_err(dev, "failed to select sleep pin state: %d\n", ret);
			return ret;
		}

		ret = regulator_disable(shid->supply);
		if (ret) {
			dev_err(dev, "failed to disable regulator\n");
			return ret;
		}
		shid->power_state = SPI_HID_POWER_MODE_OFF;
	} else {
		shid->power_state = SPI_HID_POWER_MODE_SLEEP;
	}

	shid->powered = false;

	return 0;
}

static struct hid_device *spi_hid_disconnect_hid(struct spi_hid *shid)
{
	struct hid_device *hid;

	mutex_lock(&shid->seq_lock);
	hid = shid->hid;
	shid->hid = NULL;
	shid->hid_creating = false;
	mutex_unlock(&shid->seq_lock);

	return hid;
}

static void spi_hid_stop_hid(struct spi_hid *shid)
{
	struct hid_device *hid;

	/* Stop possible publishers before detaching the sequencer-visible HID. */
	if (shid->works_initialized) {
		cancel_work_sync(&shid->create_device_work);
		cancel_work_sync(&shid->refresh_device_work);
	}
	hid = spi_hid_disconnect_hid(shid);
	if (hid)
		hid_destroy_device(hid);
}

static void spi_hid_disable_irq(struct spi_hid *shid);

/* _RST calls M010 which DESTROYS the device. Never call it. */
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
	if (dev->of_node) {
		/* Drive reset for at least 100 ms */
		msleep(100);
	}

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
	shid->power_state = SPI_HID_POWER_MODE_ACTIVE;

out:
	mutex_unlock(&shid->power_lock);
	if (terminal_failure) {
		/* refresh_device_work also needs power_lock. Never wait for it while
		 * holding that lock, otherwise exhausted recovery deadlocks. */
		mutex_lock(&shid->seq_lock);
		WRITE_ONCE(shid->seq_enabled, false);
		shid->poll_active = false;
		shid->stream_watchdog_active = false;
		mutex_unlock(&shid->seq_lock);
		spi_hid_disable_irq(shid);
		if (shid->works_initialized) {
			cancel_delayed_work_sync(&shid->descreq_work);
			cancel_delayed_work_sync(&shid->poll_work);
			cancel_delayed_work_sync(&shid->raw_handshake_watchdog);
			cancel_delayed_work_sync(&shid->feat_delay_work);
			cancel_delayed_work_sync(&shid->stream_watchdog);
		}
		spi_hid_stop_hid(shid);
		mutex_lock(&shid->power_lock);
		spi_hid_power_down(shid);
		mutex_unlock(&shid->power_lock);
	}
	return ret;
}

/* Forward declarations */
static int spi_hid_seq_write(struct spi_hid *shid, const u8 *buf, int len, u8 *rx, int rx_len);
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

/* Shared vendor-init command (19 bytes). Opcode 0x02 (doubled), register 0x000003,
 * content_id=0xC2, len=0x0A, payload 56 BD 0C EE 5B 44 4C. */
static const u8 vendor_init_cmd[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00,
	0x03, 0x0A, 0x00, 0x56, 0xBD, 0x0C, 0xEE,
	0x5B, 0x44, 0x4C, 0x00, 0x00
};

/* Shared SET_FEATURE command (15 bytes, doubled-opcode). */
static const u8 sf_cmd[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00,
	0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
};

/* Shared GET_FEATURE command (11 bytes). */
static const u8 gf_cmd[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0x42,
	0x00, 0x04, 0x03, 0x00, 0x06
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

	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended))
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
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended))
		return;
	mutex_lock(&shid->seq_lock);
	shid->reset_pending = true;
	mutex_unlock(&shid->seq_lock);
	/* A reset invalidates the descriptor used by a queued HID creation. */
	cancel_work_sync(&shid->create_device_work);
	if (shid->ready) {
		dev_err(dev, "Spontaneous FW reset!");
		shid->ready = false;
		shid->dir_count++;
		sysfs_notify(&dev->kobj, NULL, "ready");
	}

	mutex_lock(&shid->power_lock);
	if (shid->power_state != SPI_HID_POWER_MODE_OFF) {
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
	} else
		ret = -ESHUTDOWN;
	mutex_unlock(&shid->power_lock);
	if (ret) {
		dev_err(dev, "failed to send device reset request\n");
		schedule_work(&shid->error_work);
		return;
	}
}

static bool spi_hid_complete_response(struct spi_hid *shid, u8 report_type,
		u8 content_id, u64 generation, bool generation_required)
{
	unsigned long flags;

	spin_lock_irqsave(&shid->response_lock, flags);
	if (!shid->output_pending || shid->response_valid ||
	    shid->expected_response_type != report_type ||
	    shid->expected_response_id != content_id ||
	    (generation_required && shid->response_generation != generation)) {
		spin_unlock_irqrestore(&shid->response_lock, flags);
		return false;
	} else {
		shid->response_valid = true;
		/* A V0 response has no transaction ID. Close the transaction before
		 * waking the caller so a duplicate IRQ cannot overwrite its payload. */
		shid->output_pending = false;
		complete(&shid->output_done);
	}
	spin_unlock_irqrestore(&shid->response_lock, flags);
	return true;
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
	unsigned long flags;
	u64 generation;
	bool response_valid;
	int ret = 0;

	/* The caller holds shid->lock. Drop it before waiting for the response
	 * transaction mutex so another caller cannot block completion by holding
	 * shid->lock while this request is waking from the IRQ thread. */
	mutex_unlock(&shid->lock);
	mutex_lock(&shid->response_mutex);
	mutex_lock(&shid->lock);
	if (!shid->ready) {
		ret = -ENODEV;
		goto out;
	}

	/* A completion is single-use: never let a prior response satisfy this request. */
	spin_lock_irqsave(&shid->response_lock, flags);
	generation = ++shid->response_generation;
	reinit_completion(&shid->output_done);
	shid->expected_response_type = expected_response_type;
	shid->expected_response_id = report->content_id;
	shid->output_pending = true;
	shid->response_valid = false;
	spin_unlock_irqrestore(&shid->response_lock, flags);
	ret = spi_hid_send_output_report(shid, output_register,
			report);
	if (ret) {
		spin_lock_irqsave(&shid->response_lock, flags);
		if (shid->response_generation == generation) {
			shid->output_pending = false;
			shid->response_valid = false;
			shid->response_generation++;
		}
		spin_unlock_irqrestore(&shid->response_lock, flags);
		dev_err(dev, "failed to transfer output report\n");
		goto out;
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
	spin_lock_irqsave(&shid->response_lock, flags);
	response_valid = ret > 0 && shid->response_generation == generation &&
		shid->response_valid;
	if (shid->response_generation == generation) {
		shid->output_pending = false;
		shid->response_valid = false;
		shid->response_generation++;
	}
	spin_unlock_irqrestore(&shid->response_lock, flags);
	if (ret <= 0 || !response_valid) {
		if (ret == 0)
			dev_err(dev, "response timed out\n");
		else if (ret > 0)
			dev_err(dev, "response completed without valid data\n");
		shid->ready = false;
		sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
		schedule_work(&shid->error_work);
		if (ret == 0)
			ret = -ETIMEDOUT;
		else if (ret > 0)
			ret = -EPROTO;
		goto out;
	}
	ret = 0;

out:
	mutex_unlock(&shid->response_mutex);
	return ret;
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

static int spi_hid_create_device(struct spi_hid *shid)
{
	struct hid_device *hid;
	struct device *dev = &shid->spi->dev;
	int ret;

	hid = hid_allocate_device();

	if (!hid) {
		dev_err(dev, "Failed to allocate hid device\n");
		return -ENOMEM;
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

	/* HID callbacks require shid->hid during registration, while the IRQ
	 * sequencer must not feed reports until registration has completed. */
	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    shid->reset_pending || shid->hid || shid->hid_creating) {
		mutex_unlock(&shid->seq_lock);
		hid_destroy_device(hid);
		return -ESHUTDOWN;
	}
	shid->hid = hid;
	shid->hid_creating = true;
	mutex_unlock(&shid->seq_lock);

	ret = hid_add_device(hid);
	/*
	 * hid_add_device() reports whether report-parsing succeeded, not
	 * whether the device was created. The true failure signal is when
	 * hid->driver remains NULL after the call.
	 */
	if (!ret && !hid->driver) {
		dev_warn(dev, "SEQ: hid_add_device succeeded but no driver bound to it\n");
		ret = -ENODEV;
	}
	if (ret) {
		dev_err(dev, "Failed to add hid device: %d\n", ret);
		spi_hid_disconnect_hid(shid);
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

	mutex_lock(&shid->seq_lock);
	shid->hid_creating = false;
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended)) {
		shid->hid = NULL;
		mutex_unlock(&shid->seq_lock);
		hid_destroy_device(hid);
		return -ESHUTDOWN;
	}
	mutex_unlock(&shid->seq_lock);

	return 0;
}

static void spi_hid_create_device_work(struct work_struct *work)
{
	struct spi_hid *shid =
		container_of(work, struct spi_hid, create_device_work);
	struct device *dev = &shid->spi->dev;
	int ret;

	trace_spi_hid_create_device_work(shid);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended))
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
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended))
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
	u8 tx[5];
	struct spi_transfer xf[2];
	struct spi_message msg;
	int ret;

	lockdep_assert_held(&shid->seq_lock);

	tx[0] = 0x0B;
	tx[1] = (reg >> 16) & 0xff;
	tx[2] = (reg >> 8) & 0xff;
	tx[3] = reg & 0xff;
	tx[4] = 0xFF;

	memset(rx, 0, rx_len);
	memset(xf, 0, sizeof(xf));
	xf[0].tx_buf = tx;
	xf[0].len = 5;
	xf[1].rx_buf = rx;
	xf[1].len = rx_len;

	spi_message_init(&msg);
	spi_message_add_tail(&xf[0], &msg);
	spi_message_add_tail(&xf[1], &msg);
	seq_dbg(shid, 2, "read begin reg=0x%06x len=%d state=%s(%d)\n",
		reg, rx_len, spi_hid_seq_state_name(shid->seq_state),
		shid->seq_state);

	ret = spi_sync(shid->spi, &msg);
	seq_dbg(shid, 2, "read complete reg=0x%06x ret=%d\n", reg, ret);
	seq_dbg(shid, 3, "SEQ: read reg=0x%06x len=%d ret=%d raw=[%*ph]\n",
		reg, rx_len, ret, min(rx_len, 16), rx);
	return ret;
}

static int spi_hid_seq_read(struct spi_hid *shid, u8 *rx, int rx_len)
{
	return spi_hid_seq_read_reg(shid, shid->desc.input_register, rx, rx_len);
}

/* Drain and stage a synchronous HID response received by the active
 * sequencer. The five-byte controller preamble is retained in data_buf. */
static void spi_hid_seq_handle_sync_response(struct spi_hid *shid, int type,
		u16 blen)
{
	struct spi_hid_protocol_content content;
	unsigned long flags;
	u64 generation;
	u8 *body = shid->data_buf;
	bool pending;
	u32 read_len;

	lockdep_assert_held(&shid->seq_lock);

	spin_lock_irqsave(&shid->response_lock, flags);
	pending = shid->output_pending && !shid->response_valid &&
		shid->expected_response_type == type;
	generation = shid->response_generation;
	spin_unlock_irqrestore(&shid->response_lock, flags);

	if (blen < SPI_HID_INPUT_BODY_LEN ||
	    blen > sizeof(shid->response.body) + sizeof(shid->response.content)) {
		dev_warn(&shid->spi->dev,
			 "SEQ: invalid synchronous response length %u for type %d\n",
			 blen, type);
		return;
	}

	read_len = blen + 5;
	if (read_len > shid->data_buf_len) {
		dev_warn(&shid->spi->dev,
			 "SEQ: synchronous response exceeds transport buffer (%u > %u)\n",
			 read_len, shid->data_buf_len);
		return;
	}
	if (spi_hid_seq_read(shid, body, read_len)) {
		dev_warn(&shid->spi->dev, "SEQ: synchronous response read failed\n");
		return;
	}
	if (!pending)
		return;

	if (spi_hid_protocol_parse_content(body + 5, blen, &content)) {
		dev_warn(&shid->spi->dev,
			 "SEQ: malformed synchronous response type %d\n", type);
		return;
	}
	memcpy(shid->response.body, body + 5, SPI_HID_INPUT_BODY_LEN);
	memcpy(shid->response.content, content.data, content.data_length);
	if (!spi_hid_complete_response(shid, type, content.content_id,
					generation, true))
		dev_warn(&shid->spi->dev,
			 "SEQ: stale synchronous response type %d ID 0x%x\n",
			 type, content.content_id);
}

static int spi_hid_seq_write_speed(struct spi_hid *shid, const u8 *buf, int len,
				    u8 *rx, int rx_len, u32 speed_hz)
{
	struct spi_transfer xf[2];
	struct spi_message msg;
	int ret;

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

	seq_dbg(shid, 2, "write begin op=0x%02x len=%d rx=%d speed=%u state=%s(%d)\n",
		buf[0], len, rx_len, speed_hz,
		spi_hid_seq_state_name(shid->seq_state), shid->seq_state);

	ret = spi_sync(shid->spi, &msg);
	seq_dbg(shid, 2, "write complete op=0x%02x ret=%d\n", buf[0], ret);
	return ret;
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
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, reason);
	return 0;
}

/* raw_mode handshake watchdog: SET_FEATURE
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

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) || !shid->raw_mode_active ||
	    shid->raw_handshake_confirmed)
		goto out;

	if (shid->raw_handshake_retries_left <= 0) {
		if (shid->raw_probe_attempts < 2) {
			/* First-attempt failure is common after cold boot.
			 * Retry the entire discovery sequence after a 5s delay
			 * instead of immediately falling back to standard HID. */
			shid->raw_probe_attempts++;
			shid->raw_handshake_retries_left = RAW_HANDSHAKE_MAX_RETRIES;
			shid->raw_handshake_confirmed = false;
			dev_warn(dev, "SEQ: raw handshake failed (attempt %u/3), restarting in 5s...\n",
				 shid->raw_probe_attempts + 1);
			cancel_delayed_work(&shid->feat_delay_work);
			shid->feat_delay_pending = false;
			/* Reset state and let the IRQ thread re-discover. */
			spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_RESET, SPI_HID_SEQ_WATCHDOG);
			shid->raw_handshake_wait_feature_defers = 0;
			mutex_unlock(&shid->seq_lock);
			msleep(5000);
			mutex_lock(&shid->seq_lock);
			if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
			    !READ_ONCE(shid->seq_enabled) || !shid->raw_mode_active) {
				mutex_unlock(&shid->seq_lock);
				return;
			}
			/* Restart: D2→D0 power cycle then DESCREQ. */
			if (spi_hid_vendor_init(shid)) {
				mutex_unlock(&shid->seq_lock);
				return;
			}
			if (spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0)) {
				mutex_unlock(&shid->seq_lock);
				return;
			}
			spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_WATCHDOG);
			mutex_unlock(&shid->seq_lock);
			return;
		}
		dev_err(dev, "SEQ: raw_mode handshake failed after %d attempts, falling back to standard HID\n",
			shid->raw_probe_attempts + 1);
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
		if (!shid->hid)
			schedule_work(&shid->create_device_work);
		dev_info(dev, "SEQ: raw handshake failed; using standard HID\n");
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_WATCHDOG);
		shid->raw_handshake_wait_feature_defers = 0;
		goto out;
	}

	/* Don't interrupt an in-progress GET_FEATURE/SET_FEATURE handshake.
	 * Defer up to 3 times (6 seconds total), then force retry. */
	if (shid->seq_state == SPI_HID_SEQ_WAIT_FEATURE) {
		shid->raw_handshake_wait_feature_defers++;
		seq_dbg(shid, 1, "SEQ: watchdog found WAIT_FEATURE active, defer #%u\n",
			shid->raw_handshake_wait_feature_defers);
		if (shid->raw_handshake_wait_feature_defers <= 3) {
			schedule_delayed_work(&shid->raw_handshake_watchdog,
					      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
			goto out;
		}
		shid->raw_handshake_wait_feature_defers = 0;
	}
	shid->raw_handshake_retries_left--;
	dev_warn(dev, "SEQ: raw_mode handshake watchdog: no heatmap data after %dms, retrying (%d left)\n",
		 RAW_HANDSHAKE_TIMEOUT_MS, shid->raw_handshake_retries_left);
	/* Windows-style recovery: SET_POWER(D2→D0) instead of _PS3→_PS0.
	 * The D2→D0 cycle is a "soft" reset that doesn't cut physical power,
	 * matching how Windows recovers from a failed feature handshake.
	 *
	 * Cancel any pending feat_delay_work (otherwise
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
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_WATCHDOG);
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

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) || !shid->feat_delay_pending)
		goto out;
	shid->feat_delay_pending = false;

	if (shid->seq_state != SPI_HID_SEQ_WAIT_RPT) {
		seq_dbg(shid, 1, "SEQ: feat_delay_work: state changed to %d, skipping\n",
			shid->seq_state);
		goto out;
	}
	if (skip_getfeat) {
		seq_dbg(shid, 1, "SEQ: delayed vendor init + SET_FEATURE\n");
		if (spi_hid_seq_write(shid, vendor_init_cmd, sizeof(vendor_init_cmd), NULL, 0))
			goto retry_watchdog;
		usleep_range(36000, 39000);
		if (spi_hid_seq_write(shid, sf_cmd, sizeof(sf_cmd), NULL, 0))
			goto retry_watchdog;
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FEATURE_REQUEST);
		mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
			msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		goto out;
	}

	seq_dbg(shid, 1, "SEQ: raw_mode=1 -> GET_FEATURE after delay, WAIT_FEATURE\n");
	if (spi_hid_seq_write(shid, gf_cmd, sizeof(gf_cmd), NULL, 0)) {
		goto retry_watchdog;
	}
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_FEATURE, SPI_HID_SEQ_FEATURE_REQUEST);
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
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) ||
	    shid->seq_state != SPI_HID_SEQ_WAIT_DESC)
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
		seq_dbg(shid, 1, "SEQ: poll-work: still RESET_RSP, DESCREQ failed\n");
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FALLBACK);
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
		if (!shid->hid)
			schedule_work(&shid->create_device_work);
	} else {
		seq_dbg(shid, 1, "SEQ: poll-work: unexpected type=%d, retrying...\n", type);
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
	}
out:
	mutex_unlock(&shid->seq_lock);
}


/* Heatmap blob detection — raw multi-touch pipeline
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
 * Only raw_mode=0 belongs to the standard safety profile. Raw pipeline
 * controls are load-time-only so an experiment cannot change live input
 * semantics mid-stream. See docs/PARAMETERS.md for the full contract. */

/* ── Operating mode ────────────────────────────────────────────── */
static bool raw_mode;
module_param(raw_mode, bool, 0444);
MODULE_PARM_DESC(raw_mode,
	"0 = standard HID mode (single-touch, Report ID 0x40); "
	"1 = raw DFT heatmap mode (send GET_FEATURE/SET_FEATURE, multi-touch blob detection)");

static bool raw_input_beta;
module_param(raw_input_beta, bool, 0444);
MODULE_PARM_DESC(raw_input_beta,
	"Experimental: publish beta multitouch input from raw CapImg frames (default disabled)");

static bool acpi_probe_power_cycle = false;
module_param(acpi_probe_power_cycle, bool, 0444);
MODULE_PARM_DESC(acpi_probe_power_cycle,
	"Experimental ACPI _PS3->_PS0 power cycle at probe (default disabled)");

/* SET_FEATURE handshake experiments:
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

module_param(sl4a_debug_level, int, 0444);
MODULE_PARM_DESC(sl4a_debug_level, "Log verbosity: 0=errors, 1=transitions, 2=per-frame, 3=full hex");

/* ── Raw-mode handshake timing (EXPERIMENTAL) ───────────────────── */
module_param(getfeat_delay_ms, int, 0444);
MODULE_PARM_DESC(getfeat_delay_ms,
	"Experimental delay in ms between RPT_DESC and GET_FEATURE (default 0; Windows observed about 5900)");

static int stream_watchdog_ms = 0;  /* disabled by default, too aggressive for raw mode */
module_param(stream_watchdog_ms, int, 0444);
MODULE_PARM_DESC(stream_watchdog_ms,
	"Runtime streaming watchdog interval in ms (0=disable, Windows uses 2000)");

static int stream_watchdog_max_retries = 3;
module_param(stream_watchdog_max_retries, int, 0444);
MODULE_PARM_DESC(stream_watchdog_max_retries,
	"Max re-init retries before giving up");

/* ── Runtime recovery ──────────────────────────────────────────── */

static void spi_hid_stream_watchdog_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(to_delayed_work(work), struct spi_hid, stream_watchdog);
	struct device *dev = &shid->spi->dev;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) || !shid->stream_watchdog_active)
		goto out;
	if (shid->seq_state != SPI_HID_SEQ_DONE)
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
			spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_RESET, SPI_HID_SEQ_WATCHDOG);
			shid->seq_enabled = true;
			mshw0231_raw_reset(shid);

			{
			if (spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0)) {
				dev_warn(dev, "SEQ: stream watchdog DESCREQ failed\n");
			} else {
				spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_WATCHDOG);
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
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled))
		goto out;
	if (!shid->poll_active || shid->seq_state != SPI_HID_SEQ_DONE)
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
	if (type == 1 && !shid->hid_creating && (shid->hid ||
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
			rl = shid->data_buf[5] | (shid->data_buf[6] << 8);

			shid->stat_data++;
			seq_dbg(shid, 2, "SEQ: poller cid=0x%02x len=%u\n",
				 shid->data_buf[7], rl);

			if (rl >= 3 && rl - 3 > avail) {
				dev_warn_ratelimited(dev,
					"SEQ: poller DATA report len=%u exceeds buffer (avail=%u), dropped\n",
					rl, avail);
				shid->stat_frames_dropped++;
				goto resched;
			}

			if (shid->raw_mode_active && shid->data_buf[7] == 0x0C &&
			    shid->touch_input) {
				ret = mshw0231_raw_consume_v0(shid, &shid->data_buf[5], rblen - 5);
				if (ret) {
					dev_warn(dev, "SEQ: poller CapImg decode failed: %d (rblen=%u)\n", ret, rblen);
					shid->stat_frames_dropped++;
				}
		} else if (rl > 3 && rl - 3 <= avail) {
			if (shid->hid) {
				int hret = hid_input_report(shid->hid,
					HID_INPUT_REPORT,
					&shid->data_buf[7],
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
	return;
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

	/* Bug fix: this used to compare seq_state's numeric value against
	 * seq_enabled (bool), which collapsed to "not WAIT_DESC" and fired on
	 * every IRQ while parked in any other state — including DONE, the
	 * steady-state loop that handles every touch report. */
	if (shid->seq_state != shid->seq_dbg_last_state) {
		seq_dbg(shid, 1, "SEQ: thread seq_state=%s(%d)\n",
			spi_hid_seq_state_name(shid->seq_state), shid->seq_state);
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
	seq_dbg(shid, 2, "SEQ[state=%s(%d)] type=%d hdr=[%*ph] dt=%lld us%s\n",
		 spi_hid_seq_state_name(shid->seq_state), shid->seq_state,
		 type, 4, &hdr[5], dbg_dt_us,
		 shid->seq_dbg_expect_fast ? (dbg_dt_us >= 0 && dbg_dt_us < 5000 ?
		 " <<< FAST IRQ AFTER DESCREQ: WRITE REACHED DEVICE" :
		 " <<< slow IRQ: DESCREQ ignored (device just re-reset)") : "");
	shid->seq_dbg_expect_fast = false;
	if (type < 0) {
		seq_dbg(shid, 1, "SEQ: no header found\n");
		/* In WAIT_RESET, drain any body data and send DESCREQ anyway */
		if (shid->seq_state == SPI_HID_SEQ_WAIT_RESET) {
			u8 body_drain[64];
			spi_hid_seq_read(shid, body_drain, sizeof(body_drain));
			seq_dbg(shid, 1, "SEQ: body drain done, forcing DESCREQ@0x000001...\n");
			spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0);
			spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_FALLBACK);
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
	if (blen > SZ_8K)
		blen = SZ_8K;

	switch (shid->seq_state) {
	case SPI_HID_SEQ_INVALID:
		dev_warn_ratelimited(dev,
			"SEQ: invalid state, dropping frame\n");
		break;
	case SPI_HID_SEQ_WAIT_RESET:
		seq_handle_reset(shid, type, blen, &shid->seq_dbg_expect_fast);
		break;
	case SPI_HID_SEQ_WAIT_DESC:
		seq_handle_desc(shid, type, blen);
		break;
	case SPI_HID_SEQ_WAIT_RPT:
		seq_handle_rpt(shid, type, blen);
		break;
	case SPI_HID_SEQ_VENDOR_INIT:
		seq_handle_vendor(shid, type, blen);
		break;
	case SPI_HID_SEQ_DONE:
		seq_handle_data(shid, type, blen);
		break;
	case SPI_HID_SEQ_WAIT_FEATURE:
		seq_handle_feat(shid, type, blen);
		break;
	}
out:
	mutex_unlock(&shid->seq_lock);
	return result;
}

/* ── State handler: WAIT_RESET ───────────────────────────────────── */
static void seq_handle_reset(struct spi_hid *shid, int type, u16 blen, bool *expect_fast)
{
	if (type == 3) {
		u8 body[20];

		shid->stat_reset_rsp++;
		if (spi_hid_seq_read(shid, body, sizeof(body)))
			return;
		seq_dbg(shid, 3, "SEQ[WAIT_RESET]: RESET_RSP body-drain=[%*ph], sending DESCREQ\n",
			 20, body);
		if (spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE))
			return;
		seq_dbg(shid, 1, "SEQ[WAIT_RESET]: DESCREQ sent, waiting for DEVICE_DESC IRQ\n");
		*expect_fast = true;
	} else if (type == 7) {
		seq_dbg(shid, 1, "SEQ[WAIT_RESET]: DEVICE_DESC without RESET_RSP, handing to desc handler\n");
		seq_handle_desc(shid, type, blen);
	} else if (type == 8) {
		seq_dbg(shid, 1, "SEQ[WAIT_RESET]: RPT_DESC without RESET_RSP, handing to rpt handler\n");
		seq_handle_rpt(shid, type, blen);
	} else {
		seq_dbg(shid, 1, "SEQ[WAIT_RESET]: unexpected type=%d in WAIT_RESET, forcing DESCREQ\n",
			type);
		{
			u8 body_drain[64];
			spi_hid_seq_read(shid, body_drain, sizeof(body_drain));
		}
		if (spi_hid_seq_write(shid, seq_descreq, sizeof(seq_descreq), NULL, 0)) {
			dev_warn(&shid->spi->dev, "SEQ: fallback DESCREQ write failed\n");
			return;
		}
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_FALLBACK);
	}
}

/* ── State handler: WAIT_DESC ────────────────────────────────────── */
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
			if (spi_hid_validate_dev_desc(&raw, sizeof(raw))) {
				dev_warn(&shid->spi->dev, "SEQ: invalid DEVICE_DESC\n");
				return;
			}
			spi_hid_parse_dev_desc(&raw, &shid->desc);
			shid->reset_pending = false;
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
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_RPT, SPI_HID_SEQ_DEVICE_DESCRIPTOR);
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in WAIT_DESC, sending DESCREQ directly\n");
		if (spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE))
			return;
		seq_dbg(shid, 1, "SEQ: DESCREQ sent synchronously, waiting for next IRQ\n");
	}
}

/* ── State handler: WAIT_RPT ─────────────────────────────────────── */
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
				dev_info(&shid->spi->dev, "SEQ: report descriptor %u bytes read from wire\n", len);
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
					if (spi_hid_seq_write(shid, vendor_init_cmd, sizeof(vendor_init_cmd), NULL, 0)) {
						dev_warn(&shid->spi->dev, "SEQ: vendor init write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					usleep_range(36000, 39000);
					seq_dbg(shid, 1, "SEQ: SET_FEATURE -> DONE\n");
					if (spi_hid_seq_write(shid, sf_cmd, sizeof(sf_cmd), NULL, 0)) {
							dev_warn(&shid->spi->dev, "SEQ: SET_FEATURE write failed\n");
							schedule_delayed_work(&shid->raw_handshake_watchdog,
								msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
							return;
						}
					spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_REPORT_DESCRIPTOR);
				}
			} else {
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
					if (spi_hid_seq_write(shid, vendor_init_cmd, sizeof(vendor_init_cmd), NULL, 0)) {
						dev_warn(&shid->spi->dev, "SEQ: vendor init write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					usleep_range(36000, 39000);
					if (spi_hid_seq_write(shid, gf_cmd, sizeof(gf_cmd), NULL, 0)) {
						dev_warn(&shid->spi->dev, "SEQ: GET_FEATURE write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_FEATURE, SPI_HID_SEQ_FEATURE_REQUEST);
					schedule_delayed_work(&shid->raw_handshake_watchdog,
							      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
				}
			}
		} else {
			spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_REPORT_DESCRIPTOR);
		}
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in WAIT_RPT, sending DESCREQ directly\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	}
}

/* ── State handler: WAIT_FEAT_RESP / WAIT_FEATURE ────────────────── */
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
				ret = spi_hid_seq_write_speed(shid, sf_cmd, sizeof(sf_cmd),
								  NULL, 0, setfeat_speed_hz);
			if (ret) {
				dev_warn(&shid->spi->dev, "SEQ: SET_FEATURE write failed: %d\n", ret);
				return;
			}
		}
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FEATURE_RESPONSE);
	} else if (type == 3) {
		u8 body[16];
		u32 rblen = min_t(u32, blen + 5, sizeof(body));

		shid->stat_reset_rsp++;
		if (rblen && spi_hid_seq_read(shid, body, rblen))
			return;
		seq_dbg(shid, 1, "SEQ: RESET_RSP in WAIT_FEATURE, sending DESCREQ directly\n");
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_RESET_RESPONSE);
	}
}

/* ── State handler: VENDOR_INIT ──────────────────────────────────── */
static void seq_handle_vendor(struct spi_hid *shid, int type, u16 blen)
{
	if (type == 1) {
		seq_dbg(shid, 1, "SEQ: VENDOR_INIT: got DATA! Creating HID device...\n");
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FALLBACK);
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
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FALLBACK);
		shid->ready = true;
		shid->keep_powered = true;
		if (!shid->hid) {
			seq_dbg(shid, 1, "SEQ: creating HID device with hardcoded descriptors...\n");
			schedule_work(&shid->create_device_work);
		}
	}
}

/* ── State handler: DONE ─────────────────────────────────────────── */
static void seq_handle_data(struct spi_hid *shid, int type, u16 blen)
{
	struct device *dev = &shid->spi->dev;

	if (type == SPI_HID_REPORT_TYPE_COMMAND_RESP ||
	    type == SPI_HID_REPORT_TYPE_GET_FEATURE_RESP ||
	    type == SPI_HID_REPORT_TYPE_REPORT_DESC) {
		spi_hid_seq_handle_sync_response(shid, type, blen);
		return;
	}
	if (type == 3) {
		u8 body[20];

		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: Device reset detected in DONE. Re-initializing sequencer...\n");
		if (spi_hid_seq_read(shid, body, sizeof(body)))
			return;
		spi_hid_seq_restart_discovery(shid, SPI_HID_SEQ_DEVICE_RESET);
		return;
	}
	if (type != 1)
		return;
	if (shid->hid_creating ||
	    (!shid->hid && !(shid->raw_mode_active && shid->touch_input)))
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
		seq_dbg(shid, 2, "SEQ: DONE cid=0x%02x len=%u\n", body[7], rl);
		if (rl >= 3 && rl - 3 > avail) {
			dev_warn_ratelimited(dev,
				"SEQ: DATA report len=%u exceeds buffer (avail=%u), dropped\n",
				rl, avail);
			shid->stat_frames_dropped++;
			return;
		}

		if (shid->raw_mode_active && body[7] == 0x0C && shid->touch_input) {
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
			if (raw_input_beta) {
				cret = mshw0231_raw_consume_v0(shid, &body[5], rblen - 5);
				if (cret) {
					dev_warn(dev, "SEQ: CapImg decode failed: %d (rblen=%u)\n", cret, rblen);
					shid->stat_frames_dropped++;
					return;
				}
			}
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

	/* 100% wire-read report descriptor: the PIO TX_COUNT=3 fix
	 * eliminated the old n*64+55 byte corruption.  hid_parse_report()
	 * serves as the only validation — if the wire bytes don't parse,
	 * fall back to the hardcoded copy. */
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

static const struct acpi_device_id spi_hid_acpi_match[] = {
	{ "MSHW0231", 0 },	/* Surface touch controller */
	{},
};

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

	return sysfs_emit(buf, "reset_rsp=%u\ndevice_desc=%u\nrpt_desc=%u\ndata=%u\ngetfeat_resp=%u\nframes_dropped=%u\nirq_count=%u\nwire_patches=%u\n",
		shid->stat_reset_rsp, shid->stat_device_desc, shid->stat_rpt_desc,
		shid->stat_data, shid->stat_getfeat_resp,
		shid->stat_frames_dropped, shid->stat_irq_count,
		shid->stat_wire_patches);
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
		"removing=%u\nsuspended=%u\nseq_enabled=%u\nirq_requested=%u\nirq_enabled=%u\nworks_initialized=%u\n",
		READ_ONCE(shid->removing), READ_ONCE(shid->suspended),
		READ_ONCE(shid->seq_enabled), shid->irq_requested,
		shid->irq_enabled, shid->works_initialized);
}
static DEVICE_ATTR_RO(lifecycle_status);

static const struct attribute *const spi_hid_attributes[] = {
	&dev_attr_ready.attr,
	&dev_attr_bus_error_count.attr,
	&dev_attr_device_initiated_reset_count.attr,
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

static void spi_hid_disable_irq(struct spi_hid *shid)
{
	if (shid->irq_requested && shid->irq_enabled) {
		disable_irq(shid->irq);
		shid->irq_enabled = false;
	}
}

static void spi_hid_free_irq(struct spi_hid *shid)
{
	if (shid->irq_requested) {
		free_irq(shid->irq, shid);
		shid->irq_requested = false;
	}
}

/*
 * Driver probe: called when the SPI device is enumerated via ACPI.
 *
 * Initialization sequence:
 *   1. Parse ACPI resources (SPI bus, GPIO, power regulators)
 *   2. Allocate driver state and initialize locks/work items
 *   3. Discover device via DESCREQ → DEVICE_DESC → RPT_DESC
 *   4. Create HID device via hid_add_device()
 *   5. If raw_mode enabled: activate via vendor-init + SET_FEATURE ID5=01
 *   6. Start sequencer and input polling/IRQ
 *
 * Auto-retries on cold boot handshake failure (up to 3 attempts).
 * Returns 0 on success, negative errno on failure.
 */
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
	mutex_init(&shid->response_mutex);
	spin_lock_init(&shid->input_lock);
	spin_lock_init(&shid->response_lock);
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
	} else {
		/* ACPI _INI has already powered the MSHW0231 before probe. */
		shid->powered = true;
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
	shid->raw_probe_attempts = 0;
	shid->raw_mode_active = raw_mode; /* false → heatmap/raw code dead; infrastructure stays zeroed for mode switch */
	shid->seq_dbg_last_state = SPI_HID_SEQ_INVALID;

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
				dev_err(dev, "SEQ: ACPI _PS3 failed: %s\n",
					acpi_format_exception(status));
				ret = -EIO;
				goto err1;
			}
			seq_dbg(shid, 1, "ACPI _PS3 complete\n");
			msleep(50);
			seq_dbg(shid, 1, "ACPI _PS0 begin\n");
			status = acpi_evaluate_object(h, "_PS0", NULL, NULL);
			if (ACPI_FAILURE(status)) {
				dev_err(dev, "SEQ: ACPI _PS0 failed: %s\n",
					acpi_format_exception(status));
				ret = -EIO;
				goto err1;
			}
			seq_dbg(shid, 1, "ACPI _PS0 complete\n");
			msleep(100);
		}
	}

	shid->seq_enabled = true;
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_RESET, SPI_HID_SEQ_PROBE);
	shid->ready = shid->seq_state >= SPI_HID_SEQ_DONE ? true : false;
	shid->keep_powered = true;

	/* Wait for device to stabilize after ACPI _INI power-on.
	 * _INI is called by the ACPI subsystem before probe() and handles
	 * GPIO power sequencing. The device is already powered and sending
	 * RESET_RSP. Do NOT call _RST/M009/M010 — power cycle kills the
	 * The _RST method calls M010 which destroys the device until cold reboot. */
	seq_dbg(shid, 1, "probe settling delay begin\n");
	msleep(300);
	seq_dbg(shid, 1, "probe settling delay complete\n");
	shid->desc.input_register = 0x000000;

	dev_info(dev, "SEQ: device powered by ACPI _INI, arming IRQ\n");

	/* Create multitouch input device for heatmap-to-touch pipeline.
	 * Bug fix: this was created unconditionally,
	 * even in the default raw_mode=0 configuration where it never receives
	 * a single event (only heatmap_process_frame() feeds it, and that's
	 * only called when raw_mode is set) — exposing a second, permanently
	 * dead "MSHW0231 Touchscreen" input device alongside the real
	 * hid-core one and risking userspace picking the wrong one. Switching
	 * raw_mode 0->1 always goes through a fresh probe (module
	 * reload/rebind), so gating creation on raw_mode here loses no
	 * capability. */
	ret = mshw0231_raw_input_register(shid);
	if (ret)
		goto err1_touch;

	mshw0231_raw_init(shid);
	mshw0231_raw_reset(shid);

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
	shid->irq_requested = true;
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
	goto err1;

err1:
	dev_err(dev, "TRACE[hid] probe failed ret=%d\n", ret);
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_PROBE_FAILED, ret);
	mutex_lock(&shid->seq_lock);
	WRITE_ONCE(shid->removing, true);
	WRITE_ONCE(shid->seq_enabled, false);
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	mutex_unlock(&shid->seq_lock);
	spi_hid_disable_irq(shid);
	spi_hid_cancel_workers(shid);
	spi_hid_free_irq(shid);
	mutex_lock(&shid->power_lock);
	if (spi_hid_power_down(shid))
		dev_warn(dev, "probe cleanup left device powered\n");
	mutex_unlock(&shid->power_lock);
	spi_hid_stop_hid(shid);
	if (shid->touch_input) {
		input_unregister_device(shid->touch_input);
		shid->touch_input = NULL;
	}
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);
	mutex_lock(&shid->seq_lock);
	kfree(shid->heatmap_buf);
	shid->heatmap_buf = NULL;
	shid->heatmap_len = 0;
	shid->heatmap_capacity = 0;
	mutex_unlock(&shid->seq_lock);

err0:
	return ret;
}

static void spi_hid_remove(struct spi_device *spi)
{
	struct spi_hid *shid = spi_get_drvdata(spi);
	struct device *dev = &spi->dev;

	dev_info(dev, "removing driver instance\n");
	seq_dbg(shid, 1, "remove begin irq_enabled=%u ready=%u state=%s(%d)\n",
		shid->irq_enabled, shid->ready,
		spi_hid_seq_state_name(shid->seq_state), shid->seq_state);
	trace_spi_hid_lifecycle(shid, SPI_HID_LIFECYCLE_REMOVE, 0);
	mutex_lock(&shid->seq_lock);
	WRITE_ONCE(shid->removing, true);
	WRITE_ONCE(shid->seq_enabled, false);
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	mutex_unlock(&shid->seq_lock);
	spi_hid_disable_irq(shid);
	spi_hid_cancel_workers(shid);
	spi_hid_free_irq(shid);
	mutex_lock(&shid->power_lock);
	if (spi_hid_power_down(shid))
		dev_warn(dev, "remove left device powered\n");
	mutex_unlock(&shid->power_lock);
	spi_hid_stop_hid(shid);
	if (shid->touch_input) {
		input_unregister_device(shid->touch_input);
		shid->touch_input = NULL;
	}
	sysfs_remove_files(&dev->kobj, spi_hid_attributes);
	mutex_lock(&shid->seq_lock);
	kfree(shid->heatmap_buf);
	shid->heatmap_buf = NULL;
	shid->heatmap_len = 0;
	shid->heatmap_capacity = 0;
	mutex_unlock(&shid->seq_lock);
	dev_info(dev, "TRACE[hid] remove complete\n");
}

static const struct spi_device_id spi_hid_id_table[] = {
	{ "hid", 0 },
	{ "hid-over-spi", 0 },
	{ },
};

static int spi_hid_suspend(struct device *dev)
{
	struct spi_device *spi = to_spi_device(dev);
	struct spi_hid *shid = spi_get_drvdata(spi);

	seq_dbg(shid, 1, "PM: suspend\n");
	mutex_lock(&shid->seq_lock);
	WRITE_ONCE(shid->suspended, true);
	WRITE_ONCE(shid->seq_enabled, false);
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	shid->raw_handshake_confirmed = false;
	shid->feat_delay_pending = false;
	mutex_unlock(&shid->seq_lock);

	spi_hid_disable_irq(shid);
	spi_hid_cancel_workers(shid);
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
	shid->raw_probe_attempts = 0;
	shid->feat_delay_pending = false;
	mshw0231_raw_reset(shid);
	WRITE_ONCE(shid->seq_enabled, true);
	shid->seq_state = SPI_HID_SEQ_WAIT_RESET;
	WRITE_ONCE(shid->suspended, false);
	mutex_unlock(&shid->seq_lock);

	if (shid->irq_requested && !shid->irq_enabled) {
		enable_irq(shid->irq);
		shid->irq_enabled = true;
	}

	if (shid->raw_mode_active) {
		mutex_lock(&shid->seq_lock);
		if (!READ_ONCE(shid->removing) && !READ_ONCE(shid->suspended) &&
		    READ_ONCE(shid->seq_enabled))
			spi_hid_vendor_init(shid);
		mutex_unlock(&shid->seq_lock);
	}

	return 0;
}

static const struct dev_pm_ops spi_hid_pm_ops = {
	.suspend = spi_hid_suspend,
	.resume = spi_hid_resume,
};

static struct spi_driver spi_hid_driver = {
	.driver = {
		.name	= "sl4a_spi_hid",
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
MODULE_AUTHOR("Syax89");
MODULE_LICENSE("GPL");
