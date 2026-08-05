#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Tests for runner list and dry-run output rendering."""

from __future__ import annotations

import unittest
from pathlib import Path

from build_variants.catalog import ManifestCatalog
from build_variants.manifest import BuildVariantManifest
from build_variants.output import (
    OutputError,
    format_command,
    render_catalog,
    render_variant_plan,
    render_variant_plans,
)
from build_variants.planning import VariantPlan


class RunnerOutputTests(unittest.TestCase):
    def manifest(
        self,
        name: str,
        description: str,
        references,
    ) -> BuildVariantManifest:
        return BuildVariantManifest(
            schema_version=1,
            name=name,
            description=description,
            cmake_tests=tuple(references),
            path=Path(f"/manifests/{name}.json"),
        )

    def plan(
        self,
        name: str,
        *,
        operating_system: str = "macos",
        variant_build_directory: Path = Path("/tmp/build path"),
        install_wheel: bool = False,
        pip_install_options=(),
    ) -> VariantPlan:
        manifest = self.manifest(
            name,
            f"Variant {name}",
            [f"cmake/tests/{name}.cmake"],
        )

        return VariantPlan(
            manifest=manifest,
            operating_system=operating_system,
            compiler="/usr/bin/c++",
            variant_build_directory=variant_build_directory,
            python_executable=Path("/usr/bin/python3"),
            install_wheel=install_wheel,
            pip_install_options=tuple(pip_install_options),
            configure_command=(
                "cmake",
                "-C",
                f"/source path/cmake/tests/{name}.cmake",
                "-S",
                "/source path",
                "-B",
                str(variant_build_directory),
            ),
            build_command=(
                "cmake",
                "--build",
                str(variant_build_directory),
                "--target",
                "BUILD",
                "--parallel",
                "4",
            ),
        )

    def test_posix_command_rendering_quotes_spaces(self) -> None:
        rendered = format_command(
            (
                "cmake",
                "-S",
                "/source path",
                "-B",
                "/build path",
            ),
            "macos",
        )

        self.assertEqual(
            rendered,
            "cmake -S '/source path' -B '/build path'",
        )

    def test_windows_command_rendering_quotes_spaces(self) -> None:
        rendered = format_command(
            (
                "cmake",
                "--build",
                r"C:\build path",
                "--config",
                "Release",
            ),
            "windows",
        )

        self.assertEqual(
            rendered,
            'cmake --build "C:\\build path" --config Release',
        )

    def test_empty_command_is_rejected(self) -> None:
        with self.assertRaisesRegex(
            OutputError,
            "at least one argument",
        ):
            format_command((), "linux")

    def test_non_string_argument_is_rejected(self) -> None:
        with self.assertRaisesRegex(
            OutputError,
            "must be a string",
        ):
            format_command(
                ("cmake", 4),
                "linux",
            )

    def test_null_byte_is_rejected(self) -> None:
        with self.assertRaisesRegex(
            OutputError,
            "null bytes",
        ):
            format_command(
                ("cmake", "bad\x00argument"),
                "linux",
            )

    def test_unknown_operating_system_is_rejected(self) -> None:
        with self.assertRaisesRegex(
            OutputError,
            "operating system must be one of",
        ):
            format_command(
                ("cmake",),
                "plan9",
            )

    def test_catalog_rendering_preserves_order(self) -> None:
        alpha = self.manifest(
            "alpha",
            "Alpha variant",
            ["cmake/tests/alpha.cmake"],
        )
        beta = self.manifest(
            "beta",
            "Beta variant",
            [
                "cmake/tests/beta.cmake",
                "cmake/tests/common.cmake",
            ],
        )

        catalog = ManifestCatalog(
            manifests=(alpha, beta),
        )

        self.assertEqual(
            render_catalog(catalog),
            (
                (
                    "alpha: Alpha variant "
                    "[cmake/tests/alpha.cmake]"
                ),
                (
                    "beta: Beta variant "
                    "[cmake/tests/beta.cmake, "
                    "cmake/tests/common.cmake]"
                ),
            ),
        )

    def test_variant_plan_rendering(self) -> None:
        plan = self.plan("alpha")

        lines = render_variant_plan(plan)

        self.assertEqual(
            lines[0],
            "manifest: alpha",
        )
        self.assertEqual(
            lines[1],
            "variant-build-directory: /tmp/build path",
        )
        self.assertEqual(
            lines[2],
            (
                "configure-command: "
                "cmake -C "
                "'/source path/cmake/tests/alpha.cmake' "
                "-S '/source path' "
                "-B '/tmp/build path'"
            ),
        )
        self.assertEqual(
            lines[3],
            (
                "build-command: "
                "cmake --build '/tmp/build path' "
                "--target BUILD --parallel 4"
            ),
        )
        self.assertEqual(
            lines[4],
            "install-wheel: disabled",
        )

    def test_variant_plan_rendering_reports_enabled_install(
        self,
    ) -> None:
        lines = render_variant_plan(
            self.plan(
                "alpha",
                install_wheel=True,
            )
        )

        self.assertEqual(
            lines[4],
            "install-wheel: enabled",
        )


    def test_multiple_plan_rendering_preserves_order(self) -> None:
        lines = render_variant_plans(
            (
                self.plan("beta"),
                self.plan("alpha"),
            )
        )

        self.assertEqual(
            lines[0],
            "manifest: beta",
        )
        self.assertEqual(
            lines[5],
            "",
        )
        self.assertEqual(
            lines[6],
            "manifest: alpha",
        )


if __name__ == "__main__":
    unittest.main()
