#!/usr/bin/env python3
"""Repository-level coverage test for the canonical manifest catalog."""

from __future__ import annotations

import unittest
from pathlib import Path

from build_variants.catalog import load_catalog


class RepositoryManifestCatalogTests(unittest.TestCase):
    def test_catalog_covers_each_publishable_cmake_test_once(
        self,
    ) -> None:
        repository = Path(__file__).resolve().parents[4]
        manifest_directory = (
            repository
            / "src/scripts/build_variants/manifests"
        )
        cmake_test_directory = repository / "cmake/tests"

        excluded_aliases = {
            "cmake/tests/aos2_seg_fe_epec.cmake",
            "cmake/tests/ch2_epic_fixed.cmake",
            "cmake/tests/epec_fixed.cmake",
            "cmake/tests/pol3_pmp_sms_epic.cmake",
            "cmake/tests/sm_pmp_sms_epic.cmake",
        }

        excluded_alias_names = {
            Path(reference).stem
            for reference in excluded_aliases
        }

        canonical_references = {
            path.relative_to(repository).as_posix()
            for path in cmake_test_directory.glob("*.cmake")
        } - excluded_aliases

        deferred_references = {
            "cmake/tests/ss_epekd.cmake",
        }

        expected_references = (
            canonical_references - deferred_references
        )

        catalog = load_catalog(
            manifest_directory,
            repository,
        )

        actual_references = tuple(
            reference
            for manifest in catalog.manifests
            for reference in manifest.cmake_tests
        )

        self.assertEqual(len(canonical_references), 137)
        self.assertEqual(
            deferred_references,
            canonical_references & deferred_references,
        )
        self.assertEqual(len(catalog.manifests), 136)
        self.assertEqual(len(expected_references), 136)
        self.assertEqual(
            len(actual_references),
            len(set(actual_references)),
        )
        self.assertEqual(
            set(actual_references),
            expected_references,
        )
        self.assertTrue(
            excluded_alias_names.isdisjoint(catalog.names)
        )


if __name__ == "__main__":
    unittest.main()
