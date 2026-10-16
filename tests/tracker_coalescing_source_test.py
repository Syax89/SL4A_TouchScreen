#!/usr/bin/env python3
from pathlib import Path
import sys

root = Path(__file__).resolve().parent.parent
raw = (root / "driver/mshw0231-raw.c").read_text()
errors = []

def check(cond, msg):
    if not cond:
        errors.append(msg)

check("raw_ghost_merge(" not in raw,
      "destructive pre-association ghost merge is still present")
check("raw_post_assoc_coalesce(" in raw,
      "post-association coalescing policy missing")

start = raw.find("/* ── Stage 6: Hungarian global assignment ── */")
end = raw.find("/* ── Stage 8: slot state machine ── */", start)
check(start >= 0 and end > start, "cannot isolate tracker stage ordering")
if start >= 0 and end > start:
    block = raw[start:end]
    p_assoc = block.find("raw_hungarian_match(")
    p_coalesce = block.find("raw_post_assoc_coalesce(")
    check(0 <= p_assoc < p_coalesce,
          "coalescing must run after Hungarian association")

check("established_a && established_b" in raw,
      "two-established-track preservation rule missing")
check("established_a != established_b" in raw,
      "established-vs-new continuity rule missing")
check("shid->blob_slot_state[loser_slot] == 1" in raw,
      "provisional state-1 survivor reconciliation missing")
check("shid->blob_slot_state[winner_slot] == 0" in raw,
      "provisional reconciliation is not limited to a fresh winner slot")
check("(u32)pdx <= bmd && (u32)pdy <= bmd" in raw,
      "provisional reconciliation is not bounded by Hungarian range")
check("assigned_slot[winner] = loser_slot" in raw,
      "survivor does not inherit valid provisional history")

if errors:
    for e in errors:
        print("FAIL:", e)
    sys.exit(1)

print("tracker association-before-coalescing source test: PASS")
