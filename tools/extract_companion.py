#!/usr/bin/env python3
"""
Extract the companion device (Device B) activation sequence from Windows CSV traces.
Outputs: human-readable timeline, C header, and diagnostic summary.

Usage: python3 extract_companion.py [--csv traces/surface_boot_auto.csv] [--c-header] [--summary]
"""
import sys, os, re, argparse

HEX_RE = re.compile(r'0x([0-9A-Fa-f]+)\s*$')
DIR_RE = re.compile(r'"(ToDevice|FromDevice)\s*"')
ACT_RE = re.compile(r'\{([0-9a-fA-F-]+)\}')
CLK_RE = re.compile(r',\s*(1\d{17})\s*,')
CONN_RE = re.compile(r'"([0-9a-f]{16})"')


class Transfer:
    __slots__ = ('clk', 'conn', 'opcode', 'tx', 'rx')

    def __init__(self, clk, conn, opcode, tx=None, rx=None):
        self.clk = clk
        self.conn = conn
        self.opcode = opcode
        self.tx = tx or ""
        self.rx = rx or ""

    def tx_bytes(self):
        return bytes.fromhex(self.tx) if self.tx else b""

    def rx_bytes(self):
        return bytes.fromhex(self.rx) if self.rx else b""

    def tx_len(self):
        return len(self.tx_bytes())

    def rx_len(self):
        return len(self.rx_bytes())

    def opcode_str(self):
        b = self.tx_bytes()
        if not b:
            return "??"
        return f"0x{b[0]:02X}"


def parse_csv(path):
    act_conn = {}
    pending_dir = None
    events = []
    with open(path, errors="replace") as f:
        next(f)
        for line in f:
            parts = line.split(",", 1)
            name = parts[0].strip()
            ev = parts[1].strip() if len(parts) > 1 else ""
            if "GPIO-ClassExtension" in name and "InterruptInvokeDeviceIsrStart" in line:
                m = CLK_RE.search(line)
                events.append((int(m.group(1)) if m else 0, "IRQ", None, None))
                continue
            if ev == "Start":
                a = ACT_RE.search(line)
                c = CONN_RE.search(line)
                if a and c:
                    act_conn[a.group(1)] = c.group(1)
                continue
            if "IoSpbPayloadTdStart" in line:
                d = DIR_RE.search(line)
                pending_dir = d.group(1) if d else "?"
            elif "IoSpbPayloadTdBuffer" in line:
                m = HEX_RE.search(line.rstrip())
                a = ACT_RE.search(line)
                if m:
                    conn = act_conn.get(a.group(1) if a else "", "?" * 16)
                    events.append((CLK_RE.search(line).group(1) if CLK_RE.search(line) else 0,
                                   pending_dir or "?",
                                   conn,
                                   m.group(1).upper()))

    txns = []
    cur_clk = 0
    cur_conn = None
    cur_tx = None
    for clk_idx, kind, conn, data in events:
        clk = int(clk_idx) if isinstance(clk_idx, str) else clk_idx
        if kind == "IRQ":
            txns.append(Transfer(clk, "--", "IRQ"))
            continue
        if kind == "ToDevice":
            cur_clk = clk
            cur_conn = conn
            cur_tx = data
        else:
            if cur_tx is not None and cur_conn == conn and abs(clk - cur_clk) < 500000:
                txns.append(Transfer(cur_clk, conn, "TXN", cur_tx, data))
                cur_tx = None
            else:
                txns.append(Transfer(clk, conn, "TXN", None, data))
    return txns


COMPANION_CONNS = {"18", "19", "1a", "18", "19", "1A"}


def cshort(c):
    return c[-2:] if len(c) >= 2 else "??"


def extract_companion(txns):
    result = []
    prev = None
    for t in txns:
        cc = cshort(t.conn)
        if cc not in COMPANION_CONNS and cc != "--":
            continue
        gap = f"+{(t.clk - prev.clk) / 10:.0f}us" if prev else ""
        op = t.opcode_str()
        if t.opcode == "IRQ":
            result.append({"type": "IRQ", "gap": gap, "ts": t.clk})
            prev = t
            continue
        result.append({
            "type": "TXN",
            "conn": cc,
            "gap": gap,
            "opcode": op,
            "tx": t.tx_bytes(),
            "rx": t.rx_bytes(),
            "ts": t.clk,
        })
        prev = t
    return result


def print_timeline(seq):
    print(f"{'='*100}")
    print(f"  COMPANION DEVICE (Device B) — Full Activation Sequence")
    print(f"{'='*100}")
    idx = 0
    for s in seq:
        if s["type"] == "IRQ":
            print(f"  {s['gap']:>12s}  [IRQ]")
            continue
        idx += 1
        tx = s["tx"]
        rx = s["rx"]
        label = OPCODES.get(tx[0], f"op{tx[0]:02X}") if tx else "RX-ONLY"
        print(f"  #{idx:3d}  {s['gap']:>10s}  [conn {s['conn']}] [{label:6}]  "
              f"TX={len(tx):4d}B  RX={len(rx):4d}B  "
              f"tx={tx[:28].hex(' ').upper()}{'...' if len(tx)>28 else ''}")
        if rx:
            print(f"       {' '*29}  rx={rx[:28].hex(' ').upper()}{'...' if len(rx)>28 else ''}")
        if tx and tx[0] == 0xB0:
            # FW blocks - compact
            pass


