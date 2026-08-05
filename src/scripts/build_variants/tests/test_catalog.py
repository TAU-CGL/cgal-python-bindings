#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Tests for build-variant manifest discovery and selection."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from build_variants.catalog import (
    CatalogError,
    load_catalog,
)


class ManifestCatalogTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.source_directory = Path(
            self.temporary_directory.name
        ).resolve()

        self.manifest_directory = (
            self.source_directory
            / "src/scripts/build_variants/manifests"
        )
        self.cmake_test_directory = (
            self.source_directory / "cmake/tests"
        )

        self.manifest_directory.mkdir(parents=True)
        self.cmake_test_directory.mkdir(parents=True)

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def write_variant(self, name: str) -> Path:
        cmake_reference = f"cmake/tests/{name}.cmake"

        (
            self.source_directory / cmake_reference
        ).write_text(
            f'set(VARIANT "{name}" CACHE STRING "" FORCE)\n',
            encoding="utf-8",
        )

        manifest_path = (
            self.manifest_directory / f"{name}.json"
        )

        manifest_path.write_text(
            json.dumps(
                {
                    "schema_version": 1,
                    "name": name,
                    "description": f"Variant {name}",
                    "cmake_tests": [
                        cmake_reference,
                    ],
                },
                indent=2,
            )
            + "\n",
            encoding="utf-8",
        )

        return manifest_path

    def test_catalog_is_sorted_by_manifest_filename(self) -> None:
        self.write_variant("zeta")
        self.write_variant("alpha")

        catalog = load_catalog(
            self.manifest_directory,
            self.source_directory,
        )

        self.assertEqual(
            catalog.names,
            ("alpha", "zeta"),
        )

    def test_non_json_files_are_ignored(self) -> None:
        self.write_variant("alpha")

        (
            self.manifest_directory / "notes.txt"
        ).write_text(
            "not a manifest\n",
            encoding="utf-8",
        )

        nested_directory = (
            self.manifest_directory / "nested"
        )
        nested_directory.mkdir()

        (
            nested_directory / "nested.json"
        ).write_text(
            "{}\n",
            encoding="utf-8",
        )

        catalog = load_catalog(
            self.manifest_directory,
            self.source_directory,
        )

        self.assertEqual(
            catalog.names,
            ("alpha",),
        )

    def test_catalog_selects_manifest_by_exact_name(self) -> None:
        self.write_variant("alpha")
        self.write_variant("beta")

        catalog = load_catalog(
            self.manifest_directory,
            self.source_directory,
        )

        manifest = catalog.get("beta")

        self.assertEqual(manifest.name, "beta")
        self.assertEqual(
            manifest.cmake_tests,
            ("cmake/tests/beta.cmake",),
        )

    def test_unknown_name_lists_available_manifests(self) -> None:
        self.write_variant("alpha")
        self.write_variant("beta")

        catalog = load_catalog(
            self.manifest_directory,
            self.source_directory,
        )

        with self.assertRaisesRegex(
            CatalogError,
            "available manifests: alpha, beta",
        ):
            catalog.get("missing")

    def test_missing_manifest_directory_is_rejected(self) -> None:
        with self.assertRaisesRegex(
            CatalogError,
            "manifest directory does not exist",
        ):
            load_catalog(
                self.source_directory / "missing",
                self.source_directory,
            )

    def test_repository_catalog_discovers_epec(self) -> None:
        repository = Path(__file__).resolve().parents[4]
        manifest_directory = (
            repository
            / "src/scripts/build_variants/manifests"
        )

        catalog = load_catalog(
            manifest_directory,
            repository,
        )

        self.assertIn("epec", catalog.names)

        manifest = catalog.get("epec")

        self.assertEqual(
            manifest.cmake_tests,
            ("cmake/tests/epec.cmake",),
        )


if __name__ == "__main__":
    unittest.main()
