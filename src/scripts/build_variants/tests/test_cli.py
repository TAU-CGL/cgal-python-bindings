#!/usr/bin/env python3
"""Tests for runner command-line parsing."""

from __future__ import annotations

import tempfile
import unittest
from pathlib import Path
from unittest import mock

from build_variants.cli import (
    CliError,
    detect_operating_system,
    parse_arguments,
    positive_integer,
)


class RunnerCliTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()

        self.source_directory = self.root / "source"
        self.manifest_directory = self.root / "manifests"
        self.build_directory = self.root / "build"

        self.source_directory.mkdir()
        self.manifest_directory.mkdir()
        self.build_directory.mkdir()

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def parse(self, arguments):
        return parse_arguments(
            arguments,
            default_source_directory=self.source_directory,
            default_manifest_directory=self.manifest_directory,
            default_build_directory=self.build_directory,
        )

    def test_minimal_manifest_defaults(self) -> None:
        parsed = self.parse(["epec"])

        self.assertEqual(parsed.manifests, ("epec",))
        self.assertFalse(parsed.all_manifests)
        self.assertEqual(parsed.build_type, "Release")
        self.assertFalse(parsed.fixed_library_name)
        self.assertFalse(parsed.install_wheel)
        self.assertEqual(parsed.pip_install_options, ())
        self.assertEqual(parsed.jobs, 4)
        self.assertFalse(parsed.dry_run)
        self.assertFalse(parsed.continue_on_error)

    def test_all_approved_options_parse(self) -> None:
        cgal_dir = self.root / "cgal"
        nanobind_dir = self.root / "nanobind"
        python_executable = self.root / "python"
        log_directory = self.root / "logs"

        parsed = self.parse(
            [
                "epec",
                "sm_sms_epic",
                "--build-type",
                "Debug",
                "--fixed-library-name",
                "--operating-system",
                "macos",
                "--compiler",
                "/usr/bin/c++",
                "--generate-run",
                "--abort-after-run-generation",
                "--manifest-directory",
                str(self.manifest_directory),
                "--source-directory",
                str(self.source_directory),
                "--build-directory",
                str(self.build_directory),
                "--cgal-dir",
                str(cgal_dir),
                "--python",
                str(python_executable),
                "--install-wheel",
                "--pip-install-option=--user",
                "--pip-install-option=--no-deps",
                "--nanobind-dir",
                str(nanobind_dir),
                "--jobs",
                "7",
                "--dry-run",
                "--continue-on-error",
                "--log-directory",
                str(log_directory),
            ]
        )

        self.assertEqual(
            parsed.manifests,
            ("epec", "sm_sms_epic"),
        )
        self.assertEqual(parsed.build_type, "Debug")
        self.assertTrue(parsed.fixed_library_name)
        self.assertEqual(parsed.operating_system, "macos")
        self.assertEqual(parsed.compiler, "/usr/bin/c++")
        self.assertTrue(parsed.generate_run)
        self.assertTrue(parsed.abort_after_run_generation)
        self.assertEqual(parsed.jobs, 7)
        self.assertTrue(parsed.dry_run)
        self.assertTrue(parsed.continue_on_error)
        self.assertTrue(parsed.install_wheel)
        self.assertEqual(
            parsed.pip_install_options,
            ("--user", "--no-deps"),
        )
        self.assertEqual(parsed.cgal_dir, cgal_dir.resolve())
        self.assertEqual(
            parsed.python_executable,
            python_executable.resolve(),
        )
        self.assertEqual(
            parsed.nanobind_dir,
            nanobind_dir.resolve(),
        )
        self.assertEqual(
            parsed.log_directory,
            log_directory.resolve(),
        )

    def test_list_does_not_require_manifest(self) -> None:
        parsed = self.parse(["--list"])

        self.assertTrue(parsed.list_manifests)
        self.assertEqual(parsed.manifests, ())

    def test_all_selects_catalog_instead_of_explicit_names(
        self,
    ) -> None:
        parsed = self.parse(["--all"])

        self.assertTrue(parsed.all_manifests)
        self.assertEqual(parsed.manifests, ())

    def test_all_with_explicit_manifest_is_rejected(self) -> None:
        with self.assertRaises(SystemExit) as context:
            self.parse(["epec", "--all"])

        self.assertEqual(context.exception.code, 2)

    def test_missing_manifest_without_list_is_rejected(self) -> None:
        with self.assertRaises(SystemExit) as context:
            self.parse([])

        self.assertEqual(context.exception.code, 2)

    def test_duplicate_manifest_is_rejected(self) -> None:
        with self.assertRaises(SystemExit) as context:
            self.parse(["epec", "epec"])

        self.assertEqual(context.exception.code, 2)

    def test_abort_requires_generate_run(self) -> None:
        with self.assertRaises(SystemExit) as context:
            self.parse(
                [
                    "epec",
                    "--abort-after-run-generation",
                ]
            )

        self.assertEqual(context.exception.code, 2)

    def test_nonpositive_jobs_are_rejected(self) -> None:
        for value in ("0", "-1", "not-a-number"):
            with self.subTest(value=value):
                with self.assertRaises(SystemExit) as context:
                    self.parse(
                        [
                            "epec",
                            "--jobs",
                            value,
                        ]
                    )

                self.assertEqual(context.exception.code, 2)

    def test_pip_install_option_requires_install_wheel(self) -> None:
        with self.assertRaises(SystemExit) as context:
            self.parse(
                [
                    "epec",
                    "--pip-install-option=--user",
                ]
            )

        self.assertEqual(context.exception.code, 2)

    def test_positive_integer(self) -> None:
        self.assertEqual(positive_integer("5"), 5)

    def test_supported_operating_system_detection(self) -> None:
        cases = {
            "Darwin": "macos",
            "Linux": "linux",
            "Windows": "windows",
        }

        for reported, expected in cases.items():
            with self.subTest(reported=reported):
                with mock.patch(
                    "build_variants.cli.platform.system",
                    return_value=reported,
                ):
                    self.assertEqual(
                        detect_operating_system(),
                        expected,
                    )

    def test_unknown_operating_system_is_rejected(self) -> None:
        with mock.patch(
            "build_variants.cli.platform.system",
            return_value="Plan9",
        ):
            with self.assertRaisesRegex(
                CliError,
                "unsupported operating system",
            ):
                detect_operating_system()


if __name__ == "__main__":
    unittest.main()
