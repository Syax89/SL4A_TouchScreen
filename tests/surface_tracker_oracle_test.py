#!/usr/bin/env python3
"""Regression checks for recovered Surface post-report coalescing."""

from pathlib import Path
import sys
from optional_contract import skip_optional_contract


ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / "tools"))

try:
    from surface_tracker_oracle import (  # noqa: E402
        Contact,
        MSHW0231_ASSOCIATION_RADIUS,
        MSHW0231_COALESCE_DISTANCE_SQUARED,
        MSHW0231_CONTINUITY_RADIUS,
        association_within_radius,
        coalesce_contacts,
    )
except ModuleNotFoundError:
    skip_optional_contract("surface tracker oracle module is not present")


assert MSHW0231_ASSOCIATION_RADIUS == 0.5450090169906616
assert MSHW0231_CONTINUITY_RADIUS == 1.2180980443954468
assert MSHW0231_COALESCE_DISTANCE_SQUARED == 36.0

assert association_within_radius(0.0, 0.0, 0.5, 0.0)
assert not association_within_radius(0.0, 0.0, 0.6, 0.0)
assert association_within_radius(
    0.0, 0.0, 1.0, 0.0, single_track_continuity=True
)
assert not association_within_radius(
    0.0, 0.0, MSHW0231_CONTINUITY_RADIUS, 0.0, single_track_continuity=True
)

merged = coalesce_contacts([
    Contact(x=10.0, y=10.0, status=1, group=4, track=0),
    Contact(x=14.0, y=13.0, status=1, group=7, track=1),
])
assert [contact.group for contact in merged] == [4, 4]
assert [contact.status for contact in merged] == [7, 7]

# The Windows comparison is strict: a distance squared of exactly 36 does not merge.
boundary = coalesce_contacts([
    Contact(x=0.0, y=0.0, status=1, group=1, track=0),
    Contact(x=6.0, y=0.0, status=1, group=2, track=1),
])
assert [contact.group for contact in boundary] == [1, 2]
assert [contact.status for contact in boundary] == [1, 1]

# Ineligible records must not trigger a group rewrite.
ineligible = coalesce_contacts([
    Contact(x=0.0, y=0.0, status=7, group=1, track=0),
    Contact(x=1.0, y=1.0, status=1, group=2, track=1),
])
assert [contact.group for contact in ineligible] == [1, 2]
assert [contact.status for contact in ineligible] == [7, 1]

print("surface tracker oracle: PASS")
