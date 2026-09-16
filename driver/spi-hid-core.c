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
#include <linux/version.h>	/* LINUX_VERSION_CODE, for the guarded callbacks */
#include <linux/sched.h>
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
#include "spi-hid-wire-frames.h"
#include "spi-hid-capimg.h"
#include "mshw0231-raw.h"
#include "mshw0231-raw-constants.h"
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
/* Wire format of the host->device sequencer frames; the frames themselves live
 * in driver/spi-hid-wire-frames.h. 0 (the default) sends the Windows-identical
 * single-opcode frame, non-zero restores the legacy doubled-opcode form. */
static bool wire_double_opcode = SPI_HID_WIRE_DOUBLE_DEFAULT;
/* Deprecated alias: 1 asked for the frame without the doubled opcode, which is
 * the default now, so it only matters as an override of wire_double_opcode=1.
 * Kept declared so existing modprobe.d drop-ins keep loading. */
static bool setfeat_no_double;
/* Experimental: never write a feature GET_REPORT on the wire while running
 * standard HID. Issue #4 suspects the connect-time feature query is what keeps
 * the controller from ever starting to stream. */
static bool skip_std_getfeat;
/* Experimental: when the standard-mode startup liveness check sees no input
 * data after DONE, run the existing ACPI recovery instead of only logging. */
static bool std_liveness_recover;
/* Standard-mode backstop: kick discovery when the controller has produced no IRQ
 * at all since power-up or resume (0 = off; see the watchdog comment). */
static int wait_reset_kick_ms;
/* Sync-request timeout: covers the ~3.6 s measured device settle before it
 * answers feature queries (original protocol doc cited ~5.9 s). The old
 * hardcoded 1000 ms timed out on a cold-boot feature query. */
static int sync_timeout_ms = SPI_HID_PROTOCOL_SYNC_TIMEOUT_MS_DEFAULT;
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
#define RAW_HANDSHAKE_TIMEOUT_MS 2000
#define RAW_HANDSHAKE_MAX_RETRIES 3
#define RAW_HANDSHAKE_COLD_BOOT_RETRY_DELAY_MS 5000

/* Standard-mode backstop for a device that says nothing at all after power-up
 * or a D2->D0 transition. Opt-in through wait_reset_kick_ms (0 = off): see the
 * comment on the watchdog itself for why the default is off. */
#define WAIT_RESET_MAX_WRITE_RETRIES 3

/* Defined with the other SET_FEATURE plumbing further down; the skip_getfeat
 * paths above it need it. */
static int spi_hid_seq_write_setfeat(struct spi_hid *shid);

/* Raw-mode Report ID 6 configuration read: the write+read pair used by the
 * paths with no WAIT_FEATURE state to lean on, and the retain/log half shared
 * with the reply handler. Defined with the other SET_FEATURE plumbing. */
static void spi_hid_getfeat6_read(struct spi_hid *shid);
static void spi_hid_getfeat6_retain(struct spi_hid *shid, const u8 *body, u32 body_len);

static void spi_hid_seq_set_state(struct spi_hid *shid,
		enum spi_hid_seq_state new_state, enum spi_hid_seq_reason reason);
static void spi_hid_arm_wait_reset_watchdog(struct spi_hid *shid);

static void spi_hid_seq_set_state(struct spi_hid *shid,
		enum spi_hid_seq_state new_state, enum spi_hid_seq_reason reason)
{
	enum spi_hid_seq_state old_state = shid->seq_state;

	/* Standard mode has no other timer covering a device that never answers
	 * power-up or resume with a RESET_RSP. Armed before the unchanged-state
	 * early return: the ACPI recovery parks in WAIT_RESET, and a power cycle
	 * owes a fresh RESET_RSP even when the state itself does not change. */
	if (new_state == SPI_HID_SEQ_WAIT_RESET)
		spi_hid_arm_wait_reset_watchdog(shid);

	/* Raw mode, every pre-DONE state: a device that never answers the
	 * DESCREQ — or answers every IRQ with RESET_RSP, as the field unit does —
	 * parks in WAIT_DESC where nothing else is armed. descreq_work only
	 * re-reads the input register (the IRQ thread drains each frame first),
	 * and the raw watchdog used to be armed at DONE only, so the stall was
	 * silent and permanent at every debug level. WAIT_RPT has the same shape
	 * (device descriptor received, report descriptor never), and WAIT_RESET
	 * at cold probe is the last one (a controller that never sends a
	 * RESET_RSP at all — the resume path arms this separately because it
	 * assigns the state directly). Armed before the unchanged-state early
	 * return because the RESET_RSP loop re-enters WAIT_DESC with the state
	 * unchanged; schedule_delayed_work() is a no-op on an already-pending
	 * work item, so the timeout starts on first entry. The watchdog's own
	 * WAIT_FEATURE defer keeps a slow-but-live handshake from being
	 * interrupted. */
	if (shid->raw_mode_active && !shid->raw_handshake_confirmed &&
	    (new_state == SPI_HID_SEQ_WAIT_RESET ||
	     new_state == SPI_HID_SEQ_WAIT_DESC || new_state == SPI_HID_SEQ_WAIT_RPT))
		schedule_delayed_work(&shid->raw_handshake_watchdog,
				      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));

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

	/* Safety net: the device's data-ready IRQ is edge-triggered and can be
	 * lost if it fires while we're still inside the SET_FEATURE write path
	 * (mutex held). Without this, a missed edge means no further IRQ ever
	 * arrives and the stream stalls forever with zero data. Arm a periodic
	 * poller alongside the IRQ path as a backstop; whichever one observes
	 * data first confirms the handshake and the other stays a no-op. */
	if (new_state == SPI_HID_SEQ_DONE && shid->raw_mode_active) {
		if (!shid->poll_interval_ms)
			shid->poll_interval_ms = 20;
		shid->poll_active = true;
		schedule_delayed_work(&shid->poll_work, msecs_to_jiffies(shid->poll_interval_ms));

		/* A "successful" SET_FEATURE write does not guarantee the device
		 * actually starts streaming (observed hardware/firmware race: the
		 * SPI transaction ACKs but no data ever follows). Both success
		 * paths that reach DONE skip arming this watchdog entirely, so
		 * without it a silently-inactive activation polls forever with
		 * no automatic recovery. Arm it here unconditionally; it is a
		 * no-op once raw_handshake_confirmed is set. */
		if (!shid->raw_handshake_confirmed)
			schedule_delayed_work(&shid->raw_handshake_watchdog,
					      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
	}
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
	}
	hid = spi_hid_disconnect_hid(shid);
	if (hid)
		hid_destroy_device(hid);
}

static void spi_hid_disable_irq(struct spi_hid *shid);

/* _RST calls M010 which DESTROYS the device. Never call it.
 * ACPI recovery is a real _PS3->_PS0 power cycle (mirrors the
 * acpi_probe_power_cycle probe experiment). The sequencer is re-armed to
 * WAIT_RESET BEFORE the cycle so the device's power-on RESET_RSP lands
 * deterministically in WAIT_RESET and restarts descriptor discovery. If
 * the ACPI evaluation fails, the re-arm stays in place and discovery
 * restarts on the next IRQ instead of leaving ready=false with no re-arm.
 * Caller must NOT hold power_lock: this function takes seq_lock. */
static int spi_hid_reset_via_acpi(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	acpi_handle h = ACPI_HANDLE(dev);
	acpi_status status;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled)) {
		mutex_unlock(&shid->seq_lock);
		return -ESHUTDOWN;
	}
	/* Park the sequencer in WAIT_RESET before the power cycle: every
	 * discovery handler also restarts discovery on a type-3 RESET_RSP,
	 * so even a stray pre-_PS3 IRQ cannot wedge the re-arm. */
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_RESET, SPI_HID_SEQ_DEVICE_RESET);
	mutex_unlock(&shid->seq_lock);

	if (!h) {
		dev_warn(dev, "no ACPI handle, skipping power toggle\n");
		return 0;
	}

	dev_info(dev, "ACPI power cycle _PS3 -> _PS0\n");
	status = acpi_evaluate_object(h, "_PS3", NULL, NULL);
	if (ACPI_FAILURE(status)) {
		dev_warn(dev, "ACPI _PS3 failed: %s, skipping power toggle\n",
			 acpi_format_exception(status));
		return 0;
	}
	msleep(50);
	status = acpi_evaluate_object(h, "_PS0", NULL, NULL);
	if (ACPI_FAILURE(status)) {
		/* _PS3 already powered the part down: report the failure so the
		 * caller keeps power_state OFF (the part is dark), with the
		 * WAIT_RESET re-arm already in place for when it comes back. */
		dev_warn(dev, "ACPI _PS0 failed: %s, part likely unpowered\n",
			 acpi_format_exception(status));
		return -EIO;
	}
	msleep(100);
	return 0;
}

static int spi_hid_error_handler(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	int ret = 0;
	bool terminal_failure = false;
	bool acpi_recovery = false;

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

	/* Recovery needs a live transport. A caller can have parked the
	 * sequencer to stop a storm (the IRQ-storm breaker clears seq_enabled)
	 * and both the ACPI and the pinctrl paths need descriptor discovery to
	 * run again afterwards, so re-enable it here. The terminal-failure
	 * path above is unaffected: it returns with the sequencer off on
	 * purpose. */
	WRITE_ONCE(shid->seq_enabled, true);

	/* ACPI (non-DT) recovery needs seq_lock, so it runs after power_lock
	 * is released below; the two mutexes are never nested. */
	if (!dev->of_node)
		acpi_recovery = true;

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
		shid->power_state = SPI_HID_POWER_MODE_ACTIVE;
	}

