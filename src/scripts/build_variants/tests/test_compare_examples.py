#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Repository-level tests for curated example output normalization."""

from __future__ import annotations

import importlib.util
import json
import sys
import tempfile
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


class ComparisonResultTests(unittest.TestCase):
    def test_prepare_pair_dir_removes_stale_outputs(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            work_dir = Path(tmp)
            stale_dir = work_dir / "raster_pair" / "cpp_output"
            stale_dir.mkdir(parents=True)
            stale_file = stale_dir / "visual.ppm"
            stale_file.write_bytes(b"stale")

            pair_dir = COMPARE_EXAMPLES.prepare_pair_dir(
                work_dir,
                "raster_pair",
            )

            self.assertEqual(pair_dir, work_dir / "raster_pair")
            self.assertTrue(pair_dir.is_dir())
            self.assertFalse(stale_file.exists())
            self.assertEqual(list(pair_dir.iterdir()), [])

    def test_stdout_comparator_preserves_exact_behavior(self) -> None:
        pair = COMPARE_EXAMPLES.PAIRS["pol2_polygon"]

        exact = COMPARE_EXAMPLES.compare_stdout(
            pair,
            "same\n",
            "same\n",
        )
        whitespace_only = COMPARE_EXAMPLES.compare_stdout(
            pair,
            "same value\n",
            "same   value\n",
        )

        self.assertTrue(exact.passed)
        self.assertTrue(exact.exact_match)
        self.assertFalse(whitespace_only.passed)
        self.assertFalse(whitespace_only.exact_match)
        self.assertTrue(
            whitespace_only.metrics["normalized_match"],
        )


class RasterComparisonTests(unittest.TestCase):
    @staticmethod
    def image(
        pixels: bytes,
        *,
        width: int = 2,
        height: int = 1,
        channels: int = 3,
    ):
        return COMPARE_EXAMPLES.RasterImage(
            width=width,
            height=height,
            channels=channels,
            pixels=pixels,
        )

    def test_exact_raster_match(self) -> None:
        image = self.image(bytes((1, 2, 3, 4, 5, 6)))
        result = COMPARE_EXAMPLES.compare_raster_images(
            image,
            image,
        )

        self.assertTrue(result.passed)
        self.assertTrue(result.exact_match)
        self.assertEqual(result.metrics["max_abs_error"], 0)
        self.assertEqual(result.metrics["values_over_tolerance"], 0)

    def test_small_difference_within_tolerance(self) -> None:
        cpp_image = self.image(
            bytes((10, 20, 30, 40, 50, 60)),
        )
        python_image = self.image(
            bytes((11, 20, 30, 40, 50, 60)),
        )
        result = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
            abs_tolerance=1,
        )

        self.assertTrue(result.passed)
        self.assertFalse(result.exact_match)
        self.assertEqual(result.metrics["max_abs_error"], 1)
        self.assertEqual(result.metrics["values_over_tolerance"], 0)

    def test_real_raster_regression_fails(self) -> None:
        cpp_image = self.image(
            bytes((10, 20, 30, 40, 50, 60)),
        )
        python_image = self.image(
            bytes((20, 20, 30, 40, 50, 60)),
        )
        result = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
            abs_tolerance=1,
        )

        self.assertFalse(result.passed)
        self.assertFalse(result.exact_match)
        self.assertEqual(result.metrics["values_over_tolerance"], 1)
        self.assertGreater(result.metrics["bad_fraction"], 0.0)

    def test_bad_fraction_policy_is_pixel_based(self) -> None:
        cpp_image = self.image(
            bytes((10, 20, 30, 40, 50, 60)),
        )
        python_image = self.image(
            bytes((20, 20, 30, 40, 50, 60)),
        )
        allowed = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
            abs_tolerance=1,
            max_bad_fraction=0.5,
        )
        too_strict = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
            abs_tolerance=1,
            max_bad_fraction=1.0 / 6.0,
        )

        self.assertTrue(allowed.passed)
        self.assertFalse(too_strict.passed)
        self.assertEqual(allowed.metrics["compared_pixels"], 2)
        self.assertEqual(allowed.metrics["pixels_over_tolerance"], 1)
        self.assertEqual(allowed.metrics["values_over_tolerance"], 1)
        self.assertEqual(allowed.metrics["bad_fraction"], 0.5)
        self.assertAlmostEqual(
            allowed.metrics["value_bad_fraction"],
            1.0 / 6.0,
        )

    def test_dimension_mismatch_fails_without_hiding_shape(self) -> None:
        cpp_image = self.image(
            bytes((1, 2, 3, 4, 5, 6)),
        )
        python_image = self.image(
            bytes((1, 2, 3)),
            width=1,
        )
        result = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
        )

        self.assertFalse(result.passed)
        self.assertEqual(
            result.reason,
            "raster shape mismatch",
        )
        self.assertEqual(result.metrics["cpp_width"], 2)
        self.assertEqual(result.metrics["python_width"], 1)

    def test_transparent_rgb_is_normalized_only_when_requested(self) -> None:
        cpp_image = self.image(
            bytes((255, 0, 0, 0)),
            width=1,
            channels=4,
        )
        python_image = self.image(
            bytes((0, 255, 0, 0)),
            width=1,
            channels=4,
        )

        normalized = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
        )
        raw = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
            normalize_transparent_rgb=False,
        )

        self.assertTrue(normalized.passed)
        self.assertTrue(normalized.exact_match)
        self.assertFalse(raw.passed)
        self.assertFalse(raw.exact_match)

    def test_raster_constructor_rejects_invalid_byte_count(self) -> None:
        with self.assertRaises(ValueError):
            self.image(bytes((1, 2, 3)))

    def test_alpha_diagnostics_reflect_comparison_policy(self) -> None:
        cases = (
            (
                self.image(
                    bytes((255, 0, 0, 0)),
                    width=1,
                    channels=4,
                ),
                self.image(
                    bytes((0, 255, 0, 0)),
                    width=1,
                    channels=4,
                ),
                False,
            ),
            (
                self.image(
                    bytes((0, 0, 0, 0)),
                    width=1,
                    channels=4,
                ),
                self.image(
                    bytes((0, 0, 0, 255)),
                    width=1,
                    channels=4,
                ),
                True,
            ),
        )

        for cpp_image, python_image, normalize in cases:
            with self.subTest(normalize=normalize):
                result = COMPARE_EXAMPLES.compare_raster_images(
                    cpp_image,
                    python_image,
                    normalize_transparent_rgb=normalize,
                )
                self.assertFalse(result.passed)

                with tempfile.TemporaryDirectory() as tmp:
                    output_dir = Path(tmp)
                    COMPARE_EXAMPLES.write_raster_diagnostics(
                        output_dir,
                        result,
                        cpp_image,
                        python_image,
                    )
                    heatmap = COMPARE_EXAMPLES.read_ppm(
                        output_dir / "diff_heatmap.ppm"
                    )

                self.assertGreater(max(heatmap.pixels), 0)

    def test_failure_diagnostics_are_machine_and_human_readable(
        self,
    ) -> None:
        cpp_image = self.image(
            bytes((10, 20, 30, 40, 50, 60)),
        )
        python_image = self.image(
            bytes((20, 20, 30, 40, 50, 60)),
        )
        result = COMPARE_EXAMPLES.compare_raster_images(
            cpp_image,
            python_image,
            abs_tolerance=1,
        )

        with tempfile.TemporaryDirectory() as tmp:
            output_dir = Path(tmp)
            COMPARE_EXAMPLES.write_raster_diagnostics(
                output_dir,
                result,
                cpp_image,
                python_image,
            )

            expected = {
                "comparison.json",
                "cpp.ppm",
                "python.ppm",
                "diff_heatmap.ppm",
                "side_by_side.ppm",
            }
            self.assertEqual(
                {path.name for path in output_dir.iterdir()},
                expected,
            )

            payload = json.loads(
                (output_dir / "comparison.json").read_text()
            )
            self.assertEqual(payload["kind"], "raster")
            self.assertFalse(payload["passed"])
            self.assertEqual(
                payload["metrics"]["max_abs_error"],
                10,
            )




