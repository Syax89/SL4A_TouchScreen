/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Host->device command frame contract.
 *
 * driver/spi-hid-core.c cannot be compiled on this host (no kernel headers), so
 * this test compiles the exact header the driver builds its frames from
 * (driver/spi-hid-wire-frames.h) and compares every frame against the bytes the
 * Windows stack puts on the SPB bus, taken verbatim from
 * captures/wintrace/surface_init.csv (TXN 634377432 onwards).
 *
 * It fails if anyone
 *   - sends the leading opcode twice again (the default must be the Windows
 *     single-opcode form),
 *   - changes a byte of a command frame,
 *   - replaces the constant 0C EE 5B trailer with zero padding,
 *   - flips SPI_HID_WIRE_DOUBLE_DEFAULT, or
 *   - reintroduces a local frame table in driver/spi-hid-core.c.
 */

#define _DEFAULT_SOURCE
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../driver/spi-hid-wire-frames.h"
#include "../driver/spi-hid-protocol.h"

/* The default wire mode is the whole point: the frames must be the Windows
 * ones unless wire_double_opcode is set explicitly. */
_Static_assert(SPI_HID_WIRE_DOUBLE_DEFAULT == 0,
	       "the Windows-identical frames must be the default wire mode");

static unsigned int g_passed, g_failed;
#define CHECK(cond, msg) do { \
	g_passed++; \
	if (!(cond)) { g_failed++; fprintf(stderr, "FAIL: %s\n  %s:%d: %s\n", msg, __FILE__, __LINE__, #cond); } \
} while (0)

/* ── Windows reference frames ──────────────────────────────────── */

/* SET_POWER D2, command register 0x000004, 14 bytes. The trace carries the D0
 * twin (TXN 634377432); the two differ only in the payload byte. */
static const uint8_t ref_setpower_d2[] = {
	0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00, 0x04, 0x00, 0x01, 0x02, 0x0C, 0xEE, 0x5B
};
/* SET_POWER D0, command register 0x000004, 14 bytes (TXN 634377432). */
static const uint8_t ref_setpower_d0[] = {
	0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00, 0x04, 0x00, 0x01, 0x01, 0x0C, 0xEE, 0x5B
};
/* SET_FEATURE Report ID 0x56 with the device key, 18 bytes (Clock-Time 134276314683446347 in captures/wintrace/surface_init.csv). */
static const uint8_t ref_vendor_init[] = {
	0x02, 0x00, 0x00, 0x03, 0xC2, 0x00, 0x03, 0x0A,
	0x00, 0x56, 0xBD, 0x0C, 0xEE, 0x5B, 0x44, 0x4C, 0x00, 0x00
};
/* SET_FEATURE Report ID 0x56 with an all-FF payload: the reference's stream
 * stop / re-enumeration teardown (surface_init.csv txn #0257). Same frame as
 * the enable above, six bytes different. */
static const uint8_t ref_vendor_stop[] = {
	0x02, 0x00, 0x00, 0x03, 0xC2, 0x00, 0x03, 0x0A,
	0x00, 0x56, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
/* SET_FEATURE Report ID 5, payload 01, 14 bytes (Clock-Time 134276314683843667 in captures/wintrace/surface_init.csv). */
static const uint8_t ref_set_feature5[] = {
	0x02, 0x00, 0x00, 0x03, 0x82, 0x00, 0x03, 0x04,
	0x00, 0x05, 0x01, 0x0C, 0xEE, 0x5B
};
/* GET_FEATURE Report ID 6, 10 bytes (Clock-Time 134276314683821940 in captures/wintrace/surface_init.csv). */
static const uint8_t ref_get_feature6[] = {
	0x02, 0x00, 0x00, 0x03, 0x42, 0x00, 0x04, 0x03, 0x00, 0x06
};
/* DESCREQ register 1 (device descriptor), 10 bytes. */
static const uint8_t ref_descreq_device[] = {
	0x02, 0x00, 0x00, 0x01, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00
};
/* DESCREQ register 2 (report descriptor), 10 bytes. */
static const uint8_t ref_descreq_report[] = {
	0x02, 0x00, 0x00, 0x02, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00
};

/* ── Legacy doubled-opcode frames ──────────────────────────────── */

/* What the driver sent before this change, per command: the leading opcode on
 * the wire twice, and zero padding where Windows sends 0C EE 5B. Kept so the
 * wire_double_opcode=1 fallback keeps working exactly as before. */
static const uint8_t old_setpower_d2[] = {
	0x02, 0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00,
	0x04, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00
};
static const uint8_t old_setpower_d0[] = {
	0x02, 0x02, 0x00, 0x00, 0x04, 0x82, 0x00, 0x00,
	0x04, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00
};
static const uint8_t old_vendor_init[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00, 0x03, 0x0A, 0x00,
	0x56, 0xBD, 0x0C, 0xEE, 0x5B, 0x44, 0x4C, 0x00, 0x00
};
static const uint8_t old_vendor_stop[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0xC2, 0x00, 0x03, 0x0A, 0x00,
	0x56, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};
static const uint8_t old_set_feature5[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00, 0x03,
	0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00
};
static const uint8_t old_get_feature6[] = {
	0x02, 0x02, 0x00, 0x00, 0x03, 0x42, 0x00, 0x04, 0x03, 0x00, 0x06
};
static const uint8_t old_descreq_device[] = {
	0x02, 0x02, 0x00, 0x00, 0x01, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00
};
static const uint8_t old_descreq_report[] = {
	0x02, 0x02, 0x00, 0x00, 0x02, 0x42, 0x00, 0x00, 0x03, 0x00, 0x00
};

/* ── Byte comparison helpers ───────────────────────────────────── */

static void check_frame(const char *name, struct spi_hid_wire_frame frame,
			const uint8_t *ref, unsigned int ref_len)
{
	if (frame.len != ref_len) {
		g_failed++;
		fprintf(stderr, "FAIL: %s length: built %u bytes, reference %u bytes\n",
			name, frame.len, ref_len);
		g_passed++;
		return;
	}
	if (memcmp(frame.bytes, ref, ref_len) != 0) {
		unsigned int i;

		g_failed++;
		fputs("FAIL: ", stderr);
		fprintf(stderr, "%s bytes differ from the reference\n", name);
		for (i = 0; i < ref_len; i++) {
			if (frame.bytes[i] != ref[i])
				fprintf(stderr, "  byte %u: built 0x%02X, reference 0x%02X\n",
					i, frame.bytes[i], ref[i]);
		}
	}
	g_passed++;
}

static void check_descreq(const char *name, unsigned int reg,
			  const uint8_t *ref, unsigned int ref_len,
			  int double_opcode)
{
	uint8_t built[SPI_HID_WIRE_DESCREQ_MAX];
	struct spi_hid_wire_frame frame;
	unsigned int len;

	memset(built, 0xA5, sizeof(built));
	len = spi_hid_wire_descreq(built, reg, double_opcode);
	frame.bytes = built;
	frame.len = len;
	check_frame(name, frame, ref, ref_len);
	CHECK(len <= SPI_HID_WIRE_DESCREQ_MAX, "DESCREQ fits its buffer");
}

/* ── The default wire mode is the Windows one ──────────────────── */

static void test_default_frames_match_windows(void)
{
	check_frame("SET_POWER D2", spi_hid_wire_set_power_d2(SPI_HID_WIRE_DOUBLE_DEFAULT),
		    ref_setpower_d2, sizeof(ref_setpower_d2));
	check_frame("SET_POWER D0", spi_hid_wire_set_power_d0(SPI_HID_WIRE_DOUBLE_DEFAULT),
		    ref_setpower_d0, sizeof(ref_setpower_d0));
	check_frame("SET_FEATURE 0x56", spi_hid_wire_vendor_init(SPI_HID_WIRE_DOUBLE_DEFAULT),
		    ref_vendor_init, sizeof(ref_vendor_init));
	check_frame("SET_FEATURE 0x56 all-FF stop", spi_hid_wire_vendor_stop(SPI_HID_WIRE_DOUBLE_DEFAULT),
		    ref_vendor_stop, sizeof(ref_vendor_stop));
	check_frame("SET_FEATURE 5", spi_hid_wire_set_feature5(SPI_HID_WIRE_DOUBLE_DEFAULT),
		    ref_set_feature5, sizeof(ref_set_feature5));
	check_frame("GET_FEATURE 6", spi_hid_wire_get_feature6(SPI_HID_WIRE_DOUBLE_DEFAULT),
		    ref_get_feature6, sizeof(ref_get_feature6));
	check_descreq("DESCREQ device descriptor", SPI_HID_WIRE_DESCREQ_DEVICE_REG,
		      ref_descreq_device, sizeof(ref_descreq_device),
		      SPI_HID_WIRE_DOUBLE_DEFAULT);
	check_descreq("DESCREQ report descriptor", SPI_HID_WIRE_DESCREQ_REPORT_REG,
		      ref_descreq_report, sizeof(ref_descreq_report),
		      SPI_HID_WIRE_DOUBLE_DEFAULT);
}

/* ── The legacy form stays reachable and unchanged ─────────────── */

static void test_doubled_frames_unchanged(void)
{
	check_frame("legacy SET_POWER D2", spi_hid_wire_set_power_d2(1),
		    old_setpower_d2, sizeof(old_setpower_d2));
	check_frame("legacy SET_POWER D0", spi_hid_wire_set_power_d0(1),
		    old_setpower_d0, sizeof(old_setpower_d0));
	check_frame("legacy SET_FEATURE 0x56", spi_hid_wire_vendor_init(1),
		    old_vendor_init, sizeof(old_vendor_init));
	/* The stop is the enable with an all-FF payload: nothing else separates
	 * them, so nothing else can be allowed to drift. */
	check_frame("legacy SET_FEATURE 0x56 STOP", spi_hid_wire_vendor_stop(1),
		    old_vendor_stop, sizeof(old_vendor_stop));
	check_frame("legacy SET_FEATURE 5", spi_hid_wire_set_feature5(1),
		    old_set_feature5, sizeof(old_set_feature5));
	check_frame("legacy GET_FEATURE 6", spi_hid_wire_get_feature6(1),
		    old_get_feature6, sizeof(old_get_feature6));
	check_descreq("legacy DESCREQ device descriptor",
		      SPI_HID_WIRE_DESCREQ_DEVICE_REG, old_descreq_device,
		      sizeof(old_descreq_device), 1);
	check_descreq("legacy DESCREQ report descriptor",
		      SPI_HID_WIRE_DESCREQ_REPORT_REG, old_descreq_report,
		      sizeof(old_descreq_report), 1);
}

/* ── Structural invariants ─────────────────────────────────────── */

static void test_frame_shape(void)
{
	struct spi_hid_wire_frame frames[5];
	unsigned int i;

	frames[0] = spi_hid_wire_set_power_d2(0);
	frames[1] = spi_hid_wire_set_power_d0(0);
	frames[2] = spi_hid_wire_vendor_init(0);
	frames[3] = spi_hid_wire_set_feature5(0);
	frames[4] = spi_hid_wire_get_feature6(0);

	for (i = 0; i < 5; i++) {
		CHECK(frames[i].len >= 10, "frame is long enough");
		CHECK(frames[i].bytes[0] == SPI_HID_WIRE_OPCODE,
		      "frame starts with the write opcode");
		CHECK(frames[i].bytes[1] != SPI_HID_WIRE_OPCODE,
		      "the write opcode is not sent twice");
	}
	/* Trailer: the two one-byte-payload commands pad with 0C EE 5B, and the
	 * vendor-init device key embeds the same field. */
	for (i = 0; i < 2; i++) {
		const struct spi_hid_wire_frame *f = &frames[i];

		CHECK(f->bytes[f->len - 3] == SPI_HID_WIRE_TRAILER_0 &&
		      f->bytes[f->len - 2] == SPI_HID_WIRE_TRAILER_1 &&
		      f->bytes[f->len - 1] == SPI_HID_WIRE_TRAILER_2,
		      "SET_POWER trailer is 0C EE 5B");
	}
	CHECK(frames[3].bytes[frames[3].len - 3] == SPI_HID_WIRE_TRAILER_0 &&
	      frames[3].bytes[frames[3].len - 2] == SPI_HID_WIRE_TRAILER_1 &&
	      frames[3].bytes[frames[3].len - 1] == SPI_HID_WIRE_TRAILER_2,
	      "SET_FEATURE 5 trailer is 0C EE 5B");
	CHECK(frames[2].bytes[11] == SPI_HID_WIRE_TRAILER_0 &&
	      frames[2].bytes[12] == SPI_HID_WIRE_TRAILER_1 &&
	      frames[2].bytes[13] == SPI_HID_WIRE_TRAILER_2,
	      "vendor-init device key embeds 0C EE 5B");
}

/* ── The declared reply layout the driver reads against ───────── */

static void test_getfeat6_layout(void)
{
	/* 5 pad + u16 total_length + u8 content_id + 119 payload bytes. */
	_Static_assert(SPI_HID_GETFEAT6_PREAMBLE_LEN == 5,
		       "reply preamble is five bytes");
	_Static_assert(SPI_HID_GETFEAT6_CONTENT_LEN == 122,
		       "reply content is 122 bytes");
	_Static_assert(SPI_HID_GETFEAT6_PAYLOAD_LEN == 119,
		       "reply payload is 119 bytes");
	_Static_assert(SPI_HID_GETFEAT6_CONTENT_LEN ==
			       3 + SPI_HID_GETFEAT6_PAYLOAD_LEN,
		       "content length is the V0 three-byte header plus payload");
	CHECK(SPI_HID_GETFEAT6_READ_LEN == 127,
	      "the read covers the preamble plus the whole 122-byte content");
	CHECK(SPI_HID_GETFEAT6_READ_LEN ==
		     SPI_HID_GETFEAT6_PREAMBLE_LEN + SPI_HID_GETFEAT6_CONTENT_LEN,
	      "read length is preamble plus content");
	/* The frame itself is exactly the trace bytes, 10 bytes long. */
	CHECK(spi_hid_wire_get_feature6(0).len == sizeof(ref_get_feature6),
	      "GET_FEATURE 6 is a 10-byte frame");
	/* The driver recognises the reply by this content ID, so it has to be the
	 * ID the reference frame asks for. */
	CHECK(spi_hid_wire_get_feature6(0).bytes[9] == SPI_HID_GETFEAT6_REPORT_ID,
	      "the frame asks for the report ID the reply check expects");
}

/* ── Binary32 payload logging ──────────────────────────────────── */

static void check_f32(const char *what, uint8_t a, uint8_t b, uint8_t c, uint8_t d,
		      const char *expected)
{
	uint8_t le[4] = { a, b, c, d };
	char text[SPI_HID_WIRE_F32_TEXT_LEN];

	spi_hid_wire_fmt_f32(le, text, sizeof(text));
	CHECK(strcmp(text, expected) == 0, what);
	if (strcmp(text, expected) != 0)
		fprintf(stderr, "  %s: got \"%s\", expected \"%s\"\n", what, text, expected);
}

static void test_f32_formatting(void)
{
	/* Little-endian wire order, values from the captured report ID 6 reply. */
	check_f32("178.0", 0x00, 0x00, 0x32, 0x43, "178.000");
	check_f32("182.0", 0x00, 0x00, 0x36, 0x43, "182.000");
	check_f32("90.0", 0x00, 0x00, 0xB4, 0x42, "90.000");
	check_f32("1.0", 0x00, 0x00, 0x80, 0x3F, "1.000");
	check_f32("2.0", 0x00, 0x00, 0x00, 0x40, "2.000");
	check_f32("20.0", 0x00, 0x00, 0xA0, 0x41, "20.000");
	check_f32("2.4", 0x9A, 0x99, 0x19, 0x40, "2.400");
	check_f32("0.1", 0xCD, 0xCC, 0xCC, 0x3D, "0.100");
	check_f32("-1.0", 0x00, 0x00, 0x80, 0xBF, "-1.000");
	check_f32("0.0", 0x00, 0x00, 0x00, 0x00, "0.000");
	check_f32("inf", 0x00, 0x00, 0x80, 0x7F, "inf");
	check_f32("-inf", 0x00, 0x00, 0x80, 0xFF, "-inf");
	check_f32("nan", 0x00, 0x00, 0xC0, 0x7F, "nan");

	{
		uint8_t row_in[8] = { 0x00, 0x00, 0x80, 0x3F,   /* 1.0 */
				      0x00, 0x00, 0xA0, 0x41 }; /* 20.0 */
		char row[SPI_HID_WIRE_F32_ROW_LEN];
		unsigned int n = spi_hid_wire_fmt_f32_row(row_in, sizeof(row_in),
							  row, sizeof(row));

		CHECK(n == 2, "row formatter reports two values");
		CHECK(strcmp(row, "1.000 20.000") == 0, "row formatter space-separates");
		if (strcmp(row, "1.000 20.000") != 0)
			fprintf(stderr, "  row: got \"%s\"\n", row);
	}
}

/* ── The driver really uses this header ───────────────────────── */

static char *slurp(const char *path)
{
	FILE *f = fopen(path, "rb");
	long size;
	char *buf;

	if (!f)
		return NULL;
	if (fseek(f, 0, SEEK_END) != 0 || (size = ftell(f)) < 0 ||
	    fseek(f, 0, SEEK_SET) != 0) {
		fclose(f);
		return NULL;
	}
	buf = malloc((size_t)size + 1);
	if (!buf) {
		fclose(f);
		return NULL;
	}
	if (fread(buf, 1, (size_t)size, f) != (size_t)size) {
		free(buf);
		fclose(f);
		return NULL;
	}
	buf[size] = '\0';
	fclose(f);
	return buf;
}

/* `make -C tests` runs this with tests/ as the working directory, but the
 * binary is also usable by hand from the repository root. */
static char *slurp_driver_source(void)
{
	static const char *paths[] = {
		"../driver/spi-hid-core.c",
		"driver/spi-hid-core.c",
	};
	unsigned int i;

	for (i = 0; i < sizeof(paths) / sizeof(paths[0]); i++) {
		char *text = slurp(paths[i]);

		if (text)
			return text;
	}
	return NULL;
}

static void test_driver_uses_header(void)
{
	char *core = slurp_driver_source();

	if (!core) {
		g_failed++;
		g_passed++;
		fprintf(stderr, "FAIL: cannot locate driver/spi-hid-core.c\n");
		return;
	}
	CHECK(strstr(core, "#include \"spi-hid-wire-frames.h\"") != NULL,
	      "spi-hid-core.c includes the frame header");
	CHECK(strstr(core, "SPI_HID_WIRE_DOUBLE_DEFAULT") != NULL,
	      "spi-hid-core.c initialises the parameter from the header default");
	CHECK(strstr(core, "spi_hid_wire_set_feature5") != NULL,
	      "spi-hid-core.c builds SET_FEATURE through the header");
	CHECK(strstr(core, "spi_hid_wire_get_feature6") != NULL,
	      "spi-hid-core.c builds GET_FEATURE 6 through the header");
	CHECK(strstr(core, "spi_hid_wire_descreq") != NULL,
	      "spi-hid-core.c builds DESCREQ through the header");
	CHECK(strstr(core, "spi_hid_wire_set_power_d0") != NULL &&
	      strstr(core, "spi_hid_wire_set_power_d2") != NULL,
	      "spi-hid-core.c builds SET_POWER through the header");
	/* Every doubled-opcode table lives in the header now: a literal doubled
	 * frame reappearing in the driver means a local table came back. */
	CHECK(strstr(core, "0x02, 0x02") == NULL,
	      "no doubled-opcode frame literal in spi-hid-core.c");
	free(core);
}

static void test_read_approval_frame(void)
{
	/* The read approval: nine bytes, register at offset 7, address field
	 * zero. Byte-level guard on the frame that asks the device for a
	 * descriptor; the traces are the reference (0B 00 00 00 FF 00 00 0R 00). */
	static const SPI_HID_WIRE_U8 want3[SPI_HID_READ_APPROVAL_LEN] = {
		0x0B, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x03, 0x00
	};
	static const SPI_HID_WIRE_U8 want4[SPI_HID_READ_APPROVAL_LEN] = {
		0x0B, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x04, 0x00
	};
	/* The same frame while reading a response back: the content type of the
	 * request goes at offset 6 and its content id at offset 8
	 * (GET_FEATURE(6) then the stream's SET_FEATURE on register 0x0A). */
	static const SPI_HID_WIRE_U8 want_getfeat6[10] = {
		0x0B, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x04, 0x03, 0x00, 0x06
	};
	static const SPI_HID_WIRE_U8 want_stream[10] = {
		0x0B, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x03, 0x0A, 0x00, 0x56
	};
	SPI_HID_WIRE_U8 buf[16];
	unsigned int n;

	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval(buf, 0x0003, 0x00, 0x00);
	CHECK(n == SPI_HID_READ_APPROVAL_LEN, "read approval length is nine bytes");
	CHECK(!memcmp(buf, want3, sizeof(want3)),
	      "descriptor read approval is 0B 00 00 00 FF 00 00 03 00");
	n = spi_hid_wire_read_approval(buf, 0x0004, 0x00, 0x00);
	CHECK(n == SPI_HID_READ_APPROVAL_LEN, "read approval length is nine bytes");
	CHECK(!memcmp(buf, want4, sizeof(want4)),
	      "descriptor read approval is 0B 00 00 00 FF 00 00 04 00");
	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval(buf, 0x0003, 0x04, 0x06);
	CHECK(n == 10, "a read approval that names a request is ten bytes");
	CHECK(!memcmp(buf, want_getfeat6, sizeof(want_getfeat6)),
	      "GET_FEATURE(6) read approval is 0B 00 00 00 FF 00 04 03 00 06");
	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval(buf, 0x000A, 0x03, 0x56);
	CHECK(n == 10, "a read approval that names a request is ten bytes");
	CHECK(!memcmp(buf, want_stream, sizeof(want_stream)),
	      "stream read approval is 0B 00 00 00 FF 00 03 0A 00 56");

	/* All three variants, through the function the driver actually calls.
	 * The wrapper above builds variant 0 only, so every pin on it stayed
	 * green while the LEGACY branch — the one the default once selected —
	 * had no coverage at all. */
	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval_variant(buf, 0x0003, 0x00, 0x00, 0);
	CHECK(n == SPI_HID_READ_APPROVAL_LEN, "variant 0 is nine bytes");
	CHECK(!memcmp(buf, want3, sizeof(want3)), "variant 0 is the reference frame");
	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval_variant(buf, 0x0003, 0x00, 0x00, 1);
	CHECK(n == 5, "variant 1 (legacy) is five bytes");
	CHECK(buf[0] == 0x0B && buf[1] == 0x00 && buf[2] == 0x00 && buf[3] == 0x03 &&
	      buf[4] == 0xFF, "variant 1 carries the register in the address field");
	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval_variant(buf, 0x0003, 0x00, 0x00, 2);
	CHECK(n == SPI_HID_READ_APPROVAL_LEN, "variant 2 is nine bytes");
	CHECK(buf[3] == 0x03, "variant 2 carries the register in both places");

	/* The frame the reference sends FIRST, at boot, before it asks for
	 * anything: its own read of REGISTER 0, `0B 00 00 00 FF 00 00 00 00` in
	 * tools/parse_spi.py's output (surface_boot_auto.csv). The device answers
	 * it with the reset frame, and the whole campaign's read-register
	 * confusion ends in this assertion — it was the one frame with no test. */
	memset(buf, 0xAA, sizeof(buf));
	n = spi_hid_wire_read_approval_variant(buf, 0x0000, 0x00, 0x00, 0);
	CHECK(n == 9 && buf[0] == 0x0B && buf[1] == 0x00 && buf[2] == 0x00 &&
	      buf[3] == 0x00 && buf[4] == 0xFF && buf[5] == 0x00 && buf[6] == 0x00 &&
	      buf[7] == 0x00 && buf[8] == 0x00,
	      "register 0 in the reference shape is the reference's own first read");

	/* The frame typing itself, called with the buffers the field produced.
	 * These assertions are the reason the logic moved into the protocol
	 * header: five pins on this repository were demonstrated decorative by
	 * adversarial legs — satisfied by a comment, by an `#if 0` block, by a
	 * string literal — while the real code was neutralised. A call cannot be
	 * satisfied by text: if the detector disappears or weakens, this fails to
	 * compile or fails here. */
	{
		/* THE RESET. From the reference's own boot trace (tools/parse_spi.py,
		 * which now prints every frame's hex): a nine-byte read of register 0
		 * answers with exactly these bytes, and the parser labels them
		 * RESET_RSP. High nibble 3, sync present. Two of my own changes once
		 * rejected this frame as an "idle" one and promoted the drain below to
		 * a reset — the device behaved correctly the whole time. */
		static const uint8_t reset[9] = {
			0xff, 0xff, 0xff, 0xff, 0xff, 0x32, 0x10, 0x00, 0x5a
		};
		/* THE DRAIN. The second read in the reference answers with this; it is
		 * the reset's gutted body, not a second reset, and it carries no sync
		 * so it is not a frame at all. */
		static const uint8_t drain[9] = {
			0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x00, 0x00, 0x00
		};
		/* A 3 with anything else behind it is data, not a reset. */
		static const uint8_t data3[9] = {
			0xff, 0xff, 0xff, 0xff, 0xff, 0x03, 0x01, 0x02, 0x03
		};
		/* The ordinary path still works: 0x12 before the sync → type 1, DATA */
		static const uint8_t data[7] = {
			0x00, 0x00, 0x00, 0x12, 0x34, 0x00, 0x5a
		};
		int off = -1;

		CHECK(spi_hid_protocol_frame_type(reset, sizeof(reset), &off) == 3,
		      "the reference's RESET_RSP 32 10 00 5a types as 3");
		CHECK(off == 5, "and its frame starts where the preamble ends");
		CHECK(spi_hid_protocol_frame_type(drain, sizeof(drain), NULL) == -1,
		      "the drain 03 00 00 00 is not a reset of its own — no sync, no frame");
		CHECK(spi_hid_protocol_frame_type(data3, sizeof(data3), NULL) == -1,
		      "a bare 3 with data behind it is not a reset");
		off = -1;
		CHECK(spi_hid_protocol_frame_type(data, sizeof(data), &off) == 1,
		      "an ordinary sync frame still types as DATA");
		/* The panel's own answer family, verbatim from the field bundles: a
		 * three-byte native prefix (01 <status> EE) followed by the same frame
		 * the reference device sends. The header sits at offset 8 and the sync
		 * at 11 — invisible to a nine-byte read, which is why no field payload
		 * was ever typed. Measured with a harness over this header. */
		static const unsigned char prefixed_reset[12] = {
			0x01, 0xff, 0xee, 0xff, 0xff, 0xff, 0xff, 0xff, 0x32, 0x10,
			0x00, 0x5a };
		static const unsigned char prefixed_desc[12] = {
			0x01, 0x07, 0xee, 0xff, 0xff, 0xff, 0xff, 0xff, 0x72, 0x80,
			0x00, 0x5a };

		off = -1;
		CHECK(spi_hid_protocol_frame_type(prefixed_reset, sizeof(prefixed_reset), &off) == 3,
		      "the panel's prefixed reset types as 3");
		CHECK(off == 8, "and its frame starts three bytes past the reference offset");
		off = -1;
		CHECK(spi_hid_protocol_frame_type(prefixed_desc, sizeof(prefixed_desc), &off) == 7,
		      "the panel's prefixed descriptor types as 7");
		CHECK(off == 8, "at the same offset");
	}
}

int main(void)
{
	fprintf(stderr, "wire_frames_test: running...\n");

	test_default_frames_match_windows();
	test_doubled_frames_unchanged();
	test_frame_shape();
	test_getfeat6_layout();
	test_f32_formatting();
	test_driver_uses_header();
	test_read_approval_frame();

	fprintf(stderr, "wire_frames_test: %u assertions, %u failures\n",
		g_passed, g_failed);
	return g_failed ? 1 : 0;
}
