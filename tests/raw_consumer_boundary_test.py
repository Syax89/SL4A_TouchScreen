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
# Anchor the ordering INSIDE the DONE handler that consumes. The first
# `!shid->raw_handshake_confirmed` occurrence sits ~3000 lines earlier in an
# unrelated function, so the whole-file index comparison was satisfied before
# the handler's guard was even written — a leg deleted the guard at :3224 and
# this pin stayed green. rsplit: the first occurrence is the forward
# declaration, which has no body.
handler = core.rsplit("static void seq_handle_data(", 1)[1].split("\n}", 1)[0]
assert handler.index("!shid->raw_handshake_confirmed") < handler.index(
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
