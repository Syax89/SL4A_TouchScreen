#!/usr/bin/env python3
"""Extract SPB/GPIO transactions from Windows ETW CSV traces."""

from __future__ import annotations

import argparse
import csv
import re
from dataclasses import dataclass, field
from pathlib import Path

HEX_RE = re.compile(r"0x([0-9a-fA-F]+)")


def describe(data: bytes) -> str:
    """Return a conservative description of a V0 response header."""
    for offset in range(max(0, len(data) - 3)):
        if data[offset + 3] != 0x5A or (data[offset] & 0x0F) != 2:
            continue
        report_type = data[offset] >> 4
        body_length = ((data[offset + 1] >> 4) | (data[offset + 2] << 4)) * 4
        return f"v0 type=0x{report_type:X} body={body_length}B header={offset}"
    return "no V0 header"


@dataclass
class Transfer:
    direction: str
    length: int
    data: bytes = b""


@dataclass
class Transaction:
    index: int
    activity: str
    start_time: int
    submitter_pid: int = 0
    submitter_tid: int = 0
    total_len: int = 0
    transfer_count: int = 0
    connection: str = ""
    transfers: list[Transfer] = field(default_factory=list)


def user_data(row: list[str]) -> list[str]:
    return [c.strip() for c in row[19:]]


def parse_int(s: str, default: int = 0) -> int:
    s = s.strip().strip('"')
    try:
        return int(s, 0)
    except ValueError:
        return default


def parse_payload_hex(fields: list[str]) -> bytes:
    for field in reversed(fields):
        match = HEX_RE.search(field)
        if not match:
            continue
        hexstr = match.group(1)
        if len(hexstr) % 2:
            hexstr = "0" + hexstr
        return bytes.fromhex(hexstr)
    return b""


def parse_csv(path: Path) -> tuple[list[Transaction], list[tuple[int, str, list[str]]]]:
    transactions: list[Transaction] = []
    gpio_events: list[tuple[int, str, list[str]]] = []
    current: Transaction | None = None
    current_td: Transfer | None = None
    last_connection_by_activity: dict[str, str] = {}
    last_submitter_by_activity: dict[str, tuple[int, int]] = {}

    with path.open(newline="", encoding="utf-8", errors="replace") as f:
        reader = csv.reader(f, skipinitialspace=True)
        for row in reader:
            if len(row) < 19 or row[0].strip() == "Event Name":
                continue

            provider = row[0].strip()
            event_type = row[1].strip()
            activity = row[14].strip()
            clock = parse_int(row[16])
            fields = user_data(row)

            if "GPIO-ClassExtension" in provider and event_type.startswith("Interrupt"):
                gpio_events.append((clock, event_type, fields))
                continue

            if "SPB-ClassExtension" not in provider:
                continue

            if event_type == "Start" and len(fields) >= 2:
                # UserData: handle, connection-id, ...
                if fields[1].startswith('"') or re.fullmatch(r"[0-9a-fA-F]+", fields[1].strip('"')):
                    last_connection_by_activity[activity] = fields[1].strip('"')
                last_submitter_by_activity[activity] = (parse_int(row[9]), parse_int(row[10]))

            elif event_type == "IoSpbPayloadStart":
                current = Transaction(
                    index=len(transactions),
                    activity=activity,
                    start_time=clock,
                    submitter_pid=last_submitter_by_activity.get(activity, (0, 0))[0],
                    submitter_tid=last_submitter_by_activity.get(activity, (0, 0))[1],
                    total_len=parse_int(fields[0]) if fields else 0,
                    transfer_count=parse_int(fields[1]) if len(fields) > 1 else 0,
                    connection=last_connection_by_activity.get(activity, ""),
                )
                current_td = None

            elif event_type == "IoSpbPayloadTdStart" and current is not None:
                direction = fields[1].strip('" ') if len(fields) > 1 else ""
                length = parse_int(fields[2]) if len(fields) > 2 else 0
                current_td = Transfer(direction=direction, length=length)
                current.transfers.append(current_td)

            elif event_type == "IoSpbPayloadTdBuffer" and current_td is not None:
                current_td.data = parse_payload_hex(fields)

            elif event_type == "IoSpbPayloadStop" and current is not None:
                transactions.append(current)
                current = None
                current_td = None

    return transactions, gpio_events


def tx_opcode(tx: bytes) -> str:
    return f"0x{tx[0]:02X}" if tx else "--"


def summarize(txn: Transaction) -> str:
    tx = next((t for t in txn.transfers if t.direction.startswith("ToDevice")), None)
    rx = next((t for t in txn.transfers if t.direction.startswith("FromDevice")), None)
    tx_data = tx.data if tx else b""
    rx_data = rx.data if rx else b""
    parts = [
        f"#{txn.index:04d}",
        f"t={txn.start_time}",
        f"pid={txn.submitter_pid or '?'}",
        f"tid={txn.submitter_tid or '?'}",
        f"conn={txn.connection or '?'}",
        f"xfers={txn.transfer_count}",
        f"op={tx_opcode(tx_data)}",
        f"tx_len={len(tx_data)}",
        f"rx_len={len(rx_data)}",
    ]
    if tx_data:
        parts.append(f"tx={tx_data[:16].hex(' ').upper()}")
    if rx_data:
        parts.append(f"rx={rx_data[:16].hex(' ').upper()}")
        parts.append(describe(rx_data))
    return " | ".join(parts)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv", type=Path)
    parser.add_argument("--connection", help="substring filter for connection id")
    parser.add_argument("--limit", type=int, default=80)
    parser.add_argument("--gpio", action="store_true", help="also print GPIO interrupt events")
    args = parser.parse_args()

    transactions, gpio_events = parse_csv(args.csv)
    count = 0
    for txn in transactions:
        if args.connection and args.connection not in txn.connection:
            continue
        print(summarize(txn))
        count += 1
        if args.limit and count >= args.limit:
            break

    if args.gpio:
        print("\nGPIO interrupt events:")
        for clock, event_type, fields in gpio_events[: args.limit or None]:
            print(f"t={clock} | {event_type} | {', '.join(fields)}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