out:
	mutex_unlock(&shid->power_lock);
	if (acpi_recovery) {
		ret = spi_hid_reset_via_acpi(shid);
		if (ret && ret != -ESHUTDOWN)
			dev_err(dev, "Reset failed\n");
		/* Either the cycle completed (0) or we bailed before touching
		 * power (-ESHUTDOWN): the part is powered in both cases, so
		 * re-advertise ACTIVE; a racing remove/suspend owns the rest.
		 * Any other error (-EIO: _PS0 failed) means the part is dark:
		 * leave power_state OFF. */
		if (ret == 0 || ret == -ESHUTDOWN) {
			mutex_lock(&shid->power_lock);
			shid->power_state = SPI_HID_POWER_MODE_ACTIVE;
			mutex_unlock(&shid->power_lock);
		}
	}
	if (terminal_failure) {
		/* The worker on the other side takes power_lock. Never wait for it
		 * while holding that lock, otherwise exhausted recovery deadlocks. */
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
			cancel_delayed_work_sync(&shid->raw_probe_retry_work);
			cancel_delayed_work_sync(&shid->feat_delay_work);
			cancel_delayed_work_sync(&shid->stream_watchdog);
			cancel_delayed_work_sync(&shid->wait_reset_watchdog);
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

/* ── Sequencer command frames ──────────────────────────────────────
 *
 * Every host->device frame the sequencer sends is built by
 * driver/spi-hid-wire-frames.h, which holds the Windows bytes as the single
 * source of truth (asserted byte for byte by tests/wire_frames_test.c). The
 * wrappers below pick the Windows form by default and the legacy doubled
 * opcode form when wire_double_opcode=1.
 *
 * All of them are called with seq_lock held, like spi_hid_seq_write(). */

/* Whether command frames use the legacy doubled leading opcode. */
static bool spi_hid_wire_doubled(void)
{
	return wire_double_opcode;
}

/* SET_FEATURE decides separately: the deprecated setfeat_no_double=1 asked for
 * the frame without the doubled opcode, which is the default now, so it can
 * only take effect as an override of wire_double_opcode=1. */
static bool spi_hid_wire_doubled_setfeat(void)
{
	return wire_double_opcode && !setfeat_no_double;
}

/* DESCREQ for the device descriptor (register 0x000001), used by every state
 * transition that needs to re-request it. The report-descriptor DESCREQ
 * (register 0x000002) is built at its one call site in seq_handle_desc(). */
static int spi_hid_seq_write_descreq(struct spi_hid *shid)
{
	shid->read_resp_type = 0;
	shid->read_resp_content_id = 0;
	u8 frame[SPI_HID_WIRE_DESCREQ_MAX];
	unsigned int len = spi_hid_wire_descreq(frame, SPI_HID_WIRE_DESCREQ_DEVICE_REG,
						spi_hid_wire_doubled());

	return spi_hid_seq_write(shid, frame, (int)len, NULL, 0);
}

/* The raw stream: the reference reads it from register 0x0A with content id
 * 0x56 (boot trace #0004-#0873 — 4309 bytes = 5 + 4304 after a nine-byte
 * header read that says 'type 0x1 body 4304'). Both are used by the reads and
 * by the enable below, so they sit here, before either. */
#define SPI_HID_RAW_STREAM_REGISTER 0x0A
#define SPI_HID_RAW_STREAM_CONTENT_ID 0x56

/* SET_FEATURE Report ID 0x56 (vendor init / device key). */
static int spi_hid_seq_write_vendor_init(struct spi_hid *shid)
{
	struct spi_hid_wire_frame frame = spi_hid_wire_vendor_init(spi_hid_wire_doubled());

	/* The reads that follow name this request, like the reference's do. */
	shid->read_resp_type = SPI_HID_CONTENT_TYPE_SET_FEATURE;
	shid->read_resp_content_id = SPI_HID_RAW_STREAM_CONTENT_ID;

	return spi_hid_seq_write(shid, frame.bytes, (int)frame.len, NULL, 0);
}

/* GET_FEATURE Report ID 6 (probe-time calibration read). */
static int spi_hid_seq_write_get_feature6(struct spi_hid *shid)
{
	struct spi_hid_wire_frame frame = spi_hid_wire_get_feature6(spi_hid_wire_doubled());

	shid->read_resp_type = SPI_HID_CONTENT_TYPE_GET_FEATURE;
	shid->read_resp_content_id = SPI_HID_GETFEAT6_REPORT_ID;

	return spi_hid_seq_write(shid, frame.bytes, (int)frame.len, NULL, 0);
}

/* Windows vendor init: SET_POWER (D2→D0) on command_register 0x0004.
 * Sent on every cold boot / D3→D0 transition before DESCREQ; the device
 * streams DATA type=1 immediately afterward (no DESCREQ needed).
 * Wire format from the SPB trace (captures/wintrace/surface_init.csv
 * TXN 634377432 is the D0 frame; D2 is the same with payload byte 0x02):
 *   D2: 02 00 00 04 82 00 00 04 00 01 02 0C EE 5B
 *   D0: 02 00 00 04 82 00 00 04 00 01 01 0C EE 5B
 * Uses seq_write (V2 body magic 0x82), not spi_hid_set_power (which uses the
 * different internal opcode 0x08 encoding). */
static int spi_hid_vendor_init(struct spi_hid *shid)
{
	struct spi_hid_wire_frame d2 = spi_hid_wire_set_power_d2(spi_hid_wire_doubled());
	struct spi_hid_wire_frame d0 = spi_hid_wire_set_power_d0(spi_hid_wire_doubled());
	int ret;

	ret = spi_hid_seq_write(shid, d2.bytes, (int)d2.len, NULL, 0);
	if (ret)
		return ret;
	msleep(100);
	ret = spi_hid_seq_write(shid, d0.bytes, (int)d0.len, NULL, 0);
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
* Abort a synchronous transaction whose reply can no longer arrive (PM is
* quiescing the transport, or the device is being removed). Mirrors
* spi_hid_complete_response(): closing the transaction and waking the waiter
* lets the caller fail fast instead of burning the whole sync timeout on a
* device that cannot answer.
*/
static void spi_hid_abort_pending_sync(struct spi_hid *shid)
{
	unsigned long flags;

	spin_lock_irqsave(&shid->response_lock, flags);
	if (shid->output_pending) {
		shid->output_pending = false;
		shid->response_valid = false;
		shid->response_generation++;
		complete(&shid->output_done);
	}
	spin_unlock_irqrestore(&shid->response_lock, flags);
}

/*
* This function shouldn't be called from the interrupt thread context since it
* waits for completion that gets completed in one of the future runs of the
* interrupt thread.
*/
static int spi_hid_sync_request(struct spi_hid *shid, u16 output_register,
		struct spi_hid_output_report *report, u8 expected_response_type,
		enum spi_hid_sync_kind kind)
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
	/* The read approval has to name the request it reads the response of: the
	 * reference writes that request's content type at offset 6 and its content
	 * id at offset 8 (GET_FEATURE/6, SET_FEATURE/0x56, descriptors 0/0). */
	shid->read_resp_type = report->content_type;
	shid->read_resp_content_id = report->content_id;
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
	/* PM or removal can quiesce the transport between the ready check above
	 * and the wait below, and with the IRQ disabled nothing will complete
	 * this transaction: fail it now instead of waiting out the timeout. */
	if (READ_ONCE(shid->suspended) || READ_ONCE(shid->removing)) {
		spin_lock_irqsave(&shid->response_lock, flags);
		if (shid->response_generation == generation) {
			shid->output_pending = false;
			shid->response_valid = false;
			shid->response_generation++;
		}
		spin_unlock_irqrestore(&shid->response_lock, flags);
		dev_dbg(dev, "request aborted, device is going away\n");
		ret = -ENODEV;
		goto out;
	}
	mutex_unlock(&shid->lock);
	ret = wait_for_completion_interruptible_timeout(&shid->output_done,
			msecs_to_jiffies(sync_timeout_ms));
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
		/* A suspend or removal that raced this request aborted the
		 * transaction: that is not a device failure, and scheduling a
		 * recovery cycle here would fight the PM transition or the
		 * teardown. */
		if (READ_ONCE(shid->suspended) || READ_ONCE(shid->removing)) {
			dev_dbg(dev, "request failed, device is going away\n");
			ret = -ENODEV;
			goto out;
		}
		if (spi_hid_protocol_sync_timeout_fatal(kind)) {
			if (ret == 0)
				dev_err(dev, "response timed out\n");
			else if (ret > 0)
				dev_err(dev, "response completed without valid data\n");
			shid->ready = false;
			sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
			schedule_work(&shid->error_work);
		} else {
			/* A feature-query failure is not fatal: the input stream
			 * is IRQ-driven and independent of feature-query success.
			 * The HID client gets the error and falls back to defaults;
			 * the transport keeps running. */
			dev_warn(dev, "feature query %s, device stays running\n",
				 ret == 0 ? "timed out" : "returned invalid data");
		}
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
	    shid->hid || shid->hid_creating) {
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

/*
 * Install the descriptor set a fallback uses when the device never told us its
 * own. These are the Windows stack's values for MSHW0231 (vendor 045E, product
 * 0C19, report-descriptor register 0x0002, length 936 = the hardcoded report
 * descriptor in hardcoded_rd.h).
 *
 * The version field matters as much as the registers: without it a fallback
 * hands spi_hid_create_device_work() a zeroed descriptor, that function rejects
 * version 0, schedules the ACPI error path, and the panel stays dead — which is
 * exactly what happened in the field when the raw handshake failed and the
 * "fall back to standard HID" path had nothing to create the device with.
 *
 * Called with seq_lock held.
 */
static void spi_hid_use_hardcoded_desc(struct spi_hid *shid)
{
	shid->desc.hid_version = 0x0100;
	shid->desc.report_descriptor_length = 936;
	shid->desc.report_descriptor_register = 0x0002;
	shid->desc.input_register = 0x0000;
	/* 0x2000, not 0x1000: the raw frames are 4309 bytes (5 + 4304) and a
	 * 4096 cap truncates them. The real descriptor reports it this way; the
	 * value is the cap the standard-mode reads use (raw mode uses the
	 * buffer's own size). */
	shid->desc.max_input_length = 0x2000;
	shid->desc.output_register = 0x0003;
	shid->desc.max_output_length = 0x0100;
	shid->desc.command_register = 0x0004;
	shid->desc.vendor_id = 0x045E;
	shid->desc.product_id = 0x0C19;
	shid->desc.version_id = 0x0100;
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

/*
 * Enable the raw stream.
 *
 * The reference sends this SET_FEATURE after the handshake and before it starts
 * reading the stream — boot trace #0531, byte for byte:
 *
 *	02 00 00 03 C2 | 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00
 *	                 |  pad  |  |  |  \_____ payload (7) ____/
 *	                 |  SET_FEATURE
 *	                 |     |  content id 0x56
 *	                 |     register to stream from (0x0A)
 *	                 total content length (10 = 7 + 3)
 *
 * Without it the device does not stream and every read of that register has
 * nothing to answer with, however well formed the read is.
 */
static int spi_hid_raw_enable_stream(struct spi_hid *shid)
{
	/* The plain vendor-init frame is exactly the reference's enable
	 * (02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00), built and
	 * sent by the same code the handshake uses — one frame, one path, so a
	 * fix to either is a fix to both. */
	struct spi_hid_wire_frame frame = spi_hid_wire_vendor_init(0);

	if (!shid->raw_mode_active)
		return 0;

	dev_info(&shid->spi->dev,
		 "SEQ: enabling the raw stream (SET_FEATURE 0x%02x on register 0x%02x)\n",
		 SPI_HID_RAW_STREAM_CONTENT_ID, SPI_HID_RAW_STREAM_REGISTER);

	return spi_hid_seq_write(shid, frame.bytes, (int)frame.len, NULL, 0);
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
			&report, SPI_HID_REPORT_TYPE_GET_FEATURE_RESP,
			SPI_HID_SYNC_FEATURE);
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
	u8 *tx = shid->read_tx_buf;
	u32 tx_len;
	unsigned int n;
	struct spi_transfer xf[2];
	struct spi_message msg;
	int ret;

	lockdep_assert_held(&shid->seq_lock);

	/* The reference read approval, from the traces: nine bytes, the register
	 * at offset 7, the address field (bytes 1..3) zero, and the request
	 * clocked out padded with zeros to the length of the response it asks
	 * for (nine bytes for a four-byte header, 5 + body for a body).
	 *
	 *   tx = 0B 00 00 00 FF 00 00 0R 00 [00 ...]
	 *         ^  ^^^^^^^^^  ^        ^
	 *         |  address=0  |        register
	 *         opcode       placeholder
	 *
	 * The device decodes the register from offset 7. A five-byte frame with
	 * the register in the address field — what this function used to build —
	 * asks for register 0 and is answered with the device's RESET_RSP, which
	 * is why discovery never saw a descriptor and why every read looked like
	 * a reset. */
	if (!tx || !shid->read_tx_len) {
		seq_dbg(shid, 1, "SEQ: read reg=0x%06x without a request buffer\n", reg);
		return -ENOMEM;
	}

	/* The reference names the content id only when it reads a body; a
	 * nine-byte read (a header) carries none, whatever request it answers. */
	n = spi_hid_wire_read_approval_variant(tx, reg, shid->read_resp_type,
					       rx_len > SPI_HID_READ_APPROVAL_LEN ?
					       shid->read_resp_content_id : 0,
					       read_frame_variant);
	/* The request is the frame and nothing more. The trace's `tx_len` for a
	 * body read is the length of the SPB *buffer*, not what the reference
	 * clocks out — its controller segments the transfer (TX_COUNT=3 per
	 * continuation, see docs/AMDSPI_DECOMP.md). Padding the request to the
	 * response length, as this did, clocks hundreds of stray bytes out
	 * before every read: the field bundle went from a RESET_RSP per second
	 * to none at all with the controller and the bus healthy. */
	tx_len = n;
	if (tx_len > shid->read_tx_len)
		tx_len = shid->read_tx_len;

	memset(rx, 0, rx_len);
	memset(xf, 0, sizeof(xf));
	xf[0].tx_buf = tx;
	xf[0].len = tx_len;
	xf[1].rx_buf = rx;
	xf[1].len = rx_len;

	spi_message_init(&msg);
	spi_message_add_tail(&xf[0], &msg);
	spi_message_add_tail(&xf[1], &msg);
	seq_dbg(shid, 2, "read begin reg=0x%06x len=%d state=%s(%d)\n",
		reg, rx_len, spi_hid_seq_state_name(shid->seq_state),
		shid->seq_state);

	ret = spi_sync(shid->spi, &msg);
	if (ret) {
		shid->bus_error_count++;
		shid->bus_last_error = ret;
	}
	seq_dbg(shid, 2, "read complete reg=0x%06x ret=%d\n", reg, ret);
	seq_dbg(shid, 3, "SEQ: read reg=0x%06x len=%d ret=%d raw=[%*ph]\n",
		reg, rx_len, ret, min(rx_len, 16), rx);
	return ret;
}

static int spi_hid_seq_read(struct spi_hid *shid, u8 *rx, int rx_len)
{
	return spi_hid_seq_read_reg(shid, shid->desc.input_register, rx, rx_len);
}

/*
 * Read a request's response.
 *
 * Windows reads it from the output register: in the boot trace it writes the
 * DESCREQ to the descriptor register and then reads the DEVICE_DESC (type 7)
 * and its body from register 3 — a plain read, no interrupt in between — and
 * the report descriptor (type 8) the same way. The device pushes its own
 * events (RESET_RSP) on the input register, which is where this driver read
 * everything. On the field unit that register only ever yields RESET_RSP, so
 * the response was never seen and discovery stayed in WAIT_DESC forever.
 *
 * Both registers are tried, response register first: a device (or a build
 * older than this one) may still answer on the input register, and the caller
 * only sees the first read that succeeded.
 */
static int spi_hid_seq_read_resp(struct spi_hid *shid, u8 *rx, int rx_len)
{
	u32 resp_reg = shid->desc.output_register ? shid->desc.output_register : 0x0003;
	int ret;

	if (resp_reg != shid->desc.input_register) {
		ret = spi_hid_seq_read_reg(shid, resp_reg, rx, rx_len);
		if (!ret) {
			seq_dbg(shid, 2, "SEQ: response read from register 0x%06x\n",
				resp_reg);
			return 0;
		}
		seq_dbg(shid, 2, "SEQ: register 0x%06x read failed (%d), trying the input register\n",
			resp_reg, ret);
	}
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
	bool owned;
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
	/* Re-check ownership right before touching the shared buffer: the
	 * transaction can be closed while the body above is being read (PM
	 * aborts it, another request supersedes it), and a late frame from the
	 * previous generation must not land in the current caller's response. */
	spin_lock_irqsave(&shid->response_lock, flags);
	owned = shid->output_pending && !shid->response_valid &&
		shid->response_generation == generation;
	spin_unlock_irqrestore(&shid->response_lock, flags);
	if (!owned) {
		dev_warn(&shid->spi->dev,
			 "SEQ: stale synchronous response type %d ID 0x%x\n",
			 type, content.content_id);
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
	if (ret) {
		/* bus_error_count/bus_last_error are what the sysfs attribute and
		 * the diagnostic bundle read; without this they could only ever
		 * report 0. */
		shid->bus_error_count++;
		shid->bus_last_error = ret;
	}
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
	shid->read_resp_type = 0;
	shid->read_resp_content_id = 0;
	int ret;

	/* Re-discovery means the touchscreen is not usable until it completes:
	 * leaving `ready` set lets HID clients interleave sync requests with the
	 * sequencer's DESCREQ and steal its responses. */
	if (shid->ready) {
		shid->ready = false;
		sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
	}

	ret = spi_hid_seq_write_descreq(shid);
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

/* Restart raw-mode discovery: D2->D0 power cycle then a fresh DESCREQ.
 * On success, re-arms the watchdog with a margin longer than
 * getfeat_delay_ms so the next feat_delay_work always has time to fire
 * before the watchdog does; on failure, re-arms it at the short timeout
 * so a transient SPI error still gets another attempt instead of leaving
 * the sequencer stuck with no further retry.
 *
 * Caller holds seq_lock and has already confirmed removing/suspended/
 * seq_enabled/raw_mode_active are still valid. */
static void raw_handshake_restart_discovery(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;

	/* Re-discovery: no touchscreen until the descriptor arrives again, and a
	 * client sync request interleaving here would steal sequencer frames. */
	if (shid->ready) {
		shid->ready = false;
		sysfs_notify(&dev->kobj, NULL, "ready");
	}

	if (spi_hid_vendor_init(shid)) {
		dev_warn(dev, "SEQ: raw watchdog vendor recovery failed\n");
		schedule_delayed_work(&shid->raw_handshake_watchdog,
				      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		return;
	}
	if (spi_hid_seq_write_descreq(shid)) {
		dev_warn(dev, "SEQ: raw watchdog DESCREQ retry failed\n");
		schedule_delayed_work(&shid->raw_handshake_watchdog,
				      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		return;
	}
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_WATCHDOG);
	/* mod_, not schedule_: entering WAIT_DESC already armed the same work at
	 * RAW_HANDSHAKE_TIMEOUT_MS, and schedule_delayed_work() on a pending
	 * item is a no-op — which silently turned the intended extra
	 * getfeat_delay_ms + 1000 ms of settling time into nothing. */
	mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
			 msecs_to_jiffies(getfeat_delay_ms + RAW_HANDSHAKE_TIMEOUT_MS + 1000));
}

/* Cold-boot retry continuation: fires RAW_HANDSHAKE_COLD_BOOT_RETRY_DELAY_MS
 * after the watchdog gives up on the first handshake attempt. Runs as its
 * own delayed work instead of an inline msleep() so the watchdog callback
 * never blocks its workqueue for 5 seconds. */
static void spi_hid_raw_probe_retry_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, raw_probe_retry_work.work);

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) || !shid->raw_mode_active ||
	    shid->raw_handshake_confirmed)
		goto out;
	raw_handshake_restart_discovery(shid);
out:
	mutex_unlock(&shid->seq_lock);
}