OPCODES = {0x00: "cmd00", 0x70: "cmd70", 0xB0: "FW_BLK",
           0xB1: "FW_EXEC", 0x28: "cmd28", 0x22: "cmd22",
           0x24: "cmd24", 0x25: "cmd25", 0x26: "cmd26"}


def print_summary(seq):
    print(f"\n{'='*100}")
    print(f"  SUMMARY")
    print(f"{'='*100}")
    counts = {}
    sizes = {}
    conns = set()
    for s in seq:
        if s["type"] == "IRQ":
            continue
        tx = s["tx"]
        op = tx[0] if tx else 0xFF
        name = OPCODES.get(op, f"0x{op:02X}")
        counts[name] = counts.get(name, 0) + 1
        sizes[name] = sizes.get(name, []) + [len(tx)]
        conns.add(s["conn"])
    print(f"  Connections used: {sorted(conns)}")
    print(f"  Total transactions: {sum(counts.values())}")
    print(f"  {'-'*50}")
    for name, cnt in sorted(counts.items()):
        sz = sizes[name]
        print(f"  {name:8s}  ×{cnt:3d}  size=({min(sz)}-{max(sz)})B  total={sum(sz)}B")
    print()


def print_compact_sequence(seq):
    """Print a dense summary of opcode transitions."""
    print(f"\n{'='*100}")
    print(f"  COMPACT SEQUENCE (for manual replication)")
    print(f"{'='*100}")
    steps = []
    i = 0
    while i < len(seq):
        s = seq[i]
        if s["type"] == "IRQ":
            steps.append(f"IRQ")
            i += 1
            continue
        tx = s["tx"]
        op = tx[0] if tx else 0xFF
        name = OPCODES.get(op, f"0x{op:02X}")
        if op == 0xB0:
            # Count consecutive FW blocks
            cnt = 0
            while i < len(seq) and seq[i]["type"] != "IRQ":
                ttx = seq[i]["tx"]
                if ttx and ttx[0] == 0xB0:
                    cnt += 1
                else:
                    break
                i += 1
            steps.append(f"{name} ×{cnt} (241B each)")
            continue
        elif op == 0x22:
            cnt = 0
            while i < len(seq) and seq[i]["type"] != "IRQ":
                ttx = seq[i]["tx"]
                if ttx and ttx[0] == 0x22:
                    cnt += 1
                else:
                    break
                i += 1
            steps.append(f"{name} ×{cnt} (241B each)")
            continue
        steps.append(f"{name} ({s['conn']}) TX={len(tx)}B RX={len(s['rx'])}B")
        i += 1

    line = ""
    for s in steps:
        if len(line) + len(s) + 5 > 90:
            print(f"  {line}")
            line = ""
        line += f"  → {s}"
    if line:
        print(f"  {line}")
    print()


def print_timeline_full(seq, touch_txns):
    """Print a merged timeline showing both touchscreen and companion."""
    print(f"\n{'='*100}")
    print(f"  MERGED TIMELINE: Touchscreen (conn 0b) + Companion (conn 18/19/1a)")
    print(f"{'='*100}")
    # Build combined list
    combined = []
    for t in touch_txns:
        if t.opcode == "TXN":
            cc = cshort(t.conn)
            if cc != "0b":
                continue
            combined.append((t.clk, "TOUCH", t.opcode_str(), t.tx_bytes(), t.rx_bytes()))
        elif t.opcode == "IRQ":
            combined.append((t.clk, "IRQ", "", b"", b""))
    for s in seq:
        if s["type"] == "IRQ":
            combined.append((s["ts"], "IRQ", "", b"", b""))
        else:
            tx = s["tx"]
            op = tx[0] if tx else 0xFF
            name = OPCODES.get(op, f"0x{op:02X}")
            combined.append((s["ts"], f"COMP({s['conn']})", name, tx, s["rx"]))

    combined.sort(key=lambda x: x[0])

    prev_clk = 0
    for ts, src, op, tx, rx in combined:
        gap = f"+{(ts - prev_clk) / 10:.0f}us" if prev_clk else ""
        prev_clk = ts
        if op == "IRQ":
            print(f"  {gap:>12s}  {'─ GPIO IRQ ─':40s}")
            continue
        rlabel = f" RX={len(rx)}B" if rx else ""
        tlabel = f"TX={len(tx)}B"
        print(f"  {gap:>12s}  [{src:12s}] [{op:6s}]  {tlabel}  {rlabel}")


