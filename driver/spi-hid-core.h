/* SPDX-License-Identifier: GPL-2.0 */
/*
 * spi-hid-core.h
 *
 * Copyright (c) 2020 Microsoft Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */

#ifndef SPI_HID_CORE_H
#define SPI_HID_CORE_H

#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/completion.h>
#include <linux/pinctrl/consumer.h>
#include <linux/spi/spi.h>
#include <linux/spinlock.h>
#include <linux/types.h>

/*
 * spi-hid-dev events which may occur on the event callback function.
 * The event callback function may be called in interupt thread context and
 * should not be blocked or run for a long time, it is adviced that all work
 * resulting from these events are executed on a separate work queue thread.
 * EVENT_NONE is a noop event, can be ignored
 * EVENT_ERROR indicates an error has occurred and the device should be reset.
 *   Client must ensure all fetched input reports are out of scope and reset
 *   the hardware which automatically reinitializes the device and bus.
 * EVENT_RESET indicates that the device has reset, either because an error has
 *   occurred, or as a result of a hard device reset (from init or error event).
 *   Client must ensure all fetched input reports are out of scope and indicate
 *   that it is ready to restart the driver by calling the _restart() function.
 * EVENT_READY indicates that the device is initialized and ready to generate
 *   input reports and receive output reports.
 * EVENT_INPUT_REPORT indicates that at least one unsolicited input report is
 *   available to be fetched by calling the _input_report() function.
 * EVENT_RESPONSE indicates that an output report response is available to be
 *   fetched by calling the _input_report() function. The response event is
 *   functionally equivialent to the input report event to the driver, but
 *   gives the client an opportunity to fetch unsolicited reports on a
 *   different thread to responses, with a higher priority if necessary to
 *   reduce potential input latency.
 * EVENT_WAKEUP indicates that an irq has arrived from a device in a sleep
 *   power state. The client is responsible for handling the irq and set device
 *   into an awake power state.
 */
#define SPI_HID_DEV_EVENT_NONE			0
#define SPI_HID_DEV_EVENT_ERROR			1
#define SPI_HID_DEV_EVENT_RESET			2
#define SPI_HID_DEV_EVENT_READY			3
#define SPI_HID_DEV_EVENT_INPUT_REPORT		4
#define SPI_HID_DEV_EVENT_RESPONSE		5
#define SPI_HID_DEV_EVENT_WAKEUP		6

#define SPI_HID_BUS_STOP			0
#define SPI_HID_BUS_ERROR_SPI_QUEUE		1
#define SPI_HID_BUS_ERROR_SPI_STATUS		2
#define SPI_HID_BUS_ERROR_SYNC_BYTE		3
#define SPI_HID_BUS_ERROR_VERSION		4
#define SPI_HID_BUS_ERROR_BUF_SIZE		5
#define SPI_HID_BUS_ERROR_RESET			6
#define SPI_HID_BUS_ERROR_STOP			7

/* Protocol constants */
#define SPI_HID_READ_APPROVAL_CONSTANT		0xff
#define SPI_HID_INPUT_HEADER_SYNC_BYTE		0x5a

#define SPI_HID_INPUT_HEADER_VERSION		0x02
#define SPI_HID_OUTPUT_HEADER_VERSION		0x02

#define SPI_HID_READ_APPROVAL_OPCODE_READ	0x0b
#define SPI_HID_OUTPUT_HEADER_OPCODE_WRITE	0x02

#define SPI_HID_DEFAULT_INPUT_REGISTER		0x0000
#define SPI_HID_SUPPORTED_VERSION		0x0100

/* Protocol message size constants */
#define SPI_HID_READ_APPROVAL_LEN		5
#define SPI_HID_READ_APPROVAL_MAX		9
#define SPI_HID_INPUT_HEADER_LEN		4
#define SPI_HID_INPUT_BODY_LEN			3

#define SPI_HID_OUTPUT_HEADER_LEN		6
#define SPI_HID_OUTPUT_BODY_LEN			4