static void spi_hid_raw_handshake_watchdog(struct work_struct *work)
{
	struct spi_hid *shid = container_of(work, struct spi_hid, raw_handshake_watchdog.work);
	struct device *dev = &shid->spi->dev;
	struct input_dev *stale_input = NULL;

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
			schedule_delayed_work(&shid->raw_probe_retry_work,
					      msecs_to_jiffies(RAW_HANDSHAKE_COLD_BOOT_RETRY_DELAY_MS));
			goto out;
		}
		dev_err(dev, "SEQ: raw_mode handshake failed after %d attempts, falling back to standard HID\n",
			shid->raw_probe_attempts + 1);
		/* 0x0A is the raw stream's register and nothing else: the standard
		 * HID input reports are read from the descriptor's own register.
		 * Leaving the override in place made the standard path read the
		 * wrong register, which is a panel that answers nothing — the
		 * fallback worked before that override existed. */
		shid->desc.input_register = shid->std_input_register ?
			shid->std_input_register : SPI_HID_DEFAULT_INPUT_REGISTER;
		dev_info(dev, "SEQ: standard HID reads register 0x%06x again\n",
			 shid->desc.input_register);
		/* The descriptor is NOT "already acquired" here, whatever the old
		 * comment said: this branch runs because discovery never finished,
		 * so desc is zero and create_device_work() would reject version 0,
		 * schedule the ACPI power cycle and leave the panel dead. Install
		 * the hardcoded set first — then this fallback actually publishes
		 * a standard HID touchscreen. Stop the experimental input path and
		 * instantiate standard HID without requiring a module reload. */
		if (!shid->desc.hid_version)
			spi_hid_use_hardcoded_desc(shid);
		shid->raw_mode_active = false;
		shid->poll_active = false;
		shid->stream_watchdog_active = false;
		shid->feat_delay_pending = false;
		/* Unregistering an input device can sleep, and the IRQ thread and
		 * poller both need seq_lock: hand the device over here and release
		 * it after the lock is dropped. */
		if (shid->touch_input) {
			stale_input = shid->touch_input;
			shid->touch_input = NULL;
		}
		if (!shid->hid)
			schedule_work(&shid->create_device_work);
		/* The re-discovery that cleared `ready` never completes for a silent
		 * device, and this fallback is exactly that case: restoring a
		 * standard HID device with `ready` false would leave its clients
		 * answering -ENODEV forever. */
		if (!shid->ready) {
			shid->ready = true;
			sysfs_notify(&dev->kobj, NULL, "ready");
		}
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
	 * feat_delay_work's settle timer (~3.6 s measured Windows gap; the
	 * original protocol doc cited ~5.9 s) and our 2000ms watchdog timer
	 * race — the watchdog always fires first, creating an infinite reset
	 * loop). */
	cancel_delayed_work(&shid->feat_delay_work);
	shid->feat_delay_pending = false;
	raw_handshake_restart_discovery(shid);
out:
	mutex_unlock(&shid->seq_lock);
	if (stale_input)
		input_unregister_device(stale_input);
}

