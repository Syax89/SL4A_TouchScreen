#!/usr/bin/env python3
"""Keep the isolated SET experiment separately gated and write-free afterward."""

from pathlib import Path


source = Path(__file__).parents[1] / "driver" / "spi-hid-core.c"
text = source.read_text()
header = (Path(__file__).parents[1] / "driver" / "spi-hid-core.h").read_text()

assert "static bool isolated_set_test;" in text
assert "isolated_set_test requires raw_capture_only=1 and raw_transition_once=0" in text
assert "module_param(isolated_set_test, bool, 0444);" in text
assert "SPI_HID_ISOLATED_SET_OBSERVING" in header
assert "SPI_HID_ISOLATED_SET_RESET" in header
assert "static const u8 get_id6[11]" in text
assert "0x02, 0x02, 0x00, 0x00, 0x03, 0x42," in text
assert "0x00, 0x04, 0x03, 0x00, 0x06," in text
assert "static const u8 set_vector[15]" in text
assert "0x02, 0x02, 0x00, 0x00, 0x03, 0x82, 0x00," in text
assert "0x03, 0x04, 0x00, 0x05, 0x01, 0x00, 0x00, 0x00," in text
assert "usleep_range(4500, 5500);" in text
assert "msecs_to_jiffies(RAW_TRANSITION_DELAY_MS)" in text
assert "shid->seq_state != 4 || !shid->hid" in text
assert "spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_REPORT_DESCRIPTOR);" in text
assert "return -EBUSY;" in text[text.index("static int spi_hid_ll_raw_request"):]
assert "isolated_set_ring" in text
assert "SPI_HID_REPORT_TYPE_COMMAND_RESP" in text
assert "SPI_HID_REPORT_TYPE_GET_FEATURE_RESP" in text
assert "Reset is terminal after the harness arms: never recover or retry." in text
assert "#define ISOLATED_SET_OBSERVATION_TIMEOUT_MS 45000" in text
assert "isolated_set_overflow" in text

print("isolated SET safety contract: PASS")
