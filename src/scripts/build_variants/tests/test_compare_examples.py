#!/usr/bin/env python3
"""Repository-level tests for curated example output normalization."""

from __future__ import annotations

import importlib.util
import sys
import unittest
from pathlib import Path


def load_compare_examples_module():
    repository = Path(__file__).resolve().parents[4]
    module_path = (
        repository
        / "src/python_examples/cgalpy_examples/compare_examples.py"
    )

    spec = importlib.util.spec_from_file_location(
        "cgalpy_compare_examples",
        module_path,
    )

    if spec is None or spec.loader is None:
        raise RuntimeError(
            f"Unable to load comparison module: {module_path}"
        )

    module = importlib.util.module_from_spec(spec)
    sys.modules[spec.name] = module

    try:
        spec.loader.exec_module(module)
    except Exception:
        sys.modules.pop(spec.name, None)
        raise

    return module


COMPARE_EXAMPLES = load_compare_examples_module()


class ComparableStdoutTests(unittest.TestCase):
    def test_timing_normalization_accepts_scientific_notation(
        self,
    ) -> None:
        pair = COMPARE_EXAMPLES.PAIRS["ms2_approx_inset"]

        cpp_stdout = (
            "Inset computation took 1.5e-05 seconds.\n"
        )
        python_stdout = (
            "Inset computation took 0.000802 seconds.\n"
        )
        expected = (
            "Inset computation took <TIME> seconds.\n"
        )

        self.assertEqual(
            COMPARE_EXAMPLES.comparable_stdout(
                pair,
                cpp_stdout,
            ),
            expected,
        )
        self.assertEqual(
            COMPARE_EXAMPLES.comparable_stdout(
                pair,
                python_stdout,
            ),
            expected,
        )


if __name__ == "__main__":
    unittest.main()
