/* SPDX-License-Identifier: GPL-2.0 */
#define _DEFAULT_SOURCE
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../driver/spi-hid-protocol.h"

static unsigned int g_passed, g_failed;
#define CHECK(cond, msg) do { \
	g_passed++; \
	if (!(cond)) { g_failed++; fprintf(stderr, "FAIL: %s\n  %s:%d: %s\n", msg, __FILE__, __LINE__, #cond); } \
} while (0)

/* ── Header decode: valid report types ─────────────────────────── */

static void test_decode_all_types(void)
{
	struct spi_hid_protocol_header h;
	const struct {
		spi_hid_proto_u8 raw[4];
		spi_hid_proto_u8 type;
		spi_hid_proto_u16 len;
	} vectors[] = {
		{{0x12,0x10,0x00,0x5a}, 1, 4},    /* DATA, 4B body */
		{{0x32,0x10,0x00,0x5a}, 3, 4},    /* RESET_RSP */
		{{0x42,0x10,0x00,0x5a}, 4, 4},    /* COMMAND_RESP */
		{{0x52,0x10,0x00,0x5a}, 5, 4},    /* GET_FEAT_RESP */
		{{0x72,0x80,0x00,0x5a}, 7, 32},   /* DEVICE_DESC */
		{{0x82,0xb0,0x0e,0x5a}, 8, 940},  /* RPT_DESC, 940B */
	};
	int n = (int)(sizeof(vectors) / sizeof(vectors[0]));

	for (int i = 0; i < n; i++) {
		spi_hid_protocol_decode_header(vectors[i].raw, &h);
		CHECK(h.report_type == vectors[i].type, "decode: report_type");
		CHECK(h.version == 2, "decode: version");
		CHECK(h.report_length == vectors[i].len, "decode: report_length");
		CHECK(h.sync_const == 0x5a, "decode: sync_const");
		CHECK(h.length_reserved == 0, "decode: length_reserved");
		CHECK(h.fragment_id == 0, "decode: fragment_id is zero (V0)");
	}
}

/* ── Header decode: boundary values ────────────────────────────── */

static void test_decode_boundary(void)
{
	struct spi_hid_protocol_header h;
	spi_hid_proto_u8 max_len[] = {0x12, 0xf0, 0xff, 0x5a};
	spi_hid_proto_u8 zero_len[] = {0x12, 0x00, 0x00, 0x5a};
	spi_hid_proto_u8 resvd_set[] = {0x12, 0x1f, 0x00, 0x5a};

	spi_hid_protocol_decode_header(max_len, &h);
	CHECK(h.version == 2, "boundary: version");
	CHECK(h.report_type == 1, "boundary: type");
	CHECK(h.report_length == 16380, "boundary: max_len = 0xFFF * 4 = 16380");
	CHECK(h.length_reserved == 0, "boundary: max_len reserved still 0");

	spi_hid_protocol_decode_header(zero_len, &h);
	CHECK(h.report_length == 0, "boundary: zero_len = 0");
	CHECK(h.length_reserved == 0, "boundary: zero_len reserved");

	spi_hid_protocol_decode_header(resvd_set, &h);
	CHECK(h.length_reserved == 0x0f, "boundary: reserved nibble fully set");
	CHECK(h.report_length == 4, "boundary: length disregards reserved nibble");
}

/* ── Output encoder: boundary registers and lengths ────────────── */

static void test_encode_output(void)
{
	spi_hid_proto_u8 raw[6];
	const spi_hid_proto_u8 descreq[] = {0x02, 0x00, 0x00, 0x01, 0x42, 0x00};
	const spi_hid_proto_u8 max_addr[] = {0x02, 0xff, 0xff, 0xff, 0x42, 0x00};

	/* DESCREQ: register 1, length 4 */
	spi_hid_protocol_encode_output_header(raw, 1, 4);
	CHECK(!memcmp(raw, descreq, 6), "encode: DESCREQ");

	/* Max register address (24-bit) */
	spi_hid_protocol_encode_output_header(raw, 0xFFFFFF, 4);
	CHECK(!memcmp(raw, max_addr, 6), "encode: max register address");

	/* Zero-length output */
	spi_hid_protocol_encode_output_header(raw, 0, 0);
	CHECK(raw[4] == SPI_HID_PROTOCOL_VERSION, "encode: zero-length version byte");
	CHECK(raw[5] == 0, "encode: zero-length len_hi");

	/* Max output length (0xFFF * 4 = 4092) */
	spi_hid_protocol_encode_output_header(raw, 0x100, 4092);
	CHECK(raw[4] == (SPI_HID_PROTOCOL_VERSION | (0xc << 4)), "encode: max len_lo nibble");
	CHECK(raw[5] == 0xff, "encode: max len_hi");
}

/* ── Read approval encoder ─────────────────────────────────────── */

static void test_encode_read(void)
{
	spi_hid_proto_u8 raw[5];
	const spi_hid_proto_u8 zero_reg[] = {0x0b, 0x00, 0x00, 0x00, 0xff};
	const spi_hid_proto_u8 max_reg[] = {0x0b, 0xff, 0xff, 0xff, 0xff};

	spi_hid_protocol_encode_read_approval(raw, 0);
	CHECK(!memcmp(raw, zero_reg, 5), "encode_read: register 0");

	spi_hid_protocol_encode_read_approval(raw, 0xFFFFFF);
	CHECK(!memcmp(raw, max_reg, 5), "encode_read: max register");

	spi_hid_protocol_encode_read_approval(raw, 0x000002);
	CHECK(raw[1] == 0x00 && raw[2] == 0x00 && raw[3] == 0x02, "encode_read: RPT_DESC register");
}

/* ── Header search ─────────────────────────────────────────────── */

static void test_find_header(void)
{
	int off;
	spi_hid_proto_u8 at_zero[] = {0x72, 0x80, 0x00, 0x5a};
	spi_hid_proto_u8 with_garbage[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0x72, 0x80, 0x00, 0x5a};
	spi_hid_proto_u8 multiple[] = {0x32, 0x10, 0x00, 0x5a, 0xff, 0x72, 0x80, 0x00, 0x5a};
	spi_hid_proto_u8 no_match[] = {0x71, 0x80, 0x00, 0x5a, 0xff, 0xff};
	spi_hid_proto_u8 short_buf[] = {0x12, 0x10, 0x00};
	spi_hid_proto_u8 bare_sync[] = {0x01, 0x02, 0x03, 0x5a, 0x04, 0x05, 0x06};
	spi_hid_proto_u8 trailing[] = {0x32, 0x10, 0x00, 0x5a, 0xff, 0xff};

	/* Exact match at start */
	CHECK(spi_hid_protocol_find_header(at_zero, 4, &off) == 7, "search: at offset 0");
	CHECK(off == 0, "search: offset 0");

	/* Match after garbage */
	CHECK(spi_hid_protocol_find_header(with_garbage, 9, &off) == 7, "search: after garbage");
	CHECK(off == 5, "search: offset after garbage");

	/* Multiple headers: first wins */
	CHECK(spi_hid_protocol_find_header(multiple, 9, &off) == 3, "search: first of two");
	CHECK(off == 0, "search: first offset");

	/* No match (wrong version) */
	CHECK(spi_hid_protocol_find_header(no_match, 6, &off) == -1, "search: wrong version");

	/* Buffer too short for a full header */
	CHECK(spi_hid_protocol_find_header(short_buf, 3, NULL) == -1, "search: too short");

	/* Sync byte exists but version nibble doesn't match */
	CHECK(spi_hid_protocol_find_header(bare_sync, 7, NULL) == -1, "search: sync but wrong version");

	/* Match, trailing garbage, NULL offset */
	CHECK(spi_hid_protocol_find_header(trailing, 6, NULL) == 3, "search: NULL offset ok");
}

/* ── Fuzz roundtrip: encode → decode → verify ─────────────────── */

static void test_fuzz_roundtrip(void)
{
	srand((unsigned int)time(NULL));

	for (int i = 0; i < 5000; i++) {
		spi_hid_proto_u8 raw[4];
		struct spi_hid_protocol_header h;

		raw[0] = (spi_hid_proto_u8)((rand() & 0xf0) | 0x02); /* force version=2 */
		raw[1] = (spi_hid_proto_u8)(rand() & 0xf0);           /* reserved nibble = 0 */
		raw[2] = (spi_hid_proto_u8)(rand() & 0xff);
		raw[3] = 0x5a;

		spi_hid_protocol_decode_header(raw, &h);

		/* All decoded fields must be self-consistent */
		CHECK(h.sync_const == 0x5a, "fuzz: sync_const");
		CHECK(h.version == 2, "fuzz: version");
		CHECK(h.length_reserved == 0, "fuzz: reserved zero");
		CHECK(h.fragment_id == 0, "fuzz: fragment_id zero (V0)");

		/* report_type must be in [0, 15] */
		CHECK(h.report_type <= 15, "fuzz: report_type <= 15");

		/* report_length must be a multiple of 4 and <= 4092 */
		CHECK((h.report_length & 3) == 0, "fuzz: len multiple of 4");
		/* report_length = ((raw[1]>>4 | raw[2]<<4) * 4), max 0xFFF*4 = 16380 */
		CHECK(h.report_length <= 16380, "fuzz: len <= 16380");

		/* report_length must be reconstructible from raw bytes */
		spi_hid_proto_u16 len_from_raw = (((spi_hid_proto_u16)raw[1] >> 4) |
						  ((spi_hid_proto_u16)raw[2] << 4)) * 4;
		CHECK(h.report_length == len_from_raw, "fuzz: len matches raw");
	}
}

/* ── Output encoder fuzz roundtrip ─────────────────────────────── */

static void test_fuzz_output_roundtrip(void)
{
	srand((unsigned int)time(NULL) ^ 0xdead);

	for (int i = 0; i < 5000; i++) {
		spi_hid_proto_u8 raw[6];
		spi_hid_proto_u16 reg = (spi_hid_proto_u16)(rand() & 0xFFFFFF);
		/* Max encodable real bytes: 12 bits → 0xFFF = 4095 */
		spi_hid_proto_u16 outlen = (spi_hid_proto_u16)(rand() & 0xFFF);

		spi_hid_protocol_encode_output_header(raw, reg, outlen);

		/* Verify structural invariants */
		CHECK(raw[0] == SPI_HID_PROTOCOL_WRITE_OPCODE, "fuzz_out: opcode");
		CHECK(raw[1] == (spi_hid_proto_u8)(reg >> 16), "fuzz_out: reg_hi");
		CHECK(raw[2] == (spi_hid_proto_u8)(reg >> 8), "fuzz_out: reg_mid");
		CHECK(raw[3] == (spi_hid_proto_u8)(reg), "fuzz_out: reg_lo");
		CHECK((raw[4] & 0x0f) == SPI_HID_PROTOCOL_VERSION, "fuzz_out: version");
		CHECK(raw[5] == (spi_hid_proto_u8)(outlen >> 4), "fuzz_out: len_hi");

		/* The low nibble of raw[4] + raw[5] should encode outlen */
		spi_hid_proto_u16 decoded_len = ((spi_hid_proto_u16)(raw[4] >> 4) & 0xf) |
						((spi_hid_proto_u16)raw[5] << 4);
		CHECK(decoded_len == outlen, "fuzz_out: decoded_len matches");
	}
}

/* ── Find header with offset=NULL ──────────────────────────────── */

static void test_find_header_null_offset(void)
{
	spi_hid_proto_u8 raw[] = {0x12, 0x10, 0x00, 0x5a};
	CHECK(spi_hid_protocol_find_header(raw, 4, NULL) == 1, "null_offset: type");
}

static void test_find_header_too_short(void)
{
	spi_hid_proto_u8 empty[] = {};
	spi_hid_proto_u8 two[] = {0x12, 0x10};
	spi_hid_proto_u8 three[] = {0x12, 0x10, 0x00};

	CHECK(spi_hid_protocol_find_header(empty, 0, NULL) == -1, "short: empty");
	CHECK(spi_hid_protocol_find_header(two, 2, NULL) == -1, "short: 2 bytes");
	CHECK(spi_hid_protocol_find_header(three, 3, NULL) == -1, "short: 3 bytes");
}

int main(void)
{
	fprintf(stderr, "protocol_test: running...\n");

	test_decode_all_types();
	test_decode_boundary();
	test_encode_output();
	test_encode_read();
	test_find_header();
	test_find_header_null_offset();
	test_find_header_too_short();
	test_fuzz_roundtrip();
	test_fuzz_output_roundtrip();

	fprintf(stderr, "protocol_test: %u assertions, %u failures\n",
		g_passed, g_failed);
	return g_failed ? 1 : 0;
}
