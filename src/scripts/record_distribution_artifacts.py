#!/usr/bin/env python3

from __future__ import annotations

import argparse
import email
import hashlib
import json
import os
import pathlib
import re
import sys
import tarfile
import tempfile
import zipfile


def canonicalize_distribution_name(value: str) -> str:
    return re.sub(r"[-_.]+", "-", value).lower()


def calculate_sha256(filename: pathlib.Path) -> str:
    digest = hashlib.sha256()

    with filename.open("rb") as input_file:
        for block in iter(lambda: input_file.read(1024 * 1024), b""):
            digest.update(block)

    return digest.hexdigest()


def read_metadata(
    raw_metadata: bytes,
    source_description: str,
) -> email.message.Message:
    metadata = email.message_from_bytes(raw_metadata)

    if not metadata.get("Name"):
        raise ValueError(
            f"{source_description} metadata has no Name field"
        )

    if not metadata.get("Version"):
        raise ValueError(
            f"{source_description} metadata has no Version field"
        )

    return metadata


def validate_identity(
    metadata: email.message.Message,
    expected_name: str,
    expected_version: str,
    source_description: str,
) -> None:
    actual_name = metadata.get("Name", "")
    actual_version = metadata.get("Version", "")

    if (
        canonicalize_distribution_name(actual_name)
        != canonicalize_distribution_name(expected_name)
    ):
        raise ValueError(
            f"{source_description} name is {actual_name!r}; "
            f"expected {expected_name!r}"
        )

    if actual_version != expected_version:
        raise ValueError(
            f"{source_description} version is {actual_version!r}; "
            f"expected {expected_version!r}"
        )


def inspect_wheel(
    wheel_file: pathlib.Path,
    expected_name: str,
    expected_version: str,
) -> dict[str, object]:
    with zipfile.ZipFile(wheel_file) as archive:
        corrupt_entry = archive.testzip()

        if corrupt_entry is not None:
            raise ValueError(
                f"wheel contains a corrupt entry: {corrupt_entry}"
            )

        names = archive.namelist()

        metadata_names = [
            name
            for name in names
            if name.endswith(".dist-info/METADATA")
        ]
        wheel_metadata_names = [
            name
            for name in names
            if name.endswith(".dist-info/WHEEL")
        ]

        if len(metadata_names) != 1:
            raise ValueError(
                "wheel must contain exactly one "
                f".dist-info/METADATA file; found {len(metadata_names)}"
            )

        if len(wheel_metadata_names) != 1:
            raise ValueError(
                "wheel must contain exactly one "
                f".dist-info/WHEEL file; found {len(wheel_metadata_names)}"
            )

        package_metadata = read_metadata(
            archive.read(metadata_names[0]),
            "wheel",
        )
        validate_identity(
            package_metadata,
            expected_name,
            expected_version,
            "wheel",
        )

        wheel_metadata = email.message_from_bytes(
            archive.read(wheel_metadata_names[0])
        )
        root_is_purelib = wheel_metadata.get("Root-Is-Purelib", "")
        tags = wheel_metadata.get_all("Tag") or []

        if root_is_purelib.lower() != "false":
            raise ValueError(
                "wheel Root-Is-Purelib must be false; "
                f"found {root_is_purelib!r}"
            )

        if not tags:
            raise ValueError("wheel metadata contains no Tag fields")

    return {
        "filename": wheel_file.name,
        "sha256": calculate_sha256(wheel_file),
        "size_bytes": wheel_file.stat().st_size,
        "tags": tags,
    }


def inspect_sdist(
    sdist_file: pathlib.Path,
    expected_name: str,
    expected_version: str,
) -> dict[str, object]:
    with tarfile.open(sdist_file, mode="r:gz") as archive:
        member_names = archive.getnames()
        top_level_names = {
            pathlib.PurePosixPath(name).parts[0]
            for name in member_names
            if pathlib.PurePosixPath(name).parts
        }

        if len(top_level_names) != 1:
            raise ValueError(
                "source distribution must have exactly one "
                f"top-level directory; found {len(top_level_names)}"
            )

        top_level_name = next(iter(top_level_names))
        root_pkg_info = f"{top_level_name}/PKG-INFO"

        if root_pkg_info not in member_names:
            raise ValueError(
                f"source distribution is missing {root_pkg_info}"
            )

        extracted_file = archive.extractfile(root_pkg_info)

        if extracted_file is None:
            raise ValueError(
                f"unable to read {root_pkg_info}"
            )

        package_metadata = read_metadata(
            extracted_file.read(),
            "source distribution",
        )
        validate_identity(
            package_metadata,
            expected_name,
            expected_version,
            "source distribution",
        )

    return {
        "filename": sdist_file.name,
        "sha256": calculate_sha256(sdist_file),
        "size_bytes": sdist_file.stat().st_size,
    }


def write_manifest(
    manifest_file: pathlib.Path,
    payload: dict[str, object],
) -> None:
    manifest_file.parent.mkdir(parents=True, exist_ok=True)

    with tempfile.NamedTemporaryFile(
        mode="w",
        encoding="utf-8",
        dir=manifest_file.parent,
        prefix=f".{manifest_file.name}.",
        suffix=".tmp",
        delete=False,
    ) as temporary_file:
        json.dump(
            payload,
            temporary_file,
            indent=2,
            sort_keys=True,
        )
        temporary_file.write("\n")
        temporary_name = pathlib.Path(temporary_file.name)

    os.replace(temporary_name, manifest_file)


def parse_arguments() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Validate the generated wheel and source distribution, "
            "then record their actual backend-selected filenames."
        )
    )
    parser.add_argument(
        "--dist-dir",
        required=True,
        type=pathlib.Path,
    )
    parser.add_argument(
        "--distribution-name",
        required=True,
    )
    parser.add_argument(
        "--version",
        required=True,
    )
    parser.add_argument(
        "--manifest",
        required=True,
        type=pathlib.Path,
    )
    return parser.parse_args()


def main() -> int:
    arguments = parse_arguments()
    dist_dir = arguments.dist_dir.resolve()
    manifest_file = arguments.manifest.resolve()

    if not dist_dir.is_dir():
        print(
            f"error: distribution directory does not exist: {dist_dir}",
            file=sys.stderr,
        )
        return 1

    wheel_files = sorted(dist_dir.glob("*.whl"))
    sdist_files = sorted(dist_dir.glob("*.tar.gz"))

    if len(wheel_files) != 1:
        print(
            "error: expected exactly one wheel in "
            f"{dist_dir}; found {len(wheel_files)}",
            file=sys.stderr,
        )
        return 1

    if len(sdist_files) != 1:
        print(
            "error: expected exactly one source distribution in "
            f"{dist_dir}; found {len(sdist_files)}",
            file=sys.stderr,
        )
        return 1

    try:
        wheel_data = inspect_wheel(
            wheel_files[0],
            arguments.distribution_name,
            arguments.version,
        )
        sdist_data = inspect_sdist(
            sdist_files[0],
            arguments.distribution_name,
            arguments.version,
        )

        payload = {
            "distribution_name": arguments.distribution_name,
            "version": arguments.version,
            "wheel": wheel_data,
            "sdist": sdist_data,
        }

        write_manifest(manifest_file, payload)
    except (
        OSError,
        tarfile.TarError,
        ValueError,
        zipfile.BadZipFile,
    ) as error:
        print(f"error: {error}", file=sys.stderr)
        return 1

    print(f"WHEEL {wheel_files[0]}")
    print(f"SDIST {sdist_files[0]}")
    print(f"MANIFEST {manifest_file}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
