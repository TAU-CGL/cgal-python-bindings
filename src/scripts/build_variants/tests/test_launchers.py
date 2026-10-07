#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Tests for local generated build-variant launchers."""

from __future__ import annotations

import stat
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from build_variants.launchers import (
    LauncherError,
    launcher_path,
    render_launcher,
    write_launcher,
    write_launchers,
)
from build_variants.manifest import BuildVariantManifest


class LauncherGenerationTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()
        self.output_directory = self.root / "output"
        self.output_directory.mkdir()

        self.runner = self.root / "run"
        self.runner.write_text(
            (
                "import sys\n"
                "print('\\n'.join(sys.argv[1:]))\n"
                "raise SystemExit(7)\n"
            ),
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def manifest(
        self,
        name: str,
    ) -> BuildVariantManifest:
        return BuildVariantManifest(
            schema_version=1,
            name=name,
            description=f"Variant {name}",
            cmake_tests=(
                f"cmake/tests/{name}.cmake",
            ),
            path=self.root / f"{name}.json",
        )

    def test_launcher_path_uses_requested_directory(
        self,
    ) -> None:
        path = launcher_path(
            self.output_directory,
            "alpha",
        )

        self.assertEqual(
            path,
            self.output_directory / "run_alpha",
        )

    def test_render_launcher_contains_runner_and_manifest(
        self,
    ) -> None:
        content = render_launcher(
            self.runner,
            "alpha",
        )

        self.assertTrue(
            content.startswith("#!/usr/bin/env python3\n")
        )
        self.assertIn(
            f'RUNNER = "{self.runner}"',
            content,
        )
        self.assertIn(
            'MANIFEST = "alpha"',
            content,
        )
        self.assertIn(
            "+ sys.argv[1:]",
            content,
        )

    def test_generated_launcher_forwards_arguments_and_exit_code(
        self,
    ) -> None:
        path = write_launcher(
            self.output_directory,
            self.runner,
            self.manifest("alpha"),
        )

        result = subprocess.run(
            [
                sys.executable,
                str(path),
                "--build-type",
                "Debug",
            ],
            text=True,
            capture_output=True,
            check=False,
        )

        self.assertEqual(result.returncode, 7)
        self.assertEqual(result.stderr, "")
        self.assertEqual(
            result.stdout.splitlines(),
            [
                "alpha",
                "--build-type",
                "Debug",
            ],
        )

    def test_write_launcher_sets_executable_bits(
        self,
    ) -> None:
        path = write_launcher(
            self.output_directory,
            self.runner,
            self.manifest("alpha"),
        )

        mode = path.stat().st_mode

        self.assertTrue(mode & stat.S_IXUSR)
        self.assertTrue(mode & stat.S_IXGRP)
        self.assertTrue(mode & stat.S_IXOTH)

    def test_write_launcher_is_idempotent(
        self,
    ) -> None:
        manifest = self.manifest("alpha")

        first = write_launcher(
            self.output_directory,
            self.runner,
            manifest,
        )
        content_before = first.read_bytes()

        second = write_launcher(
            self.output_directory,
            self.runner,
            manifest,
        )

        self.assertEqual(first, second)
        self.assertEqual(
            second.read_bytes(),
            content_before,
        )

    def test_write_launcher_refuses_different_existing_file(
        self,
    ) -> None:
        destination = (
            self.output_directory / "run_alpha"
        )
        destination.write_text(
            "unrelated content\n",
            encoding="utf-8",
        )

        with self.assertRaisesRegex(
            LauncherError,
            "refusing to overwrite",
        ):
            write_launcher(
                self.output_directory,
                self.runner,
                self.manifest("alpha"),
            )

        self.assertEqual(
            destination.read_text(encoding="utf-8"),
            "unrelated content\n",
        )

    def test_write_launchers_preserves_manifest_order(
        self,
    ) -> None:
        paths = write_launchers(
            self.output_directory,
            self.runner,
            (
                self.manifest("beta"),
                self.manifest("alpha"),
            ),
        )

        self.assertEqual(
            paths,
            (
                self.output_directory / "run_beta",
                self.output_directory / "run_alpha",
            ),
        )

    def test_write_launchers_rejects_duplicate_names(
        self,
    ) -> None:
        with self.assertRaisesRegex(
            LauncherError,
            "must not be repeated",
        ):
            write_launchers(
                self.output_directory,
                self.runner,
                (
                    self.manifest("alpha"),
                    self.manifest("alpha"),
                ),
            )

    def test_write_launchers_preflights_all_destinations(
        self,
    ) -> None:
        conflicting_path = (
            self.output_directory / "run_beta"
        )
        conflicting_path.write_text(
            "unrelated content\n",
            encoding="utf-8",
        )

        with self.assertRaisesRegex(
            LauncherError,
            "refusing to overwrite",
        ):
            write_launchers(
                self.output_directory,
                self.runner,
                (
                    self.manifest("alpha"),
                    self.manifest("beta"),
                ),
            )

        self.assertFalse(
            (self.output_directory / "run_alpha").exists()
        )
        self.assertEqual(
            conflicting_path.read_text(encoding="utf-8"),
            "unrelated content\n",
        )

    def test_missing_paths_are_rejected(self) -> None:
        with self.subTest("missing output directory"):
            with self.assertRaisesRegex(
                LauncherError,
                "output directory does not exist",
            ):
                launcher_path(
                    self.root / "missing",
                    "alpha",
                )

        with self.subTest("missing runner"):
            with self.assertRaisesRegex(
                LauncherError,
                "runner entry point does not exist",
            ):
                render_launcher(
                    self.root / "missing-runner",
                    "alpha",
                )


if __name__ == "__main__":
    unittest.main()
