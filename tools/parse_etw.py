#!/usr/bin/env python3
"""Parse an ETW-CSV SPB trace (Surface HID-over-SPI) into an ordered transcript,
tagging each SPB transaction with its connection ID (SPB target device):
  0x0b = touchscreen HID   (handle 0x7F74AA5D37F8)
  0x18/0x19/0x1a = companion device (vendor init + firmware upload)

Each transaction: connection, direction(s), payload bytes, opcode label, GPIO IRQs.

Usage: parse_etw.py <trace.csv> [max] [conn_hex]
  conn_hex: optional filter, e.g. 0b or 1a or 18 (last byte of connection id)
"""
import sys, re

path  = sys.argv[1]
limit = int(sys.argv[2]) if len(sys.argv) > 2 else 60
connf = sys.argv[3].lower() if len(sys.argv) > 3 else None

act_re = re.compile(r'\{([0-9a-fA-F-]+)\}')
hex_re = re.compile(r'0x([0-9A-Fa-f]+)\s*$')
dir_re = re.compile(r'"(ToDevice|FromDevice)\s*"')
clk_re = re.compile(r',\s*(1\d{17})\s*,')
conn_re = re.compile(r'"([0-9a-f]{16})"')   # connection id in Start events

def clock(l):
    m = clk_re.search(l); return int(m.group(1)) if m else None
def act(l):
    m = act_re.search(l); return m.group(1) if m else None

OP = {0x0B:"READ",0x02:"WRITE",0x00:"cmd00",0x70:"cmd70",0xB0:"FWblk",
      0xB1:"FWexec",0x22:"cmd22",0x24:"cmd24",0x25:"cmd25",0x26:"cmd26"}

act_conn = {}        # activity id -> connection id (from Start events)
events = []          # (clock, kind, data)
pending_dir = None

with open(path, errors="replace") as f:
    next(f)
    for line in f:
        name = line.split(",",1)[0].strip()
        ev = line.split(",")[1].strip() if "," in line else ""
        if "GPIO-ClassExtension" in name and "InterruptInvokeDeviceIsrStart" in line:
            events.append((clock(line),"IRQ",None)); continue
        if ev == "Start":
            a = act(line); c = conn_re.search(line)
            if a and c: act_conn[a] = c.group(1)
            continue
        if "IoSpbPayloadTdStart" in line:
            d = dir_re.search(line); pending_dir = d.group(1) if d else "?"
        elif "IoSpbPayloadTdBuffer" in line:
            m = hex_re.search(line.rstrip())
            if m: events.append((clock(line), pending_dir or "?", (m.group(1).upper(), act(line))))

# assemble transactions
txns = []; cur = None
for clk,kind,data in events:
    if kind == "IRQ":
        txns.append(("IRQ",clk,None)); cur=None; continue
    hexb, a = data
    conn = act_conn.get(a, "?"*16)
    if kind == "ToDevice":
        cur = {"clk":clk,"conn":conn,"tx":hexb,"rx":None}; txns.append(("TXN",cur))
    else:
        if cur is not None and cur["rx"] is None and cur["conn"]==conn:
            cur["rx"]=hexb
        else:
            cur={"clk":clk,"conn":conn,"tx":None,"rx":hexb}; txns.append(("TXN",cur))

def fmt(h,maxb=28):
    if not h: return ""
    b=[h[i:i+2] for i in range(0,len(h),2)]
    s=" ".join(b[:maxb]);
    if len(b)>maxb: s+=f" …(+{len(b)-maxb}B)"
    return s
def opof(tx):
    if not tx: return " ?? "
    return OP.get(int(tx[0:2],16), f"op{tx[0:2]}")
def cshort(c): return c[-2:] if c and c!="?"*16 else "??"

print(f"# {path}  max={limit} filter={connf or 'ALL'}\n")
prev=None; n=0
for item in txns:
    if item[0]=="IRQ":
        clk=item[1]; gap=f"+{(clk-prev)/10:.0f}us" if prev and clk else ""
        if not connf: print(f"          ── GPIO IRQ ── {gap}")
        prev=clk; continue
    t=item[1]; c=cshort(t["conn"])
    if connf and c!=connf: continue
    clk=t["clk"]; gap=f"+{(clk-prev)/10:9.0f}us" if prev and clk else " "*11
    print(f"{gap} [conn {c}] [{opof(t['tx']):5}] TX={fmt(t['tx']):<44} RX={fmt(t['rx'])}")
    prev=clk; n+=1
    if n>=limit: break
