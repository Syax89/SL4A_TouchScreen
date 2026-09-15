#!/usr/bin/env python3
"""Pin the installer's failure-path contract (adversarial reviews R12/R14/R16).

These are the strings and code shapes the fix passes keep touching: the
diagnostic bundle's write check, the `-o` validation, the DKMS version parse
(both real `dkms status` shapes) and the ownership markers. A revert of any of
them must fail here — the shell suite alone never noticed (review R16 found that
none of the round-3 corrections was pinned by any test).
"""

from pathlib import Path

root = Path(__file__).parents[1]
tool = (root / "tools" / "sl4a-touch.sh").read_text()

# Diagnostic bundle: a failed redirect or a write that stopped early must be
# reported, never glossed over with "Diagnostic bundle written" for a file that
# nothing wrote (a stale bundle from a previous run is non-empty too).
assert "bundle_status=$?" in tool
assert "set +e +o pipefail" in tool          # pipefail off for the block, or a
assert "set -e -o pipefail" in tool          # dmesg|grep miss fails a good bundle
assert 'if [ "$bundle_status" -ne 0 ] || [ ! -s "$OUT" ] || \\' in tool
assert "! grep -q '^--- dmesg' \"$OUT\"" in tool
assert "the diagnostic bundle could not be written to" in tool

# `-o`: symlink, non-regular file, foreign file and empty path are all refused,
# and the "is it ours" test looks at the first line only.
assert "refusing to write the bundle through the symlink" in tool
assert "refusing to overwrite $OUT: not a regular file" in tool
assert "it is not a diagnostic bundle" in tool
assert '"-o requires a non-empty path"' in tool
assert 'head -n 1 "$OUT"' in tool

# DKMS cleanup: both real `dkms status` shapes parse the same way, a legacy
# 2.x line matches nothing, and only a plausible version can reach
# `dkms remove -v` and the `rm -rf` that follows it.
assert "printf '%s\\n' \"$line\" | sed -n" in tool
assert "''|*[!A-Za-z0-9.+~_-]*) continue ;;" in tool
assert tool.count("^PACKAGE_NAME=\"sl4a-touch\"[[:space:]]*$") >= 3

# A stale registration that survives must be loud: it can win the next kernel
# update, which is the whole reason the cleanup exists.
assert "stays registered and can win the next kernel update" in tool

# Every menu tput is failure-tolerant under `set -e`: a TERM without
# cuu/ed/cnorm used to kill the interactive flow mid-selection.
for seq in ("tput civis", "tput cuu", "tput ed", "tput cnorm"):
    for line in tool.splitlines():
        if line.strip().startswith(seq):
            assert line.rstrip().endswith("|| true"), line

print("installer recovery contract: PASS")
