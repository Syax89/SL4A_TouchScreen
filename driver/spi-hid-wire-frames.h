/* SPDX-License-Identifier: GPL-2.0 */
#ifndef SPI_HID_WIRE_FRAMES_H
#define SPI_HID_WIRE_FRAMES_H

/*
 * Host->device command frames for the SPI-HID sequencer path.
 *
 * Every frame below is a byte-for-byte copy of what the Windows stack puts on
 * the SPB bus while it brings the touch controller up. The reference is the
 * SPB trace captures/wintrace/surface_init.csv from TXN 634377432 onwards,
 * whose write frames are, in order:
 *
 *   SET_POWER D0      02 00 00 04 82 00 00 04 00 01 01 0C EE 5B
 *   SET_FEATURE 0x56  02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00
 *   DESCREQ reg 1     02 00 00 01 42 00 00 03 00 00
 *   DESCREQ reg 2     02 00 00 02 42 00 00 03 00 00
 *   SET_FEATURE 0x56  02 00 00 03 C2 00 03 0A 00 56 BD 0C EE 5B 44 4C 00 00
 *   GET_FEATURE id 6  02 00 00 03 42 00 04 03 00 06
 *   SET_FEATURE id 5  02 00 00 03 82 00 03 04 00 05 01 0C EE 5B
 *
 * Two properties are load-bearing and are asserted byte for byte by
 * tests/wire_frames_test.c:
 *
 *   - the write opcode appears exactly once, as the first byte. The driver
 *     historically sent it twice (02 02 ...), which is not what the device is
 *     given by Windows.
 *   - the short command bodies (payloads of one or two bytes) are padded with
 *     the constant 0C EE 5B field, not with zeros. The same three bytes appear
 *     on every frame that carries a trailer, so they are a fixed
 *     key/check field, not data.
 *
 * The legacy doubled-opcode form is still emitted when the module parameter
 * wire_double_opcode=1, so both forms live side by side here and every caller
 * picks one through the wire_double_opcode value it passes in.
 */

#ifdef __KERNEL__
#include <linux/kernel.h>
#include <linux/types.h>
#define SPI_HID_WIRE_U8 u8
#else
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#define SPI_HID_WIRE_U8 uint8_t
#endif

/* Write opcode (spi_hid_protocol_encode_output_header()). */
#define SPI_HID_WIRE_OPCODE 0x02

/* Constant key/check field carried in the trailer of the short command
 * bodies. It is byte-identical on every reference frame and must never be
 * "computed" or replaced with zero padding. */
#define SPI_HID_WIRE_TRAILER_0 0x0C
#define SPI_HID_WIRE_TRAILER_1 0xEE
#define SPI_HID_WIRE_TRAILER_2 0x5B

/* The default wire mode: the Windows-identical single-opcode frame. The
 * module parameter is initialised from this value and the host test asserts
 * it, so flipping the default cannot pass unnoticed. */
#define SPI_HID_WIRE_DOUBLE_DEFAULT 0

/* Longest frame built here (DESCREQ in the legacy doubled form). */
#define SPI_HID_WIRE_DESCREQ_MAX 11

/* DESCREQ registers: the device descriptor is always 0x000001, the report
 * descriptor register is the value the device descriptor reports (0x000002 on
 * the Surface devices, see the trace rows quoted above). */
#define SPI_HID_WIRE_DESCREQ_DEVICE_REG 0x000001
#define SPI_HID_WIRE_DESCREQ_REPORT_REG 0x000002

/* One command frame: bytes and their length, both fixed per wire mode. */
struct spi_hid_wire_frame {
	const SPI_HID_WIRE_U8 *bytes;
	unsigned int len;
};

/* Pick the frame for the requested wire mode. */
static inline struct spi_hid_wire_frame spi_hid_wire_pick(
		const SPI_HID_WIRE_U8 *plain, unsigned int plain_len,
		const SPI_HID_WIRE_U8 *doubled, unsigned int doubled_len,
		int double_opcode)
{
	struct spi_hid_wire_frame frame;

	if (double_opcode) {
		frame.bytes = doubled;
		frame.len = doubled_len;
	} else {
		frame.bytes = plain;
		frame.len = plain_len;
	}
	return frame;
}

#define SPI_HID_WIRE_PICK(plain, doubled, double_opcode) \
	spi_hid_wire_pick((plain), (unsigned int)sizeof(plain), \
			  (doubled), (unsigned int)sizeof(doubled), \
			  (double_opcode))

/* SET_POWER D2 (doze), command register 0x000004, 14 bytes. Windows emits the
 * D0 twin in the trace; the two differ only in the payload byte (02 = D2). */
static inline struct spi_hid_wire_frame spi_hid_wire_set_power_d2(int double_opcode)
{
	static const SPI_HID_WIRE_U8 plain[] = {
		0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00, 0x04,
		0x00, 0x01, 0x02, SPI_HID_WIRE_TRAILER_0, SPI_HID_WIRE_TRAILER_1,
		SPI_HID_WIRE_TRAILER_2
	};
	static const SPI_HID_WIRE_U8 doubled[] = {
		0x02, 0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00,
		0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00
	};

	return SPI_HID_WIRE_PICK(plain, doubled, double_opcode);
}