/* GET_FEATURE delayed work (Windows: ~3.6 s measured gap between RPT_DESC
 * and GET_FEATURE; the original protocol doc cited ~5.9 s).
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
		seq_dbg(shid, 1, "SEQ: delayed vendor init + GET_FEATURE(6) + SET_FEATURE\n");
		if (spi_hid_seq_write_vendor_init(shid))
			goto retry_watchdog;
		usleep_range(36000, 39000);
		/* Same Windows order as the inline path: the Report ID 6
		 * configuration read runs before the heatmap is enabled, and
		 * regardless of skip_getfeat. */
		spi_hid_getfeat6_read(shid);
		if (spi_hid_seq_write_setfeat(shid))
			goto retry_watchdog;
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FEATURE_REQUEST);
		mod_delayed_work(system_wq, &shid->raw_handshake_watchdog,
			msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
		goto out;
	}

	seq_dbg(shid, 1, "SEQ: raw_mode=1 -> vendor init + GET_FEATURE after delay, WAIT_FEATURE\n");
	usleep_range(68000, 72000);
	if (spi_hid_seq_write_vendor_init(shid)) {
		goto retry_watchdog;
	}
	usleep_range(36000, 39000);
	if (spi_hid_seq_write_get_feature6(shid)) {
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
	u32 resp_reg;
	int type, hdr_off;
	int i, got = -1;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) ||
	    (shid->seq_state != SPI_HID_SEQ_WAIT_DESC &&
	     shid->seq_state != SPI_HID_SEQ_WAIT_RPT))
		goto out;

	/* Both registers can carry a frame (see spi_hid_seq_read_resp): the one
	 * a device answers requests on, and the one it pushes its own events on.
	 * This poller used to read the input register only, so a device that
	 * answers on the other stayed in discovery forever. Take the first
	 * register whose read yields a frame header. */
	resp_reg = shid->desc.output_register ? shid->desc.output_register : 0x0003;
	for (i = 0; i < 2 && got < 0; i++) {
		u32 reg = i == 0 ? resp_reg : shid->desc.input_register;

		if (i == 1 && reg == resp_reg)
			break;
		if (spi_hid_seq_read_reg(shid, reg, hdr, sizeof(hdr)))
			continue;
		type = spi_hid_seq_hdr_type(hdr, sizeof(hdr), &hdr_off);
		if (type >= 0 && hdr_off == 5)
			got = i;
	}
	if (got < 0) {
		seq_dbg(shid, 2, "SEQ: poll-work: no frame header on either register, retrying...\n");
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
		goto out;
	}
	seq_dbg(shid, 2, "SEQ: poll-work: type=%d reg=0x%06x raw=[%*ph]\n", type,
		got == 0 ? resp_reg : shid->desc.input_register, 9, hdr);
	if (type == 7) {
		u16 blen = (((hdr[6] >> 4) & 0xF)) | (hdr[7] << 4);

		blen *= 4;
		if (blen > SZ_8K)
			blen = SZ_8K;
		dev_info(&shid->spi->dev, "SEQ: poll-work: GOT DEVICE_DESC (blen=%u), handling it here\n",
			 blen);
		/* The IRQ edge for this frame was lost, so the IRQ thread will never
		 * see it: run the same handler it would have run, otherwise the
		 * poller drops the very descriptor it exists to recover. */
		seq_handle_desc(shid, type, blen);
		/* Keep polling: the reference asks for the report descriptor and
		 * reads it back straight away instead of waiting for an interrupt,
		 * and without this the poller stops here — leaving WAIT_RPT with no
		 * timer at all. */
		if (shid->seq_state == SPI_HID_SEQ_WAIT_RPT)
			schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(20));
	} else if (type == 8) {
		u16 blen = (((hdr[6] >> 4) & 0xF)) | (hdr[7] << 4);

		blen *= 4;
		if (blen > SZ_8K)
			blen = SZ_8K;
		dev_info(&shid->spi->dev, "SEQ: poll-work: GOT RPT_DESC (blen=%u), handling it here\n",
			 blen);
		/* Same handler the IRQ thread would have run for this state. */
		seq_handle_rpt(shid, type, blen);
	} else if (type == 3) {
		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: poll-work: still RESET_RSP, DESCREQ failed\n");
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FALLBACK);
		shid->ready = true;
		dev_warn(&shid->spi->dev, "SEQ: poll-work: DESCREQ failed, using hardcoded fallback descriptors\n");
		/* Parity with the raw fallback: a client waiting on `ready` must be
		 * woken here too (review R15). */
		sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
		/* Hardcode and create device */
		spi_hid_use_hardcoded_desc(shid);
		/* Raw mode suppresses the standard HID device everywhere else (see the
		 * wire-descriptor path); the fallback must not hand userspace a second
		 * publisher while the raw pipeline owns the panel. */
		if (!shid->hid && !shid->raw_mode_active)
			schedule_work(&shid->create_device_work);
	} else {
		seq_dbg(shid, 1, "SEQ: poll-work: unexpected type=%d, retrying...\n", type);
		schedule_delayed_work(&shid->descreq_work, msecs_to_jiffies(100));
	}
out:
	mutex_unlock(&shid->seq_lock);
}


/* Arm (or re-arm) the standard-mode WAIT_RESET kick timer and restart its budget.
 * Must be called with seq_lock held. A no-op unless wait_reset_kick_ms is set:
 * the backstop is opt-in, off by default (issue #4 field experiment). */
static void spi_hid_arm_wait_reset_watchdog(struct spi_hid *shid)
{
	lockdep_assert_held(&shid->seq_lock);
	/* `irq_requested` too: before the line is armed no edge can be counted, so
	 * arming then would let a short interval kick a device that was never given
	 * the chance to signal (probe settles for 300 ms after the state is set).
	 * Probe re-arms once the IRQ is up (review R15). */
	if (wait_reset_kick_ms <= 0 || !shid->works_initialized ||
	    READ_ONCE(shid->suspended) || READ_ONCE(shid->removing) ||
	    !READ_ONCE(shid->irq_requested) || shid->raw_mode_active)
		return;

	shid->wait_reset_kicks = 0;
	shid->wait_reset_irqs = READ_ONCE(shid->stat_irq_edges);
	mod_delayed_work(system_wq, &shid->wait_reset_watchdog,
			 msecs_to_jiffies(wait_reset_kick_ms));
}

/* Opt-in backstop for a device that answers power-up or resume with nothing at
 * all: a RESET_RSP is expected, and without this the sequencer sits in
 * WAIT_RESET with no timer armed, `ready` false and no touchscreen until a
 * module reload or a suspend/resume (the cold-boot shape of issue #4).
 *
 * Two properties keep it safe:
 *  - it only acts when the controller produced ZERO IRQ edges since the timer
 *    was armed, so a frame it is still holding (a slow RESET_RSP, a threaded
 *    handler queued behind seq_lock) is never consumed out from under the IRQ
 *    thread: no read happens here at all;
 *  - the kick is a plain DESCREQ write with no power sequencing, sent once per
 *    entry into WAIT_RESET (the counter bounds retries of a *failed* write).
 *    After it the existing descriptor poller takes over and reads every 100 ms
 *    until the device answers or resets. If a frame arrives after the kick, the
 *    WAIT_DESC path answers the late RESET_RSP with its own DESCREQ: the worst
 *    case is two back-to-back DESCREQs, which no measurement in tree can rule
 *    out, so it is on the field-test list. Standard mode only; raw mode retries
 *    cold boots on its own. Off by default because the safe interval is
 *    measured, not known. */
static void spi_hid_wait_reset_watchdog(struct work_struct *work)
{
	struct spi_hid *shid = container_of(to_delayed_work(work), struct spi_hid,
					    wait_reset_watchdog);
	struct device *dev = &shid->spi->dev;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) || shid->raw_mode_active ||
	    shid->seq_state != SPI_HID_SEQ_WAIT_RESET)
		goto out;

	if (READ_ONCE(shid->stat_irq_edges) != shid->wait_reset_irqs) {
		/* The controller did signal: the IRQ thread owns that frame, so
		 * re-arm around the new edge count without touching the buffer. */
		shid->wait_reset_irqs = shid->stat_irq_edges;
		mod_delayed_work(system_wq, &shid->wait_reset_watchdog,
				 msecs_to_jiffies(wait_reset_kick_ms));
		goto out;
	}

	if (shid->wait_reset_kicks >= WAIT_RESET_MAX_WRITE_RETRIES) {
		dev_warn(dev,
			 "SEQ: DESCREQ write to a silent controller failed %u times, giving up until the next reload or resume\n",
			 shid->wait_reset_kicks);
		goto out;
	}
	shid->wait_reset_kicks++;
	dev_warn(dev, "SEQ: no RESET_RSP and no IRQ at all after %d ms, forcing DESCREQ\n",
		 wait_reset_kick_ms);

	if (spi_hid_seq_write_descreq(shid)) {
		dev_warn(dev, "SEQ: recovery DESCREQ write failed, retrying\n");
		mod_delayed_work(system_wq, &shid->wait_reset_watchdog,
				 msecs_to_jiffies(wait_reset_kick_ms));
		goto out;
	}
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_DESC, SPI_HID_SEQ_FALLBACK);
	seq_dbg(shid, 1, "SEQ: DESCREQ sent without a RESET_RSP, waiting for the descriptor\n");
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
 *   byte[26..]   capacitive node magnitudes (72 columns x 48 rows, row-major,
 *                1 byte each, 72-byte row stride)
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

module_param(sync_timeout_ms, int, 0444);

/* Which shape the read approval has. The traces put the register at offset 7
 * with the address field zero; the field device answers nothing to that and
 * something to the older five-byte shape. A frame that silences a device is
 * not settled by argument: one reload per variant, and the bundle says which
 * one the hardware accepted. */
static int read_frame_variant = SPI_HID_READ_FRAME_REFERENCE;
module_param(read_frame_variant, int, 0444);
MODULE_PARM_DESC(read_frame_variant,
	"Read approval shape: 0=reference (register at offset 7), 1=legacy (5 bytes, register in the address field), 2=both");

MODULE_PARM_DESC(sync_timeout_ms,
	"Timeout in ms for synchronous requests (default 6000: covers the ~3.6 s measured device "
	"settle before feature queries are answered; the original protocol doc cited ~5.9 s)");

/* SET_FEATURE handshake experiments:
 * always writes fine at the driver level but the device silently stops responding
 * afterward most of the time. Two testable hypotheses, toggled independently so results
 * aren't conflated: (1) the write happens too fast/at the wrong SPI clock speed for the
 * touch chip to sample correctly; (2) our seq_write() opcode-doubling quirk doesn't
 * apply the same way to this specific, longer write. */
static uint setfeat_speed_hz;
module_param(setfeat_speed_hz, uint, 0444);
MODULE_PARM_DESC(setfeat_speed_hz,
	"Override SPI clock speed (Hz) for the SET_FEATURE write only; 0 = bus default (33.33MHz)");

/* Wire format of every host->device sequencer frame (SET_POWER, DESCREQ,
 * SET_FEATURE, GET_FEATURE). Default 0 sends exactly what the Windows stack
 * puts on the bus: a single 0x02 opcode and the constant 0C EE 5B trailer.
 * 1 restores the legacy Linux form with the opcode sent twice and a zeroed
 * trailer, kept for A/B experiments. */
