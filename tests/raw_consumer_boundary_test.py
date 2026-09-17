#!/usr/bin/env python3
"""Keep raw payload handling isolated from transport ownership.

Producer-side checks read the CODE view (comments, strings and disabled
preprocessor blocks stripped — P16 wave: a trailing comment carrying the
handler marker plus both needles kept the ordering pin green while the real
guard was deleted)."""
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from driver_source_sanity_test import code_view

root = Path(__file__).parents[1]
core = code_view((root / "driver" / "spi-hid-core.c").read_text())
raw = code_view((root / "driver" / "mshw0231-raw.c").read_text())
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
# The poller's handshake guard is the SAME gate on the other consume path,
# and it was wholly unpinned (P16 wave, A:C4): replacing it with
# `if (shid->raw_mode_active)` retired the handshake on ANY frame — the exact
# defect the handler's own comment warns about — with this test and the
# sanity test both green (in-tree proven). Mirror the handler's ordering pin
# inside the poller slice: the confirming-frame guard comes first.
poll = core.rsplit("static void spi_hid_poll_work", 1)[1].split("\n}", 1)[0]
assert "!shid->raw_handshake_confirmed" in poll
assert "spi_hid_protocol_raw_confirms_handshake(shid->data_buf[7], rl)" in poll
assert poll.index("!shid->raw_handshake_confirmed") < poll.index(
    "mshw0231_raw_consume_v0(shid, &shid->data_buf[5]")
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
