#!/usr/bin/env python3
"""Render deterministic output for runner list and dry-run modes."""

from __future__ import annotations

import shlex
import subprocess
from typing import Sequence, Tuple

from .catalog import ManifestCatalog
from .planning import VariantPlan


_SUPPORTED_OPERATING_SYSTEMS = {
    "linux",
    "macos",
    "windows",
}


class OutputError(ValueError):
    """Raised when runner output cannot be rendered safely."""


def format_command(
    command: Sequence[str],
    operating_system: str,
) -> str:
    """Render an argument sequence without executing it."""

    if operating_system not in _SUPPORTED_OPERATING_SYSTEMS:
        raise OutputError(
            "operating system must be one of: "
            "linux, macos, windows"
        )

    arguments = tuple(command)

    if not arguments:
        raise OutputError(
            "command must contain at least one argument"
        )

    for argument in arguments:
        if not isinstance(argument, str):
            raise OutputError(
                "every command argument must be a string"
            )

        if "\x00" in argument:
            raise OutputError(
                "command arguments must not contain null bytes"
            )

    if operating_system == "windows":
        return subprocess.list2cmdline(list(arguments))

    return " ".join(
        shlex.quote(argument)
        for argument in arguments
    )


def render_catalog(
    catalog: ManifestCatalog,
) -> Tuple[str, ...]:
    """Render validated manifests in deterministic catalog order."""

    lines = []

    for manifest in catalog.manifests:
        references = ", ".join(manifest.cmake_tests)

        lines.append(
            f"{manifest.name}: "
            f"{manifest.description} "
            f"[{references}]"
        )

    return tuple(lines)


def render_variant_plan(
    plan: VariantPlan,
) -> Tuple[str, ...]:
    """Render one complete variant plan for dry-run output."""

    return (
        f"manifest: {plan.manifest.name}",
        f"build-variant-directory: {plan.build_variant_directory}",
        (
            "configure-command: "
            f"{format_command(plan.configure_command, plan.operating_system)}"
        ),
        (
            "build-command: "
            f"{format_command(plan.build_command, plan.operating_system)}"
        ),
        (
            "install-wheel: "
            + ("enabled" if plan.install_wheel else "disabled")
        ),
    )


def render_variant_plans(
    plans: Sequence[VariantPlan],
) -> Tuple[str, ...]:
    """Render several plans while preserving their supplied order."""

    lines = []

    for index, plan in enumerate(plans):
        if index:
            lines.append("")

        lines.extend(render_variant_plan(plan))

    return tuple(lines)
