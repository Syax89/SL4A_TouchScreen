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

/* Protocol constants */
#define SPI_HID_DEFAULT_INPUT_REGISTER		0x0000
#define SPI_HID_SUPPORTED_VERSION		0x0100

/* Protocol message size constants */
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

#define SPI_HID_POWER_MODE_ACTIVE		0x01 /* "Active" - D0 */
#define SPI_HID_POWER_MODE_SLEEP		0x02 /* "Doze" - D2 */
#define SPI_HID_POWER_MODE_OFF			0x03
#define SPI_HID_POWER_MODE_WAKING_SLEEP		0x04 /* "Suspend" - D3/D3* */

/* Heatmap blob detection limits */
#define HEATMAP_MAX_CELLS   4300
#define HEATMAP_MAX_BLOBS   20
#define HEATMAP_MAX_SLOTS   47   /* match Windows DLL blob slot count */
#define SLOT_HISTORY_DEPTH  10

/* Eight fixed-size V0 bodies keep capture bounded to roughly 34 KiB. */
#define SPI_HID_RAW_CAPTURE_SLOTS 8
#define SPI_HID_RAW_CAPTURE_BODY_LENGTH 4304


enum spi_hid_seq_state {
	SPI_HID_SEQ_INVALID = -1,
	SPI_HID_SEQ_WAIT_RESET = 0,
	SPI_HID_SEQ_WAIT_DESC = 1,
	SPI_HID_SEQ_WAIT_RPT = 2,
	SPI_HID_SEQ_VENDOR_INIT = 3,
	SPI_HID_SEQ_DONE = 4,
	SPI_HID_SEQ_WAIT_FEATURE = 5,
};

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

struct spi_hid_output_report {
	u8 content_type;
	u16 content_length;
	u8 content_id;
	u8 *content;
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

	struct spi_hid_device_descriptor desc;    /* Parsed hardware descriptor */
	struct spi_hid_output_buf output;         /* Output report buffer (host→device) */
	struct spi_hid_input_buf response;        /* Output response buffer */

	spinlock_t		input_lock;   /* Protects IRQ-shared performance data */

	u32 device_descriptor_register;    /* Register address for device descriptor */
	u32 hid_desc_addr;                 /* HID descriptor register address from ACPI */
	u8 power_state;                    /* D0 (1 active), D2 (2 doze), D3 (3 off) */
	u8 attempts;                       /* Probe retry counter */

	/*
	 * ready flag indicates that the FW is ready to accept commands and requests.
	 * The FW becomes ready after sending the report descriptor.
	 */
	bool ready;
	bool hid_creating;            /* HID exists but hid_add_device() is in progress */
	bool reset_pending;           /* Reject HID creation until a new descriptor arrives */
	/*
	 * refresh_in_progress is set to true while the refresh_device worker thread
	 * is destroying and recreating the hidraw device. When this flag is set to
	 * true, the ll_close and ll_open functions will not cause power state changes
	 */
	bool refresh_in_progress;

	bool irq_requested;         /* request_threaded_irq() succeeded */
	bool irq_enabled;           /* Driver has not disabled the requested IRQ */
	bool suspended;             /* PM has quiesced driver I/O */
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

	/*
	 * Locking hierarchy for code that needs more than one lock:
	 *
	 *   power_lock -> lock -> seq_lock -> output_lock
	 *
	 * response_lock is a leaf spinlock and only protects response transaction
	 * metadata.
	 * response_mutex serializes requests and is acquired without lock held.
	 */
	struct mutex lock;            /* Top-level driver state mutex */
	/* Serializes sequencer SPI transfers and sequencer-owned state. */
	struct mutex seq_lock;
	struct mutex power_lock;      /* Serializes power state transitions */
	struct mutex output_lock;     /* Serializes output report access */
	struct mutex response_mutex;  /* Serializes synchronous response transactions */
	struct completion output_done; /* Signaled when output report completes */
	spinlock_t response_lock;      /* Protects the synchronous response transaction */
	u8 expected_response_type;    /* Expected response report type */
	u8 expected_response_id;      /* Expected response content ID */
	bool output_pending;          /* Output report sent, awaiting response */
	bool response_valid;          /* response contains the current transaction */
	u64 response_generation;      /* Rejects late responses from prior requests */

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
	 */
	enum spi_hid_seq_state seq_state;
	bool seq_enabled;               /* Sequencer is active */
	unsigned long seq_last_valid_jiffies; /* Last valid IRQ timestamp */
	u32 seq_storm_count;           /* Consecutive invalid IRQ count (storm detection) */

