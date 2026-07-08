#!/usr/bin/env python3
"""Reusable parser for ETW CSV dumps (SPB-ClassExtension + GPIO-ClassExtension).

Extracts SPI transactions (SpbCx) as Txn objects with their ToDevice/FromDevice
TDs (transfer descriptors) and exact bytes, plus interleaved GPIO interrupt events.

Usage:
  python3 parse_etw.py <file.csv> [--n N] [--all-writes] [--approvals] [--b0]
"""
import csv, sys, re
from dataclasses import dataclass, field

SPB = "Microsoft-Windows-SPB-ClassExtension"
GPIO = "Microsoft-Windows-GPIO-ClassExtension"

@dataclass
class TD:
    direction: str   # ToDevice / FromDevice
    length: int
    data: bytes

@dataclass
class Txn:
    idx: int
    line_start: int
    time_start: int      # Clock-Time of Start (1010)
    time_complete: int   # Clock-Time of IoComplete
    activity: str
    tds: list = field(default_factory=list)
    status: str = ""
    info: str = ""       # user data from Start

    @property
    def kind(self):
        dirs = [t.direction for t in self.tds]
        if dirs == ["ToDevice", "FromDevice"]:
            return "Sequence(W+R)"
        if dirs == ["ToDevice"]:
            return "Write"
        if dirs == ["FromDevice"]:
            return "Read"
        return "+".join(dirs) or "?"

@dataclass
class GpioEv:
    line: int
    time: int
    typ: str
    data: str

def parse(path):
    """Returns (txns, gpio_events) in chronological order of appearance."""
    txns = []
    gpio = []
    open_act = {}   # activity id -> Txn
    with open(path, newline='', encoding='utf-8', errors='replace') as f:
        rdr = csv.reader(f, skipinitialspace=True)
        header = next(rdr)
        for ln, row in enumerate(rdr, start=2):
            if not row:
                continue
            name = row[0].strip()
            if name == SPB:
                typ = row[1].strip()
                act = row[14].strip()
                # Clock-Time is column 16 (0-based), but rows have extra fields:
                # the layout is fixed up to col 16; User Data starts at col 19.
                t = int(row[16].strip())
                ud = [c.strip() for c in row[19:]]
                if typ == "Start":
                    txn = Txn(idx=len(txns), line_start=ln, time_start=t,
                              time_complete=0, activity=act,
                              info=",".join(ud))
                    txns.append(txn)
                    open_act[act] = txn
                elif typ == "IoSpbPayloadTdStart":
                    txn = open_act.get(act)
                    if txn is not None:
                        d = ud[1].strip('" ').strip()
                        txn.tds.append(TD(direction=d, length=0, data=b""))
                elif typ == "IoSpbPayloadTdBuffer":
                    txn = open_act.get(act)
                    if txn is not None and txn.tds:
                        length = int(ud[1])
                        hexs = ud[2]
                        m = re.match(r'0x([0-9A-Fa-f]+)', hexs)
                        data = bytes.fromhex(m.group(1)) if m else b""
                        td = txn.tds[-1]
                        td.length = length
                        td.data = data
                elif typ == "IoComplete":
                    txn = open_act.get(act)
                    if txn is not None:
                        txn.time_complete = t
                        txn.status = ud[1] if len(ud) > 1 else ""
                elif typ == "Stop":
                    open_act.pop(act, None)
            elif name == GPIO:
                typ = row[1].strip()
                t = int(row[16].strip())
                ud = ",".join(c.strip() for c in row[19:])
                gpio.append(GpioEv(line=ln, time=t, typ=typ, data=ud))
    return txns, gpio

def hexs(b):
    return " ".join(f"{x:02X}" for x in b)

def fmt_time(t, t0):
    return f"+{(t - t0)/10000.0:12.3f}ms"

def dump_first(txns, gpio, n, t0):
    """Prints the first n txns with the interleaved GPIO ISR events."""
    isr = [g for g in gpio if g.typ in ("InterruptInvokeDeviceIsrStart", "InterruptPinState")]
    events = [("TXN", t.time_start, t) for t in txns[:n]] + \
             [("GPIO", g.time, g) for g in isr if not txns or g.time <= txns[min(n, len(txns))-1].time_complete + 50000]
    events.sort(key=lambda e: e[1])
    for kind, t, obj in events:
        if kind == "TXN":
            x = obj
            print(f"\n[TXN #{x.idx:3d}] line {x.line_start}  t={fmt_time(x.time_start, t0)}  {x.kind}  status={x.status}")
            for td in x.tds:
                print(f"    {td.direction:10s} len={td.length:4d}  {hexs(td.data)}")
        else:
            g = obj
            print(f"  >> GPIO {g.typ}  line {g.line}  t={fmt_time(g.time, t0)}  [{g.data}]")

if __name__ == "__main__":
    path = sys.argv[1]
    n = 25
    for i, a in enumerate(sys.argv):
        if a == "--n":
            n = int(sys.argv[i+1])
    txns, gpio = parse(path)
    t0 = txns[0].time_start if txns else 0
    print(f"# {len(txns)} SPB transactions, {len(gpio)} GPIO events")

    if "--b0" in sys.argv:
        from collections import Counter
        c = Counter()
        for x in txns:
            for td in x.tds:
                if td.direction == "ToDevice" and td.data:
                    c[(td.data[0], td.length)] += 1
        for (op, ln), cnt in sorted(c.items()):
            print(f"opcode 0x{op:02X} len={ln}: {cnt} write")
        sys.exit(0)

    if "--approvals" in sys.argv:
        for x in txns:
            if x.tds and x.tds[0].data[:1] == b"\x0b":
                resp = x.tds[1].data if len(x.tds) > 1 else b""
                print(f"#{x.idx:4d} t={fmt_time(x.time_start,t0)} MOSI={hexs(x.tds[0].data)} | MISO={hexs(resp)}")
        sys.exit(0)

    dump_first(txns, gpio, n, t0)
