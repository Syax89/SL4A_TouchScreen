// SPDX-License-Identifier: GPL-2.0
#include <stdio.h>
#include <stdlib.h>

#include "../driver/spi-hid-protocol.h"

static int failures;

#define CHECK(cond, msg) do { \
	if (!(cond)) { \
		fprintf(stderr, "FAIL: %s\n", msg); \
		failures++; \
	} \
} while (0)

int main(void)
{
	const unsigned int header_len = 4;
	const unsigned int capimg_body_len = 4304;
	unsigned int cursor = header_len;
	unsigned int published = 0;

	/* The field failure: a DATA header is already consumed while
	 * hid_add_device() is still in progress. The body must be drained but
	 * must not be published through HID yet. */
	CHECK(spi_hid_protocol_data_should_drain(1, 1, 1),
	      "hid_creating frame must be drained");
	if (spi_hid_protocol_data_should_drain(1, 1, 1))
		cursor += capimg_body_len;
	if (spi_hid_protocol_hid_publish_ready(1, 1))
		published++;

	CHECK(cursor == header_len + capimg_body_len,
	      "first CapImg body was not fully consumed");
	CHECK(published == 0,
	      "HID report published before hid_add_device completed");

	/* The next frame must begin exactly after the drained body rather than
	 * inside a stale ~4.3 KiB payload. */
	CHECK(cursor == 4308, "next frame is not aligned after drain");

	/* Also cover the smaller pre-hid_creating window: once the tracker exists,
	 * a DATA body must still be drained even if the HID object is not ready. */
	CHECK(spi_hid_protocol_data_should_drain(0, 0, 1),
	      "registered tracker must keep DATA body drain active");

	/* With neither HID lifecycle nor tracker ownership, the handler may ignore
	 * DATA, preserving the default standard-mode behavior. */
	CHECK(!spi_hid_protocol_data_should_drain(0, 0, 0),
	      "unowned DATA should remain ignorable");

	CHECK(spi_hid_protocol_hid_publish_ready(1, 0),
	      "registered HID must publish after creation completes");
	CHECK(!spi_hid_protocol_hid_publish_ready(0, 0),
	      "absent HID must never publish");

	if (failures)
		return EXIT_FAILURE;

	puts("std CapImg DATA lifecycle behavior test: PASS");
	return EXIT_SUCCESS;
}