class PpmIoTests(unittest.TestCase):
    def test_ppm_round_trip(self) -> None:
        image = COMPARE_EXAMPLES.RasterImage(
            width=2,
            height=1,
            channels=3,
            pixels=bytes((1, 2, 3, 4, 5, 6)),
        )
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "image.ppm"
            COMPARE_EXAMPLES.write_ppm(path, image)
            loaded = COMPARE_EXAMPLES.read_ppm(path)

        self.assertEqual(loaded, image)

    def test_ppm_rejects_wrong_magic(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "image.ppm"
            path.write_bytes(b"P3\n1 1\n255\n\x00\x00\x00")
            with self.assertRaises(ValueError):
                COMPARE_EXAMPLES.read_ppm(path)

    def test_ppm_rejects_wrong_payload_size(self) -> None:
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "image.ppm"
            path.write_bytes(b"P6\n2 1\n255\n\x00\x00\x00")
            with self.assertRaises(ValueError):
                COMPARE_EXAMPLES.read_ppm(path)


class RasterPairRegistryTests(unittest.TestCase):
    def assert_zero_tolerance_raster_pair(self, name: str) -> None:
        pair = COMPARE_EXAMPLES.PAIRS[name]

        self.assertEqual(pair.comparison_kind, "raster")
        self.assertEqual(pair.output_filename, "visual.ppm")
        self.assertEqual(pair.raster_abs_tolerance, 0)
        self.assertEqual(pair.raster_max_bad_fraction, 0.0)

    def test_aos2_visual_raster_pair_is_zero_tolerance(self) -> None:
        self.assert_zero_tolerance_raster_pair(
            "aos2_visual_buffer_raster"
        )

    def test_pol3_visual_raster_pair_is_zero_tolerance(self) -> None:
        self.assert_zero_tolerance_raster_pair(
            "pol3_visual_buffer_raster"
        )

    def test_preexisting_pairs_remain_stdout_pairs(self) -> None:
        raster_names = {
            "aos2_visual_buffer_raster",
            "pol3_visual_buffer_raster",
        }
        for name, pair in COMPARE_EXAMPLES.PAIRS.items():
            if name in raster_names:
                continue
            self.assertEqual(
                pair.comparison_kind,
                "stdout",
                msg=name,
            )




if __name__ == "__main__":
    unittest.main()
