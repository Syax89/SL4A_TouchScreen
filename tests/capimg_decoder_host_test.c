/* SPDX-License-Identifier: GPL-2.0 */
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "../driver/spi-hid-capimg.h"

#define BODY_LENGTH 4304
#define PAYLOAD_OFFSET 5
#define CONTAINER_LENGTH 4297
#define HEADER_LENGTH 7
#define HEATMAP_OFFSET HEADER_LENGTH
#define HEATMAP_LENGTH (16 + SPI_HID_CAPIMG_RASTER_SAMPLES)
#define VENDOR_OFFSET (HEATMAP_OFFSET + HEATMAP_LENGTH)
#define VENDOR_LENGTH (CONTAINER_LENGTH - VENDOR_OFFSET)

static unsigned int passed;
static unsigned int failed;

#define CHECK(condition, message) do { \
	passed++; \
	if (!(condition)) { \
		failed++; \
		fprintf(stderr, "FAIL: %s (%s:%d)\n", message, __FILE__, __LINE__); \
	} \
} while (0)

static void put_le16(uint8_t *destination, uint16_t value)
{
	destination[0] = (uint8_t)value;
	destination[1] = (uint8_t)(value >> 8);
}

static void put_le32(uint8_t *destination, uint32_t value)
{
	destination[0] = (uint8_t)value;
	destination[1] = (uint8_t)(value >> 8);
	destination[2] = (uint8_t)(value >> 16);
	destination[3] = (uint8_t)(value >> 24);
}

static void make_valid_body(uint8_t body[BODY_LENGTH])
{
	uint8_t *payload = body + PAYLOAD_OFFSET;

	memset(body, 0, BODY_LENGTH);
	body[0] = 0xce;
	body[1] = 0x10;
	body[2] = 0x0c;
	put_le32(payload, CONTAINER_LENGTH);
	put_le32(payload + HEATMAP_OFFSET, HEATMAP_LENGTH);
	put_le16(payload + HEATMAP_OFFSET + 4, 0x0100);
	payload[HEATMAP_OFFSET + 6] = 1;
	payload[HEATMAP_OFFSET + 7] = 8;
	put_le32(payload + HEATMAP_OFFSET + 12, SPI_HID_CAPIMG_RASTER_SAMPLES);
	for (size_t i = 0; i < SPI_HID_CAPIMG_RASTER_SAMPLES; i++)
		payload[HEATMAP_OFFSET + 16 + i] = (uint8_t)i;
	put_le32(payload + VENDOR_OFFSET, VENDOR_LENGTH);
	put_le16(payload + VENDOR_OFFSET + 4, 0xff00);
}

static void test_valid_v0_layout(void)
{
	uint8_t body[BODY_LENGTH];
	struct spi_hid_capimg_raster raster = {0};

	make_valid_body(body);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == 0,
	      "valid V0 layout is accepted");
	CHECK(raster.samples == body + PAYLOAD_OFFSET + HEATMAP_OFFSET + 16,
	      "decoded raster aliases the heatmap samples");
	CHECK(raster.samples[0] == 0 && raster.samples[3455] == 127,
	      "decoded raster contains the heatmap sample bytes");
}

static void test_reject_truncated_body_or_container(void)
{
	uint8_t body[BODY_LENGTH];
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	CHECK(spi_hid_capimg_decode_v0(body, 4299, &raster) == -EINVAL,
	      "body shorter than the accepted V0 range is rejected");
	CHECK(spi_hid_capimg_decode_v0(body, 4300, &raster) == -EINVAL,
	      "container extending beyond a truncated body is rejected");
}

static void test_reject_duplicate_sections(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le16(payload + VENDOR_OFFSET + 4, 0x0100);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "duplicate heatmap section is rejected");

	make_valid_body(body);
	put_le32(payload + VENDOR_OFFSET, HEADER_LENGTH);
	put_le32(payload + VENDOR_OFFSET + HEADER_LENGTH,
		 VENDOR_LENGTH - HEADER_LENGTH);
	put_le16(payload + VENDOR_OFFSET + HEADER_LENGTH + 4, 0xff00);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "duplicate vendor section is rejected");
}

static void test_reject_invalid_section_lengths(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET, HEADER_LENGTH - 1);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "section shorter than its header is rejected");

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET, CONTAINER_LENGTH - HEATMAP_OFFSET + 1);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "section longer than the remaining container is rejected");
}

static void test_tracked_v0_frames(void)
{
	static const char *const fixtures[] = {
		"../captures/id5-20260718/frames/raw-slot-0.v0",
		"../captures/id5-20260718/frames/raw-slot-1.v0",
		"../captures/id5-20260718/frames/raw-slot-2.v0",
		"../captures/id5-20260718/frames/raw-slot-3.v0",
		"../captures/id5-20260718/frames/raw-slot-4.v0",
		"../captures/id5-20260718/frames/raw-slot-5.v0",
		"../captures/id5-20260718/frames/raw-slot-6.v0",
		"../captures/id5-20260718/frames/raw-slot-7.v0",
	};
	unsigned int decoded = 0;

	for (size_t i = 0; i < sizeof(fixtures) / sizeof(fixtures[0]); i++) {
		uint8_t body[BODY_LENGTH];
		struct spi_hid_capimg_raster raster;
		FILE *fixture = fopen(fixtures[i], "rb");

		CHECK(fixture != NULL, "tracked V0 fixture opens");
		if (!fixture)
			continue;
		CHECK(fread(body, 1, sizeof(body), fixture) == sizeof(body),
		      "tracked V0 fixture has expected body length");
		CHECK(fgetc(fixture) == EOF, "tracked V0 fixture has no trailing bytes");
		fclose(fixture);
		CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == 0,
		      "tracked V0 fixture decodes with the driver decoder");
		decoded++;
	}
	CHECK(decoded > 0, "at least one tracked fixture was decoded");
}

