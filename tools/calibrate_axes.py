#!/usr/bin/env python3
"""Retired raw-mode calibration prototype."""

import sys


def main():
    print("Raw calibration is disabled: the raw frame layout is unvalidated.")
    print("Follow docs/RAW_MODE_VALIDATION.md before enabling coordinates.")
    return 2


if __name__ == "__main__":
    sys.exit(main())
