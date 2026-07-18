#!/usr/bin/env python3
"""Extract the hash-pinned Windows timeline after the canonical ID5 SET."""

from __future__ import annotations

import argparse
import hashlib
from pathlib import Path

from analyze_raw_corpus import EXPECTED_SHA256, response_header, transfers
from parse_spb_csv import parse_csv


SET_ID5 = bytes.fromhex("0200000382000304000501000000")


def parse_body(data: bytes) -> tuple[int, int] | None:
    if len(data) < 8 or data[:5] != b"\xff" * 5:
        return None
    body = data[5:]
    total = body[0] | (body[1] << 8)
    if total < 3 or total > len(body):
        return None
    return total, body[2]


def describe_write(data: bytes) -> str:
    if len(data) < 10 or data[0] != 0x02:
        return data.hex(" ")
    content_type = data[6]
    semantic_length = data[7] | (data[8] << 8)
    if content_type != 0x03 or semantic_length < 4:
        return data.hex(" ")
    content_id = data[9]
    payload_len = semantic_length - 4
    payload = data[10:10 + payload_len]
    return f"set_feature:id=0x{content_id:02x}:data={payload.hex(' ')}"


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "csv",
        nargs="?",
        type=Path,
        default=Path(__file__).parents[1] / "traces" / "surface_boot_auto.csv",
    )
    args = parser.parse_args()

    expected = EXPECTED_SHA256.get(args.csv.name)
    digest = hashlib.sha256(args.csv.read_bytes()).hexdigest()
    if expected and digest != expected:
        raise SystemExit(f"immutable trace hash mismatch: {args.csv}")

    transactions, _ = parse_csv(args.csv)
    set_index = next(
        index for index, transaction in enumerate(transactions)
        if transfers(transaction)[0] == SET_ID5
    )
    set_txn = transactions[set_index]
    pending: tuple[int, int, int] | None = None

    print(f"trace={args.csv.name}")
    print(f"sha256={digest}")
    print(f"set_index={set_txn.index}")
    print(f"set_time_100ns={set_txn.start_time}")
    print(f"connection={set_txn.connection}")
    print(f"set_submitter_pid={set_txn.submitter_pid}")
    print(f"set_submitter_tid={set_txn.submitter_tid}")
    print("kind,delta_ms,transaction,header,aligned_body,semantic_len,content_id,prefix,submitter_pid,submitter_tid")

    for transaction in transactions[set_index + 1:]:
        if transaction.connection != set_txn.connection:
            continue
        tx, rx = transfers(transaction)
        delta_ms = (transaction.start_time - set_txn.start_time) / 10_000
        if tx and tx[0] == 0x02:
            print(
                f"write,{delta_ms:.4f},{transaction.index},,,,,{describe_write(tx)},"
                f"{transaction.submitter_pid},{transaction.submitter_tid}"
            )
        header = response_header(rx)
        if header:
            pending = (transaction.index, header[0], header[1])
            continue
        if not pending:
            continue
        header_index, report_type, body_length = pending
        pending = None
        parsed = parse_body(rx)
        if report_type != 1 or not parsed:
            continue
        total, content_id = parsed
        print(
            f"data,{delta_ms:.4f},{transaction.index},{header_index},"
            f"{body_length},{total},0x{content_id:02x},{rx[5:21].hex(' ')},"
            f"{transaction.submitter_pid},{transaction.submitter_tid}"
        )
        if content_id == 0x0C:
            break

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
