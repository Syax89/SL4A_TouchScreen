#!/usr/bin/env python3
"""Model the recovered MSHW0231 Surface post-report coalescing rule."""

from __future__ import annotations

from dataclasses import dataclass


MSHW0231_ASSOCIATION_RADIUS = 0.5450090169906616
MSHW0231_CONTINUITY_RADIUS = 1.2180980443954468
MSHW0231_COALESCE_DISTANCE_SQUARED = 36.0


@dataclass
class Contact:
    x: float
    y: float
    status: int
    group: int
    track: int


def coalesce_contacts(
    contacts: list[Contact],
    distance_squared: float = MSHW0231_COALESCE_DISTANCE_SQUARED,
) -> list[Contact]:
    """Apply FUN_1806025c0's group rewrite and status-1 ghost marking."""
    result = [Contact(**contact.__dict__) for contact in contacts]

    for first, left in enumerate(result):
        if left.status not in (1, 3):
            continue
        for second in range(first + 1, len(result)):
            right = result[second]
            if right.status not in (1, 3) or left.group == right.group:
                continue
            dx = left.x - right.x
            dy = left.y - right.y
            if dx * dx + dy * dy >= distance_squared:
                continue

            replaced_group = right.group
            for contact in result:
                if contact.group == replaced_group:
                    contact.group = left.group
            if left.status == 1:
                left.status = 7
            if right.status == 1:
                right.status = 7

    return result


def association_within_radius(
    track_x: float,
    track_y: float,
    candidate_x: float,
    candidate_y: float,
    *,
    single_track_continuity: bool = False,
) -> bool:
    """Apply FUN_180607a20's strict squared-distance acceptance gate."""
    radius = (
        MSHW0231_CONTINUITY_RADIUS
        if single_track_continuity
        else MSHW0231_ASSOCIATION_RADIUS
    )
    dx = track_x - candidate_x
    dy = track_y - candidate_y
    return dx * dx + dy * dy < radius * radius
