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

int main(void)
{
	fprintf(stderr, "capimg_decoder_host_test: running...\n");
	test_valid_v0_layout();
	test_reject_truncated_body_or_container();
	test_reject_duplicate_sections();
	test_reject_invalid_section_lengths();
	fprintf(stderr, "capimg_decoder_host_test: %u assertions, %u failures\n",
		passed, failed);
	return failed != 0;
}