/* SET_POWER D0 (active), command register 0x000004, 14 bytes. */
static inline struct spi_hid_wire_frame spi_hid_wire_set_power_d0(int double_opcode)
{
	static const SPI_HID_WIRE_U8 plain[] = {
		0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00, 0x04,
		0x00, 0x01, 0x01, SPI_HID_WIRE_TRAILER_0, SPI_HID_WIRE_TRAILER_1,
		SPI_HID_WIRE_TRAILER_2
	};
	static const SPI_HID_WIRE_U8 doubled[] = {
		0x02, 0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00,
		0x04, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00
	};

	return SPI_HID_WIRE_PICK(plain, doubled, double_opcode);
}

/* SET_FEATURE Report ID 0x56 (vendor init / device key), command register
 * 0x000003, 18 bytes. The seven payload bytes are the report ID followed by
 * the six-byte device key, which embeds the same 0C EE 5B field. */
static inline struct spi_hid_wire_frame spi_hid_wire_vendor_init(int double_opcode)
{
	static const SPI_HID_WIRE_U8 plain[] = {
		0x02, 0x00, 0x00, 0x03, 0xC2, 0x00, 0x03, 0x0A, 0x00,
		0x56, 0xBD, SPI_HID_WIRE_TRAILER_0, SPI_HID_WIRE_TRAILER_1,
		SPI_HID_WIRE_TRAILER_2, 0x44, 0x4C, 0x00, 0x00
	};
	static const SPI_HID_WIRE_U8 doubled[] = {
		0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00, 0x03, 0x0A, 0x00,
		0x56, 0xBD, SPI_HID_WIRE_TRAILER_0, SPI_HID_WIRE_TRAILER_1,
		SPI_HID_WIRE_TRAILER_2, 0x44, 0x4C, 0x00, 0x00
	};

	return SPI_HID_WIRE_PICK(plain, doubled, double_opcode);
}

/* SET_FEATURE Report ID 5 (enables the heatmap), command register 0x000003,
 * 14 bytes: one payload byte (01 = enable) and a three-byte trailer. */
static inline struct spi_hid_wire_frame spi_hid_wire_set_feature5(int double_opcode)
{
	static const SPI_HID_WIRE_U8 plain[] = {
		0x02, 0x00, 0x00, 0x03, 0x82, 0x00, 0x03, 0x04,
		0x00, 0x05, 0x01, SPI_HID_WIRE_TRAILER_0, SPI_HID_WIRE_TRAILER_1,
		SPI_HID_WIRE_TRAILER_2
	};
	static const SPI_HID_WIRE_U8 doubled[] = {
		0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00, 0x03,
		0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
	};

	return SPI_HID_WIRE_PICK(plain, doubled, double_opcode);
}

/* GET_FEATURE Report ID 6, command register 0x000003, 10 bytes. The device
 * answers with a 122-byte content body:
 *   5 pad | u16 total_length (122) | u8 content_id (6) | 119 payload bytes
 * read back ~0.5 ms later (captures/wintrace/surface_init.csv). The layout is
 * declared here because it is wire format; the driver keeps the reply in
 * struct spi_hid_getfeat6 and never interprets the values yet. */
#define SPI_HID_GETFEAT6_PREAMBLE_LEN	5
#define SPI_HID_GETFEAT6_CONTENT_LEN	122
#define SPI_HID_GETFEAT6_PAYLOAD_LEN	119
#define SPI_HID_GETFEAT6_READ_LEN \
	(SPI_HID_GETFEAT6_PREAMBLE_LEN + SPI_HID_GETFEAT6_CONTENT_LEN)

/* The reply carries the report ID it answers for as its content ID. */
#define SPI_HID_GETFEAT6_REPORT_ID	0x06

static inline struct spi_hid_wire_frame spi_hid_wire_get_feature6(int double_opcode)
{
	static const SPI_HID_WIRE_U8 plain[] = {
		0x02, 0x00, 0x00, 0x03, 0x42, 0x00, 0x04, 0x03, 0x00, 0x06
	};
	static const SPI_HID_WIRE_U8 doubled[] = {
		0x02, 0x02, 0x00, 0x00, 0x03, 0x42, 0x00, 0x04, 0x03, 0x00, 0x06
	};

	return SPI_HID_WIRE_PICK(plain, doubled, double_opcode);
}

/* DESCREQ for `reg`: the device-descriptor register 0x000001
 * (02 00 00 01 42 00 00 03 00 00) or the report-descriptor register 0x000002
 * that the device descriptor reports (02 00 00 02 42 00 00 03 00 00).
 *
 * The register is the one field of the sequencer frames that is not a
 * compile-time constant, so this frame is built into the caller's buffer
 * instead of being a table. Header byte 4 is the protocol version ORed with
 * the body length nibble (see spi_hid_protocol_encode_output_header()): a
 * four-byte body renders 0x42.
 *
 * `out` must hold SPI_HID_WIRE_DESCREQ_MAX bytes; returns the length. */
