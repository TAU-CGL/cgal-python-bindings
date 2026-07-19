#!/usr/bin/env python3
"""Tests for cross-platform CMake command construction."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from build_variants.commands import (
    CommandError,
    ConfigureOptions,
    build_build_command,
    build_configure_command,
)
from build_variants.manifest import load_manifest


class ConfigureCommandTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()

        self.source_directory = self.root / "source"
        self.build_root = self.root / "build"
        self.manifest_directory = (
            self.source_directory
            / "src/scripts/build_variants/manifests"
        )
        self.cmake_test_directory = (
            self.source_directory / "cmake/tests"
        )

        self.manifest_directory.mkdir(parents=True)
        self.cmake_test_directory.mkdir(parents=True)
        self.build_root.mkdir()

        (
            self.cmake_test_directory / "first.cmake"
        ).write_text(
            'set(FIRST ON CACHE BOOL "" FORCE)\n',
            encoding="utf-8",
        )

        (
            self.cmake_test_directory / "second.cmake"
        ).write_text(
            'set(SECOND ON CACHE BOOL "" FORCE)\n',
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def write_manifest(
        self,
        name: str = "example",
        references=None,
    ):
        if references is None:
            references = [
                "cmake/tests/first.cmake",
            ]

        path = self.manifest_directory / f"{name}.json"

        path.write_text(
            json.dumps(
                {
                    "schema_version": 1,
                    "name": name,
                    "description": "Test manifest",
                    "cmake_tests": references,
                },
                indent=2,
            )
            + "\n",
            encoding="utf-8",
        )

        return load_manifest(
            path,
            self.source_directory,
        )

    def test_minimal_release_command(self) -> None:
        manifest = self.write_manifest()

        command = build_configure_command(
            manifest,
            ConfigureOptions(
                source_directory=self.source_directory,
                build_directory=self.build_root / "example",
            ),
        )

        self.assertEqual(
            command,
            (
                "cmake",
                "-C",
                str(
                    (
                        self.cmake_test_directory
                        / "first.cmake"
                    ).resolve()
                ),
                "-DCMAKE_BUILD_TYPE:STRING=Release",
                "-DCGALPY_FIXED_LIBRARY_NAME:BOOL=OFF",
                "-S",
                str(self.source_directory),
                "-B",
                str((self.build_root / "example").resolve()),
            ),
        )

    def test_multiple_preloads_preserve_manifest_order(self) -> None:
        manifest = self.write_manifest(
            references=[
                "cmake/tests/second.cmake",
                "cmake/tests/first.cmake",
            ],
        )

        command = build_configure_command(
            manifest,
            ConfigureOptions(
                source_directory=self.source_directory,
                build_directory=self.build_root / "ordered",
            ),
        )

        second_index = command.index(
            str(
                (
                    self.cmake_test_directory
                    / "second.cmake"
                ).resolve()
            )
        )
        first_index = command.index(
            str(
                (
                    self.cmake_test_directory
                    / "first.cmake"
                ).resolve()
            )
        )
        build_type_index = command.index(
            "-DCMAKE_BUILD_TYPE:STRING=Release"
        )

        self.assertLess(second_index, first_index)
        self.assertLess(first_index, build_type_index)

    def test_runner_overrides_follow_preloads(self) -> None:
        manifest = self.write_manifest(
            references=[
                "cmake/tests/first.cmake",
                "cmake/tests/second.cmake",
            ],
        )

        cgal_dir = self.root / "cgal-build"
        nanobind_dir = self.root / "nanobind"
        python_executable = self.root / "python"

        cgal_dir.mkdir()
        nanobind_dir.mkdir()
        python_executable.write_text(
            "",
            encoding="utf-8",
        )

        command = build_configure_command(
            manifest,
            ConfigureOptions(
                source_directory=self.source_directory,
                build_directory=self.build_root / "debug",
                cmake_executable="/path with spaces/cmake",
                build_type="Debug",
                fixed_library_name=True,
                compiler="/usr/bin/clang++",
                cgal_dir=cgal_dir,
                python_executable=python_executable,
                nanobind_dir=nanobind_dir,
            ),
        )

        last_preload_index = command.index(
            str(
                (
                    self.cmake_test_directory
                    / "second.cmake"
                ).resolve()
            )
        )

        expected_overrides = (
            "-DCMAKE_BUILD_TYPE:STRING=Debug",
            "-DCGALPY_FIXED_LIBRARY_NAME:BOOL=ON",
            "-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/clang++",
            f"-DCGAL_DIR:PATH={cgal_dir.resolve()}",
            "-DPython_EXECUTABLE:FILEPATH="
            f"{python_executable.resolve()}",
            f"-Dnanobind_DIR:PATH={nanobind_dir.resolve()}",
        )

        for override in expected_overrides:
            self.assertGreater(
                command.index(override),
                last_preload_index,
            )

        self.assertEqual(
            command[0],
            "/path with spaces/cmake",
        )

    def test_macos_build_command(self) -> None:
        build_directory = self.build_root / "macos"

        command = build_build_command(
            build_directory,
            jobs=4,
            build_type="Release",
            operating_system="macos",
        )

        self.assertEqual(
            command,
            (
                "cmake",
                "--build",
                str(build_directory.resolve()),
                "--target",
                "BUILD",
                "--parallel",
                "4",
            ),
        )

    def test_linux_build_command(self) -> None:
        build_directory = self.build_root / "linux"

        command = build_build_command(
            build_directory,
            jobs=7,
            build_type="Debug",
            operating_system="linux",
            cmake_executable="/opt/cmake/bin/cmake",
        )

        self.assertEqual(
            command,
            (
                "/opt/cmake/bin/cmake",
                "--build",
                str(build_directory.resolve()),
                "--target",
                "BUILD",
                "--parallel",
                "7",
            ),
        )

    def test_windows_build_command_includes_configuration(
        self,
    ) -> None:
        build_directory = self.build_root / "windows"

        command = build_build_command(
            build_directory,
            jobs=2,
            build_type="Debug",
            operating_system="windows",
        )

        self.assertEqual(
            command,
            (
                "cmake",
                "--build",
                str(build_directory.resolve()),
                "--target",
                "BUILD",
                "--config",
                "Debug",
                "--parallel",
                "2",
            ),
        )

    def test_invalid_build_jobs_are_rejected(self) -> None:
        for jobs in (0, -1, True):
            with self.subTest(jobs=jobs):
                with self.assertRaisesRegex(
                    CommandError,
                    "jobs must be a positive integer",
                ):
                    build_build_command(
                        self.build_root / "invalid-jobs",
                        jobs=jobs,
                        build_type="Release",
                        operating_system="macos",
                    )

    def test_invalid_build_operating_system_is_rejected(
        self,
    ) -> None:
        with self.assertRaisesRegex(
            CommandError,
            "operating system must be one of",
        ):
            build_build_command(
                self.build_root / "invalid-os",
                jobs=4,
                build_type="Release",
                operating_system="plan9",
            )


    def test_invalid_build_type_is_rejected(self) -> None:
        manifest = self.write_manifest()

        with self.assertRaisesRegex(
            CommandError,
            "build type must be one of",
        ):
            build_configure_command(
                manifest,
                ConfigureOptions(
                    source_directory=self.source_directory,
                    build_directory=self.build_root / "invalid",
                    build_type="RelWithDebInfo",
                ),
            )

    def test_in_source_build_is_rejected(self) -> None:
        manifest = self.write_manifest()

        with self.assertRaisesRegex(
            CommandError,
            "must be detached",
        ):
            build_configure_command(
                manifest,
                ConfigureOptions(
                    source_directory=self.source_directory,
                    build_directory=(
                        self.source_directory / "build"
                    ),
                ),
            )


if __name__ == "__main__":
    unittest.main()