module_param(wire_double_opcode, bool, 0444);
MODULE_PARM_DESC(wire_double_opcode,
	"Send the legacy doubled leading opcode (02 02 ..) instead of the "
	"Windows-identical single-opcode frames (default 0)");

/* Deprecated alias, kept so existing modprobe.d drop-ins keep loading:
 * setfeat_no_double=1 requested the SET_FEATURE frame without the doubled
 * opcode, which is the default now. Use wire_double_opcode=1 to restore the old
 * doubled frame; setfeat_no_double=1 then still opts SET_FEATURE back out. */
module_param(setfeat_no_double, bool, 0444);
MODULE_PARM_DESC(setfeat_no_double,
	"Deprecated alias: 1 = send SET_FEATURE without the doubled leading opcode "
	"(now the default; overrides wire_double_opcode=1 for this frame only)");

/* One place for the SET_FEATURE quirks, so every path that writes it honours
 * setfeat_no_double / setfeat_speed_hz / wire_double_opcode. The two
 * skip_getfeat paths used to send the plain frame at bus speed, which voided
 * both switches for exactly the experiments they exist for. Caller holds
 * seq_lock. */
static int spi_hid_seq_write_setfeat(struct spi_hid *shid)
{
	struct spi_hid_wire_frame frame =
		spi_hid_wire_set_feature5(spi_hid_wire_doubled_setfeat());

	shid->read_resp_type = SPI_HID_CONTENT_TYPE_SET_FEATURE;
	shid->read_resp_content_id = 5;	/* the heatmap report this enables */

	return spi_hid_seq_write_speed(shid, frame.bytes, (int)frame.len, NULL, 0,
				       setfeat_speed_hz);
}

/* ── GET_FEATURE Report ID 6 (raw-mode configuration read) ──────────
 *
 * Windows reads report ID 6 between the report descriptor and the SET_FEATURE
 * that enables the heatmap. In the SPB trace the 10-byte frame
 * 02 00 00 03 42 00 04 03 00 06 is answered ~0.5 ms later with a 122-byte
 * content body:
 *
 *   5 pad | u16 total_length (122) | u8 content_id (6) | 119 payload bytes
 *
 * The payload is a block of IEEE-754 binary32 values (in the captured sample a
 * 55-byte header followed by 16 aligned values). The field layout is not
 * mapped, so this release only keeps and logs the reply: nothing here may
 * change device behaviour, and a failed read must never hold up the probe.
 *
 * This belongs to the raw-mode init sequence and therefore runs regardless of
 * skip_getfeat, which only controls whether the standard-mode handshake waits
 * for the reply (the installer's raw profile sets skip_getfeat=Y). */

/* At most three read attempts, 1 ms apart: the device answered after 0.5 ms in
 * the trace, so a handful of tries is enough and the whole step stays bounded
 * at a few milliseconds. */
#define SPI_HID_GETFEAT6_ATTEMPTS 3
#define SPI_HID_GETFEAT6_ATTEMPT_US 1000

/* Retain and log an already-read reply. `body` is the 5-byte controller
 * preamble followed by the content, exactly what spi_hid_seq_read() returns.
 * Caller holds seq_lock. */
static void spi_hid_getfeat6_retain(struct spi_hid *shid, const u8 *body, u32 body_len)
{
	struct spi_hid_protocol_content content;
	u32 off;

	if (body_len <= SPI_HID_GETFEAT6_PREAMBLE_LEN ||
	    spi_hid_protocol_parse_content(body + SPI_HID_GETFEAT6_PREAMBLE_LEN,
					   body_len - SPI_HID_GETFEAT6_PREAMBLE_LEN,
					   &content)) {
		seq_dbg(shid, 1, "SEQ: GET_FEATURE(6) reply not parseable, ignored\n");
		return;
	}
	if (shid->getfeat6.valid)
		seq_dbg(shid, 2, "SEQ: GET_FEATURE(6) reply replaced\n");

	shid->getfeat6.valid = true;
	shid->getfeat6.body_len = body_len;
	shid->getfeat6.total_length = content.total_length;
	shid->getfeat6.content_id = content.content_id;
	shid->getfeat6.payload_len = min_t(u16, content.data_length,
					   SPI_HID_GETFEAT6_PAYLOAD_LEN);
	memset(shid->getfeat6.payload, 0, sizeof(shid->getfeat6.payload));
	if (shid->getfeat6.payload_len)
		memcpy(shid->getfeat6.payload, content.data,
		       shid->getfeat6.payload_len);

	seq_dbg(shid, 2, "SEQ: GET_FEATURE(6) reply kept: id=%u total=%u payload=%u bytes\n",
		content.content_id, content.total_length,
		shid->getfeat6.payload_len);
	for (off = 0; off < shid->getfeat6.payload_len; off += 32) {
		u32 chunk = min_t(u32, 32, shid->getfeat6.payload_len - off);

		seq_dbg(shid, 2, "  GET_FEATURE(6) payload+%u: %*ph\n",
			off, chunk, shid->getfeat6.payload + off);
	}
	for (off = 0; off + 4 <= shid->getfeat6.payload_len;
	     off += 4 * SPI_HID_WIRE_F32_PER_LINE) {
		char row[SPI_HID_WIRE_F32_ROW_LEN];
		u32 words = min_t(u32, SPI_HID_WIRE_F32_PER_LINE,
				  (shid->getfeat6.payload_len - off) / 4);

		spi_hid_wire_fmt_f32_row(shid->getfeat6.payload + off, words * 4,
					 row, sizeof(row));
		seq_dbg(shid, 2, "  GET_FEATURE(6) payload as binary32 [%u..%u]: %s\n",
			off / 4, off / 4 + words - 1, row);
	}
}

/* Write the Report ID 6 frame, then read, retain and log the reply without
 * waiting for an IRQ (the path with no WAIT_FEATURE state to lean on).
 * Best effort and bounded: the probe continues either way. Caller holds
 * seq_lock. */
static void spi_hid_getfeat6_read(struct spi_hid *shid)
{
	struct device *dev = &shid->spi->dev;
	u8 body[SPI_HID_GETFEAT6_READ_LEN];
	unsigned int attempt;

	if (spi_hid_seq_write_get_feature6(shid)) {
		dev_warn(dev, "SEQ: GET_FEATURE(6) write failed, continuing\n");
		return;
	}

	for (attempt = 0; attempt < SPI_HID_GETFEAT6_ATTEMPTS; attempt++) {
		usleep_range(SPI_HID_GETFEAT6_ATTEMPT_US,
			     SPI_HID_GETFEAT6_ATTEMPT_US + 500);
		memset(body, 0, sizeof(body));
		if (spi_hid_seq_read(shid, body, sizeof(body))) {
			dev_warn(dev, "SEQ: GET_FEATURE(6) reply read failed, continuing\n");
			return;
		}
		/* The content ID sits after the pad and the two-byte length: an
		 * answer to anything else means the device has not staged the
		 * reply yet. */
		if (body[SPI_HID_GETFEAT6_PREAMBLE_LEN + 2] == SPI_HID_GETFEAT6_REPORT_ID) {
			spi_hid_getfeat6_retain(shid, body, sizeof(body));
			return;
		}
	}
	seq_dbg(shid, 1, "SEQ: GET_FEATURE(6) no reply after %u attempts, continuing\n",
		SPI_HID_GETFEAT6_ATTEMPTS);
}

module_param(skip_getfeat, bool, 0444);
MODULE_PARM_DESC(skip_getfeat,
	"Skip the standard-mode feature-read handshake (no WAIT_FEATURE state). "
	"The raw-mode Report ID 6 configuration read still runs");

module_param(skip_std_getfeat, bool, 0444);
MODULE_PARM_DESC(skip_std_getfeat,
	"Experimental: answer feature GET_REPORT with -EOPNOTSUPP in standard HID, "
	"so nothing is written to SPI for a feature query (issue #4 A/B switch)");

module_param(std_liveness_recover, bool, 0444);
MODULE_PARM_DESC(std_liveness_recover,
	"Experimental: run the ACPI recovery when the standard-mode startup "
	"liveness check finds no input data after DONE (needs std_liveness_ms)");

module_param(wait_reset_kick_ms, int, 0444);
MODULE_PARM_DESC(wait_reset_kick_ms,
	"Experimental standard-mode backstop in ms (0=disable, the default): when "
	"the controller has produced no IRQ at all since power-up or resume, send "
	"one DESCREQ after this delay (a failed write is retried up to 3 times). "
	"No power sequencing is involved. After the kick the descriptor poller "
	"keeps reading every 100 ms until the device answers or resets, so a dead "
	"controller is polled rather than left silent (issue #4)");

module_param(sl4a_debug_level, int, 0644);
MODULE_PARM_DESC(sl4a_debug_level, "Log verbosity: 0=errors, 1=transitions, 2=per-frame, 3=full hex");

/* ── Raw-mode handshake timing (EXPERIMENTAL) ───────────────────── */
module_param(getfeat_delay_ms, int, 0444);
MODULE_PARM_DESC(getfeat_delay_ms,
	"Experimental delay in ms between RPT_DESC and GET_FEATURE (default 0; "
	"Windows measured ~3.6 s, original protocol doc cited ~5.9 s)");

static int stream_watchdog_ms = 2000;
module_param(stream_watchdog_ms, int, 0444);
MODULE_PARM_DESC(stream_watchdog_ms,
	"Runtime streaming watchdog interval in ms (0=disable, Windows uses 2000)");

static int stream_watchdog_max_retries = 3;
module_param(stream_watchdog_max_retries, int, 0444);
MODULE_PARM_DESC(stream_watchdog_max_retries,
	"Max re-init retries before giving up");

/* ── Standard-mode startup liveness (EXPERIMENTAL, detection only) ── */

/* In standard HID mode the driver reaches DONE and reports ready even when the
 * controller never starts streaming: the input stream is event driven, so a
 * device that is silent because nobody touched the screen looks exactly like a
 * device that never started, and the raw-mode watchdog does not run here.
 * Issue #4 has a cold boot where the controller only started after a
 * suspend/resume, leaving the touchscreen dead and the driver quiet about it.
 *
 * When enabled, this runs the stream watchdog once after DONE in standard mode
 * and reports what it saw. It deliberately does NOT recover: "idle" and "dead"
 * cannot be told apart from the frame counters alone, so a re-init here could
 * power-cycle a perfectly healthy touchscreen. Enable it to measure a device,
 * then read dmesg and protocol_stats after a cold boot. */
static int std_liveness_ms;   /* 0 = off (detection only; see issue #4) */
module_param(std_liveness_ms, int, 0444);
MODULE_PARM_DESC(std_liveness_ms,
	"Experimental standard-mode startup liveness check in ms (0=disable). "
	"Logs, never recovers, until a healthy idle device is proven to emit "
	"a startup burst (see issue #4)");

/* ── Runtime recovery ──────────────────────────────────────────── */

