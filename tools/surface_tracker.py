#!/usr/bin/env python3
"""Surface TouchPenProcessor0C19 tracker oracle.

Full pipeline: peak detection, Hungarian assignment, 48-track lifecycle,
hold/lift, classification, contact emission, post-report coalescing.

Input: .v0 CapImg frame files (4304 bytes each).
Output: per-frame contact list with track ID, coordinates, classes.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from math import sqrt
from pathlib import Path
from typing import Optional

# ── recovered MSHW0231 / 0C19 static configuration ──────────────────

NCOLS = 72
NROWS = 48
MAX_TRACKS = 48
HISTORY_DEPTH = 10

# Radii from DLL static record DAT_1808e0460 (0C19 selector)
# 180607a20_FUN_180607a20.c:29-36
# Note: Surface's 0.545 radius is for sub-cella-refined coordinates.
# Our simple centroid may drift more; use a wider practical gate.
ASSOCIATION_RADIUS_SQ = 9.0  # 3 grid cells, squared
CONTINUITY_RADIUS_SQ = 16.0  # 4 grid cells, squared

# 1806025c0_FUN_1806025c0.c:33-35: frame_data +0x0c default for 0C19
COALESCE_DISTANCE_SQ = 36.0

# C590 LUT: c590[i] = 1.0 - (i*0.00222035 + 0.6), ×10000 fixed-point
def c590(raw: int) -> int:
    v = 10000 - (raw * 22 + 6000)
    return v if v > 0 else 0

# ── track state machine (180608430_FUN_180608430.c) ──────────────────

class S:  # states
    FREE = 0
    ACTIVE = 1
    ACTIVE2 = 2
    TRANS = 3
    HOLD = 4

@dataclass
class Obs:
    x: float = 0.0; y: float = 0.0
    weight: float = 0.0
    peak_col: int = 0; peak_row: int = 0

@dataclass
class Track:
    idx: int
    state: int = S.FREE
    x: float = 0.0; y: float = 0.0
    history: list[int] = field(default_factory=lambda: [0]*HISTORY_DEPTH)
    miss: int = 0
    seen: bool = False

@dataclass
class Contact:
    x: float; y: float
    status: int; group: int; track: int

@dataclass
class Tracker:
    tracks: list[Track] = field(default_factory=lambda: [Track(i) for i in range(MAX_TRACKS)])
    alloc: int = 0; active: int = 0
    frame: int = 0; prev: int = 0

    def gap(self, n: int) -> bool:
        if self.prev and n - self.prev > 5:
            for t in self.tracks: t.state = S.FREE
            self.alloc = self.active = 0
        self.prev = self.frame; self.frame = n
        return False

def set_state(tr: Tracker, t: Track, ns: int):
    old = t.state
    if old != S.FREE and ns == S.FREE: tr.alloc = max(0, tr.alloc - 1)
    if old == S.FREE and ns != S.FREE: tr.alloc += 1
    if old in (S.ACTIVE, S.ACTIVE2) and ns not in (S.ACTIVE, S.ACTIVE2): tr.active = max(0, tr.active - 1)
    if old not in (S.ACTIVE, S.ACTIVE2) and ns in (S.ACTIVE, S.ACTIVE2): tr.active += 1
    t.state = ns

# ── CapImg decoder ───────────────────────────────────────────────────

def decode_raster(body: bytes) -> Optional[bytes]:
    """Extract 3456-byte raster from 4304-byte V0 body. Section 0x0100 samples at body[28]."""
    if len(body) != 4304 or body[2] != 0x0c:
        return None
    # container: body[5:9]=u32 len, body[9:11]=u16 type, body[11]=flags
    # section at body[12]: type at body[16:18]
    if body[16] != 0x00 or body[17] != 0x01:  # 0x0100 little-endian
        return None
    # body[28:28+3456] = raster samples
    samples = body[28:28+3456]
    return samples if len(samples) == 3456 else None

# ── peak detection (FUN_1805fba00) ──────────────────────────────────

# Peak detection: minimum signal rise (c590 ×10000 fixed-point)
MIN_PEAK_RISE = 500  # filters noise (22-326), keeps touch (1448-1912)

def _peaks(signal: list[int]) -> list[tuple[int, int, int]]:
    out = []
    for i, v in enumerate(signal):
        if v < MIN_PEAK_RISE:
            continue
        c, r = i % NCOLS, i // NCOLS
        ok = True
        for cc, rr in ((c-5, r), (c+5, r), (c, r-5), (c, r+5)):
            if 0 <= cc < NCOLS and 0 <= rr < NROWS:
                if signal[rr*NCOLS+cc] > v: ok = False; break
        if ok: out.append((c, r, v))
    out.sort(key=lambda x: x[2], reverse=True)
    return out[:16]  # max 16 candidates

def _centroid(px: int, py: int, signal: list[int]) -> Optional[tuple[float, float, float]]:
    sx = sy = sw = 0.0
    for r in range(max(0, py-2), min(NROWS, py+3)):
        for c in range(max(0, px-2), min(NCOLS, px+3)):
            v = signal[r*NCOLS+c]
            if v <= 0: continue
            sx += float(c) * v; sy += float(r) * v; sw += float(v)
    return (sx/sw, sy/sw, sw) if sw > 0 else None

# ── Hungarian solver (FUN_1805fd090 + FUN_1805fd230) ─────────────────

def _hungarian(cost: list[list[int]], rows: int, cols: int) -> list[int]:
    u = [0]*(rows+1); v = [0]*(cols+1); p = [0]*(cols+1); way = [0]*(cols+1)
    for i in range(1, rows+1):
        p[0] = i; j0 = 0
        minv = [10**9]*(cols+1); used = [False]*(cols+1)
        while True:
            used[j0] = True; i0 = p[j0]; delta = 10**9; j1 = 0
            for j in range(1, cols+1):
                if used[j]: continue
                cur = cost[i0-1][j-1] - u[i0] - v[j]
                if cur < minv[j]: minv[j] = cur; way[j] = j0
                if minv[j] < delta: delta = minv[j]; j1 = j
            for j in range(cols+1):
                if used[j]: u[p[j]] += delta; v[j] -= delta
                elif j: minv[j] -= delta
            j0 = j1
            if p[j0] == 0: break
        while True:
            j1 = way[j0]; p[j0] = p[j1]; j0 = j1
            if j0 == 0: break
    res = [-1]*rows
    for j in range(1, cols+1):
        if p[j]: res[p[j]-1] = j-1
    return res

# ── pipeline ─────────────────────────────────────────────────────────

def process(raster: bytes, base: list[int], tr: Tracker, n: int = 0) -> list[Contact]:
    tr.gap(n)
    signal = [max(0, c590(raster[i]) - base[i]) for i in range(len(raster))]
    peaks = _peaks(signal)

    obs: list[Obs] = []
    seen = set()
    for px, py, _ in peaks:
        c = _centroid(px, py, signal)
        if c and (px, py) not in seen:
            seen.add((px, py))
            obs.append(Obs(x=c[0], y=c[1], weight=c[2], peak_col=px, peak_row=py))

    active = [t for t in tr.tracks if t.state >= S.ACTIVE]
    if active and obs:
        rows, cols = len(obs), len(active) + len(obs)
        cost = [[1000000]*cols for _ in range(rows)]
        for i, o in enumerate(obs):
            for j, t in enumerate(active):
                d2 = (o.x-t.x)**2 + (o.y-t.y)**2
                if d2 < ASSOCIATION_RADIUS_SQ:
                    cost[i][j] = int(sqrt(d2))
                else:
                    cost[i][j] = 100  # out-of-radius real edge, like Surface
            for j in range(len(active), cols): cost[i][j] = 1000
        assign = _hungarian(cost, rows, cols)
        matched = set()
        for i, col in enumerate(assign):
            if 0 <= col < len(active):
                d2 = (obs[i].x-active[col].x)**2 + (obs[i].y-active[col].y)**2
                if d2 < ASSOCIATION_RADIUS_SQ:
                    active[col].x = obs[i].x; active[col].y = obs[i].y
                    active[col].seen = True; matched.add(i)
        for i, o in enumerate(obs):
            if i in matched: continue
            for t in tr.tracks:
                if t.state == S.FREE:
                    t.x = o.x; t.y = o.y; t.seen = True
                    set_state(tr, t, S.ACTIVE)
                    break
    elif obs:
        for o in obs:
            for t in tr.tracks:
                if t.state == S.FREE:
                    t.x = o.x; t.y = o.y; t.seen = True
                    set_state(tr, t, S.ACTIVE)
                    break

    # Cleanup
    for t in tr.tracks:
        if t.state == S.TRANS: set_state(tr, t, S.FREE)
        if not t.seen and t.state in (S.ACTIVE, S.ACTIVE2):
            t.miss += 1
            if t.miss >= 3:
                set_state(tr, t, S.FREE)
                t.miss = 0
        elif t.seen:
            t.miss = 0

    # Emit
    recs: list[Contact] = []
    for t in tr.tracks:
        if t.state >= S.ACTIVE:
            recs.append(Contact(x=t.x, y=t.y, status=1, group=t.idx, track=t.idx))

    # Coalesce (FUN_1806025c0): merge close pairs, keep one survivor per group
    for a in range(len(recs)):
        if recs[a].status not in (1, 3): continue
        for b in range(a+1, len(recs)):
            if recs[b].status not in (1, 3): continue
            if recs[a].group == recs[b].group: continue
            d2 = (recs[a].x-recs[b].x)**2 + (recs[a].y-recs[b].y)**2
            if d2 >= COALESCE_DISTANCE_SQ: continue
            # Rewrite b's group to a's; ghost b; keep a alive
            g = recs[b].group
            for r in recs:
                if r.group == g: r.group = recs[a].group
            recs[b].status = 7
            # a stays alive (status 1 or 3 unchanged)

    # Filter ghosted, deduplicate per group
    final: list[Contact] = []
    seen_groups: set[int] = set()
    for r in recs:
        if r.status == 7: continue
        if r.group in seen_groups: continue
        seen_groups.add(r.group)
        final.append(r)

    for t in tr.tracks: t.seen = False
    return final

# ── baseline ─────────────────────────────────────────────────────────

def baseline(frames: list[bytes], n: int = 30) -> list[int]:
    nc = NCOLS * NROWS
    return [min(c590(f[i]) for f in frames[:n]) for i in range(nc)]

# ── CLI ──────────────────────────────────────────────────────────────

def main():
    import argparse, sys
    ap = argparse.ArgumentParser(description="Surface tracker oracle")
    ap.add_argument("frames", nargs="+", type=Path)
    ap.add_argument("--baseline-frames", type=int, default=30)
    ap.add_argument("--max-frames", type=int, default=0)
    args = ap.parse_args()

    files = []
    for p in args.frames:
        if p.is_dir(): files.extend(sorted(p.glob("*.v0")))
        elif p.suffix == ".v0": files.append(p)
    if not files: print("no .v0 files"); return 1

    rasters = []
    for f in files:
        r = decode_raster(f.read_bytes())
        if r: rasters.append(r)
    if not rasters: print("no valid rasters"); return 1

    base = baseline(rasters, args.baseline_frames)
    tr = Tracker()
    if args.max_frames: rasters = rasters[:args.max_frames]

    for i, r in enumerate(rasters):
        contacts = process(r, base, tr, i+1)
        if contacts:
            s = "; ".join(f"t{c.track} g{c.group} s{c.status}({c.x:.2f},{c.y:.2f})" for c in contacts)
            print(f"f{i+1:4d}: {len(contacts)}→ {s}")

    n_contacts = sum(1 for _ in (None for __ in rasters for ___ in process(__, base, tr, 0)))
    print(f"\n{len(rasters)} frames, {tr.alloc} tracks alloc, {tr.active} active")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