/* Protocol message type constants */
#define SPI_HID_REPORT_TYPE_DATA		0x01
#define SPI_HID_REPORT_TYPE_RESET_RESP		0x03
#define SPI_HID_REPORT_TYPE_COMMAND_RESP	0x04
#define SPI_HID_REPORT_TYPE_GET_FEATURE_RESP	0x05
#define SPI_HID_REPORT_TYPE_DEVICE_DESC		0x07
#define SPI_HID_REPORT_TYPE_REPORT_DESC		0x08

#define SPI_HID_CONTENT_TYPE_COMMAND		0x00
#define SPI_HID_CONTENT_TYPE_SET_FEATURE	0x03
#define SPI_HID_CONTENT_TYPE_GET_FEATURE	0x04
#define SPI_HID_CONTENT_TYPE_OUTPUT_REPORT	0x05

#define SPI_HID_COMMAND_SET_POWER		0x01

#define SPI_HID_POWER_SUPPORT_NONE		0x01
#define SPI_HID_POWER_SUPPORT_NO_RESP		0x02
#define SPI_HID_POWER_SUPPORT_RESP		0x03

#define SPI_HID_POWER_MODE_ACTIVE		0x01 /* "Active" - D0 */
#define SPI_HID_POWER_MODE_SLEEP		0x02 /* "Doze" - D2 */
#define SPI_HID_POWER_MODE_OFF			0x03
#define SPI_HID_POWER_MODE_WAKING_SLEEP		0x04 /* "Suspend" - D3/D3* */

#define SPI_HID_HEARTBEAT_REPORT_ID		0xFE
#define SPI_HID_RIGHT_SCREEN_TOUCH_HEAT_MAP_REPORT_ID 0X0A
#define SPI_HID_LEFT_SCREEN_TOUCH_HEAT_MAP_REPORT_ID 0X3C

#define SPI_HID_MAX_LATENCIES			64

#define SPI_HID_INPUT_STAGE_IDLE	0
#define SPI_HID_INPUT_STAGE_BODY	1

/* Heatmap blob detection limits */
#define HEATMAP_MAX_CELLS   4300
#define HEATMAP_MAX_BLOBS   10
#define HEATMAP_MAX_SLOTS   47   /* match Windows DLL blob slot count */

struct spi_hid_device_desc_raw {
	__le16 wDeviceDescLength;
	__le16 bcdVersion;
	__le16 wReportDescLength;
	__le16 wReportDescRegister;
	__le16 wInputRegister;
	__le16 wMaxInputLength;
	__le16 wOutputRegister;
	__le16 wMaxOutputLength;
	__le16 wCommandRegister;
	__le16 wVendorID;
	__le16 wProductID;
	__le16 wVersionID;
	__le16 wFlags;
	__u8 reserved[4];
} __packed;

struct spi_hid_device_descriptor {
	u16 hid_version;
	u16 device_descriptor_register;
	u16 report_descriptor_length;
	u16 report_descriptor_register;
	u16 input_register;
	u16 max_input_length;
	u16 output_register;
	u16 max_output_length;
	u16 command_register;
	u16 vendor_id;
	u16 product_id;
	u16 version_id;
	u8 device_power_support;
	u8 power_response_delay;
};

struct spi_hid_input_buf {
	__u8 header[SPI_HID_INPUT_HEADER_LEN];
	__u8 body[SPI_HID_INPUT_BODY_LEN];
	u8 content[SZ_8K];
};

struct spi_hid_output_buf {
	__u8 header[SPI_HID_OUTPUT_HEADER_LEN];
	__u8 body[SPI_HID_OUTPUT_BODY_LEN];
	u8 content[SZ_8K];
};

struct spi_hid_input_report {
	u8 report_type;
	u16 content_length;
	u8 content_id;
	u8 *content;
};

struct spi_hid_output_report {
	u8 content_type;
	u16 content_length;
	u8 content_id;
	u8 *content;
};

