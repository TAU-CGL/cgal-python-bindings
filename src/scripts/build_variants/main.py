#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Command-line entry point for the build-variant runner."""

from __future__ import annotations

import sys
from pathlib import Path
from typing import Optional, Sequence, TextIO

from .application import run_application
from .cli import parse_arguments


def default_source_directory() -> Path:
    """Return the repository root containing this runner package."""

    return Path(__file__).resolve().parents[3]


def main(
    argv: Optional[Sequence[str]] = None,
    *,
    stdout: Optional[TextIO] = None,
    stderr: Optional[TextIO] = None,
    source_directory: Optional[Path] = None,
    manifest_directory: Optional[Path] = None,
    build_directory: Optional[Path] = None,
) -> int:
    """Parse arguments and invoke the tested runner application."""

    resolved_source_directory = (
        Path(source_directory).expanduser().resolve()
        if source_directory is not None
        else default_source_directory()
    )

    resolved_manifest_directory = (
        Path(manifest_directory).expanduser().resolve()
        if manifest_directory is not None
        else (
            resolved_source_directory
            / "src/scripts/build_variants/manifests"
        )
    )

    resolved_build_directory = (
        Path(build_directory).expanduser().resolve()
        if build_directory is not None
        else (
            Path.home()
            / "build/cgalpy"
        ).resolve()
    )

    arguments = parse_arguments(
        (
            tuple(argv)
            if argv is not None
            else tuple(sys.argv[1:])
        ),
        default_source_directory=resolved_source_directory,
        default_manifest_directory=resolved_manifest_directory,
        default_build_directory=resolved_build_directory,
    )

    return run_application(
        arguments,
        stdout=(
            stdout
            if stdout is not None
            else sys.stdout
        ),
        stderr=(
            stderr
            if stderr is not None
            else sys.stderr
        ),
    )


if __name__ == "__main__":
    raise SystemExit(main())
