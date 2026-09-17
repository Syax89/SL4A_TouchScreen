/* SPDX-License-Identifier: GPL-2.0 */
#ifndef SPI_HID_PROTOCOL_H
#define SPI_HID_PROTOCOL_H

#ifdef __KERNEL__
#include <linux/types.h>
typedef u8 spi_hid_proto_u8;
typedef u16 spi_hid_proto_u16;
#else
#include <stdint.h>
typedef uint8_t spi_hid_proto_u8;
typedef uint16_t spi_hid_proto_u16;
#endif

#define SPI_HID_PROTOCOL_VERSION 2
#define SPI_HID_PROTOCOL_SYNC_BYTE 0x5a
#define SPI_HID_PROTOCOL_WRITE_OPCODE 0x02
#define SPI_HID_PROTOCOL_MAX_OUTPUT_LENGTH 0x0fff

/* Synchronous request classes (spi_hid_sync_request in spi-hid-core.c).
 * Feature queries (GET_FEATURE) are independent of the IRQ-driven input
 * stream: a timeout there must not tear the transport down. The DESCRIPTOR
 * class exists for the protocol table only — this driver never issues a
 * descriptor sync (descriptor geometry is learned through the DESCREQ
 * sequencer path) — so the one kind spi_hid_sync_request() is called with
 * today is FEATURE, and the fatal classification below has no live call
 * path until one is wired (P1 double-blind wave: unreachable, not a bug;
 * do not delete the branch without re-wiring the caller first). */
enum spi_hid_sync_kind {
	SPI_HID_SYNC_FEATURE = 0,
	SPI_HID_SYNC_DESCRIPTOR = 1,
};

/* Default synchronous-request timeout in ms. The MSHW0231 answers feature
 * queries only after ~3.6 s of settle (measured from Windows trace rows;
 * the original protocol doc cited ~5.9 s), so 6000 ms covers the
 * documented worst case while the pre-fix hardcoded 1000 ms timed out on
 * a cold-boot feature query and tore the transport down. */
#define SPI_HID_PROTOCOL_SYNC_TIMEOUT_MS_DEFAULT 6000

/* Probe clamps the read-only parameter into this range (clamp once, like
 * getfeat_delay_ms). Below the minimum a timeout cannot cover one round
 * trip — and 0 makes every missed response an instant timeout — while a
 * negative value would wrap msecs_to_jiffies() into the far future and
 * lose the fail-safe entirely; the maximum bounds a typo in a modprobe.d
 * drop-in. */
#define SPI_HID_PROTOCOL_SYNC_TIMEOUT_MS_MIN 100
#define SPI_HID_PROTOCOL_SYNC_TIMEOUT_MS_MAX 60000

/* Whether a failed synchronous request of this kind must be treated as a
 * fatal transport error (1 = fatal, 0 = caller-side error only). */
static inline int spi_hid_protocol_sync_timeout_fatal(enum spi_hid_sync_kind kind)
{
	return kind != SPI_HID_SYNC_FEATURE;
}

static inline int spi_hid_protocol_output_length_valid(unsigned int output_length)
{
	return output_length <= SPI_HID_PROTOCOL_MAX_OUTPUT_LENGTH;
}

/* The only raw frame shape accepted by the passive capture path.
 *
 * Two numbers, two layers, not interchangeable: TOTAL_LENGTH is the V0
 * content header's own `total_length` (what the driver's callers extract
 * from the body), BODY_LENGTH is the FRAME — the same count plus the two
 * transport-aligned tail bytes. raw_confirms_handshake() receives the
 * semantic one; its parameter carries that name. */
#define SPI_HID_RAW_CAPTURE_CONTENT_ID 0x0c
#ifndef SPI_HID_RAW_CAPTURE_BODY_LENGTH
#define SPI_HID_RAW_CAPTURE_BODY_LENGTH 4304
#endif
#define SPI_HID_RAW_CAPTURE_TOTAL_LENGTH 4302
_Static_assert(SPI_HID_RAW_CAPTURE_BODY_LENGTH ==
	       SPI_HID_RAW_CAPTURE_TOTAL_LENGTH + 2,
	       "the raw frame body adds the two tail bytes to the semantic length");

