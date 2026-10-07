# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Subprocess tests for the required src/scripts/run entry point."""

from __future__ import annotations

import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path


class RequiredRunEntryPointTests(unittest.TestCase):
    def setUp(self) -> None:
        self.run_script = (
            Path(__file__).resolve().parents[2] / "run"
        )

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

    def invoke(
        self,
        arguments,
        *,
        environment=None,
    ):
        command = [
            sys.executable,
            str(self.run_script),
            *arguments,
            "--source-directory",
            str(self.source_directory),
            "--manifest-directory",
            str(self.manifest_directory),
            "--build-directory",
            str(self.build_directory),
        ]

        return subprocess.run(
            command,
            text=True,
            capture_output=True,
            check=False,
            cwd=self.root,
            env=environment,
        )

    def test_required_entry_point_has_python_shebang(
        self,
    ) -> None:
        first_line = self.run_script.read_text(
            encoding="utf-8"
        ).splitlines()[0]

        expected = (
            chr(35)
            + chr(33)
            + "/usr/bin/env python3"
        )

        self.assertEqual(first_line, expected)

    def test_required_entry_point_lists_manifests(
        self,
    ) -> None:
        result = self.invoke(["--list"])

        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stderr, "")
        self.assertEqual(
            result.stdout.splitlines(),
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

    def test_required_entry_point_dry_run_preserves_order(
        self,
    ) -> None:
        before = tuple(self.build_directory.iterdir())

        result = self.invoke(
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

        after = tuple(self.build_directory.iterdir())

        self.assertEqual(result.returncode, 0)
        self.assertEqual(result.stderr, "")
        self.assertLess(
            result.stdout.index("manifest: beta"),
            result.stdout.index("manifest: alpha"),
        )
        self.assertIn(
            "configure-command: cmake ",
            result.stdout,
        )
        self.assertIn(
            "build-command: cmake --build ",
            result.stdout,
        )
        self.assertEqual(before, after)

    def test_required_entry_point_reaches_executor(
        self,
    ) -> None:
        empty_path = self.root / "empty-path"
        empty_path.mkdir()

        environment = dict(os.environ)
        environment["PATH"] = str(empty_path)

        result = self.invoke(
            ["alpha"],
            environment=environment,
        )

        self.assertEqual(result.returncode, 2)
        self.assertEqual(result.stdout, "")
        self.assertIn(
            "error: could not start command 'cmake':",
            result.stderr,
        )

        build_directories = tuple(
            self.build_directory.iterdir()
        )

        self.assertEqual(
            len(build_directories),
            1,
        )
        self.assertTrue(
            build_directories[0].is_dir()
        )


if __name__ == "__main__":
    unittest.main()
