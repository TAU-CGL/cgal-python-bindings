#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Tests for the Python runner module entry point."""

from __future__ import annotations

import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from build_variants.execution import (
    CommandExecutionResult,
    VariantExecutionResult,
)
from build_variants.main import main


class RunnerMainTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()

        self.source_directory = self.root / "source"
        self.build_directory = self.root / "build"
        self.manifest_directory = (
            self.source_directory
            / "src/scripts/build_variants/manifests"
        )
        self.cmake_test_directory = (
            self.source_directory / "cmake/tests"
        )

        self.manifest_directory.mkdir(parents=True)
        self.cmake_test_directory.mkdir(parents=True)
        self.build_directory.mkdir()

        self.write_variant("alpha")
        self.write_variant("beta")

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def write_variant(self, name: str) -> None:
        reference = f"cmake/tests/{name}.cmake"

        (
            self.source_directory / reference
        ).write_text(
            f'set(VARIANT "{name}" CACHE STRING "" FORCE)\n',
            encoding="utf-8",
        )

        (
            self.manifest_directory / f"{name}.json"
        ).write_text(
            json.dumps(
                {
                    "schema_version": 1,
                    "name": name,
                    "description": f"Variant {name}",
                    "cmake_tests": [reference],
                },
                indent=2,
            )
            + "\n",
            encoding="utf-8",
        )

    def invoke(self, argv):
        stdout = io.StringIO()
        stderr = io.StringIO()

        exit_code = main(
            argv,
            stdout=stdout,
            stderr=stderr,
            source_directory=self.source_directory,
            manifest_directory=self.manifest_directory,
            build_directory=self.build_directory,
        )

        return (
            exit_code,
            stdout.getvalue(),
            stderr.getvalue(),
        )

    def test_list_mode_uses_application_layer(self) -> None:
        exit_code, stdout, stderr = self.invoke(
            ["--list"]
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout.splitlines(),
            [
                (
                    "alpha: Variant alpha "
                    "[cmake/tests/alpha.cmake]"
                ),
                (
                    "beta: Variant beta "
                    "[cmake/tests/beta.cmake]"
                ),
            ],
        )

    def test_dry_run_preserves_manifest_order(self) -> None:
        exit_code, stdout, stderr = self.invoke(
            [
                "beta",
                "alpha",
                "--dry-run",
                "--operating-system",
                "macos",
                "--compiler",
                "/usr/bin/c++",
            ]
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertLess(
            stdout.index("manifest: beta"),
            stdout.index("manifest: alpha"),
        )
        self.assertIn(
            "configure-command: cmake ",
            stdout,
        )
        self.assertIn(
            "build-command: cmake --build ",
            stdout,
        )

    def test_unknown_manifest_returns_application_error(
        self,
    ) -> None:
        exit_code, stdout, stderr = self.invoke(
            [
                "missing",
                "--dry-run",
            ]
        )

        self.assertEqual(exit_code, 2)
        self.assertEqual(stdout, "")
        self.assertIn(
            "error: unknown manifest 'missing'",
            stderr,
        )

    def test_live_execution_returns_application_status(
        self,
    ) -> None:
        def successful_execution(
            plans,
            **unused_arguments,
        ):
            return tuple(
                VariantExecutionResult(
                    plan=plan,
                    configure_result=(
                        CommandExecutionResult(
                            command=plan.configure_command,
                            return_code=0,
                            log_path=None,
                        )
                    ),
                    build_result=(
                        CommandExecutionResult(
                            command=plan.build_command,
                            return_code=0,
                            log_path=None,
                        )
                    ),
                    install_result=None,
                )
                for plan in plans
            )

        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=successful_execution,
        ):
            exit_code, stdout, stderr = self.invoke(
                ["alpha"]
            )

        self.assertEqual(exit_code, 0)
        self.assertEqual(
            stdout,
            "variant-result: alpha: success\n",
        )
        self.assertEqual(stderr, "")

    def test_dry_run_does_not_create_variant_build_directory(
        self,
    ) -> None:
        before = tuple(self.build_directory.iterdir())

        exit_code, stdout, stderr = self.invoke(
            [
                "alpha",
                "--dry-run",
            ]
        )

        after = tuple(self.build_directory.iterdir())

        self.assertEqual(exit_code, 0)
        self.assertTrue(stdout)
        self.assertEqual(stderr, "")
        self.assertEqual(before, after)


if __name__ == "__main__":
    unittest.main()
