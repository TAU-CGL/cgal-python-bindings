#!/usr/bin/env python3
"""Construct cross-platform CMake commands for build manifests."""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Optional, Tuple

from .manifest import BuildVariantManifest


_SUPPORTED_BUILD_TYPES = {
    "Release",
    "Debug",
}


class CommandError(ValueError):
    """Raised when runner command options are invalid."""


@dataclass(frozen=True)
class ConfigureOptions:
    """Options needed to construct one CMake configure command."""

    source_directory: Path
    build_variant_directory: Path
    cmake_executable: str = "cmake"
    build_type: str = "Release"
    fixed_library_name: bool = False
    compiler: Optional[str] = None
    cgal_dir: Optional[Path] = None
    python_executable: Optional[Path] = None
    nanobind_dir: Optional[Path] = None


def _existing_directory(path: Path, label: str) -> Path:
    resolved = Path(path).expanduser().resolve()

    if not resolved.is_dir():
        raise CommandError(
            f"{label} does not exist or is not a directory: {resolved}"
        )

    return resolved


def _existing_file(path: Path, label: str) -> Path:
    resolved = Path(path).expanduser().resolve()

    if not resolved.is_file():
        raise CommandError(
            f"{label} does not exist or is not a file: {resolved}"
        )

    return resolved


def _validate_detached_build(
    source_directory: Path,
    build_variant_directory: Path,
) -> None:
    try:
        build_variant_directory.relative_to(source_directory)
    except ValueError:
        return

    raise CommandError(
        "build variant directory must be detached from the source directory: "
        f"{build_variant_directory}"
    )


def build_build_command(
    build_variant_directory: Path,
    *,
    jobs: int,
    build_type: str,
    operating_system: str,
    cmake_executable: str = "cmake",
) -> Tuple[str, ...]:
    """Construct a portable CMake build command without executing it."""

    if (
        not isinstance(cmake_executable, str)
        or not cmake_executable.strip()
    ):
        raise CommandError(
            "cmake executable must be a nonempty string"
        )

    if type(jobs) is not int or jobs <= 0:
        raise CommandError(
            "jobs must be a positive integer"
        )

    if build_type not in _SUPPORTED_BUILD_TYPES:
        raise CommandError(
            "build type must be one of: Debug, Release"
        )

    if operating_system not in {
        "linux",
        "macos",
        "windows",
    }:
        raise CommandError(
            "operating system must be one of: "
            "linux, macos, windows"
        )

    resolved_build_variant_directory = (
        Path(build_variant_directory)
        .expanduser()
        .resolve()
    )

    command = [
        cmake_executable.strip(),
        "--build",
        str(resolved_build_variant_directory),
        "--target",
        "BUILD",
    ]

    if operating_system == "windows":
        command.extend(
            [
                "--config",
                build_type,
            ]
        )

    command.extend(
        [
            "--parallel",
            str(jobs),
        ]
    )

    return tuple(command)


def build_configure_command(
    manifest: BuildVariantManifest,
    options: ConfigureOptions,
) -> Tuple[str, ...]:
    """Construct a CMake configure command without executing it."""

    if (
        not isinstance(options.cmake_executable, str)
        or not options.cmake_executable.strip()
    ):
        raise CommandError("cmake executable must be a nonempty string")

    if options.build_type not in _SUPPORTED_BUILD_TYPES:
        raise CommandError(
            "build type must be one of: Debug, Release"
        )

    if type(options.fixed_library_name) is not bool:
        raise CommandError(
            "fixed_library_name must be a boolean"
        )

    if (
        options.compiler is not None
        and (
            not isinstance(options.compiler, str)
            or not options.compiler.strip()
        )
    ):
        raise CommandError(
            "compiler must be a nonempty string when provided"
        )

    source_directory = _existing_directory(
        options.source_directory,
        "source directory",
    )

    build_variant_directory = (
        Path(options.build_variant_directory)
        .expanduser()
        .resolve()
    )

    _validate_detached_build(
        source_directory,
        build_variant_directory,
    )

    command = [
        options.cmake_executable.strip(),
    ]

    for reference in manifest.cmake_tests:
        reference_path = PurePosixPath(reference)
        preload = (
            source_directory
            / Path(*reference_path.parts)
        ).resolve()

        if not preload.is_file():
            raise CommandError(
                f"CMake preload file does not exist: {preload}"
            )

        command.extend(
            [
                "-C",
                str(preload),
            ]
        )

    command.append(
        "-DCMAKE_BUILD_TYPE:STRING="
        f"{options.build_type}"
    )

    fixed_value = (
        "ON"
        if options.fixed_library_name
        else "OFF"
    )

    command.append(
        "-DCGALPY_FIXED_LIBRARY_NAME:BOOL="
        f"{fixed_value}"
    )

    if options.compiler is not None:
        command.append(
            "-DCMAKE_CXX_COMPILER:FILEPATH="
            f"{options.compiler.strip()}"
        )

    if options.cgal_dir is not None:
        cgal_dir = _existing_directory(
            options.cgal_dir,
            "CGAL directory",
        )
        command.append(
            f"-DCGAL_DIR:PATH={cgal_dir}"
        )

    if options.python_executable is not None:
        python_executable = _existing_file(
            options.python_executable,
            "Python executable",
        )
        command.append(
            "-DPython_EXECUTABLE:FILEPATH="
            f"{python_executable}"
        )

    if options.nanobind_dir is not None:
        nanobind_dir = _existing_directory(
            options.nanobind_dir,
            "nanobind directory",
        )
        command.append(
            f"-Dnanobind_DIR:PATH={nanobind_dir}"
        )

    command.extend(
        [
            "-S",
            str(source_directory),
            "-B",
            str(build_variant_directory),
        ]
    )

    return tuple(command)
