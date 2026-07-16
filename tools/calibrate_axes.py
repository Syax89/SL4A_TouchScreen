#!/usr/bin/env python3
"""Archived raw-mode calibration prototype.

The raw 72x48 heatmap grid is mapped linearly to 0..32767, but the *physical*
orientation of the grid (which corner is the origin, and which way X/Y grow) is
not knowable from the captured Windows trace — it must be measured on the real
panel. This tool asks you to touch the four screen corners and then prints the
recommended module parameters:

    swap_xy, invert_x, invert_y   (orientation)
    calib_offset_x/y, calib_scale_x/y (optional, to stretch the usable range)

No supported live raw stream exists. Do not use this tool to initiate or validate
on-device calibration; retained captures and offline analysis are the supported
calibration research path.
"""
import os, sys, glob, struct, time

SCREEN_MAX = 32767
EV_SYN, EV_KEY, EV_ABS = 0, 1, 3
BTN_TOUCH = 0x14a               # 330
ABS_MT_POSITION_X, ABS_MT_POSITION_Y = 0x35, 0x36  # 53, 54
FMT = '@llHHi'                  # timeval(16) + type(2) + code(2) + value(4) = 24
ESZ = struct.calcsize(FMT)

PARAM_DIR = "/sys/module/spi_hid/parameters"

# physical corners in the order we ask for them, normalized (x,y), origin top-left
CORNERS = [
    ("TOP-LEFT",     0.0, 0.0),
    ("TOP-RIGHT",    1.0, 0.0),
    ("BOTTOM-RIGHT", 1.0, 1.0),
    ("BOTTOM-LEFT",  0.0, 1.0),
]


def find_device(explicit):
    if explicit:
        return explicit
    for ev in sorted(glob.glob("/sys/class/input/event*")):
        namep = os.path.join(ev, "device", "name")
        try:
            with open(namep) as f:
                name = f.read().strip()
        except OSError:
            continue
        if "MSHW0231" in name or "Touchscreen" in name:
            dev = "/dev/input/" + os.path.basename(ev)
            print(f"Auto-detected: {dev}  ({name})")
            return dev
    return None


def read_current_params():
    vals = {}
    for p in ("swap_xy", "invert_x", "invert_y",
              "calib_offset_x", "calib_offset_y",
              "calib_scale_x", "calib_scale_y"):
        try:
            with open(os.path.join(PARAM_DIR, p)) as f:
                vals[p] = f.read().strip()
        except OSError:
            vals[p] = None
    return vals


def collect_corner(fd, label):
    """Wait for one press+lift, return (avg_x, avg_y) sampled while touching."""
    print(f"\n>>> Touch and hold the {label} corner, then lift...", flush=True)
    touching = False
    x = y = 0
    xs, ys = [], []
    buf = b""
    while True:
        buf += os.read(fd, ESZ * 64)
        while len(buf) >= ESZ:
            chunk, buf = buf[:ESZ], buf[ESZ:]
            _, _, etype, ecode, val = struct.unpack(FMT, chunk)
            if etype == EV_KEY and ecode == BTN_TOUCH:
                touching = (val == 1)
                if val == 0 and xs:
                    ax = sum(xs) / len(xs)
                    ay = sum(ys) / len(ys)
                    print(f"    {label}: raw=({ax:.0f},{ay:.0f})  n={len(xs)}")
                    return ax, ay
            elif etype == EV_ABS and ecode == ABS_MT_POSITION_X:
                x = val
            elif etype == EV_ABS and ecode == ABS_MT_POSITION_Y:
                y = val
            elif etype == EV_SYN and ecode == 0 and touching:
                xs.append(x); ys.append(y)


def analyze(readings):
    # readings: list aligned with CORNERS: (rawx, rawy)
    TL, TR, BR, BL = readings
    dx_h = TR[0] - TL[0]   # horizontal move (TL->TR): dominant raw axis == screen X
    dy_h = TR[1] - TL[1]
    dx_v = BL[0] - TL[0]   # vertical move (TL->BL): dominant raw axis == screen Y
    dy_v = BL[1] - TL[1]

    swap = abs(dy_h) > abs(dx_h)   # if horizontal move shows up mostly in raw Y -> swap

    if not swap:
        invert_x = dx_h < 0        # screen X <- raw X; must grow left->right
        invert_y = dy_v < 0        # screen Y <- raw Y; must grow top->bottom
    else:
        # after swap: output X comes from raw Y, output Y from raw X
        invert_x = dy_h < 0        # raw Y must grow left->right
        invert_y = dx_v < 0        # raw X must grow top->bottom

    # observed usable extent (min/max over corners), in the *reported* frame
    xs = [r[0] for r in readings]; ys = [r[1] for r in readings]
    return {
        "swap_xy": int(swap),
        "invert_x": int(invert_x),
        "invert_y": int(invert_y),
        "raw_x_range": (min(xs), max(xs)),
        "raw_y_range": (min(ys), max(ys)),
        "deltas": (dx_h, dy_h, dx_v, dy_v),
    }


def main():
    dev = find_device(sys.argv[1] if len(sys.argv) > 1 else None)
    if not dev:
        print("ERROR: touchscreen input device not found. Pass it explicitly:\n"
              "  sudo python3 calibrate_axes.py /dev/input/eventN")
        return 2

    cur = read_current_params()
    nonzero = [k for k in ("swap_xy", "invert_x", "invert_y")
               if cur.get(k) not in (None, "0", "N")]
    if nonzero:
        print("\nWARNING: orientation params are not at defaults:",
              {k: cur[k] for k in nonzero})
        print("Reload the module with defaults first for a clean measurement:")
        print("  sudo modprobe -r spi-hid && sudo modprobe spi-hid raw_mode=1")
        input("Press Enter to continue anyway, or Ctrl-C to abort... ")

    try:
        fd = os.open(dev, os.O_RDONLY)
    except OSError as e:
        print(f"ERROR opening {dev}: {e} (run with sudo?)")
        return 2

    print("\n=== Axis calibration: touch each corner as prompted ===")
    readings = []
    try:
        for label, _, _ in CORNERS:
            readings.append(collect_corner(fd, label))
    finally:
        os.close(fd)

    res = analyze(readings)
    dx_h, dy_h, dx_v, dy_v = res["deltas"]
    print("\n--- Analysis ---")
    print(f"horizontal move (TL->TR): dRawX={dx_h:+.0f} dRawY={dy_h:+.0f}")
    print(f"vertical   move (TL->BL): dRawX={dx_v:+.0f} dRawY={dy_v:+.0f}")
    print(f"raw X range: {res['raw_x_range'][0]:.0f}..{res['raw_x_range'][1]:.0f}")
    print(f"raw Y range: {res['raw_y_range'][0]:.0f}..{res['raw_y_range'][1]:.0f}")

    print("\n=== RECOMMENDED module parameters ===")
    print(f"  swap_xy={res['swap_xy']} invert_x={res['invert_x']} invert_y={res['invert_y']}")
    print("\nApply with:")
    print(f"  sudo modprobe -r spi-hid && sudo modprobe spi-hid raw_mode=1 "
          f"swap_xy={res['swap_xy']} invert_x={res['invert_x']} invert_y={res['invert_y']}")
    print("\nOr live (no reload):")
    for k in ("swap_xy", "invert_x", "invert_y"):
        print(f"  echo {res[k]} | sudo tee {PARAM_DIR}/{k}")

    # sanity check
    if abs(dx_h) < 500 and abs(dy_h) < 500:
        print("\nNOTE: horizontal corners barely moved the reading — detection may be "
              "failing (check dmesg / touch_threshold_pct) or you touched the same spot.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
