#!/usr/bin/env python3
"""Probe the vendor CLI command channel (Report ID 31 / 0x1f) found embedded
in TouchPenProcessor0C19.dll's GenericCliDescriptor.xml (docs/decomp/).
See docs/GROUND_TRUTH.md §21.5 for the discovery writeup.

Sends a single command as a 60-byte Output report, then does a short,
bounded read for any Input report that comes back. Read-only commands only
(GetVersion/GetDescriptorSize/etc.) -- never send a Set* command with this
script.

Usage: sudo python3 tools/cli_probe.py /dev/hidraw3 <cmd_id> [param_bytes_hex] [--pad=00|ff]
"""
import sys
import os
import select
import struct

REPORT_ID = 0x1f
REPORT_LEN = 60


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <hidraw_dev> <cmd_id> [param_bytes_hex] [--pad=00|ff]")
        sys.exit(1)

    dev_path = sys.argv[1]
    cmd_id = int(sys.argv[2], 0)

    pad_byte = 0x00
    rest = sys.argv[3:]
    params_hex = None
    for a in rest:
        if a.startswith("--pad="):
            pad_byte = int(a.split("=", 1)[1], 16)
        else:
            params_hex = a
    params = bytes.fromhex(params_hex) if params_hex else b""

    payload = bytes([cmd_id]) + params
    if len(payload) > REPORT_LEN:
        print("payload too long")
        sys.exit(1)
    payload = payload.ljust(REPORT_LEN, bytes([pad_byte]))

    buf = bytes([REPORT_ID]) + payload
    assert len(buf) == 1 + REPORT_LEN

    fd = os.open(dev_path, os.O_RDWR)
    try:
        print(f"-> writing {len(buf)} bytes to {dev_path}: {buf.hex()}")
        n = os.write(fd, buf)
        print(f"   write() returned {n}")

        print(f"<- listening up to 5s, filtering for report id 0x{REPORT_ID:02x}...")
        deadline = __import__("time").time() + 5.0
        noise = 0
        while True:
            remaining = deadline - __import__("time").time()
            if remaining <= 0:
                print(f"   (timeout; ignored {noise} unrelated reports, nothing on report id 0x{REPORT_ID:02x})")
                break
            r, _, _ = select.select([fd], [], [], remaining)
            if fd not in r:
                continue
            data = os.read(fd, 4096)
            if data and data[0] == REPORT_ID:
                print(f"   MATCH: got {len(data)} bytes: {data.hex()}")
                break
            else:
                noise += 1
    finally:
        os.close(fd)


if __name__ == "__main__":
    main()