struct spi_hid_protocol_header {
	spi_hid_proto_u8 version;
	spi_hid_proto_u8 report_type;
	spi_hid_proto_u8 fragment_id;
	spi_hid_proto_u8 length_reserved;
	spi_hid_proto_u16 report_length;
	spi_hid_proto_u8 sync_const;
};

/* A V0 body begins with its unpadded semantic length and content ID. */
struct spi_hid_protocol_content {
	spi_hid_proto_u16 total_length;
	spi_hid_proto_u8 content_id;
	const spi_hid_proto_u8 *data;
	spi_hid_proto_u16 data_length;
};

static inline void spi_hid_protocol_decode_header(const spi_hid_proto_u8 raw[4],
		struct spi_hid_protocol_header *header)
{
	header->version = raw[0] & 0x0f;
	header->report_type = raw[0] >> 4;
	header->fragment_id = 0; /* V0 protocol: no fragment on wire, reserved nibble */
	header->length_reserved = raw[1] & 0x0f;
	header->report_length = (((spi_hid_proto_u16)raw[1] >> 4) |
		((spi_hid_proto_u16)raw[2] << 4)) * 4;
	header->sync_const = raw[3];
}

/* Where the device-descriptor structure starts in a body read. The reference's
 * bodies begin with a five-byte FF preamble; this panel prefixes every answer
 * with three bytes (01 <status> EE), and no body read from it has been seen
 * yet — so BOTH shapes are accepted, and the offset is returned rather than
 * assumed. A prefixed body used to parse at byte 3, land on the preamble, and
 * fail validation: the caller never left discovery and nothing said why. */
static inline int spi_hid_protocol_body_offset(const spi_hid_proto_u8 *body,
					       int len)
{
	int off = 0;

	if (len >= 3 && body[0] == 0x01 && body[2] == 0xEE)
		off = 3;
	while (off + 3 < len && body[off] == 0xFF)
		off++;
	/* Never an offset PAST the buffer: a body too short to hold the
	 * structure (a three-byte prefixed stub used to return 6) returns -1,
	 * and every caller rejects that before adding anything to it.
	 * `off == len` is still returned as-is — the callers treat it as
	 * empty, not as valid. */
	return off + 3 > len ? -1 : off + 3;
}

static inline int spi_hid_protocol_encode_output_header(spi_hid_proto_u8 raw[6],
		unsigned int output_register, spi_hid_proto_u16 output_length)
{
	if (!spi_hid_protocol_output_length_valid(output_length))
		return -1;

	raw[0] = SPI_HID_PROTOCOL_WRITE_OPCODE;
	raw[1] = output_register >> 16;
	raw[2] = output_register >> 8;
	raw[3] = output_register;
	raw[4] = SPI_HID_PROTOCOL_VERSION | ((output_length & 0x0f) << 4);
	raw[5] = output_length >> 4;
	return 0;
}

static inline int spi_hid_protocol_parse_content(const spi_hid_proto_u8 *body,
		unsigned int body_length, struct spi_hid_protocol_content *content)
{
	spi_hid_proto_u16 total_length;

	if (!body || !content || body_length < 3)
		return -1;

	total_length = body[0] | ((spi_hid_proto_u16)body[1] << 8);
	if (total_length < 3 || total_length > body_length || total_length > 8192)
		return -1;

	content->total_length = total_length;
	content->content_id = body[2];
	content->data = body + 3;
	content->data_length = total_length - 3;
	return 0;
}

/* Whether a frame in raw mode confirms the experimental handshake and may
 * cancel the watchdog/poller backstop. Only the raw heatmap stream counts:
 * confirming on any frame let a standard HID report (or a stray one) retire
 * the recovery while no heatmap data was flowing, leaving raw mode parked
 * with no input and no way back. */