struct spi_hid_input_header {
	u8 version;
	u8 report_type;
	u8 fragment_id;
	u8 length_reserved;
	u16 report_length;
	u8 sync_const;
};

struct spi_hid_input_body {
	u16 content_length;
	u8 content_id;
};

struct latency_instance {
	u8 report_id;
	u16 signature;
	u64 start_time;
	u64 end_time;
};

struct spi_hid {
	struct spi_device	*spi;
	struct hid_device	*hid;

	struct spi_transfer	input_transfer[2];
	struct spi_message	input_message;

	struct spi_hid_device_descriptor desc;
	struct spi_hid_output_buf output;
	struct spi_hid_input_buf input;
	struct spi_hid_input_buf response;

	spinlock_t		input_lock;

	u32 device_descriptor_register;
	u32 input_transfer_pending;
	u32 input_stage;

	u16 hid_desc_addr;
	u8 power_state;
	u8 attempts;

	/*
	* ready flag indicates that the FW is ready to accept commands and requests.
	* The FW becomes ready after sending the report descriptor.
	*/
	bool ready;
	/*
	* refresh_in_progress is set to true while the refresh_device worker thread
	* is destroying and recreating the hidraw device. When this flag is set to
	* true, the ll_close and ll_open functions will not cause power state changes
	*/
	bool refresh_in_progress;

	bool irq_enabled;
	bool removing;
	bool works_initialized;
	int irq;
	struct gpio_desc *gpiod;
	struct delayed_work descreq_work;

	struct regulator *supply;
	struct pinctrl *pinctrl;
	struct pinctrl_state *pinctrl_reset;
	struct pinctrl_state *pinctrl_active;
	struct pinctrl_state *pinctrl_sleep;
	struct work_struct reset_work;
	struct work_struct create_device_work;
	struct work_struct refresh_device_work;
	struct work_struct error_work;

	struct mutex lock;
	/* Serializes sequencer SPI transfers and sequencer-owned state. */
	struct mutex seq_lock;
	struct mutex power_lock;
	struct mutex output_lock;
	struct completion output_done;

	__u8 read_approval[SPI_HID_READ_APPROVAL_MAX];

	u32 report_descriptor_crc32;

	u32 bus_error_count;
	int bus_last_error;

	u32 dir_count;
	u32 powered;
	bool keep_powered;

	/*
	 * Experimental IRQ-driven startup sequencer (mirrors HidSpiCx WDF
	 * automaton: reset-resp -> body read -> ConfiguringDescriptor ->
	 * ack -> descriptor request). Bypasses the blind probe RAW tests.
	 */
	int seq_state;
	bool seq_enabled;

	u64 interrupt_time_stamps[2];
	struct latency_instance latencies[64];
	u8 latency_index;
	u8 perf_mode;
	u16 touch_signature_index;

	/* 2026-07-08: report descriptor as actually read off the wire (§18.3),
	 * populated by spi_hid_seq_thread()'s RPT_DESC handling. Preferred by
	 * spi_hid_ll_parse() over hardcoded_report_descriptor when it parses
	 * successfully — see docs/NEXT_STEPS.md §C. */
	u8 wire_report_descriptor[1024];
	u32 wire_report_descriptor_len;
	bool wire_report_descriptor_rejected;

	/* Heatmap-to-touch pipeline (2026-07-08 §D).
	 * Raw sensor frames (content_id=0x0C, ~4302 bytes) are capacitive
	 * heatmaps — not standard HID input reports. We intercept them before
	 * hid_input_report(), run blob detection, and emit multitouch events
	 * on a dedicated input_dev. */
	struct input_dev *touch_input;
	bool raw_mode_active;
	u8 *heatmap_buf;          /* last captured frame buffer, kmalloc'd */
	u32 heatmap_len;
	u32 heatmap_content_id;
	u16 heatmap_grid_cols;    /* determined from frame analysis */
	u16 heatmap_grid_rows;

