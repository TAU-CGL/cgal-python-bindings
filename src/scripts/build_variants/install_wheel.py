#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Install the exact wheel recorded by a distribution-artifact manifest."""

from __future__ import annotations

import argparse
import hashlib
import json
import pathlib
import subprocess
import sys
from typing import Optional, Sequence


def calculate_sha256(filename: pathlib.Path) -> str:
    """Return the SHA-256 digest of one file."""

    digest = hashlib.sha256()

    with filename.open("rb") as input_file:
        for block in iter(
            lambda: input_file.read(1024 * 1024),
            b"",
        ):
            digest.update(block)

    return digest.hexdigest()


def resolve_wheel_path(manifest_file: pathlib.Path) -> pathlib.Path:
    """Validate the artifact manifest and return its exact wheel path."""

    resolved_manifest = (
        pathlib.Path(manifest_file)
        .expanduser()
        .resolve()
    )

    if not resolved_manifest.is_file():
        raise ValueError(
            "distribution artifact manifest does not exist or is not "
            f"a file: {resolved_manifest}"
        )

    try:
        with resolved_manifest.open(
            "r",
            encoding="utf-8",
        ) as input_file:
            payload = json.load(input_file)
    except json.JSONDecodeError as exc:
        raise ValueError(
            "distribution artifact manifest is not valid JSON: "
            f"{resolved_manifest}: {exc}"
        ) from exc
    except OSError as exc:
        raise ValueError(
            "could not read distribution artifact manifest "
            f"{resolved_manifest}: {exc}"
        ) from exc

    if not isinstance(payload, dict):
        raise ValueError(
            "distribution artifact manifest root must be an object"
        )

    wheel_data = payload.get("wheel")

    if not isinstance(wheel_data, dict):
        raise ValueError(
            "distribution artifact manifest has no wheel object"
        )

    filename = wheel_data.get("filename")

    if not isinstance(filename, str) or not filename:
        raise ValueError(
            "distribution artifact manifest wheel filename must be "
            "a non-empty string"
        )

    if (
        "\0" in filename
        or "/" in filename
        or "\\" in filename
        or pathlib.Path(filename).is_absolute()
        or pathlib.Path(filename).name != filename
    ):
        raise ValueError(
            "distribution artifact manifest wheel filename must be "
            f"a plain filename: {filename!r}"
        )

    if not filename.endswith(".whl"):
        raise ValueError(
            "distribution artifact manifest wheel filename must end "
            f"with .whl: {filename!r}"
        )

    dist_directory = resolved_manifest.parent.resolve()
    wheel_file = (dist_directory / filename).resolve()

    if wheel_file.parent != dist_directory:
        raise ValueError(
            "recorded wheel path escapes the distribution directory"
        )

    if not wheel_file.is_file():
        raise ValueError(
            "recorded wheel does not exist or is not a file: "
            f"{wheel_file}"
        )

    expected_size = wheel_data.get("size_bytes")

    if (
        not isinstance(expected_size, int)
        or isinstance(expected_size, bool)
        or expected_size < 0
    ):
        raise ValueError(
            "distribution artifact manifest wheel size_bytes must be "
            "a non-negative integer"
        )

    actual_size = wheel_file.stat().st_size

    if actual_size != expected_size:
        raise ValueError(
            f"recorded wheel size is {actual_size} bytes; "
            f"expected {expected_size}"
        )

    expected_sha256 = wheel_data.get("sha256")

    if (
        not isinstance(expected_sha256, str)
        or len(expected_sha256) != 64
        or any(
            character not in "0123456789abcdef"
            for character in expected_sha256.lower()
        )
    ):
        raise ValueError(
            "distribution artifact manifest wheel sha256 must be "
            "a 64-character hexadecimal string"
        )

    actual_sha256 = calculate_sha256(wheel_file)

    if actual_sha256 != expected_sha256.lower():
        raise ValueError(
            "recorded wheel SHA-256 does not match the wheel file"
        )

    return wheel_file


def build_pip_command(
    wheel_file: pathlib.Path,
    pip_install_options: Sequence[str] = (),
) -> tuple[str, ...]:
    """Return the pip command for the currently running interpreter."""

    if isinstance(pip_install_options, str):
        raise ValueError(
            "pip install options must be a sequence of arguments"
        )

    normalized_options = tuple(pip_install_options)

    for option in normalized_options:
        if not isinstance(option, str) or not option:
            raise ValueError(
                "pip install options must be non-empty strings"
            )

        if "\0" in option:
            raise ValueError(
                "pip install options must not contain null bytes"
            )

    return (
        sys.executable,
        "-m",
        "pip",
        "install",
        "--force-reinstall",
        *normalized_options,
        str(wheel_file),
    )


def parse_arguments(
    arguments: Optional[Sequence[str]] = None,
) -> argparse.Namespace:
    """Parse helper arguments."""

    parser = argparse.ArgumentParser(
        description=(
            "Install the exact wheel recorded by a validated "
            "distribution-artifact manifest."
        )
    )
    parser.add_argument(
        "--manifest",
        required=True,
        type=pathlib.Path,
    )
    parser.add_argument(
        "--pip-install-option",
        action="append",
        default=[],
        help=(
            "additional argument forwarded to pip install; repeat this "
            "option for multiple arguments"
        ),
    )
    return parser.parse_args(arguments)


def main(
    arguments: Optional[Sequence[str]] = None,
) -> int:
    """Validate the artifact manifest and install its wheel."""

    namespace = parse_arguments(arguments)

    try:
        wheel_file = resolve_wheel_path(
            namespace.manifest
        )
    except (OSError, ValueError) as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    try:
        command = build_pip_command(
            wheel_file,
            namespace.pip_install_option,
        )
    except ValueError as exc:
        print(f"error: {exc}", file=sys.stderr)
        return 1

    try:
        result = subprocess.run(
            command,
            check=False,
        )
    except OSError as exc:
        print(
            f"error: could not start pip installation: {exc}",
            file=sys.stderr,
        )
        return 1

    return result.returncode


if __name__ == "__main__":
    raise SystemExit(main())