static void test_reject_null_pointers(void)
{
	uint8_t body[BODY_LENGTH];
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	CHECK(spi_hid_capimg_decode_v0(NULL, sizeof(body), &raster) == -EINVAL,
	      "NULL body is rejected");
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), NULL) == -EINVAL,
	      "NULL raster is rejected");
}

static void test_reject_bad_magic_byte(void)
{
	uint8_t body[BODY_LENGTH];
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	body[1] = 0x00;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "wrong magic byte 0x00 rejected");
	body[1] = 0x11;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "wrong magic byte 0x11 rejected");
	body[1] = 0x10;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == 0,
	      "correct magic byte 0x10 accepted");
}

static void test_reject_bad_content_id(void)
{
	uint8_t body[BODY_LENGTH];
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	body[2] = 0x00;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "content_id 0x00 rejected");
	body[2] = 0x08;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "content_id 0x08 rejected");
}

static void test_reject_bad_container_length(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le32(payload, HEADER_LENGTH - 1);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "container < HEADER_LENGTH rejected");

	make_valid_body(body);
	put_le32(payload, CONTAINER_LENGTH + 1);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "container > V0_USAGE_61_LENGTH rejected");

	make_valid_body(body);
	put_le32(payload, 0);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "container length 0 rejected");

	make_valid_body(body);
	put_le32(payload, HEADER_LENGTH);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "container == HEADER_LENGTH (too small for sections) rejected");
}

static void test_body_length_boundaries(void)
{
	uint8_t body[BODY_LENGTH];
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	CHECK(spi_hid_capimg_decode_v0(body, 4299, &raster) == -EINVAL,
	      "body_length=4299 rejected (below min)");
	CHECK(spi_hid_capimg_decode_v0(body, 4300, &raster) == -EINVAL,
	      "body_length=4300 (container >= body) rejected");

	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == 0,
	      "body_length=4304 accepted");

	CHECK(spi_hid_capimg_decode_v0(body, 4400, &raster) == 0,
	      "body_length=4400 accepted (max valid)");
	CHECK(spi_hid_capimg_decode_v0(body, 4401, &raster) == -EINVAL,
	      "body_length=4401 rejected (above max)");
	CHECK(spi_hid_capimg_decode_v0(body, 4400, &raster) == 0,
	      "body_length=4400 re-accepted (boundary stable)");
}

static void test_reject_reserved_fields(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le16(payload + 4, 1);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "reserved u16 at payload+4 non-zero rejected");

	make_valid_body(body);
	payload[HEATMAP_OFFSET + 6] = 0x00;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "reserved version byte non-1 rejected");

	make_valid_body(body);
	payload[HEATMAP_OFFSET + 7] = 0x10;
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "reserved BBP byte non-8 rejected");

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET + 8, 1);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "reserved dword at heatmap+8 non-zero rejected");
}

static void test_reject_wrong_raster_samples(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET + 12, 3455);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "wrong raster sample count (3455) rejected");

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET + 12, 3457);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "wrong raster sample count (3457) rejected");

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET + 12, 0);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "zero raster sample count rejected");
}

static void test_reject_unrecognized_section_type(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le16(payload + HEATMAP_OFFSET + 4, 0x0200);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "unknown section type 0x0200 rejected");

	make_valid_body(body);
	put_le16(payload + HEATMAP_OFFSET + 4, 0x0000);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "section type 0x0000 rejected");
}

static void test_reject_missing_section(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le32(payload, CONTAINER_LENGTH - VENDOR_LENGTH);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "single-section (no vendor) frame rejected — both required");

	memset(payload, 0, CONTAINER_LENGTH);
	put_le32(payload, CONTAINER_LENGTH);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "all-zero container (no sections) rejected");
}

static void test_reject_section_length_zero(void)
{
	uint8_t body[BODY_LENGTH];
	uint8_t *payload = body + PAYLOAD_OFFSET;
	struct spi_hid_capimg_raster raster;

	make_valid_body(body);
	put_le32(payload + HEATMAP_OFFSET, 0);
	CHECK(spi_hid_capimg_decode_v0(body, sizeof(body), &raster) == -EINVAL,
	      "section length zero rejected");
}

int main(void)
{
	fprintf(stderr, "capimg_decoder_host_test: running...\n");
	test_valid_v0_layout();
	test_reject_truncated_body_or_container();
	test_reject_duplicate_sections();
	test_reject_invalid_section_lengths();
	test_tracked_v0_frames();
	test_reject_null_pointers();
	test_reject_bad_magic_byte();
	test_reject_bad_content_id();
	test_reject_bad_container_length();
	test_body_length_boundaries();
	test_reject_reserved_fields();
	test_reject_wrong_raster_samples();
	test_reject_unrecognized_section_type();
	test_reject_missing_section();
	test_reject_section_length_zero();
	fprintf(stderr, "capimg_decoder_host_test: %u assertions, %u failures\n",
		passed, failed);
	return failed != 0;
}