static void spi_hid_stream_watchdog_work(struct work_struct *work)
{
	struct spi_hid *shid = container_of(to_delayed_work(work), struct spi_hid, stream_watchdog);
	struct device *dev = &shid->spi->dev;

	mutex_lock(&shid->seq_lock);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled) || !shid->stream_watchdog_active)
		goto out;
	if (shid->seq_state != SPI_HID_SEQ_DONE) {
		/* Normal while a re-init is in flight (the recovery path parks the
		 * sequencer at WAIT_RESET/WAIT_DESC): keep the timer alive, or this
		 * single tick kills the stall monitor for good, because the arm
		 * sites skip re-arming while the active flag is still set. */
		goto resched;
	}

	if (!shid->raw_mode_active) {
		/* Once per silent episode (not once per boot): recovering on silence
		 * alone would power-cycle an idle-but-healthy touchscreen, and
		 * repeating it on every discovery cycle spent the recovery budget
		 * until the driver shut the device down for good. Observed activity
		 * and resume restore the allowance (the flag starts zeroed at probe). */
		bool silent = shid->stat_irq_count == shid->std_liveness_irqs;
		bool recover = silent && std_liveness_recover &&
			!shid->std_liveness_recovered;

		shid->stream_watchdog_active = false;
		if (!silent) {
			shid->std_liveness_recovered = false;
			dev_info(dev,
				 "SEQ: standard-mode liveness: %u IRQ(s) within %dms of DONE\n",
				 shid->stat_irq_count - shid->std_liveness_irqs,
				 std_liveness_ms);
		} else if (std_liveness_recover) {
			if (!recover)
				dev_info(dev,
					 "SEQ: standard-mode liveness: still no input data, recovery already ran for this device state\n");
			else
				dev_warn(dev,
					 "SEQ: standard-mode liveness: no input data within %dms of DONE, running ACPI recovery (once)\n",
					 std_liveness_ms);
		} else
			dev_warn(dev,
				 "SEQ: standard-mode liveness: no input data within %dms of DONE; the controller may not be streaming (issue #4)\n",
				 std_liveness_ms);

		if (recover) {
			shid->std_liveness_recovered = true;
			schedule_work(&shid->error_work);
		}
		goto out;
	}

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

			/* Same recovery the handshake watchdog runs: D2/D0 vendor init
			 * then a fresh DESCREQ, with the handshake timer re-armed, so a
			 * second silent failure is caught by the normal timeout instead
			 * of waiting for another stream-watchdog window. */
			raw_handshake_restart_discovery(shid);
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
	seq_dbg(shid, 3, "SEQ: poll_work tick (active=%d state=%d confirmed=%d)\n",
		shid->poll_active, shid->seq_state, shid->raw_handshake_confirmed);
	if (READ_ONCE(shid->removing) || READ_ONCE(shid->suspended) ||
	    !READ_ONCE(shid->seq_enabled))
		goto out;
	if (!shid->poll_active || shid->seq_state != SPI_HID_SEQ_DONE)
		goto resched;
	/* Do NOT gate on raw_handshake_confirmed here: that flag is normally set
	 * by the IRQ path, which is exactly what may never fire (lost edge).
	 * This poller must be able to confirm the handshake itself below. */

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

			/* Drop an oversized frame before anything reads it: the IRQ
			 * path validates first too, and a frame this path discards
			 * must not be able to retire the handshake (review R17c). */
			if (rl >= 3 && rl - 3 > avail) {
				dev_warn_ratelimited(dev,
					"SEQ: poller DATA report len=%u exceeds buffer (avail=%u), dropped\n",
					rl, avail);
				shid->stat_frames_dropped++;
				goto resched;
			}

			if (shid->raw_mode_active && !shid->raw_handshake_confirmed &&
			    spi_hid_protocol_raw_confirms_handshake(shid->data_buf[7], rl)) {
				shid->raw_handshake_confirmed = true;
				cancel_delayed_work(&shid->raw_handshake_watchdog);
				cancel_delayed_work(&shid->raw_probe_retry_work);
				shid->raw_probe_attempts = 0;
				shid->stream_watchdog_reinits = 0;
				if (stream_watchdog_ms > 0)
					shid->poll_active = false;
				seq_dbg(shid, 1, "SEQ: raw_mode handshake confirmed by poller (raw frame id=0x%02x)\n",
					shid->data_buf[7]);
			}

			if (shid->raw_mode_active && shid->data_buf[7] == 0x0C &&
			    shid->touch_input) {
				if (stream_watchdog_ms > 0 && !shid->stream_watchdog_active) {
					shid->stream_watchdog_active = true;
					shid->stream_watchdog_data = shid->stat_data;
					shid->stream_watchdog_misses = 0;
					shid->stream_watchdog_reinits = 0;
					schedule_delayed_work(&shid->stream_watchdog,
							      msecs_to_jiffies(stream_watchdog_ms));
				}
				/* Same gate as the IRQ path: raw_input_beta decides
				 * whether CapImg frames are published as input. */
				if (raw_input_beta) {
					ret = mshw0231_raw_consume_v0(shid, &shid->data_buf[5], rblen - 5);
					if (ret) {
						dev_warn(dev, "SEQ: poller CapImg decode failed: %d (rblen=%u)\n", ret, rblen);
						shid->stat_frames_dropped++;
					}
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

/*
 * struct bin_attribute::read() took a non-const attribute until 6.13, when the
 * const read_new() variant appeared and __BIN_ATTR()'s _Generic picked between
 * the two; 6.16 dropped the non-const callback and the kernel builds with
 * -Werror=incompatible-pointer-types, so the old signature is a build failure
 * there rather than a warning. Ubuntu 24.04 LTS (6.8) is still a supported
 * target, so both signatures have to exist. Same reasoning as the .remove
 * guard in spi-amd.c.
 */
#if LINUX_VERSION_CODE < KERNEL_VERSION(6, 13, 0)
#define SPI_HID_BIN_ATTR_PTR struct bin_attribute *attr
#else
#define SPI_HID_BIN_ATTR_PTR const struct bin_attribute *attr
#endif

/*
 * The complete last captured frame: the CapImg cell field, one byte per cell
 * (3456 bytes on MSHW0231, 4056 on MSHW0162), which is what a frame analysis
 * needs — not the 4304-byte wire body, which heatmap_buf never held.
 *
 * heatmap_debug carries the same bytes as hex, but a sysfs show() attribute is
 * limited to one page and the 78x52 panel's cell field does not fit, so its
 * tail is cut. This binary attribute streams the whole buffer instead, in as
 * many reads as the reader asks for.
 *
 * No new state and no new locking: same buffer, same length, same seq_lock as
 * heatmap_debug. Reading it on a driver with no captured frame yields EOF.
 *
 * Size 0: the length is per-device (3456/4056), so there is nothing honest to
 * advertise, and sysfs_kf_bin_read() then leaves the read length alone — the
 * handler's EOF ends the stream.
 */
static ssize_t heatmap_raw_read(struct file *filp, struct kobject *kobj,
				SPI_HID_BIN_ATTR_PTR, char *buf,
				loff_t off, size_t count)
{
	struct spi_hid *shid = dev_get_drvdata(kobj_to_dev(kobj));
	size_t n = 0;

	if (!shid)
		return -ENODEV;

	mutex_lock(&shid->seq_lock);
	if (shid->heatmap_buf && off < shid->heatmap_len) {
		n = min_t(size_t, count, shid->heatmap_len - (u32)off);
		memcpy(buf, shid->heatmap_buf + off, n);
	}
	mutex_unlock(&shid->seq_lock);

	return n;
}
static BIN_ATTR_RO(heatmap_raw, 0);

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
			/* The transport is not making progress. Stop it and hand the
			 * recovery to the error handler: parking the sequencer here
			 * used to be the end of the story (only a reload or a
			 * suspend/resume re-enabled it), with nothing in the log at
			 * the default verbosity. Ready is cleared too so userspace
			 * stops believing the touchscreen works. */
			dev_warn_ratelimited(dev,
				"SEQ: input IRQ storm (%u failed reads in <%u jiffies), parking the sequencer and scheduling recovery\n",
				shid->seq_storm_count, HZ);
			WRITE_ONCE(shid->seq_enabled, false);
			shid->seq_storm_count = 0;
			if (shid->ready) {
				shid->ready = false;
				sysfs_notify(&dev->kobj, NULL, "ready");
			}
			schedule_work(&shid->error_work);
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
			spi_hid_seq_write_descreq(shid);
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
		if (spi_hid_seq_write_descreq(shid)) {
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
		dev_info(&shid->spi->dev, "SEQ: DEVICE_DESC! reading body (%u bytes)...\n", blen);
		if (rblen < 3 || spi_hid_seq_read_resp(shid, body, rblen)) {
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
			/* Report-descriptor DESCREQ: Windows sends
			 * 02 00 00 02 42 00 00 03 00 00, with the register value
			 * coming from the device descriptor. */
			u8 dr2[SPI_HID_WIRE_DESCREQ_MAX];
			unsigned int dr2_len = spi_hid_wire_descreq(dr2,
					shid->desc.report_descriptor_register,
					spi_hid_wire_doubled());

			if (spi_hid_seq_write(shid, dr2, (int)dr2_len, NULL, 0)) {
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
		dev_info(&shid->spi->dev, "SEQ: RPT_DESC! reading body (%u bytes)...\n", blen);
		if (rblen < 3 || spi_hid_seq_read_resp(shid, body, rblen)) {
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
		/* Every flip of `ready` wakes pollers of the attribute (the rest of
		 * the file does; this happy path was the one exception). */
		sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
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
					if (spi_hid_seq_write_vendor_init(shid)) {
						dev_warn(&shid->spi->dev, "SEQ: vendor init write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					usleep_range(36000, 39000);
					/* Windows order: the Report ID 6 configuration read sits
					 * between the report descriptor and the SET_FEATURE that
					 * enables the heatmap. It belongs to the raw-mode init
					 * sequence, so it runs even with skip_getfeat=1, is
					 * best-effort, and never delays the handshake. */
					spi_hid_getfeat6_read(shid);
					seq_dbg(shid, 1, "SEQ: SET_FEATURE -> DONE\n");
					if (spi_hid_seq_write_setfeat(shid)) {
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
					if (spi_hid_seq_write_vendor_init(shid)) {
						dev_warn(&shid->spi->dev, "SEQ: vendor init write failed\n");
						schedule_delayed_work(&shid->raw_handshake_watchdog,
							msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
						return;
					}
					usleep_range(36000, 39000);
					if (spi_hid_seq_write_get_feature6(shid)) {
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
			if (std_liveness_ms > 0) {
				shid->stream_watchdog_data = shid->stat_data;
				/* Liveness is judged on IRQs, not on parsed frames:
				 * stat_data only advances for frames that reach a live
				 * HID client, so a device that did send frames while the
				 * device node was still being created looked silent. */
				shid->std_liveness_irqs = shid->stat_irq_count;
				shid->stream_watchdog_misses = 0;
				shid->stream_watchdog_active = true;
				schedule_delayed_work(&shid->stream_watchdog,
						msecs_to_jiffies(std_liveness_ms));
			}
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

		shid->stat_getfeat_resp++;
		seq_dbg(shid, 1, "SEQ: GET_FEAT_RESP! reading body (%u bytes)...\n", blen);
		/* A failed read is logged and the handshake still completes: the
		 * Report ID 6 payload is diagnostic only, so probing must not be
		 * held up by it. */
		if (rblen >= 3 && !spi_hid_seq_read(shid, body, rblen))
			spi_hid_getfeat6_retain(shid, body, rblen);
		else
			dev_warn(&shid->spi->dev, "SEQ: GET_FEATURE response read failed or was truncated, continuing\n");
		{
			int ret;

			usleep_range(4500, 5500);
			seq_dbg(shid, 1, "SEQ: sending SET_FEATURE speed=%u double=%d no_double=%d\n",
				 setfeat_speed_hz, wire_double_opcode, setfeat_no_double);
			ret = spi_hid_seq_write_setfeat(shid);
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
		sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
		if (!shid->hid && !shid->raw_mode_active)
			schedule_work(&shid->create_device_work);
		seq_handle_data(shid, type, blen);
	} else if (type == 3) {
		shid->stat_reset_rsp++;
		seq_dbg(shid, 1, "SEQ: VENDOR_INIT: got RESET_RSP, vendor init ignored. Hardcoding descriptors...\n");
		spi_hid_use_hardcoded_desc(shid);
		spi_hid_seq_set_state(shid, SPI_HID_SEQ_DONE, SPI_HID_SEQ_FALLBACK);
		shid->ready = true;
		sysfs_notify(&shid->spi->dev.kobj, NULL, "ready");
		if (!shid->hid && !shid->raw_mode_active) {
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

		/* Only the raw stream frame confirms the handshake: confirming on
		 * any frame retired the watchdog/poller while no heatmap data was
		 * flowing, and the driver then sat in raw mode with no input and
		 * no recovery left. */
		if (shid->raw_mode_active && !shid->raw_handshake_confirmed &&
		    spi_hid_protocol_raw_confirms_handshake(body[7], rl)) {
			shid->raw_handshake_confirmed = true;
			cancel_delayed_work(&shid->raw_handshake_watchdog);
			cancel_delayed_work(&shid->raw_probe_retry_work);
			/* Confirmation is progress: hand the retry budgets back so a
			 * later stall is not judged by an old failure. */
			shid->raw_probe_attempts = 0;
			shid->stream_watchdog_reinits = 0;
			/* The IRQ path just proved it delivers, so the 20 ms poll
			 * loop is redundant; keep it while the stream watchdog is
			 * disabled, where the poller is the only backstop left. */
			if (stream_watchdog_ms > 0)
				shid->poll_active = false;
			seq_dbg(shid, 1, "SEQ: raw_mode handshake confirmed (raw frame id=0x%02x)\n",
				body[7]);
		}

		if (shid->raw_mode_active && body[7] == 0x0C && shid->touch_input) {
			int cret;

			if (stream_watchdog_ms > 0 && !shid->stream_watchdog_active) {
				shid->stream_watchdog_active = true;
				shid->stream_watchdog_data = shid->stat_data;
				shid->stream_watchdog_misses = 0;
				shid->stream_watchdog_reinits = 0;
				schedule_delayed_work(&shid->stream_watchdog,
						      msecs_to_jiffies(stream_watchdog_ms));
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
				/* Report 0x40: ID, one TipSwitch byte, then X and Y
				 * as 16-bit little-endian pairs (see
				 * captures/wintrace/mshw0231_report_descriptor.txt). */
				u16 hx = body[9] | (body[10] << 8);
				u16 hy = body[11] | (body[12] << 8);
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

	/* Edge accounting for the WAIT_RESET kick watchdog: a controller that has
	 * signalled at least once is not silent, so the timer must not touch the
	 * input buffer it may still be holding a frame in. */
	shid->stat_irq_edges++;

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

	/* `hid` is published by the HID lifecycle (create/disconnect), not by
	 * `shid->lock`, and the other readers take `seq_lock` rather than this
	 * lock: taking `shid->lock` here only looked like it protected something.
	 * The test is a courtesy check and a stale read only decides the return
	 * code. `level` is ignored on purpose: the transport keeps running across
	 * HID suspend, and the D2/D0 traffic belongs to the sequencer, not to the
	 * HID core. */
	if (!READ_ONCE(shid->hid))
		ret = -ENODEV;

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

	/* Unconditional on purpose: the dangling `else` this used to carry put
	 * the unlock in the success branch only, so a failed parse of the
	 * hardcoded descriptor returned with shid->lock held — a mutex every
	 * later lock taker (IRQ thread, sysfs readers, remove) waits on forever. */
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
		/* Same window as ll_output_report: a cheap re-check that keeps a
		 * client from starting a transfer on a transport we already know is
		 * going away; the flags live under seq_lock, so this does not close
		 * the race, only narrows it. */
		if (READ_ONCE(shid->suspended) || READ_ONCE(shid->removing)) {
			ret = -ENODEV;
			break;
		}
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
		/* Experimental A/B switch (issue #4): with skip_std_getfeat the
		 * standard profile answers feature reads without touching SPI at
		 * all, so the connect-time feature query cannot leave the
		 * controller in a non-streaming state. Feature reports only:
		 * input-report reads are untouched. */
		if (skip_std_getfeat && !shid->raw_mode_active &&
		    rtype == HID_FEATURE_REPORT) {
			ret = -EOPNOTSUPP;
			break;
		}

		ret = spi_hid_get_request(shid, reportnum);
		if (ret) {
			/* Name the report and the client: the cold-boot feature
			 * query in issue #4 has never been attributed to a
			 * specific reader, and the error alone cannot tell a
			 * userspace client from an internal caller. */
			dev_err(dev, "failed to get report %u (type %u) for %s: %d\n",
				reportnum, rtype, current->comm, ret);
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
	/* `ready` is gated before the lock, and `suspended`/`removing` are
	 * published under `seq_lock`: this read narrows the window rather than
	 * closing it. Enough to stop a client blocking in spi_sync against a
	 * controller that was already quiesced when the call arrived; a suspend
	 * landing in the remaining window is bounded by the SPI core's own
	 * failure path. */
	if (READ_ONCE(shid->suspended) || READ_ONCE(shid->removing)) {
		dev_err(dev, "%s called while suspended\n", __func__);
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

/* Device-specific tuning selected by ACPI ID at probe time. The SL4
 * (MSHW0231) config reproduces the original hardcoded constants except for
 * the baseline EMA alpha: it is 7 on both devices because alpha 2 made the
 * baseline converge to raw/6 instead of resting raw (the Windows traces
 * document a 12.5% recovery rate, i.e. alpha 7 — contributed by guskog);
 * SL3 (MSHW0162) additionally uses its native 78x52 = 4056-cell panel. */
static const struct spi_hid_dev_cfg spi_hid_cfg_sl4 = {
	.capimg_raster_samples   = SPI_HID_CAPIMG_RASTER_SAMPLES, /* 3456 */
	.heatmap_baseline_needed = HEATMAP_BASELINE_FRAMES,       /* 30 */
	.heatmap_baseline_alpha  = 7,                             /* 12.5% recovery */
	.grid_cols               = 72,
	.grid_rows               = 48,
};

static const struct spi_hid_dev_cfg spi_hid_cfg_sl3 = {
	.capimg_raster_samples   = 4056, /* SL3 MSHW0162 panel: 78x52 */
	.heatmap_baseline_needed = 33,
	.heatmap_baseline_alpha  = 7,
	.grid_cols               = 78,
	.grid_rows               = 52,
};

static const struct of_device_id spi_hid_of_match[] = {
	{ .compatible = "hid-over-spi" },
	{},
};

static const struct acpi_device_id spi_hid_acpi_match[] = {
	{ "MSHW0231", (kernel_ulong_t)&spi_hid_cfg_sl4 }, /* Surface Laptop 4 (AMD) touch controller */
	{ "MSHW0162", (kernel_ulong_t)&spi_hid_cfg_sl3 }, /* Surface Laptop 3 (AMD) touch controller */
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

	/* What the name says: every RESET_RSP the device sends is, by
	 * definition, a reset the device initiated, and that is what the
	 * sequencer counts. shid->dir_count counted descriptor reads — a
	 * different thing, whose increment was removed long ago — so this
	 * attribute could only ever report 0. */
	return snprintf(buf, PAGE_SIZE, "%u\n", shid->stat_reset_rsp);
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

/* Read-only on purpose: nothing in the driver reads shid->perf_mode, so the
 * writable attribute only let users set a value that changed nothing. Wire it
 * up (or drop it) when something actually consumes it. */
static DEVICE_ATTR_RO(spi_hid_perf_mode);

static ssize_t seq_state_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf, "%d\n", shid->seq_state);
}
static DEVICE_ATTR_RO(seq_state);

static ssize_t protocol_stats_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf, "reset_rsp=%u\ndevice_desc=%u\nrpt_desc=%u\ndata=%u\ngetfeat_resp=%u\nframes_dropped=%u\nirq_count=%u\nwire_patches=%u\npoll_missed=%u\n",
		shid->stat_reset_rsp, shid->stat_device_desc, shid->stat_rpt_desc,
		shid->stat_data, shid->stat_getfeat_resp,
		shid->stat_frames_dropped, shid->stat_irq_count,
		shid->stat_wire_patches,
		shid->poll_missed);
}
static DEVICE_ATTR_RO(protocol_stats);

static ssize_t baseline_status_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	struct spi_hid *shid = dev_get_drvdata(dev);

	return sysfs_emit(buf, "frame_count=%u\nrows=%u\ncols=%u\n",
		shid->heatmap_baseline_frames, shid->heatmap_grid_rows, shid->heatmap_grid_cols);
}
static DEVICE_ATTR_RO(baseline_status);

/* Single source for what bug reports say about the build. The suite checks this
 * string against VERSION: it reported "v1.0" for nine releases (review R17c). */
#define SL4A_DRIVER_VERSION "1.7.0"

static ssize_t build_info_show(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sysfs_emit(buf, "spi-hid v%s\n", SL4A_DRIVER_VERSION);
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
	cancel_delayed_work_sync(&shid->raw_probe_retry_work);
	cancel_delayed_work_sync(&shid->feat_delay_work);
	cancel_delayed_work_sync(&shid->stream_watchdog);
	cancel_delayed_work_sync(&shid->wait_reset_watchdog);
	cancel_work_sync(&shid->create_device_work);
	cancel_work_sync(&shid->error_work);
}

static void spi_hid_disable_irq(struct spi_hid *shid)
{
	bool disable;

	/* The flag decides who owns the line, and it needs to be a real
	 * test-and-set: two racing callers (suspend vs the terminal error path)
	 * could otherwise both disable the line, leaving one enable_irq() for
	 * two disable_irq() calls and an IRQ that stays masked. The IRQ call
	 * itself must stay outside seq_lock: disable_irq() waits for the
	 * handler, and the handler takes that same lock. */
	mutex_lock(&shid->seq_lock);
	disable = shid->irq_requested && shid->irq_enabled;
	if (disable)
		shid->irq_enabled = false;
	mutex_unlock(&shid->seq_lock);

	if (disable)
		disable_irq(shid->irq);
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

	/* A negative delay would be added to the raw handshake timeout and wrap
	 * msecs_to_jiffies() into the far future: the watchdog would stay armed
	 * and never fire. Clamp once here; the parameter is read-only. */
	getfeat_delay_ms = clamp_t(int, getfeat_delay_ms, 0, 10000);

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

	/* Per-device config: ACPI match data carries the right table; anything
	 * else (DT, unknown ACPI id) falls back to the SL4 defaults. */
	shid->cfg = device_get_match_data(dev);
	if (!shid->cfg)
		shid->cfg = &spi_hid_cfg_sl4;
	dev_info(dev, "device config: grid %ux%u, %u capimg samples, baseline %u frames, EMA alpha %u\n",
		 shid->cfg->grid_cols, shid->cfg->grid_rows,
		 shid->cfg->capimg_raster_samples, shid->cfg->heatmap_baseline_needed,
		 shid->cfg->heatmap_baseline_alpha);

	ret = sysfs_create_files(&dev->kobj, spi_hid_attributes);
	if (ret) {
		dev_err(dev, "Unable to create sysfs attributes\n");
		goto err0;
	}

	/* Separate call: sysfs_create_files() takes struct attribute pointers and
	 * cannot carry the binary-frame attribute. err1 removes both. */
	ret = sysfs_create_bin_file(&dev->kobj, &bin_attr_heatmap_raw);
	if (ret) {
		dev_err(dev, "Unable to create the heatmap_raw attribute\n");
		goto err1;
	}

	ret = spi_hid_get_descriptor_reg(dev, &shid->device_descriptor_register);
	if (ret) {
		dev_err(dev, "failed to get HID descriptor register address\n");
		ret = -ENODEV;
		goto err1;
	}
	dev_info(dev, "HID desc reg = 0x%08x\n", shid->device_descriptor_register);
	/* The DESCREQ that starts discovery is built from the compile-time
	 * constant, so a device whose _DSM names another register can never
	 * answer with a DEVICE_DESC. No capture and nothing in the DLL analysis
	 * has ever seen a different value, so this is a warning, not a
	 * behaviour change: if a machine does report one, discovery failing
	 * silently is the one thing it must not do. */
	if (shid->device_descriptor_register != SPI_HID_WIRE_DESCREQ_DEVICE_REG)
		dev_warn(dev, "SEQ: _DSM reports device-descriptor register 0x%08x while the DESCREQ targets 0x%08x; discovery cannot work until the two agree\n",
			 shid->device_descriptor_register,
			 SPI_HID_WIRE_DESCREQ_DEVICE_REG);
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
		/* ACPI _INI has already powered the MSHW0231 before probe. On an
		 * OF/DT platform nothing enables the regulator and `powered` stays
		 * false, so the power-down path below early-returns and
		 * regulator_disable() is never reached: unqualified by design for
		 * now, as these parts are ACPI-only (review R1d-F6). */
		shid->powered = true;
	}


	INIT_WORK(&shid->create_device_work, spi_hid_create_device_work);
	INIT_WORK(&shid->error_work, spi_hid_error_work);
	INIT_DELAYED_WORK(&shid->descreq_work, spi_hid_seq_descreq_work);
	INIT_DELAYED_WORK(&shid->raw_handshake_watchdog, spi_hid_raw_handshake_watchdog);
	INIT_DELAYED_WORK(&shid->raw_probe_retry_work, spi_hid_raw_probe_retry_work);
	INIT_DELAYED_WORK(&shid->feat_delay_work, spi_hid_feat_delay_work);
	shid->raw_handshake_confirmed = false;
	shid->raw_handshake_retries_left = RAW_HANDSHAKE_MAX_RETRIES;
	shid->raw_probe_attempts = 0;
	shid->raw_mode_active = raw_mode; /* false → heatmap/raw code dead; infrastructure stays zeroed for mode switch */
	shid->seq_dbg_last_state = SPI_HID_SEQ_INVALID;

	INIT_DELAYED_WORK(&shid->stream_watchdog, spi_hid_stream_watchdog_work);
	shid->stream_watchdog_active = false;

	INIT_DELAYED_WORK(&shid->poll_work, spi_hid_poll_work);
	INIT_DELAYED_WORK(&shid->wait_reset_watchdog, spi_hid_wait_reset_watchdog);
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

	mutex_lock(&shid->seq_lock);
	shid->seq_enabled = true;
	spi_hid_seq_set_state(shid, SPI_HID_SEQ_WAIT_RESET, SPI_HID_SEQ_PROBE);
	shid->ready = shid->seq_state >= SPI_HID_SEQ_DONE ? true : false;
	mutex_unlock(&shid->seq_lock);

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

	/* The stream register is not in the device descriptor — the 32 bytes of
	 * the real one (boot trace #0004) do not contain it — while the trace
	 * reads every stream frame from 0x0A. The raw path is this device's, so
	 * it sets it here; if the descriptor said something else, say so: that
	 * difference is worth seeing in a bundle. */
	shid->std_input_register = shid->desc.input_register;
	if (shid->desc.input_register && shid->desc.input_register != SPI_HID_RAW_STREAM_REGISTER)
		dev_info(dev, "SEQ: stream register 0x%06x in the descriptor, 0x%02x in the reference\n",
			 shid->desc.input_register, SPI_HID_RAW_STREAM_REGISTER);
	shid->desc.input_register = SPI_HID_RAW_STREAM_REGISTER;
	spi_hid_raw_enable_stream(shid);

	mshw0231_raw_init(shid);
	mshw0231_raw_reset(shid);

	shid->data_buf = devm_kmalloc(dev, 8200, GFP_KERNEL);
	shid->data_buf_len = 8200;
	if (!shid->data_buf) {
		ret = -ENOMEM;
		goto err1;
	}

	/* Request buffer for spi_hid_seq_read_reg(): the read approval is clocked
	 * out padded to the length of the response (the reference does the same),
	 * so it is zeroed once and only its first nine bytes are rewritten. */
	shid->read_tx_buf = devm_kmalloc(dev, 8200, GFP_KERNEL);
	shid->read_tx_len = 8200;
	if (!shid->read_tx_buf) {
		ret = -ENOMEM;
		goto err1;
	}
	memset(shid->read_tx_buf, 0, shid->read_tx_len);

	seq_dbg(shid, 1, "request IRQ begin flags=0x%lx\n", irqflags);
	ret = request_threaded_irq(shid->irq, spi_hid_dev_irq, spi_hid_seq_thread,
				   irqflags, dev_name(&spi->dev), shid);
	if (ret) {
		dev_err(dev, "TRACE[hid] request IRQ failed: %d\n", ret);
		goto err1;
	}
	shid->irq_requested = true;
	shid->irq_enabled = true;
	/* Only now can an edge be counted, so only now does the backstop's "no IRQ
	 * at all" clock mean anything (issue #4). */
	mutex_lock(&shid->seq_lock);
	spi_hid_arm_wait_reset_watchdog(shid);
	mutex_unlock(&shid->seq_lock);
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
	sysfs_remove_bin_file(&dev->kobj, &bin_attr_heatmap_raw);
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
	/* ready is the only gate the feature/descriptor reads use: leaving it set
	 * let a HID client issue SPI traffic against a device being removed and
	 * wait out the whole sync timeout. */
	shid->ready = false;
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	mutex_unlock(&shid->seq_lock);
	spi_hid_disable_irq(shid);
	spi_hid_cancel_workers(shid);
	/* Nothing will answer a synchronous request now. */
	spi_hid_abort_pending_sync(shid);
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
	sysfs_remove_bin_file(&dev->kobj, &bin_attr_heatmap_raw);
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
	/* ready must follow the transport down: it is the only gate feature and
	 * descriptor reads use, so leaving it set let a HID client issue SPI
	 * traffic against a suspended controller and wait out the sync timeout.
	 * Notified like every other flip of this bit: a client blocked on the
	 * attribute needs the wakeup to re-read it. */
	if (shid->ready) {
		shid->ready = false;
		sysfs_notify(&dev->kobj, NULL, "ready");
	}
	shid->poll_active = false;
	shid->stream_watchdog_active = false;
	shid->raw_handshake_confirmed = false;
	shid->feat_delay_pending = false;
	mutex_unlock(&shid->seq_lock);

	spi_hid_disable_irq(shid);
	spi_hid_cancel_workers(shid);
	/* No reply can arrive now: release a synchronous caller immediately
	 * instead of letting it sit out the full sync timeout. */
	spi_hid_abort_pending_sync(shid);
	return 0;
}

static int spi_hid_resume(struct device *dev)
{
	struct spi_device *spi = to_spi_device(dev);
	struct spi_hid *shid = spi_get_drvdata(spi);

	seq_dbg(shid, 1, "PM: resume\n");
	mutex_lock(&shid->seq_lock);
	if (shid->ready) {
		shid->ready = false;
		sysfs_notify(&dev->kobj, NULL, "ready");
	}
	shid->raw_handshake_confirmed = false;
	shid->raw_handshake_retries_left = RAW_HANDSHAKE_MAX_RETRIES;
	shid->raw_probe_attempts = 0;
	shid->feat_delay_pending = false;
	shid->std_liveness_recovered = false;
	mshw0231_raw_reset(shid);
	WRITE_ONCE(shid->seq_enabled, true);
	shid->seq_state = SPI_HID_SEQ_WAIT_RESET;
	WRITE_ONCE(shid->suspended, false);
	/* Standard mode: the device now owes us a RESET_RSP and nothing else is
	 * armed if it stays quiet. */
	spi_hid_arm_wait_reset_watchdog(shid);
	/* Raw mode: that arm returns immediately there, so a controller coming
	 * back from resume without a RESET_RSP had no timer at all and the panel
	 * stayed dead until the next suspend/resume. The raw watchdog is a no-op
	 * once the handshake is confirmed, so arming it here is safe. */
	if (shid->raw_mode_active && !shid->raw_handshake_confirmed)
		schedule_delayed_work(&shid->raw_handshake_watchdog,
				      msecs_to_jiffies(RAW_HANDSHAKE_TIMEOUT_MS));
	mutex_unlock(&shid->seq_lock);

	/* Vendor init before the IRQ is re-enabled: it writes the D2/D0 pair and
	 * sleeps between the frames, and an IRQ processed in between would let
	 * the sequencer advance mid-init (a failed init also went unnoticed). */
	if (shid->raw_mode_active) {
		int vret = 0;

		mutex_lock(&shid->seq_lock);
		if (!READ_ONCE(shid->removing) && !READ_ONCE(shid->suspended) &&
		    READ_ONCE(shid->seq_enabled))
			vret = spi_hid_vendor_init(shid);
		mutex_unlock(&shid->seq_lock);
		if (vret)
			dev_warn(&spi->dev, "SEQ: resume vendor init failed: %d\n",
				 vret);
	}

	{
		bool arm;

		mutex_lock(&shid->seq_lock);
		arm = shid->irq_requested && !shid->irq_enabled;
		if (arm)
			shid->irq_enabled = true;
		mutex_unlock(&shid->seq_lock);
		if (arm)
			enable_irq(shid->irq);
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
