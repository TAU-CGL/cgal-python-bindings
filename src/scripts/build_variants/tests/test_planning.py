#!/usr/bin/env python3
"""Tests for runner platform and build-directory planning."""

from __future__ import annotations

import json
import tempfile
import unittest
from pathlib import Path

from build_variants.catalog import load_catalog
from build_variants.cli import RunnerArguments
from build_variants.planning import (
    PlanningError,
    build_directory_name,
    compiler_tag,
    create_variant_plan,
    create_variant_plans,
    default_compiler,
)


class VariantPlanningTests(unittest.TestCase):
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

        self.python_executable = self.root / "python"
        self.python_executable.write_text(
            "",
            encoding="utf-8",
        )

        self.write_variant("alpha")
        self.write_variant("beta")

        self.catalog = load_catalog(
            self.manifest_directory,
            self.source_directory,
        )

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

    def arguments(
        self,
        *,
        manifests=("alpha",),
        operating_system="macos",
        compiler=None,
        build_type="Release",
        fixed_library_name=False,
    ) -> RunnerArguments:
        return RunnerArguments(
            manifests=tuple(manifests),
            build_type=build_type,
            fixed_library_name=fixed_library_name,
            operating_system=operating_system,
            compiler=compiler,
            generate_run=False,
            abort_after_run_generation=False,
            manifest_directory=self.manifest_directory,
            source_directory=self.source_directory,
            build_root=self.build_root,
            cgal_dir=None,
            python_executable=self.python_executable,
            nanobind_dir=None,
            jobs=4,
            list_manifests=False,
            dry_run=True,
            continue_on_error=False,
            log_directory=None,
        )

    def test_approved_default_compilers(self) -> None:
        self.assertEqual(default_compiler("linux"), "c++")
        self.assertEqual(
            default_compiler("macos"),
            "/usr/bin/c++",
        )
        self.assertEqual(default_compiler("windows"), "msvc")

    def test_unknown_operating_system_is_rejected(self) -> None:
        with self.assertRaisesRegex(
            PlanningError,
            "unsupported operating system",
        ):
            default_compiler("plan9")

    def test_compiler_tags_are_filesystem_safe(self) -> None:
        self.assertEqual(compiler_tag("c++"), "cxx")
        self.assertEqual(compiler_tag("g++"), "gxx")
        self.assertEqual(compiler_tag("msvc"), "msvc")
        self.assertEqual(
            compiler_tag("clang-cl.exe"),
            "clang_cl_exe",
        )

        self.assertRegex(
            compiler_tag("/usr/bin/c++"),
            r"^cxx_[0-9a-f]{8}$",
        )
        self.assertRegex(
            compiler_tag("/usr/bin/clang++"),
            r"^clangxx_[0-9a-f]{8}$",
        )

    def test_compiler_paths_with_same_basename_do_not_collide(
        self,
    ) -> None:
        system_compiler = compiler_tag(
            "/usr/bin/clang++"
        )
        homebrew_compiler = compiler_tag(
            "/opt/homebrew/bin/clang++"
        )

        self.assertNotEqual(
            system_compiler,
            homebrew_compiler,
        )
        self.assertTrue(
            system_compiler.startswith("clangxx_")
        )
        self.assertTrue(
            homebrew_compiler.startswith("clangxx_")
        )

    def test_build_directory_name_encodes_policy(self) -> None:
        self.assertEqual(
            build_directory_name(
                manifest_name="epec",
                operating_system="macos",
                compiler="/usr/bin/c++",
                fixed_library_name=False,
                build_type="Release",
            ),
            (
                "epec_macos_"
                f"{compiler_tag('/usr/bin/c++')}"
                "_computed_release"
            ),
        )

        self.assertEqual(
            build_directory_name(
                manifest_name="epec",
                operating_system="windows",
                compiler="msvc",
                fixed_library_name=True,
                build_type="Debug",
            ),
            "epec_windows_msvc_fixed_debug",
        )

    def test_macos_default_compiler_is_passed_to_cmake(self) -> None:
        plan = create_variant_plan(
            self.catalog.get("alpha"),
            self.arguments(),
        )

        self.assertEqual(plan.compiler, "/usr/bin/c++")
        self.assertEqual(
            plan.build_directory,
            (
                self.build_root
                / (
                    "alpha_macos_"
                    f"{compiler_tag('/usr/bin/c++')}"
                    "_computed_release"
                )
            ).resolve(),
        )

        self.assertIn(
            "-DCMAKE_CXX_COMPILER:FILEPATH=/usr/bin/c++",
            plan.configure_command,
        )

        self.assertEqual(
            plan.build_command,
            (
                "cmake",
                "--build",
                str(plan.build_directory),
                "--target",
                "BUILD",
                "--parallel",
                "4",
            ),
        )

    def test_windows_msvc_uses_default_toolchain_selection(self) -> None:
        plan = create_variant_plan(
            self.catalog.get("alpha"),
            self.arguments(
                operating_system="windows",
            ),
        )

        self.assertEqual(plan.compiler, "msvc")
        self.assertEqual(
            plan.build_directory.name,
            "alpha_windows_msvc_computed_release",
        )

        self.assertFalse(
            any(
                argument.startswith(
                    "-DCMAKE_CXX_COMPILER:"
                )
                for argument in plan.configure_command
            )
        )

        self.assertEqual(
            plan.build_command,
            (
                "cmake",
                "--build",
                str(plan.build_directory),
                "--target",
                "BUILD",
                "--config",
                "Release",
                "--parallel",
                "4",
            ),
        )

    def test_explicit_windows_compiler_is_passed_to_cmake(self) -> None:
        plan = create_variant_plan(
            self.catalog.get("alpha"),
            self.arguments(
                operating_system="windows",
                compiler="clang-cl",
            ),
        )

        self.assertIn(
            "-DCMAKE_CXX_COMPILER:FILEPATH=clang-cl",
            plan.configure_command,
        )

    def test_plan_order_matches_requested_manifest_order(self) -> None:
        plans = create_variant_plans(
            self.catalog,
            self.arguments(
                manifests=("beta", "alpha"),
                compiler="/usr/bin/clang++",
                build_type="Debug",
                fixed_library_name=True,
            ),
        )

        self.assertEqual(
            tuple(plan.manifest.name for plan in plans),
            ("beta", "alpha"),
        )

        expected_compiler_tag = compiler_tag(
            "/usr/bin/clang++"
        )

        self.assertEqual(
            tuple(plan.build_directory.name for plan in plans),
            (
                (
                    "beta_macos_"
                    f"{expected_compiler_tag}"
                    "_fixed_debug"
                ),
                (
                    "alpha_macos_"
                    f"{expected_compiler_tag}"
                    "_fixed_debug"
                ),
            ),
        )


if __name__ == "__main__":
    unittest.main()