	/* Blob detection buffers (was static, now per-device) */
	u8  heatmap_baseline[HEATMAP_MAX_CELLS];
	u32 heatmap_baseline_cells;
	bool heatmap_have_baseline;
	u32 heatmap_baseline_frames;
	u8  heatmap_touched[HEATMAP_MAX_CELLS];
	u8  heatmap_expanded[HEATMAP_MAX_CELLS];
	u8  heatmap_frame_persistence[HEATMAP_MAX_CELLS];
	u16 heatmap_label[HEATMAP_MAX_CELLS];

	/* Blob state (was static in heatmap_process_frame) */
	u16 blob_x[HEATMAP_MAX_BLOBS];
	u16 blob_y[HEATMAP_MAX_BLOBS];
	u32 blob_wsum[HEATMAP_MAX_BLOBS];
	u32 blob_xsum[HEATMAP_MAX_BLOBS];
	u32 blob_ysum[HEATMAP_MAX_BLOBS];
	bool blob_active[HEATMAP_MAX_BLOBS];
	u16 label_equiv[256];

	/* Extended blob tracking (GROUND_TRUTH §22.4):
	 * 47 slots, each with state, duration, coordinate history. */
	u8 blob_slot_state[HEATMAP_MAX_SLOTS];      /* 0=empty 1=new 2=claimed 3=lift */
	u32 blob_slot_duration[HEATMAP_MAX_SLOTS];  /* frames in current state */
	u16 blob_slot_gx[HEATMAP_MAX_SLOTS];        /* last grid X */
	u16 blob_slot_gy[HEATMAP_MAX_SLOTS];        /* last grid Y */
	u32 blob_slot_weight[HEATMAP_MAX_SLOTS];    /* last blob weight */
	u32 blob_slot_missed[HEATMAP_MAX_SLOTS];    /* consecutive frames missed */

	/* Eigenvalue/ellipsis tracking */
	s32 eigmaj[HEATMAP_MAX_SLOTS];
	s32 eigmin[HEATMAP_MAX_SLOTS];
	s32 eigori[HEATMAP_MAX_SLOTS];	/* orientation in fixed-point degrees * 100 */

	s16 c590_lut[256];	/* pre-computed c590 signal lookup table */

	u8 *data_buf;              /* pre-allocated for seq_thread body reads */
	u32 data_buf_len;

	/* raw_mode handshake watchdog (2026-07-08, GROUND_TRUTH.md §18.7).
	 * SET_FEATURE occasionally makes the device go completely silent (no
	 * further IRQ at all, not even a RESET_RSP), so the existing
	 * IRQ-triggered retry in spi_hid_seq_thread() never gets a chance to
	 * run. Decompiling HidSpiCx.sys showed Windows hits the same
	 * intermittent failure and papers over it with a bounded, timer-based
	 * retry (2000ms timeout, 3 retries) — this mirrors that exactly. */
	struct delayed_work raw_handshake_watchdog;
	int raw_handshake_retries_left;
	u8 raw_handshake_state5_defers;
	bool raw_handshake_confirmed;

	struct delayed_work feat_delay_work;	/* GET_FEATURE delay (Windows: ~5900ms) */
	bool feat_delay_pending;

	struct delayed_work stream_watchdog;
	u32 stream_watchdog_data;
	u32 stream_watchdog_misses;
	u32 stream_watchdog_reinits;
	bool stream_watchdog_active;

	struct delayed_work poll_work;		/* active polling work */
	bool poll_active;			/* whether polling loop is running */
	u32 poll_interval_ms;			/* polling interval, default 10 */
	u32 poll_missed;			/* consecutive empty polls */

	u32 stat_reset_rsp;
	u32 stat_device_desc;
	u32 stat_rpt_desc;
	u32 stat_data;
	u32 stat_getfeat_resp;
	u32 stat_frames_dropped;
	u32 stat_irq_count;
	ktime_t seq_dbg_last_irq;
	int seq_dbg_last_state;
	bool seq_dbg_expect_fast;
};

#endif
