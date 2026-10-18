#!/usr/bin/env python3
"""Static contracts for the hardware-evidence wrappers.

These checks pin two failures exposed during the F108 cold-boot campaign:
  * beta-MT qualification must target the driver-owned
    "MSHW0231 Touchscreen" node, not the standard HID "spi 045E:0C19" node;
  * a requested evtest capture failure must make the trace/session fail instead
    of being recorded as a misleading completed evidence session.

The test is intentionally host-only: it reads the shell scripts as text and
does not need /sys, /dev/input, root, or Surface hardware.
"""

from pathlib import Path
import re
import stat

ROOT = Path(__file__).resolve().parents[1]
TOOLS = ROOT / "tools" / "hardware_evidence"


def source(name):
    return (TOOLS / name).read_text()


def require(text, needle, where):
    assert needle in text, f"{where}: missing {needle!r}"


def forbid(text, needle, where):
    assert needle not in text, f"{where}: forbidden {needle!r}"


def test_evidence_helpers_are_executable():
    for name in (
        "capture_beta_multitouch.sh",
        "capture_direct_touch.sh",
        "capture_linux_trace_bundle.sh",
        "capture_stylus.sh",
        "collect.sh",
        "run_blinded_session.sh",
    ):
        mode = (TOOLS / name).stat().st_mode
        assert mode & stat.S_IXUSR, f"{name}: owner executable bit is not set"


def test_beta_capture_identity_contract():
    text = source("capture_beta_multitouch.sh")
    require(text, '[ "$name" = "MSHW0231 Touchscreen" ] || continue',
            "capture_beta_multitouch.sh")
    require(text, 'capability_bit_set "$input/capabilities/abs" 57',
            "capture_beta_multitouch.sh")
    require(text, 'device="/dev/input/$(basename "$event_sysfs")"',
            "capture_beta_multitouch.sh")
    forbid(text, "/dev/input/event15", "capture_beta_multitouch.sh")

    # Refuse an actual sudo command, not prose in the usage heredoc.
    scrubbed = re.sub(
        r"<<'?(\w+)'?\n.*?^\1$",
        "",
        text,
        flags=re.MULTILINE | re.DOTALL,
    )
    scrubbed = re.sub(r"^[ \t]*#.*$", "", scrubbed, flags=re.MULTILINE)
    assert not re.search(r"(?m)^[ \t]*sudo(?:[ \t]|$)", scrubbed), (
        "capture_beta_multitouch.sh: must not invoke sudo"
    )


def test_bundle_propagates_beta_capture_failure():
    text = source("capture_linux_trace_bundle.sh")
    require(text, "--capture-beta-multitouch",
            "capture_linux_trace_bundle.sh")
    require(text, "capture_beta_multitouch.sh",
            "capture_linux_trace_bundle.sh")
    require(text, 'beta_multitouch_capture_exit_status=%s',
            "capture_linux_trace_bundle.sh")
    require(text, '[ "$capture_beta_multitouch" -eq 1 ] && '
                  '[ "$beta_multitouch_status" -ne 0 ]',
            "capture_linux_trace_bundle.sh")
    require(text, '[ "$capture_failed" -eq 0 ] || exit 1',
            "capture_linux_trace_bundle.sh")


def test_blinded_session_passes_beta_capture_through():
    text = source("run_blinded_session.sh")
    require(text, "--capture-beta-multitouch", "run_blinded_session.sh")
    require(text, "trace_args+=(--capture-beta-multitouch)",
            "run_blinded_session.sh")
    require(text, "beta_multitouch_capture_requested=$capture_beta_multitouch",
            "run_blinded_session.sh")
    require(text, 'status="trace-failed"', "run_blinded_session.sh")


def main():
    test_evidence_helpers_are_executable()
    test_beta_capture_identity_contract()
    test_bundle_propagates_beta_capture_failure()
    test_blinded_session_passes_beta_capture_through()
    print("hardware_evidence_contract_test: PASS")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
