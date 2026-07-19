#!/usr/bin/env python3
"""Command-line parsing for the cross-platform build-variant runner."""

from __future__ import annotations

import argparse
import platform
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Optional, Sequence, Tuple


_OPERATING_SYSTEMS = (
    "linux",
    "macos",
    "windows",
)

_BUILD_TYPES = (
    "Release",
    "Debug",
)


class CliError(ValueError):
    """Raised when parsed runner arguments are inconsistent."""


@dataclass(frozen=True)
class RunnerArguments:
    """Validated command-line arguments for the build runner."""

    manifests: Tuple[str, ...]
    build_type: str
    fixed_library_name: bool
    operating_system: str
    compiler: Optional[str]
    generate_run: bool
    abort_after_run_generation: bool
    manifest_directory: Path
    source_directory: Path
    build_root: Path
    cgal_dir: Optional[Path]
    python_executable: Path
    nanobind_dir: Optional[Path]
    jobs: int
    list_manifests: bool
    dry_run: bool
    continue_on_error: bool
    log_directory: Optional[Path]


def detect_operating_system() -> str:
    """Return the runner operating-system identifier."""

    system = platform.system().lower()

    mapping = {
        "darwin": "macos",
        "linux": "linux",
        "windows": "windows",
    }

    try:
        return mapping[system]
    except KeyError as exc:
        raise CliError(
            f"unsupported operating system: {platform.system()}"
        ) from exc


def positive_integer(value: str) -> int:
    """Parse a strictly positive integer for argparse."""

    try:
        parsed = int(value)
    except ValueError as exc:
        raise argparse.ArgumentTypeError(
            f"expected a positive integer, got {value!r}"
        ) from exc

    if parsed <= 0:
        raise argparse.ArgumentTypeError(
            f"expected a positive integer, got {value!r}"
        )

    return parsed


def create_parser(
    default_source_directory: Path,
    default_manifest_directory: Path,
    default_build_root: Path,
) -> argparse.ArgumentParser:
    """Create the runner argument parser."""

    parser = argparse.ArgumentParser(
        description=(
            "Configure and build CGAL Python binding variants "
            "from JSON manifests."
        ),
    )

    parser.add_argument(
        "manifests",
        nargs="*",
        metavar="MANIFEST",
        help="manifest name to process; may be specified more than once",
    )

    parser.add_argument(
        "--build-type",
        choices=_BUILD_TYPES,
        default="Release",
        help="CMake build type; default: Release",
    )

    parser.add_argument(
        "--fixed-library-name",
        action="store_true",
        help=(
            "use the fixed CGALPY library name; "
            "computed naming is the default"
        ),
    )

    parser.add_argument(
        "--operating-system",
        choices=_OPERATING_SYSTEMS,
        default=detect_operating_system(),
        help="target operating system; default: current platform",
    )

    parser.add_argument(
        "--compiler",
        help="C++ compiler or compiler identifier",
    )

    parser.add_argument(
        "--generate-run",
        action="store_true",
        help="generate thin run_* launchers",
    )

    parser.add_argument(
        "--abort-after-run-generation",
        action="store_true",
        help="stop after generating thin run_* launchers",
    )

    parser.add_argument(
        "--manifest-directory",
        type=Path,
        default=default_manifest_directory,
        help="directory containing JSON manifests",
    )

    parser.add_argument(
        "--source-directory",
        type=Path,
        default=default_source_directory,
        help="cgal-python-bindings source directory",
    )

    parser.add_argument(
        "--build-root",
        type=Path,
        default=default_build_root,
        help="detached root directory for variant builds",
    )

    parser.add_argument(
        "--cgal-dir",
        type=Path,
        help="configured CGAL build directory",
    )

    parser.add_argument(
        "--python",
        dest="python_executable",
        type=Path,
        default=Path(sys.executable),
        help="Python interpreter used by CMake and wheel generation",
    )

    parser.add_argument(
        "--nanobind-dir",
        type=Path,
        help="nanobind CMake package directory",
    )

    parser.add_argument(
        "--jobs",
        type=positive_integer,
        default=4,
        help="parallel build job count; default: 4",
    )

    parser.add_argument(
        "--list",
        dest="list_manifests",
        action="store_true",
        help="list validated manifests and exit",
    )

    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="print resolved commands without executing them",
    )

    parser.add_argument(
        "--continue-on-error",
        action="store_true",
        help="continue processing remaining manifests after a failure",
    )

    parser.add_argument(
        "--log-directory",
        type=Path,
        help="directory for configure and build logs",
    )

    return parser


def validate_namespace(
    parser: argparse.ArgumentParser,
    namespace: argparse.Namespace,
) -> RunnerArguments:
    """Validate parsed arguments and return an immutable value."""

    manifests = tuple(namespace.manifests)

    if len(manifests) != len(set(manifests)):
        parser.error("manifest names must not be repeated")

    if not namespace.list_manifests and not manifests:
        parser.error(
            "at least one MANIFEST is required unless --list is used"
        )

    if (
        namespace.abort_after_run_generation
        and not namespace.generate_run
    ):
        parser.error(
            "--abort-after-run-generation requires --generate-run"
        )

    return RunnerArguments(
        manifests=manifests,
        build_type=namespace.build_type,
        fixed_library_name=namespace.fixed_library_name,
        operating_system=namespace.operating_system,
        compiler=namespace.compiler,
        generate_run=namespace.generate_run,
        abort_after_run_generation=(
            namespace.abort_after_run_generation
        ),
        manifest_directory=(
            namespace.manifest_directory
            .expanduser()
            .resolve()
        ),
        source_directory=(
            namespace.source_directory
            .expanduser()
            .resolve()
        ),
        build_root=(
            namespace.build_root
            .expanduser()
            .resolve()
        ),
        cgal_dir=(
            namespace.cgal_dir.expanduser().resolve()
            if namespace.cgal_dir is not None
            else None
        ),
        python_executable=(
            namespace.python_executable
            .expanduser()
            .resolve()
        ),
        nanobind_dir=(
            namespace.nanobind_dir.expanduser().resolve()
            if namespace.nanobind_dir is not None
            else None
        ),
        jobs=namespace.jobs,
        list_manifests=namespace.list_manifests,
        dry_run=namespace.dry_run,
        continue_on_error=namespace.continue_on_error,
        log_directory=(
            namespace.log_directory.expanduser().resolve()
            if namespace.log_directory is not None
            else None
        ),
    )


def parse_arguments(
    arguments: Optional[Sequence[str]] = None,
    *,
    default_source_directory: Path,
    default_manifest_directory: Path,
    default_build_root: Path,
) -> RunnerArguments:
    """Parse and validate runner command-line arguments."""

    parser = create_parser(
        default_source_directory=default_source_directory,
        default_manifest_directory=default_manifest_directory,
        default_build_root=default_build_root,
    )

    namespace = parser.parse_args(arguments)

    return validate_namespace(parser, namespace)