	u8 perf_mode;                  /* Performance mode flag */

	/* Report descriptor read from the device during standard discovery. */
	u8 wire_report_descriptor[1024];
	u32 wire_report_descriptor_len;
	bool wire_report_descriptor_rejected; /* Parse rejected by hid_parse_report */

	/* Multi-touch input device created for raw heatmap mode. */
	struct input_dev *touch_input;
	bool raw_mode_active;           /* Device is in raw heatmap mode */
	u8 *heatmap_buf;                /* Last captured raw frame buffer, kmalloc'd */
	u32 heatmap_len;                /* byte length of the last raw frame */
	u32 heatmap_capacity;           /* allocated byte capacity of heatmap_buf */
	u32 heatmap_content_id;         /* content_id from the captured frame */
	u16 heatmap_grid_cols;          /* Heatmap columns (72) */
	u16 heatmap_grid_rows;          /* Heatmap rows (48) */

	/* Per-device blob detection buffers. */
	u8  heatmap_baseline[HEATMAP_MAX_CELLS];
	bool heatmap_have_baseline;
	u32 heatmap_baseline_frames;
	u8  heatmap_touched[HEATMAP_MAX_CELLS];
	s16 heatmap_signal[HEATMAP_MAX_CELLS];   /* precomputed c590 signal rise */
	u16 heatmap_label[HEATMAP_MAX_CELLS];    /* CCL component labels */
	u32 heatmap_queue[HEATMAP_MAX_CELLS];    /* CCL flood-fill queue */

	/* Blob state. Coordinates are fixed-point grid ×100. */
	u32 blob_x[HEATMAP_MAX_BLOBS];
	u32 blob_y[HEATMAP_MAX_BLOBS];
	u32 blob_wsum[HEATMAP_MAX_BLOBS];
	bool blob_active[HEATMAP_MAX_BLOBS];
	s32 blob_eigmaj[HEATMAP_MAX_BLOBS];
	s32 blob_eigmin[HEATMAP_MAX_BLOBS];
	s32 blob_eigori[HEATMAP_MAX_BLOBS];
	u16 cost[HEATMAP_MAX_BLOBS][HEATMAP_MAX_SLOTS]; /* Hungarian cost matrix */

	/* Slot state, duration, and coordinate history. */
	u8 blob_slot_state[HEATMAP_MAX_SLOTS];      /* 0=empty 1=new 2=claimed 3=lift */
	u32 blob_slot_duration[HEATMAP_MAX_SLOTS];  /* frames in current state */
	u32 blob_slot_gx[HEATMAP_MAX_SLOTS];        /* last grid X, fixed-point */
	u32 blob_slot_gy[HEATMAP_MAX_SLOTS];        /* last grid Y, fixed-point */
	u32 blob_slot_weight[HEATMAP_MAX_SLOTS];    /* last blob weight */
	u32 blob_slot_missed[HEATMAP_MAX_SLOTS];    /* consecutive frames missed */
	u8 blob_slot_stationary[HEATMAP_MAX_SLOTS]; /* stationary frame counter */

	/* Per-slot history ring for sway and velocity (Surface: 10 samples). */
	u32 blob_slot_hx[HEATMAP_MAX_SLOTS][SLOT_HISTORY_DEPTH];
	u32 blob_slot_hy[HEATMAP_MAX_SLOTS][SLOT_HISTORY_DEPTH];
	u8 blob_slot_hpos[HEATMAP_MAX_SLOTS];
	u8 blob_slot_hcount[HEATMAP_MAX_SLOTS];  /* valid entries */

	/* Timestamp of the last raw frame for missed-frame release. */
	unsigned long heatmap_last_frame_jiffies;

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

	/* Raw-mode handshake state for auto-retry on cold boot. */
	struct delayed_work raw_handshake_watchdog; /* Handshake timeout/retry watchdog */
	int raw_handshake_retries_left;            /* Remaining handshake attempts */
	u8 raw_handshake_wait_feature_defers;  /* WAIT_FEATURE deferral counter */
	bool raw_handshake_confirmed;             /* Handshake successfully completed */
	u8 raw_probe_attempts;                    /* Deferred probe retry counter */

	struct delayed_work feat_delay_work;      /* GET_FEATURE delay work (matches Windows ~5900ms) */
	bool feat_delay_pending;                  /* Delay work is scheduled */

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
	enum spi_hid_seq_state seq_dbg_last_state;
	bool seq_dbg_expect_fast;
};

#endif
