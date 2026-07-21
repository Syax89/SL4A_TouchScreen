#!/usr/bin/env python3
"""Pytest entry point for the repository's script-driven test suite."""

from __future__ import annotations

import subprocess
from pathlib import Path


def test_make_driven_script_suite() -> None:
    """Run the same tests as `make -C tests test` under pytest."""
    root = Path(__file__).resolve().parents[1]
    subprocess.run(["make", "-C", "tests", "test"], cwd=root, check=True)
