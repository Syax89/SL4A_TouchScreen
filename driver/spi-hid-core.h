/* SPDX-License-Identifier: GPL-2.0 */

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
#define HEATMAP_MAX_BLOBS   20
#define HEATMAP_MAX_SLOTS   47   /* match Windows DLL blob slot count */

/* Eight fixed-size V0 bodies keep capture bounded to roughly 34 KiB. */
#define SPI_HID_RAW_CAPTURE_SLOTS 8
#define SPI_HID_RAW_CAPTURE_BODY_LENGTH 4304
#define SPI_HID_ISOLATED_SET_RING_SLOTS 8
#define SPI_HID_ISOLATED_SET_BODY_LENGTH 512

enum spi_hid_isolated_set_state {
	SPI_HID_ISOLATED_SET_DISABLED,
	SPI_HID_ISOLATED_SET_WAIT_GET,
	SPI_HID_ISOLATED_SET_WAIT_SET,
	SPI_HID_ISOLATED_SET_OBSERVING,
	SPI_HID_ISOLATED_SET_COMPLETE,
	SPI_HID_ISOLATED_SET_RESET,
	SPI_HID_ISOLATED_SET_FAILED,
};

struct spi_hid_isolated_set_frame {
	__u64 timestamp_ns;
	__u16 transport_length;
	__u16 body_length;
	__u8 type;
	__u8 truncated;
	__u8 header[SPI_HID_INPUT_HEADER_LEN];
	__u8 body[SPI_HID_ISOLATED_SET_BODY_LENGTH];
} __packed;

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
	__le32 wFlags;
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
	struct spi_device	*spi;         /* SPI bus controller device */
	struct hid_device	*hid;         /* HID subsystem device handle */

	struct spi_transfer	input_transfer[2];  /* 2-phase SPI transfer descriptors */
	struct spi_message	input_message;      /* SPI message aggregating transfers */

	struct spi_hid_device_descriptor desc;    /* Parsed hardware descriptor */
	struct spi_hid_output_buf output;         /* Output report buffer (host→device) */
	struct spi_hid_input_buf input;           /* Main input buffer (device→host) */
	struct spi_hid_input_buf response;        /* Output response buffer */

	spinlock_t		input_lock;   /* Protects input transfer and buffer access */

	u32 device_descriptor_register;    /* Register address for device descriptor */
	u32 input_transfer_pending;        /* Non-zero when async input transfer active */
	u32 input_stage;                   /* 0=IDLE (header), 1=BODY (payload) */

	u16 hid_desc_addr;                 /* HID descriptor register address from ACPI */
	u8 power_state;                    /* D0 (1 active), D2 (2 doze), D3 (3 off) */
	u8 attempts;                       /* Probe retry counter */

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

	bool irq_enabled;           /* Whether the GPIO interrupt is enabled */
	bool removing;              /* Driver removal in progress */
	bool works_initialized;      /* Work items have been initialized */
	int irq;                    /* GPIO interrupt line number */
	struct gpio_desc *gpiod;    /* GPIO descriptor for device interrupt */
	struct delayed_work descreq_work; /* DESCREQ retry work */

	struct regulator *supply;    /* Power supply regulator */
	struct pinctrl *pinctrl;     /* Pin control state container */
	struct pinctrl_state *pinctrl_reset;   /* Reset pin state */
	struct pinctrl_state *pinctrl_active;  /* Active (default) pin state */
	struct pinctrl_state *pinctrl_sleep;   /* Sleep pin state */
	struct work_struct reset_work;          /* Software reset work */
	struct work_struct create_device_work;  /* HID device creation work */
	struct work_struct refresh_device_work; /* HID device refresh work */
	struct work_struct error_work;          /* Error handling work */

	struct mutex lock;            /* Top-level driver state mutex */
	/* Serializes sequencer SPI transfers and sequencer-owned state. */
	struct mutex seq_lock;
	struct mutex power_lock;      /* Serializes power state transitions */
	struct mutex output_lock;     /* Serializes output report access */
	struct mutex raw_capture_lock; /* Serializes raw capture buffer access */
	struct completion output_done; /* Signaled when output report completes */
	u8 expected_response_type;    /* Expected response report type */
	u8 expected_response_id;      /* Expected response content ID */
	bool output_pending;          /* Output report sent, awaiting response */

	__u8 read_approval[SPI_HID_READ_APPROVAL_MAX]; /* Read-approval buffer */

	u32 report_descriptor_crc32;  /* CRC32 of wire-read report descriptor */

	u32 bus_error_count;          /* Cumulative SPI bus error counter */
	int bus_last_error;           /* Last bus error code */

	u32 dir_count;                /* Counter for descriptor reads */
	u32 powered;                  /* Power state transitions completed */
	bool keep_powered;           /* Prevent suspend power-off */

	/*
	 * IRQ-driven startup sequencer state machine. Mirrors HidSpiCx WDF
	 * automaton: reset-response → body read → ConfiguringDescriptor →
	 * acknowledge → descriptor request.
	 * States: 0=WAIT_RESET 1=WAIT_DESC 2=WAIT_RPT 3=VENDOR_INIT 4=DONE 5=WAIT_FEATURE
	 */
	int seq_state;
	bool seq_enabled;               /* Sequencer is active */
	unsigned long seq_last_valid_jiffies; /* Last valid IRQ timestamp */
	u32 seq_storm_count;           /* Consecutive invalid IRQ count (storm detection) */

	u64 interrupt_time_stamps[2];  /* Ring buffer: last two IRQ timestamps */
	struct latency_instance latencies[SPI_HID_MAX_LATENCIES]; /* IRQ-to-report latency ring */
	u8 latency_index;              /* Current slot in latency ring */
	u8 perf_mode;                  /* Performance mode flag */
	u16 touch_signature_index;     /* Touch signature for latency tracking */

	/* Report descriptor read from the device during standard discovery. */
	u8 wire_report_descriptor[1024];
	u32 wire_report_descriptor_len;
	bool wire_report_descriptor_rejected; /* Parse rejected by hid_parse_report */

	/* Multi-touch input device created for raw heatmap mode. */
	struct input_dev *touch_input;
	bool raw_mode_active;           /* Device is in raw heatmap mode */
	u8 *heatmap_buf;                /* Last captured raw frame buffer, kmalloc'd */
	u32 heatmap_len;                /* byte length of heatmap_buf */
	u32 heatmap_content_id;         /* content_id from the captured frame */
	u16 heatmap_grid_cols;          /* Heatmap columns (72) */
	u16 heatmap_grid_rows;          /* Heatmap rows (48) */

	/* Per-device blob detection buffers. */
	u8  heatmap_baseline[HEATMAP_MAX_CELLS];
	u32 heatmap_baseline_cells;
	bool heatmap_have_baseline;
	u32 heatmap_baseline_frames;
	u8  heatmap_touched[HEATMAP_MAX_CELLS];
	s16 heatmap_signal[HEATMAP_MAX_CELLS];   /* precomputed c590 signal rise */
	u16 heatmap_label[HEATMAP_MAX_CELLS];    /* CCL component labels */

	/* Blob state. Coordinates are fixed-point grid ×100. */
	u32 blob_x[HEATMAP_MAX_BLOBS];
	u32 blob_y[HEATMAP_MAX_BLOBS];
	u32 blob_wsum[HEATMAP_MAX_BLOBS];
	bool blob_active[HEATMAP_MAX_BLOBS];
	u16 cost[HEATMAP_MAX_BLOBS][HEATMAP_MAX_SLOTS]; /* Hungarian cost matrix */

	/* Slot state, duration, and coordinate history. */
	u8 blob_slot_state[HEATMAP_MAX_SLOTS];      /* 0=empty 1=new 2=claimed 3=lift */
	u32 blob_slot_duration[HEATMAP_MAX_SLOTS];  /* frames in current state */
	u32 blob_slot_gx[HEATMAP_MAX_SLOTS];        /* last grid X, fixed-point */
	u32 blob_slot_gy[HEATMAP_MAX_SLOTS];        /* last grid Y, fixed-point */
	u32 blob_slot_weight[HEATMAP_MAX_SLOTS];    /* last blob weight */
	u32 blob_slot_missed[HEATMAP_MAX_SLOTS];    /* consecutive frames missed */
	u8 blob_slot_stationary[HEATMAP_MAX_SLOTS]; /* stationary frame counter */
	u8 blob_slot_blob[HEATMAP_MAX_SLOTS];       /* associated blob index */

	/* Per-slot history ring for sway and velocity (Surface: 10 samples). */
	#define SLOT_HISTORY_DEPTH 10
	u32 blob_slot_hx[HEATMAP_MAX_SLOTS][SLOT_HISTORY_DEPTH];
	u32 blob_slot_hy[HEATMAP_MAX_SLOTS][SLOT_HISTORY_DEPTH];
	u8 blob_slot_hpos[HEATMAP_MAX_SLOTS];
	u8 blob_slot_hcount[HEATMAP_MAX_SLOTS];  /* valid entries */

	/* Frame counter for gap detection (Surface: >5 frames → reset all). */
	u32 heatmap_frame_id;
	u32 heatmap_last_consecutive;

	/* Eigenvalue/ellipsis tracking */
	s32 eigmaj[HEATMAP_MAX_SLOTS];
	s32 eigmin[HEATMAP_MAX_SLOTS];
	s32 eigori[HEATMAP_MAX_SLOTS];	/* orientation in fixed-point degrees * 100 */

	s16 c590_lut[256];	/* pre-computed c590 signal lookup table */

	u8 *data_buf;              /* pre-allocated for seq_thread body reads */
	u32 data_buf_len;

	/* Passive raw capture: never changes device state or interprets payloads. */
	u8 raw_capture_frames[SPI_HID_RAW_CAPTURE_SLOTS]
		[SPI_HID_RAW_CAPTURE_BODY_LENGTH];
	u64 raw_capture_sequence[SPI_HID_RAW_CAPTURE_SLOTS];
	u64 raw_capture_timestamp_ns[SPI_HID_RAW_CAPTURE_SLOTS];
	u64 raw_capture_count;
	u32 raw_capture_next;
	u32 raw_capture_invalid;
	u32 raw_input_invalid;
	bool raw_transition_attempted;
	u32 raw_transition_scheduled;
	u32 raw_transition_get_sent;
	u32 raw_transition_get_write_failed;
	u32 raw_transition_get_response;
	u32 raw_transition_get_body_len;
	u8 raw_transition_get_body[256];
	u32 raw_transition_timeout;
	u32 raw_transition_set_sent;
	u32 raw_transition_set_write_failed;
	u32 raw_transition_state_skipped;
	u32 raw_transition_reset_before_response;

	/* Separately gated, boot-time-only GET ID6 / SET observation harness. */
	bool isolated_set_armed;
	bool isolated_set_attempted;
	u32 isolated_set_get_sent;
	u32 isolated_set_set_sent;
	u32 isolated_set_write_failed;
	u32 isolated_set_timeout;
	u32 isolated_set_reset;
	u32 isolated_set_count;
	u32 isolated_set_overflow;
	u32 isolated_set_next;
	enum spi_hid_isolated_set_state isolated_set_state;
	u8 isolated_set_header[SPI_HID_INPUT_HEADER_LEN];
	struct spi_hid_isolated_set_frame
		isolated_set_ring[SPI_HID_ISOLATED_SET_RING_SLOTS];

	/* Raw-mode handshake state for auto-retry on cold boot. */
	struct delayed_work raw_handshake_watchdog; /* Handshake timeout/retry watchdog */
	int raw_handshake_retries_left;            /* Remaining handshake attempts */
	u8 raw_handshake_state5_defers;           /* State-5 deferral counter */
	bool raw_handshake_confirmed;             /* Handshake successfully completed */
	u8 raw_probe_attempts;                    /* Deferred probe retry counter */

	struct delayed_work feat_delay_work;      /* GET_FEATURE delay work (matches Windows ~5900ms) */
	bool feat_delay_pending;                  /* Delay work is scheduled */
	struct delayed_work raw_transition_timeout_work;  /* Raw transition timeout */
	struct delayed_work isolated_set_timeout_work;    /* Isolated set timeout */
	struct delayed_work isolated_set_work;            /* Isolated set scheduling */

	struct delayed_work stream_watchdog;      /* Input stream monitoring watchdog */
	u32 stream_watchdog_data;                 /* Frames since last data */
	u32 stream_watchdog_misses;               /* Missed frame counter */
	u32 stream_watchdog_reinits;              /* Stream reinit counter */
	bool stream_watchdog_active;              /* Watchdog is active */

	struct delayed_work poll_work;            /* Active polling work item */
	bool poll_active;                         /* Polling loop running */
	u32 poll_interval_ms;                     /* Polling interval (default 10 ms) */
	u32 poll_missed;                          /* Consecutive empty polls */

	u32 stat_reset_rsp;
	u32 stat_device_desc;
	u32 stat_rpt_desc;
	u32 stat_data;
	u32 stat_getfeat_resp;
	u32 stat_frames_dropped;
	u32 stat_irq_count;
	u32 stat_wire_patches; /* descriptor bytes patched (0 = 100% wire-read) */
	ktime_t seq_dbg_last_irq;
	int seq_dbg_last_state;
	bool seq_dbg_expect_fast;
};

#endif
