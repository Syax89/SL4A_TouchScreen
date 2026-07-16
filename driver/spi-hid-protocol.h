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
#define SPI_HID_PROTOCOL_READ_OPCODE 0x0b
#define SPI_HID_PROTOCOL_WRITE_OPCODE 0x02

/* The only raw frame shape accepted by the passive capture path. */
#define SPI_HID_RAW_CAPTURE_CONTENT_ID 0x0c
#ifndef SPI_HID_RAW_CAPTURE_BODY_LENGTH
#define SPI_HID_RAW_CAPTURE_BODY_LENGTH 4304
#endif
#define SPI_HID_RAW_CAPTURE_TOTAL_LENGTH 4302

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

static inline void spi_hid_protocol_encode_output_header(spi_hid_proto_u8 raw[6],
		unsigned int output_register, spi_hid_proto_u16 output_length)
{
	raw[0] = SPI_HID_PROTOCOL_WRITE_OPCODE;
	raw[1] = output_register >> 16;
	raw[2] = output_register >> 8;
	raw[3] = output_register;
	raw[4] = SPI_HID_PROTOCOL_VERSION | ((output_length & 0x0f) << 4);
	raw[5] = output_length >> 4;
}

static inline void spi_hid_protocol_encode_read_approval(spi_hid_proto_u8 raw[5],
		unsigned int input_register)
{
	raw[0] = SPI_HID_PROTOCOL_READ_OPCODE;
	raw[1] = input_register >> 16;
	raw[2] = input_register >> 8;
	raw[3] = input_register;
	raw[4] = 0xff;
}

static inline int spi_hid_protocol_parse_content(const spi_hid_proto_u8 *body,
		unsigned int body_length, struct spi_hid_protocol_content *content)
{
	spi_hid_proto_u16 total_length;

	if (!body || !content || body_length < 3)
		return -1;

	total_length = body[0] | ((spi_hid_proto_u16)body[1] << 8);
	if (total_length < 3 || total_length > body_length)
		return -1;

	content->total_length = total_length;
	content->content_id = body[2];
	content->data = body + 3;
	content->data_length = total_length - 3;
	return 0;
}

static inline int spi_hid_protocol_validate_raw_capture(
		const spi_hid_proto_u8 *body, unsigned int body_length,
		struct spi_hid_protocol_content *content)
{
	if (body_length != SPI_HID_RAW_CAPTURE_BODY_LENGTH ||
	    spi_hid_protocol_parse_content(body, body_length, content))
		return -1;

	return content->total_length == SPI_HID_RAW_CAPTURE_TOTAL_LENGTH &&
		content->content_id == SPI_HID_RAW_CAPTURE_CONTENT_ID &&
		content->data_length == SPI_HID_RAW_CAPTURE_TOTAL_LENGTH - 3 ? 0 : -1;
}

static inline int spi_hid_protocol_find_header(const spi_hid_proto_u8 *raw,
		int length, int *offset)
{
	int i;

	if (!raw || length < 4)
		return -1;

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

#endif /* SPI_HID_PROTOCOL_H */
