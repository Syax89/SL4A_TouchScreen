#!/usr/bin/env python3
"""Contract tests for the offline tools against the driver's own rules.

Guards the P9 fix classes:
  - surface_tracker.c590 must use the driver's C590_STEP_NUM/STEP_DEN
    (22204/1000, round-half) integer form, not a truncated step;
  - surface_tracker.decode_raster must accept/reject exactly what the
    shared driver-faithful decoder does (no vendor-first drops, no
    CE-magic-blind acceptance, no section-length blindness);
  - parse_spi must label buffer direction from the TdStart rows and
    concatenate every buffer of one direction.
"""
import csv
import struct
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

from decode_v0_capimg import decode_body  # noqa: E402
import parse_spi  # noqa: E402
import surface_tracker  # noqa: E402

RASTER = 3456
VENDOR = bytes(range(104))


def make_body(vendor_first=False, with_vendor=True, magic=b"\xce\x10\x0c",
              heatmap_len=None):
    """Build one 4304-byte V0 body: container, heatmap 0x0100, vendor 0xff00."""
    header_len = heatmap_len if heatmap_len is not None else 16 + RASTER
    heat_sec = (struct.pack("<IHBBII", header_len, 0x0100, 1, 8, 0, RASTER)
                + bytes([0x40]) * RASTER)
    vend_sec = struct.pack("<IHB", 7 + len(VENDOR), 0xFF00, 0) + VENDOR
    parts = [heat_sec] + ([vend_sec] if with_vendor else [])
    if vendor_first:
        parts.reverse()
    sections = b"".join(parts)
    payload = struct.pack("<IHB", 7 + len(sections), 0, 0) + sections
    body = bytearray(4304)
    body[0:3] = magic
    body[5:5 + len(payload)] = payload
    return bytes(body)


def test_c590_matches_the_driver_integer_form():
    # Anchors re-derived from the driver's formula:
    #   10000 - ((raw*22204 + 500)//1000 + 6000), clamped at 0
    anchors = {0: 4000, 12: 3734, 23: 3489, 100: 1780, 180: 3, 255: 0}
    for raw, expected in anchors.items():
        got = surface_tracker.c590(raw)
        assert got == expected, f"c590({raw}) = {got}, expected {expected}"


def test_decode_raster_matches_the_shared_decoder():
    body = make_body()
    raster = surface_tracker.decode_raster(body)
    assert raster == decode_body(body).raster
    assert len(raster) == RASTER


def test_decode_raster_accepts_vendor_first():
    body = make_body(vendor_first=True)
    assert surface_tracker.decode_raster(body) == decode_body(body).raster


def test_decode_raster_rejects_what_the_driver_rejects():
    assert surface_tracker.decode_raster(make_body(magic=b"\x00\x00\x0c")) is None
    assert surface_tracker.decode_raster(make_body(with_vendor=False)) is None
    assert surface_tracker.decode_raster(make_body(heatmap_len=8)) is None
    assert surface_tracker.decode_raster(b"short") is None


def _csv_rows():
    # The direction cell mirrors the REAL capture: the Windows writer emits
    # ` "ToDevice "` — quoted AND space-padded — and csv.writer re-quotes it
    # on the way in, so the parser sees the same cell the captures carry.
    # The old fixture (plain "ToDevice ") passed while the tool on a real
    # file labelled every buffer RX: the quotes stayed in the value and
    # startswith('ToDevice') was false.
    def row(etype, **cols):
        r = [""] * 22
        r[0] = "provider"
        r[1] = etype
        for idx, val in cols.items():
            r[int(idx)] = val
        return r

    return [
        row("IoSpbPayloadStart", **{"16": "1000", "19": "5", "20": "3"}),
        row("IoSpbPayloadTdStart", **{"16": "1001", "20": "FromDevice ", "21": "1"}),
        row("IoSpbPayloadTdBuffer", **{"16": "1002", "21": "0xAA"}),
        row("IoSpbPayloadTdStart", **{"16": "1003", "20": "ToDevice ", "21": "2"}),
        row("IoSpbPayloadTdBuffer", **{"16": "1004", "21": "0xBB"}),
        row("IoSpbPayloadTdBuffer", **{"16": "1005", "21": "0xCC"}),
        row("IoSpbPayloadStop", **{"16": "1006"}),
    ]


def test_parse_spi_direction_and_append():
    with tempfile.NamedTemporaryFile("w", suffix=".csv", newline="",
                                     delete=False) as tmp:
        writer = csv.writer(tmp)
        writer.writerow(["Event Name", "Type"])
        writer.writerows(_csv_rows())
        path = tmp.name
    try:
        txns = parse_spi.parse_boot_trace(path)
    finally:
        Path(path).unlink()

    assert len(txns) == 1, f"expected 1 transaction, got {len(txns)}"
    bufs = txns[0]["buffers"]
    assert [d for d, _ in bufs] == ["FromDevice", "ToDevice", "ToDevice"], bufs
    tx, rx = parse_spi.split_buffers(bufs)
    # The FromDevice buffer is FIRST: ordinal guessing would mislabel it TX,
    # and the two ToDevice buffers must concatenate (not keep only the last).
    assert (tx, rx) == (b"\xbb\xcc", b"\xaa"), (tx, rx)


def test_parse_spi_prints_tx_on_the_real_capture():
    """The tool's own claim on the committed capture: TX frames must exist and
    carry the reference's own bytes. spi-hid-protocol.h cites this tool's
    output for the boot frames; before the quote fix the TX set was empty and
    no citation could have been produced."""
    txns = parse_spi.parse_boot_trace(
        str(ROOT / "captures" / "wintrace" / "surface_init.csv"))
    tx = b""
    for t in txns:
        t_tx, _ = parse_spi.split_buffers(t.get("buffers", []))
        tx += t_tx
    assert len(tx) > 300, f"only {len(tx)} TX bytes out of the real capture"
    # The reference's own first read (register 0), and the SET_POWER D0 row.
    assert bytes.fromhex("0b000000ff00000000") in tx, "missing the reference boot read"
    assert bytes.fromhex("02000004820000040001010cee5b") in tx, "missing SET_POWER D0"


def main():
    test_c590_matches_the_driver_integer_form()
    test_decode_raster_matches_the_shared_decoder()
    test_decode_raster_accepts_vendor_first()
    test_decode_raster_rejects_what_the_driver_rejects()
    test_parse_spi_direction_and_append()
    test_parse_spi_prints_tx_on_the_real_capture()
    print("tools_contract_test: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
