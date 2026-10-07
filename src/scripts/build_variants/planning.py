#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Resolve platform policy and create build-variant execution plans."""

from __future__ import annotations

import hashlib
import re
from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Tuple

from .catalog import ManifestCatalog
from .cli import RunnerArguments
from .commands import (
    ConfigureOptions,
    build_build_command,
    build_configure_command,
)
from .manifest import BuildVariantManifest


_DEFAULT_COMPILERS = {
    "linux": "gcc",
    "macos": "clang",
    "windows": "msvc",
}

_CMAKE_COMPILERS = {
    "gcc": "g++",
    "clang": "clang++",
}


class PlanningError(ValueError):
    """Raised when a build-variant plan cannot be resolved."""


@dataclass(frozen=True)
class VariantPlan:
    """Resolved paths and commands for one semantic build variant."""

    manifest: BuildVariantManifest
    operating_system: str
    compiler: str
    variant_build_directory: Path
    python_executable: Path
    install_wheel: bool
    pip_install_options: Tuple[str, ...]
    configure_command: Tuple[str, ...]
    build_command: Tuple[str, ...]


def default_compiler(operating_system: str) -> str:
    """Return the approved default compiler identity for an OS."""

    try:
        return _DEFAULT_COMPILERS[operating_system]
    except KeyError as exc:
        supported = ", ".join(sorted(_DEFAULT_COMPILERS))
        raise PlanningError(
            f"unsupported operating system {operating_system!r}; "
            f"supported operating systems: {supported}"
        ) from exc


def resolve_compiler(
    operating_system: str,
    requested_compiler: Optional[str],
) -> str:
    """Resolve an explicit compiler or the operating-system default."""

    if requested_compiler is None:
        return default_compiler(operating_system)

    if (
        not isinstance(requested_compiler, str)
        or not requested_compiler.strip()
    ):
        raise PlanningError(
            "compiler override must be a nonempty string"
        )

    return requested_compiler.strip()


def compiler_tag(compiler: str) -> str:
    """Return a filesystem-safe stable compiler identifier."""

    normalized_compiler = compiler.replace("\\", "/")
    compiler_name = normalized_compiler.rsplit("/", 1)[-1]

    compiler_name = compiler_name.replace("++", "xx")
    compiler_name = compiler_name.replace("+", "x")

    tag = re.sub(
        r"[^A-Za-z0-9]+",
        "_",
        compiler_name,
    ).strip("_").lower()

    if not tag:
        raise PlanningError(
            f"could not derive compiler tag from {compiler!r}"
        )

    if normalized_compiler != normalized_compiler.rsplit("/", 1)[-1]:
        digest = hashlib.sha256(
            compiler.encode("utf-8")
        ).hexdigest()[:8]

        tag = f"{tag}_{digest}"

    return tag


def variant_build_directory_name(
    manifest_name: str,
    operating_system: str,
    compiler: str,
    fixed_library_name: bool,
    build_type: str,
) -> str:
    """Create a collision-resistant detached variant build directory name."""

    naming_mode = (
        "fixed"
        if fixed_library_name
        else "computed"
    )

    return "_".join(
        [
            manifest_name,
            operating_system,
            compiler_tag(compiler),
            naming_mode,
            build_type.lower(),
        ]
    )


def _cmake_compiler(
    operating_system: str,
    compiler: str,
) -> Optional[str]:
    """Map toolchain identities to optional CMake compiler arguments."""

    normalized_compiler = compiler.lower()

    if (
        operating_system == "windows"
        and normalized_compiler == "msvc"
    ):
        return None

    return _CMAKE_COMPILERS.get(
        normalized_compiler,
        compiler,
    )


def create_variant_plan(
    manifest: BuildVariantManifest,
    arguments: RunnerArguments,
) -> VariantPlan:
    """Create one configure plan without running external commands."""

    compiler = resolve_compiler(
        arguments.operating_system,
        arguments.compiler,
    )

    variant_build_directory = (
        arguments.build_directory
        / variant_build_directory_name(
            manifest_name=manifest.name,
            operating_system=arguments.operating_system,
            compiler=compiler,
            fixed_library_name=arguments.fixed_library_name,
            build_type=arguments.build_type,
        )
    ).resolve()

    configure_command = build_configure_command(
        manifest,
        ConfigureOptions(
            source_directory=arguments.source_directory,
            variant_build_directory=variant_build_directory,
            cmake_executable="cmake",
            build_type=arguments.build_type,
            fixed_library_name=arguments.fixed_library_name,
            compiler=_cmake_compiler(
                arguments.operating_system,
                compiler,
            ),
            cgal_dir=arguments.cgal_dir,
            python_executable=arguments.python_executable,
            nanobind_dir=arguments.nanobind_dir,
            quiet=arguments.quiet,
        ),
    )

    build_command = build_build_command(
        variant_build_directory,
        jobs=arguments.jobs,
        build_type=arguments.build_type,
        operating_system=arguments.operating_system,
        cmake_executable="cmake",
    )

    return VariantPlan(
        manifest=manifest,
        operating_system=arguments.operating_system,
        compiler=compiler,
        variant_build_directory=variant_build_directory,
        python_executable=arguments.python_executable,
        install_wheel=arguments.install_wheel,
        pip_install_options=arguments.pip_install_options,
        configure_command=configure_command,
        build_command=build_command,
    )


def select_manifests(
    catalog: ManifestCatalog,
    arguments: RunnerArguments,
) -> Tuple[BuildVariantManifest, ...]:
    """Select manifests in deterministic execution order."""

    if arguments.all_manifests:
        return catalog.manifests

    return tuple(
        catalog.get(name)
        for name in arguments.manifests
    )


def create_variant_plans(
    catalog: ManifestCatalog,
    arguments: RunnerArguments,
) -> Tuple[VariantPlan, ...]:
    """Create plans in selected manifest order."""

    return tuple(
        create_variant_plan(
            manifest,
            arguments,
        )
        for manifest in select_manifests(
            catalog,
            arguments,
        )
    )
