#!/usr/bin/env python3
"""Tests for build-variant manifest validation."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path
from typing import Any, Dict

from build_variants.manifest import ManifestError, load_manifest


class ManifestValidationTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.source_directory = Path(
            self.temporary_directory.name
        ).resolve()

        self.test_directory = (
            self.source_directory / "cmake/tests"
        )
        self.manifest_directory = (
            self.source_directory
            / "src/scripts/build_variants/manifests"
        )

        self.test_directory.mkdir(parents=True)
        self.manifest_directory.mkdir(parents=True)

        (
            self.test_directory / "epec.cmake"
        ).write_text(
            'set(CGALPY_KERNEL_NAME "epec" CACHE STRING "" FORCE)\n',
            encoding="utf-8",
        )

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def valid_data(self) -> Dict[str, Any]:
        return {
            "schema_version": 1,
            "name": "epec",
            "description": "EPEC kernel",
            "cmake_tests": [
                "cmake/tests/epec.cmake",
            ],
        }

    def write_manifest(
        self,
        data: Dict[str, Any],
        filename: str = "epec.json",
    ) -> Path:
        path = self.manifest_directory / filename
        path.write_text(
            json.dumps(data, indent=2) + "\n",
            encoding="utf-8",
        )
        return path

    def test_valid_manifest_loads(self) -> None:
        path = self.write_manifest(self.valid_data())

        manifest = load_manifest(
            path,
            self.source_directory,
        )

        self.assertEqual(manifest.schema_version, 1)
        self.assertEqual(manifest.name, "epec")
        self.assertEqual(
            manifest.cmake_tests,
            ("cmake/tests/epec.cmake",),
        )

    def test_unknown_field_is_rejected(self) -> None:
        data = self.valid_data()
        data["unexpected"] = True
        path = self.write_manifest(data)

        with self.assertRaisesRegex(
            ManifestError,
            "unknown fields: unexpected",
        ):
            load_manifest(path, self.source_directory)

    def test_missing_cmake_file_is_rejected(self) -> None:
        data = self.valid_data()
        data["cmake_tests"] = [
            "cmake/tests/missing.cmake",
        ]
        path = self.write_manifest(data)

        with self.assertRaisesRegex(
            ManifestError,
            "does not exist",
        ):
            load_manifest(path, self.source_directory)

    def test_parent_traversal_is_rejected(self) -> None:
        data = self.valid_data()
        data["cmake_tests"] = [
            "cmake/tests/../outside.cmake",
        ]
        path = self.write_manifest(data)

        with self.assertRaisesRegex(
            ManifestError,
            "invalid CMake test reference",
        ):
            load_manifest(path, self.source_directory)

    def test_duplicate_reference_is_rejected(self) -> None:
        data = self.valid_data()
        data["cmake_tests"] = [
            "cmake/tests/epec.cmake",
            "cmake/tests/epec.cmake",
        ]
        path = self.write_manifest(data)

        with self.assertRaisesRegex(
            ManifestError,
            "duplicate references",
        ):
            load_manifest(path, self.source_directory)

    def test_filename_and_name_must_match(self) -> None:
        path = self.write_manifest(
            self.valid_data(),
            filename="different.json",
        )

        with self.assertRaisesRegex(
            ManifestError,
            "filename must match",
        ):
            load_manifest(path, self.source_directory)

    def test_repository_epec_manifest_loads(self) -> None:
        repository = Path(__file__).resolve().parents[4]
        path = (
            repository
            / "src/scripts/build_variants/manifests/epec.json"
        )

        manifest = load_manifest(path, repository)

        self.assertEqual(manifest.name, "epec")
        self.assertEqual(
            manifest.cmake_tests,
            ("cmake/tests/epec.cmake",),
        )


if __name__ == "__main__":
    unittest.main()
