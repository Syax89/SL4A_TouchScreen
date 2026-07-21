#!/usr/bin/env python3
"""Keep the dangerous feature experiment to one GET transaction per boot."""

from pathlib import Path
from optional_contract import skip_optional_contract


source = Path(__file__).parents[1] / "driver" / "spi-hid-core.c"
text = source.read_text()
try:
    handler_start = text.index("/* ── State handler: WAIT_FEAT_RESP")
except ValueError:
    skip_optional_contract("WAIT_FEAT_RESP handler marker is not present in this driver snapshot")
handler_end = text.index("/* ── State handler: VENDOR_INIT", handler_start)
handler = text[handler_start:handler_end]
guard = "RAW_TRANSITION: GET-only transaction complete; SET is disabled"

if "send only the observed GET_FEATURE selector 4" not in text:
    skip_optional_contract("raw transition one-shot GET_FEATURE harness is not present")
assert "u8 gf[11]" in text
assert "0x02, 0x02, 0x00, 0x00, 0x03, 0x42" in text
assert "raw_transition_response_show" in text
assert "raw_transition_get_body_len = rblen" in text
assert guard in handler
assert handler.index(guard) < handler.index("spi_hid_seq_write_speed")
assert "spi_hid_seq_set_state(shid, 4, SPI_HID_SEQ_FEATURE_RESPONSE);\n\t\t\treturn;" in handler
timeout = text[text.index("static void spi_hid_raw_transition_timeout_work"):text.index("static void spi_hid_isolated_set_timeout_work")]
assert "RAW_TRANSITION: GET response timeout; returning to standard HID" in timeout
assert "spi_hid_seq_write" not in timeout.split("static void spi_hid_seq_descreq_work", 1)[0]

print("raw transition safety contract: PASS")