static inline unsigned int spi_hid_wire_descreq(SPI_HID_WIRE_U8 *out,
		unsigned int reg, int double_opcode)
{
	unsigned int n = 0;

	out[n++] = SPI_HID_WIRE_OPCODE;
	if (double_opcode)
		out[n++] = SPI_HID_WIRE_OPCODE;
	out[n++] = (reg >> 16) & 0xff;
	out[n++] = (reg >> 8) & 0xff;
	out[n++] = reg & 0xff;
	out[n++] = 0x42;
	out[n++] = 0x00;
	out[n++] = 0x00;
	out[n++] = 0x03;
	out[n++] = 0x00;
	out[n++] = 0x00;
	return n;
}

/*
 * GET_FEATURE(6) payload logging helpers.
 *
 * The payload is a block of IEEE-754 binary32 values whose field layout is not
 * mapped yet. It is only ever logged, so the conversion has to happen here:
 * the kernel cannot print %f (no floating point context, no libm). The
 * formatting below is integer math only and is exercised by the host test.
 */

/* Enough for sign, up to 20 integer digits, a dot and three decimals, plus
 * the "mantissa p exponent" fallback and the terminator. */
#define SPI_HID_WIRE_F32_TEXT_LEN 32

/* Values are rendered one per line, this many per row. */
#define SPI_HID_WIRE_F32_PER_LINE 8

/* Buffer size for one rendered row (values plus separators). */
#define SPI_HID_WIRE_F32_ROW_LEN \
	(SPI_HID_WIRE_F32_TEXT_LEN * (SPI_HID_WIRE_F32_PER_LINE + 1))

/* Render four little-endian wire bytes as decimal text with three fractional
 * digits. Values whose magnitude exceeds 2^40 are far outside any calibration
 * field and are printed as "<mantissa>p<binary exponent>" rather than as
 * plausible-looking digits for a number that does not fit. */
static inline void spi_hid_wire_fmt_f32(const SPI_HID_WIRE_U8 *le, char *out,
		unsigned int out_len)
{
	uint32_t bits = (uint32_t)le[0] | ((uint32_t)le[1] << 8) |
			((uint32_t)le[2] << 16) | ((uint32_t)le[3] << 24);
	unsigned int sign = bits >> 31;
	unsigned int exponent = (bits >> 23) & 0xff;
	uint32_t fraction = bits & 0x7fffff;
	uint64_t mantissa;
	uint64_t integer;
	unsigned int milli;
	int e;

	if (exponent == 0xff) {
		snprintf(out, out_len, "%s", fraction ? "nan" :
			 (sign ? "-inf" : "inf"));
		return;
	}
	/* The significand carries an implicit leading 1 scaled by 2^23, so the
	 * value is mantissa * 2^(exponent - 127 - 23). */
	if (exponent == 0) {
		mantissa = fraction;	/* subnormal */
		e = -126 - 23;
	} else {
		mantissa = fraction | 0x800000;
		e = (int)exponent - 127 - 23;
	}

	if (e >= 0) {
		if (e > 40) {
			snprintf(out, out_len, "%s%llup%d", sign ? "-" : "",
				 (unsigned long long)mantissa, e);
			return;
		}
		integer = mantissa << e;
		milli = 0;
	} else {
		unsigned int shift = (unsigned int)-e;
		uint64_t remainder;

		if (shift >= 24) {
			/* The integer part is zero; keep every mantissa bit as
			 * the fraction. `mantissa` is below 2^24, so shifting it
			 * after scaling by 1000 cannot overflow a u64. */
			integer = 0;
			remainder = mantissa;
		} else {
			integer = mantissa >> shift;
			remainder = mantissa & ((1ULL << shift) - 1);
		}
		if (shift >= 32)
			milli = 0;
		else
			milli = (unsigned int)((remainder * 1000 +
						(1ULL << (shift - 1))) >> shift);
		if (milli >= 1000) {
			milli -= 1000;
			integer++;
		}
	}

	snprintf(out, out_len, "%s%llu.%03u", sign ? "-" : "",
		 (unsigned long long)integer, milli);
}

/* Render `len / 4` little-endian binary32 values as one space-separated line.
 * Returns the number of values written. */
static inline unsigned int spi_hid_wire_fmt_f32_row(const SPI_HID_WIRE_U8 *payload,
		unsigned int len, char *out, unsigned int out_len)
{
	unsigned int off = 0;
	unsigned int used = 0;
	unsigned int count = 0;

	if (out_len == 0)
		return 0;
	out[0] = '\0';

	while (off + 4 <= len) {
		char one[SPI_HID_WIRE_F32_TEXT_LEN];
		unsigned int room = out_len - used;
		int written;

		spi_hid_wire_fmt_f32(payload + off, one, sizeof(one));
		written = snprintf(out + used, room, "%s%s",
				   count ? " " : "", one);
		if (written < 0 || (unsigned int)written >= room)
			break;
		used += (unsigned int)written;
		count++;
		off += 4;
	}

	return count;
}

#endif /* SPI_HID_WIRE_FRAMES_H */