def print_c_header(seq):
    """Generate C code for the companion activation sequence."""
    print(f"\n{'='*100}")
    print(f"  C HEADER: companion_activation.h")
    print(f"{'='*100}")

    # Extract FW blocks (0xB0)
    fw_blocks = []
    sec_blocks = []
    cmds = []
    for s in seq:
        if s["type"] == "IRQ":
            continue
        tx = s["tx"]
        if not tx:
            continue
        op = tx[0]
        if op == 0xB0:
            fw_blocks.append(tx)
        elif op == 0x22:
            sec_blocks.append(tx)
        else:
            cmds.append(s)

    print(f"/* Companion device (Device B) activation commands from surface_boot_auto.csv */")
    print(f"#ifndef COMPANION_ACTIVATION_H")
    print(f"#define COMPANION_ACTIVATION_H")
    print()
    print(f"/* Connection 0x18: initial activation cmd */")
    for s in cmds:
        if s["conn"] == "18":
            print(f'static const u8 companion_cmd_0x18[] = {{ {", ".join(f"0x{b:02X}" for b in s["tx"])} }};')
    print()
    print(f"/* Connection 0x19: checksum cmd + response */")
    for s in cmds:
        if s["conn"] == "19":
            print(f'static const u8 companion_cmd_0x19[] = {{ {", ".join(f"0x{b:02X}" for b in s["tx"])} }};')
            if s["rx"]:
                print(f'static const u8 companion_cmd_0x19_rx[] = {{ {", ".join(f"0x{b:02X}" for b in s["rx"])} }};')
    print()
    print(f"/* Connection 0x1A: cmd70 query */")
    for s in cmds:
        if s["conn"] == "1a" and s["tx"] and s["tx"][0] not in (0xB0, 0xB1, 0x22):
            name = OPCODES.get(s["tx"][0], f"cmd_{s['tx'][0]:02X}")
            print(f'static const u8 companion_{name}[] = {{ {", ".join(f"0x{b:02X}" for b in s["tx"])} }};')
            if s["rx"]:
                print(f'/* RX ({len(s["rx"])}B): {s["rx"][:16].hex(" ").upper()} */')
    print()
    print(f"/* Primary firmware blocks (0xB0): {len(fw_blocks)} blocks × {len(fw_blocks[0]) if fw_blocks else 0}B */")
    print(f"static const u8 companion_fw_b0[{len(fw_blocks)}][{len(fw_blocks[0]) if fw_blocks else 0}] = {{")
    for i, blk in enumerate(fw_blocks):
        h = ",\n\t".join(", ".join(f"0x{blk[j:j+16].hex(' ').replace(' ', ', 0x')}" if False else "") for j in range(0, len(blk), 16))
        # Simpler: just print hex dump
        if i < 3:
            print(f"\t/* Block {i} */", end=" ")
            for j in range(0, min(len(blk), 32)):
                print(f"0x{blk[j]:02X},", end="")
            print(" ...")
    print(f"}};")
    print()
    print(f"/* Blk0: {fw_blocks[0][:32].hex(' ').upper() if fw_blocks else 'N/A'} ... */")
    print(f"#define COMPANION_FW_B0_COUNT {len(fw_blocks)}")
    print(f"#define COMPANION_FW_B0_SIZE  {len(fw_blocks[0]) if fw_blocks else 0}")
    print(f"#define COMPANION_FW_B0_TOTAL {sum(len(b) for b in fw_blocks)}")
    print()
    print(f"/* Second firmware blocks (0x22): {len(sec_blocks)} blocks × {len(sec_blocks[0]) if sec_blocks else 0}B */")
    print(f"#define COMPANION_FW_22_COUNT {len(sec_blocks)}")
    print(f"#define COMPANION_FW_22_SIZE  {len(sec_blocks[0]) if sec_blocks else 0}")
    print(f"#define COMPANION_FW_22_TOTAL {sum(len(b) for b in sec_blocks)}")
    print()
    print(f"#endif /* COMPANION_ACTIVATION_H */")

    # Also write a binary blob file
    print()
    print(f"/* Total bytes for b0 firmware: {sum(len(b) for b in fw_blocks)} */")
    print(f"/* Total bytes for 0x22 firmware: {sum(len(b) for b in sec_blocks)} */")

    # Write raw binary
    blob_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "companion_fw_b0.bin")
    with open(blob_path, "wb") as f:
        for blk in fw_blocks:
            f.write(blk)
    print(f"/* Wrote {blob_path} */")

    blob22_path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "companion_fw_22.bin")
    with open(blob22_path, "wb") as f:
        for blk in sec_blocks:
            f.write(blk)
    print(f"/* Wrote {blob22_path} */")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--csv", default="/home/simone/spi-hid/traces/surface_boot_auto.csv")
    parser.add_argument("--c-header", action="store_true")
    parser.add_argument("--timeline", action="store_true")
    parser.add_argument("--compact", action="store_true")
    parser.add_argument("--full-timeline", action="store_true")
    args = parser.parse_args()

    txns = parse_csv(args.csv)
    seq = extract_companion(txns)

    print_summary(seq)
    if args.compact:
        print_compact_sequence(seq)
    if args.timeline:
        print_timeline(seq)
    if args.full_timeline:
        print_timeline_full(seq, txns)
    if args.c_header:
        print_c_header(seq)


if __name__ == "__main__":
    main()
