"""Helpers for tests that validate optional reverse-engineering artifacts."""

from __future__ import annotations

import sys


def skip_optional_contract(message: str) -> None:
    """Skip optional contract checks under pytest or direct script execution."""
    if "pytest" in sys.modules:
        import pytest

        pytest.skip(message, allow_module_level=True)
    print(f"SKIP: {message}")
    raise SystemExit(0)
