#!/usr/bin/env python3
"""Load and validate CGAL Python build-variant manifests."""

from __future__ import annotations

import json
import os
import re
from dataclasses import dataclass
from pathlib import Path, PurePosixPath
from typing import Any, Dict, Tuple


SCHEMA_VERSION = 1

_REQUIRED_KEYS = {
    "schema_version",
    "name",
    "description",
    "cmake_tests",
}

_NAME_PATTERN = re.compile(r"^[a-z0-9][a-z0-9_-]*$")


class ManifestError(ValueError):
    """Raised when a build-variant manifest is invalid."""


@dataclass(frozen=True)
class BuildVariantManifest:
    """Validated build-variant manifest data."""

    schema_version: int
    name: str
    description: str
    cmake_tests: Tuple[str, ...]
    path: Path


def _error(path: Path, message: str) -> ManifestError:
    return ManifestError(f"{path}: {message}")


def _validate_reference(
    manifest_path: Path,
    source_directory: Path,
    reference: Any,
) -> str:
    if not isinstance(reference, str) or not reference:
        raise _error(
            manifest_path,
            "every cmake_tests entry must be a nonempty string",
        )

    posix_path = PurePosixPath(reference)

    if posix_path.is_absolute():
        raise _error(
            manifest_path,
            f"CMake test reference must be relative: {reference}",
        )

    if any(part in {"", ".", ".."} for part in posix_path.parts):
        raise _error(
            manifest_path,
            f"invalid CMake test reference: {reference}",
        )

    if (
        len(posix_path.parts) < 3
        or posix_path.parts[0:2] != ("cmake", "tests")
        or posix_path.suffix != ".cmake"
    ):
        raise _error(
            manifest_path,
            "CMake test references must match "
            f"cmake/tests/*.cmake: {reference}",
        )

    resolved_reference = (
        source_directory / Path(*posix_path.parts)
    ).resolve()

    common_path = Path(
        os.path.commonpath(
            [
                str(source_directory),
                str(resolved_reference),
            ]
        )
    )

    if common_path != source_directory:
        raise _error(
            manifest_path,
            f"CMake test reference escapes source directory: {reference}",
        )

    if not resolved_reference.is_file():
        raise _error(
            manifest_path,
            f"CMake test file does not exist: {reference}",
        )

    return posix_path.as_posix()


def load_manifest(
    manifest_path: Path,
    source_directory: Path,
) -> BuildVariantManifest:
    """Load and validate one JSON build-variant manifest."""

    manifest_path = Path(manifest_path).resolve()
    source_directory = Path(source_directory).resolve()

    if not source_directory.is_dir():
        raise _error(
            manifest_path,
            f"source directory does not exist: {source_directory}",
        )

    try:
        raw_text = manifest_path.read_text(encoding="utf-8")
    except OSError as exc:
        raise _error(
            manifest_path,
            f"could not read manifest: {exc}",
        ) from exc

    try:
        data = json.loads(raw_text)
    except json.JSONDecodeError as exc:
        raise _error(
            manifest_path,
            f"invalid JSON at line {exc.lineno}, column {exc.colno}: "
            f"{exc.msg}",
        ) from exc

    if not isinstance(data, dict):
        raise _error(
            manifest_path,
            "manifest root must be a JSON object",
        )

    typed_data: Dict[str, Any] = data
    actual_keys = set(typed_data)

    missing_keys = sorted(_REQUIRED_KEYS - actual_keys)
    unknown_keys = sorted(actual_keys - _REQUIRED_KEYS)

    if missing_keys:
        raise _error(
            manifest_path,
            f"missing required fields: {', '.join(missing_keys)}",
        )

    if unknown_keys:
        raise _error(
            manifest_path,
            f"unknown fields: {', '.join(unknown_keys)}",
        )

    schema_version = typed_data["schema_version"]

    if type(schema_version) is not int or schema_version != SCHEMA_VERSION:
        raise _error(
            manifest_path,
            f"schema_version must be {SCHEMA_VERSION}",
        )

    name = typed_data["name"]

    if (
        not isinstance(name, str)
        or not _NAME_PATTERN.fullmatch(name)
    ):
        raise _error(
            manifest_path,
            "name must contain only lowercase letters, digits, "
            "underscores, and hyphens",
        )

    if manifest_path.stem != name:
        raise _error(
            manifest_path,
            "manifest filename must match its name field",
        )

    description = typed_data["description"]

    if not isinstance(description, str) or not description.strip():
        raise _error(
            manifest_path,
            "description must be a nonempty string",
        )

    raw_cmake_tests = typed_data["cmake_tests"]

    if (
        not isinstance(raw_cmake_tests, list)
        or not raw_cmake_tests
    ):
        raise _error(
            manifest_path,
            "cmake_tests must be a nonempty array",
        )

    cmake_tests = tuple(
        _validate_reference(
            manifest_path,
            source_directory,
            reference,
        )
        for reference in raw_cmake_tests
    )

    if len(cmake_tests) != len(set(cmake_tests)):
        raise _error(
            manifest_path,
            "cmake_tests must not contain duplicate references",
        )

    return BuildVariantManifest(
        schema_version=schema_version,
        name=name,
        description=description.strip(),
        cmake_tests=cmake_tests,
        path=manifest_path,
    )
