#!/usr/bin/env python3
"""Discover and select validated build-variant manifests."""

from __future__ import annotations

from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Tuple

from .manifest import BuildVariantManifest, load_manifest


class CatalogError(ValueError):
    """Raised when a manifest catalog cannot be loaded or queried."""


@dataclass(frozen=True)
class ManifestCatalog:
    """An ordered collection of validated build manifests."""

    manifests: Tuple[BuildVariantManifest, ...]

    @property
    def names(self) -> Tuple[str, ...]:
        """Return manifest names in deterministic order."""

        return tuple(
            manifest.name
            for manifest in self.manifests
        )

    def get(self, name: str) -> BuildVariantManifest:
        """Return one manifest by exact name."""

        if not isinstance(name, str) or not name:
            raise CatalogError(
                "manifest name must be a nonempty string"
            )

        for manifest in self.manifests:
            if manifest.name == name:
                return manifest

        available = ", ".join(self.names)

        if not available:
            available = "(none)"

        raise CatalogError(
            f"unknown manifest {name!r}; "
            f"available manifests: {available}"
        )


def load_catalog(
    manifest_directory: Path,
    source_directory: Path,
) -> ManifestCatalog:
    """Discover and validate top-level JSON manifests."""

    manifest_directory = (
        Path(manifest_directory)
        .expanduser()
        .resolve()
    )
    source_directory = (
        Path(source_directory)
        .expanduser()
        .resolve()
    )

    if not manifest_directory.is_dir():
        raise CatalogError(
            "manifest directory does not exist or is not a directory: "
            f"{manifest_directory}"
        )

    if not source_directory.is_dir():
        raise CatalogError(
            "source directory does not exist or is not a directory: "
            f"{source_directory}"
        )

    manifest_paths = sorted(
        path
        for path in manifest_directory.iterdir()
        if path.is_file()
        and path.suffix == ".json"
    )

    manifests = tuple(
        load_manifest(
            manifest_path,
            source_directory,
        )
        for manifest_path in manifest_paths
    )

    names: Dict[str, Path] = {}

    for manifest in manifests:
        previous_path = names.get(manifest.name)

        if previous_path is not None:
            raise CatalogError(
                f"duplicate manifest name {manifest.name!r}: "
                f"{previous_path} and {manifest.path}"
            )

        names[manifest.name] = manifest.path

    return ManifestCatalog(manifests=manifests)