static inline int spi_hid_protocol_raw_confirms_handshake(spi_hid_proto_u8 content_id,
		unsigned int report_length)
{
	return report_length >= 3 && content_id == SPI_HID_RAW_CAPTURE_CONTENT_ID;
}

static inline int spi_hid_protocol_find_header(const spi_hid_proto_u8 *raw,
		int length, int *offset)
{
	int i;

	if (!raw || length < 4)
		return -1;

	/* Start scan from offset 0 for robustness; the sync byte (0x5A)
	 * typically appears at offset 5-7.
	 */
	for (i = 3; i < length; i++) {
		if (raw[i] == SPI_HID_PROTOCOL_SYNC_BYTE &&
		    (raw[i - 3] & 0x0f) == SPI_HID_PROTOCOL_VERSION) {
			if (offset)
				*offset = i - 3;
			return raw[i - 3] >> 4;
		}
	}

	return -1;
}

/* Frame type of a received buffer, by the sync-based rule alone: high nibble is
 * the type, and the version nibble is 2. `32 10 00 5A` therefore types as 3 at
 * offset 5 — and the reference's own boot trace labels exactly that frame its
 * RESET_RSP, so the rule needs no help.
 *
 * A sync-less `03 00 00 00` is NOT a frame: it is the DRAIN answering the read
 * after a reset (the trace's second read). Typing it as a reset, and rejecting
 * `32 10 00 5A` in the same breath, were the two inversions this function
 * carried until the trace bytes were finally read with the right tool.
 *
 * This body lives here rather than in spi-hid-core.c so the host test harness
 * can call it with real buffers and assert on the values. Five pins in this
 * repository were demonstrated decorative by adversarial legs — satisfied by a
 * comment, by an `#if 0` block, by a string literal — and a function call is
 * the one form of this check that text cannot satisfy.
 *
 * ponytail: window 4..7 covers the preamble lengths this driver produces;
 * widen only if a caller ever reads with another one. */
static inline int spi_hid_protocol_frame_type(const spi_hid_proto_u8 *rx, int len,
					      int *hdr_off)
{
	/* The reference's own exchange, read from the boot trace with
	 * tools/parse_spi.py (which now prints the hex of every frame, not just
	 * the length):
	 *
	 *   TX 0B 00 00 00 FF 00 00 00 00   RX FF FF FF FF FF 32 10 00 5A  → RESET_RSP
	 *   TX 0B 00 00 00 FF 00 00 00 00   RX FF FF FF FF FF 03 00 00 00  → its drain
	 *   TX 02 00 00 01 42 00 00 03 00 00                                → DESCREQ
	 *   TX 0B 00 00 00 FF 00 00 03 00   RX FF FF FF FF FF 72 80 00 5A  → DEVICE_DESC
	 *
	 * So `32 10 00 5A` IS the reset response — high nibble 3, sync present, which
	 * is exactly the rule this function has always applied — and `03 00 00 00`
	 * is the DRAIN that answers the read after it, not a reset of its own.
	 *
	 * Two later "improvements" inverted that, both mine: a narrowing taken from
	 * a Cx-layer function (VerifyResetResponse tests msg[0]==3) rejected the
	 * real reset, and a detector built on top of it promoted the drain. A blind
	 * leg had even proved the narrowing's pass branch unsatisfiable — the signal
	 * that the rule was wrong, which I answered by adding a detector instead of
	 * deleting the narrowing. The field then showed the device behaving exactly
	 * like the reference while this driver answered upside down.
	 *
	 * The lesson is in the shape of the mistake: the capture is the authority on
	 * what the device SAYS, a class-driver function from another layer is not. */
	return spi_hid_protocol_find_header(rx, len, hdr_off);
}

#endif /* SPI_HID_PROTOCOL_H */
