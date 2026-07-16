#!/usr/bin/env python3
"""Summarize immutable V0 trace facts without accessing hardware."""

from __future__ import annotations

import argparse
import hashlib
import json
import statistics
from collections import Counter
from pathlib import Path

from parse_spb_csv import Transaction, parse_csv


EXPECTED_SHA256 = {
    "surface_boot_auto.csv": "2b8d965fb223bc7350ffae58f775ef820200c2b0c0ba3a6e7241511440452fb2",
    "surface_init.csv": "ab312f0d481455334fff9017d888034441a30cb2ea6f3b93376b11a8e963677a",
    "surface_touch.csv": "6733f1db5f54a9108ed2455e0607eb5244d0b0336b8df7725692df4c55d44e10",
}


SPI_READ_PREFIX = b"\xff" * 5


def response_header(data: bytes) -> tuple[int, int] | None:
    """Decode the V0 header in a dedicated five-prefix-byte read response."""
    if len(data) != 9 or data[:5] != SPI_READ_PREFIX:
        return None
    header = data[5:]
    if header[3] != 0x5A or (header[0] & 0x0F) != 2:
        return None
    return header[0] >> 4, ((header[1] >> 4) | (header[2] << 4)) * 4


def transfers(txn: Transaction) -> tuple[bytes, bytes]:
    tx = next((item.data for item in txn.transfers
               if item.direction.startswith("ToDevice")), b"")
    rx = next((item.data for item in txn.transfers
               if item.direction.startswith("FromDevice")), b"")
    return tx, rx


def summarize(path: Path) -> dict[str, object]:
    digest = hashlib.sha256(path.read_bytes()).hexdigest()
    if digest != EXPECTED_SHA256[path.name]:
        raise ValueError(f"immutable trace hash mismatch: {path}")

    transactions, _ = parse_csv(path)
    opcodes: Counter[str] = Counter()
    response_types: Counter[str] = Counter()
    data_body_lengths: Counter[str] = Counter()
    data_content_ids: Counter[str] = Counter()
    data_capture: Counter[str] = Counter()
    raw_frames: list[dict[str, object]] = []
    raw_samples: list[tuple[int, int, bytes]] = []
    vectors: list[dict[str, object]] = []
    pending_headers: dict[str, tuple[int, int, int]] = {}

    for txn in transactions:
        tx, rx = transfers(txn)
        if tx:
            opcodes[f"0x{tx[0]:02x}"] += 1
            if tx in (
                bytes.fromhex("02000003420004030006"),
                bytes.fromhex("0200000382000304000501000000"),
            ):
                vectors.append({"index": txn.index, "time_100ns": txn.start_time,
                                "connection": txn.connection, "tx": tx.hex()})
        header = response_header(rx)
        if header:
            report_type, body_length = header
            response_types[f"0x{report_type:x}"] += 1
            pending_headers[txn.connection] = (txn.index, report_type, body_length)
            continue

        pending = pending_headers.pop(txn.connection, None)
        if not pending:
            continue
        header_index, report_type, body_length = pending
        if len(rx) != 5 + body_length or rx[:5] != SPI_READ_PREFIX:
            if report_type == 1:
                data_capture["truncated"] += 1
            continue

        body = rx[5:]
        if report_type != 1:
            continue
        data_body_lengths[str(body_length)] += 1
        data_capture["complete"] += 1
        if len(body) < 3:
            continue
        data_content_ids[f"0x{body[2]:02x}"] += 1
        total_length = body[0] | (body[1] << 8)
        if body[2] != 0x0C:
            continue
        content_length = total_length - 3 if 3 <= total_length <= len(body) else None
        raw_frames.append({
            "header_index": header_index,
            "body_index": txn.index,
            "time_100ns": txn.start_time,
            "connection": txn.connection,
            "aligned_body_length": body_length,
            "total_length": total_length,
            "content_id": f"0x{body[2]:02x}",
            "content_length": content_length,
            "content_sha256": hashlib.sha256(body[3:total_length]).hexdigest()
            if content_length is not None else None,
            "tail_hex": body[total_length:].hex() if content_length is not None else None,
        })
        raw_samples.append((transactions[header_index].start_time, txn.start_time,
                            body[3:total_length]))

    return {
        "trace": path.name,
        "sha256": digest,
        "transactions": len(transactions),
        "opcodes": dict(sorted(opcodes.items())),
        "response_types": dict(sorted(response_types.items())),
        "data_body_lengths": dict(sorted(data_body_lengths.items(), key=lambda item: int(item[0]))),
        "data_content_ids": dict(sorted(data_content_ids.items())),
        "data_body_capture": dict(sorted(data_capture.items())),
        "frozen_feature_vectors": vectors,
        "raw_0c_frames": raw_frames,
        "_raw_samples": raw_samples,
    }


SEGMENTS = ((0, 2), (2, 25), (25, 3481), (3481, 3494),
            (3494, 3590), (3590, 4299))


def summarize_falsification(reports: list[dict[str, object]]) -> dict[str, object]:
    per_trace = {}
    all_payloads = []

    for report in reports:
        samples = report.pop("_raw_samples")
        payloads = [sample[2] for sample in samples]
        body_times = [sample[1] for sample in samples]
        header_to_body = [sample[1] - sample[0] for sample in samples]
        deltas = [later - earlier for earlier, later in zip(body_times, body_times[1:])]
        runs = []
        run_length = 1
        for delta in deltas:
            if delta > 200000:
                runs.append(run_length)
                run_length = 1
            else:
                run_length += 1
        if payloads:
            runs.append(run_length)

        transitions = []
        for start, end in SEGMENTS:
            changed = 0
            changed_bytes = 0
            for before, after in zip(payloads, payloads[1:]):
                difference = sum(a != b for a, b in zip(before[start:end], after[start:end]))
                changed += difference > 0
                changed_bytes += difference
            transitions.append([changed, changed_bytes])

        per_trace[report["trace"]] = {
            "payload_stream_sha256": hashlib.sha256(b"".join(payloads)).hexdigest(),
            "cadence_20ms_runs": runs,
            "body_delta_ticks": [min(deltas), statistics.median(deltas), max(deltas)],
            "header_to_body_ticks": [min(header_to_body), statistics.median(header_to_body),
                                       max(header_to_body)],
            "transitions": transitions,
        }
        all_payloads.extend(payloads)

    segments = []
    for start, end in SEGMENTS:
        spans = [payload[start:end] for payload in all_payloads]
        unique = set(spans)
        variable_offsets = sum(len({span[offset] for span in spans}) > 1
                               for offset in range(end - start))
        segments.append({
            "payload_range": [start, end],
            "unique_spans": len(unique),
            "variable_byte_offsets": variable_offsets,
            "static_sha256": hashlib.sha256(next(iter(unique))).hexdigest()
            if len(unique) == 1 else None,
        })

    return {"per_trace": per_trace, "segments": segments}


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--traces", type=Path, default=Path(__file__).parents[1] / "traces")
    args = parser.parse_args()
    reports = [summarize(args.traces / name) for name in EXPECTED_SHA256]
    print(json.dumps({"traces": reports, "falsification": summarize_falsification(reports)},
                     indent=2, sort_keys=True))
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
