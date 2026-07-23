#!/usr/bin/env python3
"""Keep raw payload handling isolated from transport ownership."""
from pathlib import Path

root = Path(__file__).parents[1]
core = (root / "driver" / "spi-hid-core.c").read_text()
raw = (root / "driver" / "mshw0231-raw.c").read_text()
kbuild = (root / "driver" / "Kbuild").read_text()

assert "mshw0231-raw.o" in kbuild
assert "spi_hid_capimg_decode_v0" not in core
assert "mshw0231_raw_consume_v0(shid, &body[5], rblen - 5)" in core
assert core.index("!shid->raw_handshake_confirmed") < core.index(
    "mshw0231_raw_consume_v0(shid, &body[5], rblen - 5)")
assert "mshw0231_raw_consume_v0(shid, &shid->data_buf[5]" in core
assert "spi_hid_capimg_decode_v0" in raw
assert "mshw0231_raw_process_samples" not in core
assert "static void mshw0231_raw_process_samples" in raw
assert "extern void mshw0231_raw_process_samples" not in raw
assert "mshw0231_raw_reset" in raw
assert "mshw0231_raw_input_register" in raw
for forbidden in ("spi_sync", "spi_async", "schedule_", "queue_work",
                  "seq_state", "spi_hid_seq_"):
    assert forbidden not in raw, forbidden

print("raw consumer boundary contract: PASS")
