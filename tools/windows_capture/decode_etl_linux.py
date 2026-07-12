#!/usr/bin/env python3
"""
Decode a Windows ETL file ON LINUX (no Windows/tracerpt needed).
Extracts per-provider counts and the raw (hex) payloads of the providers relevant
to the MSHW0231 touch investigation. Raw bytes are enough: we already know the protocol.

Setup:
    python3 -m venv etlvenv
    ./etlvenv/bin/pip install etl-parser construct
Usage:
    ./etlvenv/bin/python decode_etl_linux.py touch_runtime.etl
    ./etlvenv/bin/python decode_etl_linux.py touch_runtime.etl --dump SurfaceSerialHub,SmfClient,ACPI-MethodTrace

NB: the etl-parser library has a bug (accesses pid.data1 instead of pid.inner.data1);
we work around it here by reading the raw header, so we don't use parse_etw().
"""
import sys, argparse
from collections import Counter, defaultdict
import etl.etl as E
from etl.etl import IEtlFileObserver

NAMES = {
 "1e8d5623-6573-47f2-b69e-eea8923ce036":"SurfaceService",
 "8ddb06d0-a54b-4014-b119-5a09fb550d18":"SurfaceSerialHub",
 "0ec12c6e-794a-4b5e-8e27-6d3dc98bbd38":"SmfCore",
 "a8d5fcb2-ff94-4166-920d-3338902e77e0":"SmfClient",
 "1e8d5623-6573-47f2-b69e-eea8923ce036b":"SurfaceService",
 "3d53356d-424b-496a-a094-2154d8d2b304":"OemPanel",
 "23ac50b3-680e-7ad3-a2b3-d5dbef945870":"PowerTrackerCore",
 "2fea7205-b0b1-41ca-8609-5a1d16f3132f":"SurfaceHidMini",
 "69cd60e3-430f-4da4-b1b3-e3bdaf945875":"SurfaceHotPlug",
 "72cd9ff7-4af8-4b89-aede-5f26fda13567":"SPB-ClassExt",
 "55ab77f6-fa04-43ef-af45-688fbf500482":"GPIO-ClassExt",
 "6465da78-e7a0-4f39-b084-8f53c7c30dc6":"HIDCLASS",
 "3fa102e9-1a62-5490-7af8-6088c2f9e6be":"SurfaceTouchAndPenProd",
 "c514638f-7723-485b-bcfc-96565d735d4a":"Kernel-Acpi",
 "dab01d4d-2d48-477d-b1c3-daad0ce6f06b":"ACPI-MethodTrace",
 "331c3b3a-2005-44c2-ac5e-77220c37d6b4":"Kernel-Power",
 "9c205a39-1250-487d-abd7-e831c6290539":"Kernel-PnP",
 "96f4a050-7e31-453c-88be-9634f4e02139":"UserPnp",
}

def fmt_guid(pid):
    i = pid.inner
    d4 = bytes(i.data4)
    return "%08x-%04x-%04x-%02x%02x-%s" % (i.data1, i.data2, i.data3, d4[0], d4[1], d4[2:].hex())

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("etl")
    ap.add_argument("--dump", default="", help="list of providers (names or GUIDs) whose hex payloads to print")
    args = ap.parse_args()

    data = open(args.etl, "rb").read()
    f = E.build_from_stream(data)
    want = set(x.strip() for x in args.dump.split(",") if x.strip())

    cnt = Counter()
    payloads = defaultdict(list)
    sysrec = Counter()

    class Obs(IEtlFileObserver):
        def on_event_record(self, ev):
            try:
                hdr = ev.source.event_header
                g = fmt_guid(hdr.provider_id)
                name = NAMES.get(g, g)
                eid = hdr.event_descriptor.Id
                ud = bytes(ev.source.user_data) if ev.source.user_data else b""
                cnt[name] += 1
                if want and (name in want or g in want):
                    payloads[name].append((eid, ev.get_timestamp() if hasattr(ev,'get_timestamp') else 0, ud))
            except Exception as ex:
                cnt["<parse-error:%s>" % type(ex).__name__] += 1
        def on_trace_record(self, e): sysrec["trace"] += 1
        def on_perfinfo_trace(self, o): sysrec["perfinfo"] += 1
        def on_system_trace(self, o): sysrec["system"] += 1
        def on_win_trace(self, o): sysrec["wintrace"] += 1

    f.parse(Obs())

    print("=== PROVIDER EVENT COUNTS (manifest/ETW) ===")
    for name, c in cnt.most_common():
        print("%6d  %s" % (c, name))
    print("=== kernel/system records ===")
    for k, c in sysrec.most_common():
        print("%6d  %s" % (c, k))

    for name, evs in payloads.items():
        print("\n=== PAYLOADS: %s (%d events) ===" % (name, len(evs)))
        for eid, ts, ud in evs:
            print("  eid=%-4d ts=%s len=%d  %s" % (eid, ts, len(ud), ud.hex()))

if __name__ == "__main__":
    main()
