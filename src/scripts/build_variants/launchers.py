#!/usr/bin/env python3
"""Generate local thin launchers for build-variant manifests."""

from __future__ import annotations

import json
import os
import re
import stat
import tempfile
from pathlib import Path
from typing import Iterable, Tuple

from .manifest import BuildVariantManifest


_NAME_PATTERN = re.compile(r"^[a-z0-9][a-z0-9_-]*$")


class LauncherError(ValueError):
    """Raised when a generated launcher cannot be planned or written."""


def _validated_manifest_name(name: str) -> str:
    if (
        not isinstance(name, str)
        or _NAME_PATTERN.fullmatch(name) is None
    ):
        raise LauncherError(
            "manifest name must contain only lowercase letters, "
            "digits, underscores, and hyphens"
        )

    return name


def launcher_path(
    output_directory: Path,
    manifest_name: str,
) -> Path:
    """Return the run_<manifest> path in the requested directory."""

    output_directory = (
        Path(output_directory)
        .expanduser()
        .resolve()
    )

    if not output_directory.is_dir():
        raise LauncherError(
            "launcher output directory does not exist or is not "
            f"a directory: {output_directory}"
        )

    name = _validated_manifest_name(manifest_name)

    return output_directory / f"run_{name}"


def render_launcher(
    runner_path: Path,
    manifest_name: str,
) -> str:
    """Render one cross-platform Python launcher."""

    runner_path = (
        Path(runner_path)
        .expanduser()
        .resolve()
    )

    if not runner_path.is_file():
        raise LauncherError(
            f"runner entry point does not exist: {runner_path}"
        )

    name = _validated_manifest_name(manifest_name)

    runner_literal = json.dumps(str(runner_path))
    manifest_literal = json.dumps(name)

    return (
        "#!/usr/bin/env python3\n"
        '"""Generated build-variant launcher; do not commit."""\n'
        "\n"
        "from __future__ import annotations\n"
        "\n"
        "import subprocess\n"
        "import sys\n"
        "\n"
        f"RUNNER = {runner_literal}\n"
        f"MANIFEST = {manifest_literal}\n"
        "\n"
        "\n"
        'if __name__ == "__main__":\n'
        "    raise SystemExit(\n"
        "        subprocess.call(\n"
        "            [sys.executable, RUNNER, MANIFEST]\n"
        "            + sys.argv[1:]\n"
        "        )\n"
        "    )\n"
    )


def _make_executable(path: Path) -> None:
    current_mode = path.stat().st_mode

    path.chmod(
        current_mode
        | stat.S_IXUSR
        | stat.S_IXGRP
        | stat.S_IXOTH
    )


def write_launcher(
    output_directory: Path,
    runner_path: Path,
    manifest: BuildVariantManifest,
) -> Path:
    """Atomically create one local launcher without unsafe overwrite."""

    if not isinstance(manifest, BuildVariantManifest):
        raise LauncherError(
            "manifest must be a validated BuildVariantManifest"
        )

    destination = launcher_path(
        output_directory,
        manifest.name,
    )
    content = render_launcher(
        runner_path,
        manifest.name,
    )

    if destination.exists():
        if not destination.is_file():
            raise LauncherError(
                f"launcher destination is not a file: {destination}"
            )

        try:
            existing_content = destination.read_text(
                encoding="utf-8"
            )
        except OSError as exc:
            raise LauncherError(
                f"could not read existing launcher "
                f"{destination}: {exc}"
            ) from exc

        if existing_content != content:
            raise LauncherError(
                "refusing to overwrite an existing launcher with "
                f"different content: {destination}"
            )

        _make_executable(destination)
        return destination

    file_descriptor, temporary_name = tempfile.mkstemp(
        prefix=f".{destination.name}.",
        dir=str(destination.parent),
        text=True,
    )
    temporary_path = Path(temporary_name)

    try:
        with os.fdopen(
            file_descriptor,
            "w",
            encoding="utf-8",
            newline="\n",
        ) as stream:
            stream.write(content)

        _make_executable(temporary_path)
        os.replace(temporary_path, destination)
    except Exception:
        try:
            temporary_path.unlink()
        except OSError:
            pass

        raise

    return destination


def write_launchers(
    output_directory: Path,
    runner_path: Path,
    manifests: Iterable[BuildVariantManifest],
) -> Tuple[Path, ...]:
    """Preflight and write launchers in manifest order."""

    manifest_tuple = tuple(manifests)

    for manifest in manifest_tuple:
        if not isinstance(manifest, BuildVariantManifest):
            raise LauncherError(
                "every manifest must be a validated "
                "BuildVariantManifest"
            )

    names = tuple(
        manifest.name
        for manifest in manifest_tuple
    )

    if len(names) != len(set(names)):
        raise LauncherError(
            "manifest names must not be repeated during "
            "launcher generation"
        )

    prepared = tuple(
        (
            manifest,
            launcher_path(
                output_directory,
                manifest.name,
            ),
            render_launcher(
                runner_path,
                manifest.name,
            ),
        )
        for manifest in manifest_tuple
    )

    for _, destination, expected_content in prepared:
        if not destination.exists():
            continue

        if not destination.is_file():
            raise LauncherError(
                f"launcher destination is not a file: {destination}"
            )

        try:
            existing_content = destination.read_text(
                encoding="utf-8"
            )
        except OSError as exc:
            raise LauncherError(
                f"could not read existing launcher "
                f"{destination}: {exc}"
            ) from exc

        if existing_content != expected_content:
            raise LauncherError(
                "refusing to overwrite an existing launcher with "
                f"different content: {destination}"
            )

    return tuple(
        write_launcher(
            output_directory,
            runner_path,
            manifest,
        )
        for manifest, _, _ in prepared
    )
