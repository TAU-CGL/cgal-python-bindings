#!/usr/bin/env python3
"""Orchestrate runner listing, planning, launchers, and execution."""

from __future__ import annotations

from pathlib import Path
from typing import Optional, TextIO, Tuple

from .catalog import (
    CatalogError,
    ManifestCatalog,
    load_catalog,
)
from .cli import RunnerArguments
from .commands import CommandError
from .execution import (
    ExecutionError,
    VariantExecutionResult,
    execute_variants,
)
from .launchers import LauncherError, write_launchers
from .manifest import ManifestError
from .output import (
    OutputError,
    render_catalog,
    render_variant_plans,
)
from .planning import (
    PlanningError,
    VariantPlan,
    create_variant_plans,
)


class ApplicationError(ValueError):
    """Raised when a runner operation cannot be completed."""


_EXPECTED_ERRORS = (
    ApplicationError,
    CatalogError,
    CommandError,
    ExecutionError,
    LauncherError,
    ManifestError,
    OutputError,
    PlanningError,
)


def _prepare_catalog_output(
    catalog: ManifestCatalog,
    arguments: RunnerArguments,
) -> Tuple[str, ...]:
    """Prepare read-only list or dry-run output."""

    if arguments.list_manifests:
        return render_catalog(catalog)

    if arguments.dry_run:
        plans = create_variant_plans(
            catalog,
            arguments,
        )

        return render_variant_plans(plans)

    raise ApplicationError(
        "prepare_application_output supports only "
        "--list or --dry-run"
    )


def prepare_application_output(
    arguments: RunnerArguments,
) -> Tuple[str, ...]:
    """Prepare list or dry-run output without side effects."""

    catalog = load_catalog(
        arguments.manifest_directory,
        arguments.source_directory,
    )

    return _prepare_catalog_output(
        catalog,
        arguments,
    )


def _selected_manifests(
    catalog: ManifestCatalog,
    arguments: RunnerArguments,
):
    """Return requested manifests in command-line order."""

    return tuple(
        catalog.get(name)
        for name in arguments.manifests
    )


def _launcher_output_lines(paths) -> Tuple[str, ...]:
    """Render generated-launcher paths in generation order."""

    return tuple(
        f"generated-launcher: {path}"
        for path in paths
    )


def _write_lines(
    lines: Tuple[str, ...],
    stream: TextIO,
) -> None:
    for line in lines:
        print(line, file=stream)


def _variant_result_line(
    result: VariantExecutionResult,
) -> str:
    name = result.plan.manifest.name

    if result.succeeded:
        return f"variant-result: {name}: success"

    if not result.configure_result.succeeded:
        return (
            f"variant-result: {name}: configure-failed "
            f"(exit {result.configure_result.return_code})"
        )

    if result.build_result is None:
        raise ApplicationError(
            "successful configure result is missing its "
            f"build result for manifest {name!r}"
        )

    if not result.build_result.succeeded:
        return (
            f"variant-result: {name}: build-failed "
            f"(exit {result.build_result.return_code})"
        )

    if not result.plan.install_wheel:
        raise ApplicationError(
            "wheel installation result is present for a plan "
            f"that did not request installation: {name!r}"
        )

    if result.install_result is None:
        raise ApplicationError(
            "successful build result is missing its wheel "
            f"installation result for manifest {name!r}"
        )

    return (
        f"variant-result: {name}: install-failed "
        f"(exit {result.install_result.return_code})"
    )


def _execution_result_lines(
    plans: Tuple[VariantPlan, ...],
    results: Tuple[VariantExecutionResult, ...],
) -> Tuple[str, ...]:
    if len(results) > len(plans):
        raise ApplicationError(
            "execution returned more results than plans"
        )

    lines = []

    for index, result in enumerate(results):
        if result.plan != plans[index]:
            raise ApplicationError(
                "execution results do not match requested "
                "plan order"
            )

        lines.append(
            _variant_result_line(result)
        )

    for plan in plans[len(results):]:
        lines.append(
            "variant-result: "
            f"{plan.manifest.name}: skipped"
        )

    return tuple(lines)


def _execution_succeeded(
    plans: Tuple[VariantPlan, ...],
    results: Tuple[VariantExecutionResult, ...],
) -> bool:
    return (
        len(results) == len(plans)
        and all(
            result.succeeded
            for result in results
        )
    )


def run_application(
    arguments: RunnerArguments,
    *,
    stdout: TextIO,
    stderr: TextIO,
    current_directory: Optional[Path] = None,
) -> int:
    """Execute the selected runner operations."""

    try:
        if (
            arguments.abort_after_run_generation
            and not arguments.generate_run
        ):
            raise ApplicationError(
                "--abort-after-run-generation requires "
                "--generate-run"
            )

        catalog = load_catalog(
            arguments.manifest_directory,
            arguments.source_directory,
        )

        if arguments.list_manifests:
            _write_lines(
                render_catalog(catalog),
                stdout,
            )
            return 0

        plans: Tuple[VariantPlan, ...] = ()

        if not arguments.abort_after_run_generation:
            plans = create_variant_plans(
                catalog,
                arguments,
            )

        launcher_lines: Tuple[str, ...] = ()

        if arguments.generate_run:
            selected_manifests = _selected_manifests(
                catalog,
                arguments,
            )

            output_directory = (
                Path(current_directory)
                .expanduser()
                .resolve()
                if current_directory is not None
                else Path.cwd().resolve()
            )

            launcher_paths = write_launchers(
                output_directory,
                (
                    arguments.source_directory
                    / "src/scripts/run"
                ),
                selected_manifests,
            )

            launcher_lines = _launcher_output_lines(
                launcher_paths
            )

        if arguments.abort_after_run_generation:
            _write_lines(launcher_lines, stdout)
            return 0

        if arguments.dry_run:
            _write_lines(
                launcher_lines
                + render_variant_plans(plans),
                stdout,
            )
            return 0

        if not plans:
            raise ApplicationError(
                "at least one manifest is required "
                "for live execution"
            )

        _write_lines(launcher_lines, stdout)

        results = execute_variants(
            plans,
            continue_on_error=(
                arguments.continue_on_error
            ),
            log_directory=arguments.log_directory,
            output_stream=stdout,
        )

        _write_lines(
            _execution_result_lines(
                plans,
                results,
            ),
            stdout,
        )

        return (
            0
            if _execution_succeeded(
                plans,
                results,
            )
            else 1
        )
    except _EXPECTED_ERRORS as exc:
        print(
            f"error: {exc}",
            file=stderr,
        )
        return 2
