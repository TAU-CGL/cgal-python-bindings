#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Execute resolved build-variant plans sequentially and safely."""

from __future__ import annotations

import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Iterable, Optional, Sequence, TextIO, Tuple

from .planning import VariantPlan


_INSTALL_WHEEL_SCRIPT = (
    Path(__file__).resolve().with_name("install_wheel.py")
)


class ExecutionError(ValueError):
    """Raised when a resolved plan cannot be executed safely."""


@dataclass(frozen=True)
class CommandExecutionResult:
    """Result of one external command."""

    command: Tuple[str, ...]
    return_code: int
    log_path: Optional[Path]

    @property
    def succeeded(self) -> bool:
        """Return whether the command exited successfully."""

        return self.return_code == 0


@dataclass(frozen=True)
class VariantExecutionResult:
    """Configure and optional build results for one variant."""

    plan: VariantPlan
    configure_result: CommandExecutionResult
    build_result: Optional[CommandExecutionResult]
    install_result: Optional[CommandExecutionResult]

    @property
    def succeeded(self) -> bool:
        """Return whether every required stage succeeded."""

        if (
            not self.configure_result.succeeded
            or self.build_result is None
            or not self.build_result.succeeded
        ):
            return False

        if self.plan.install_wheel:
            return (
                self.install_result is not None
                and self.install_result.succeeded
            )

        return self.install_result is None


def _validated_command(
    command: Sequence[str],
) -> Tuple[str, ...]:
    command_tuple = tuple(command)

    if not command_tuple:
        raise ExecutionError("command must not be empty")

    for argument in command_tuple:
        if not isinstance(argument, str):
            raise ExecutionError(
                "command arguments must all be strings"
            )

        if not argument:
            raise ExecutionError(
                "command arguments must not be empty"
            )

        if "\0" in argument:
            raise ExecutionError(
                "command arguments must not contain null bytes"
            )

    return command_tuple


def _existing_working_directory(path: Path) -> Path:
    resolved = Path(path).expanduser().resolve()

    if not resolved.is_dir():
        raise ExecutionError(
            "working directory does not exist or is not a "
            f"directory: {resolved}"
        )

    return resolved


def _prepare_log_directory(
    log_directory: Optional[Path],
) -> Optional[Path]:
    if log_directory is None:
        return None

    resolved = (
        Path(log_directory)
        .expanduser()
        .resolve()
    )

    if resolved.exists() and not resolved.is_dir():
        raise ExecutionError(
            "log directory path exists but is not a directory: "
            f"{resolved}"
        )

    try:
        resolved.mkdir(
            parents=True,
            exist_ok=True,
        )
    except OSError as exc:
        raise ExecutionError(
            f"could not create log directory {resolved}: {exc}"
        ) from exc

    return resolved


def _stage_log_path(
    log_directory: Optional[Path],
    manifest_name: str,
    stage: str,
) -> Optional[Path]:
    if log_directory is None:
        return None

    return (
        log_directory
        / f"{manifest_name}.{stage}.log"
    )


def execute_command(
    command: Sequence[str],
    *,
    working_directory: Path,
    output_stream: Optional[TextIO] = None,
    log_path: Optional[Path] = None,
) -> CommandExecutionResult:
    """Execute one command while optionally streaming and logging output."""

    command_tuple = _validated_command(command)
    resolved_working_directory = (
        _existing_working_directory(
            working_directory
        )
    )

    resolved_log_path: Optional[Path] = None
    log_stream = None

    if log_path is not None:
        resolved_log_path = (
            Path(log_path)
            .expanduser()
            .resolve()
        )

        if (
            resolved_log_path.exists()
            and not resolved_log_path.is_file()
        ):
            raise ExecutionError(
                "log path exists but is not a file: "
                f"{resolved_log_path}"
            )

        try:
            resolved_log_path.parent.mkdir(
                parents=True,
                exist_ok=True,
            )
            log_stream = resolved_log_path.open(
                "w",
                encoding="utf-8",
                newline="",
            )
        except OSError as exc:
            raise ExecutionError(
                f"could not open log file "
                f"{resolved_log_path}: {exc}"
            ) from exc

    process = None

    try:
        try:
            process = subprocess.Popen(
                command_tuple,
                cwd=str(resolved_working_directory),
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding="utf-8",
                errors="replace",
            )
        except OSError as exc:
            raise ExecutionError(
                f"could not start command "
                f"{command_tuple[0]!r}: {exc}"
            ) from exc

        if process.stdout is None:
            process.kill()
            process.wait()

            raise ExecutionError(
                "subprocess output pipe was not created"
            )

        try:
            for line in process.stdout:
                if output_stream is not None:
                    output_stream.write(line)
                    output_stream.flush()

                if log_stream is not None:
                    log_stream.write(line)
                    log_stream.flush()
        finally:
            process.stdout.close()

        return_code = process.wait()
    finally:
        if (
            process is not None
            and process.poll() is None
        ):
            process.kill()
            process.wait()

        if (
            process is not None
            and process.stdout is not None
            and not process.stdout.closed
        ):
            process.stdout.close()

        if log_stream is not None:
            log_stream.close()

    return CommandExecutionResult(
        command=command_tuple,
        return_code=return_code,
        log_path=resolved_log_path,
    )


def _preflight_plan(
    plan: VariantPlan,
    log_directory: Optional[Path],
) -> None:
    if not isinstance(plan, VariantPlan):
        raise ExecutionError(
            "every plan must be a validated VariantPlan"
        )

    variant_build_directory = (
        Path(plan.variant_build_directory)
        .expanduser()
        .resolve()
    )

    if (
        variant_build_directory.exists()
        and not variant_build_directory.is_dir()
    ):
        raise ExecutionError(
            "variant build directory path exists but is not a directory: "
            f"{variant_build_directory}"
        )

    stages = ["configure", "build"]

    if plan.install_wheel:
        python_executable = (
            Path(plan.python_executable)
            .expanduser()
            .resolve()
        )

        if not python_executable.is_file():
            raise ExecutionError(
                "Python executable for wheel installation does not "
                f"exist or is not a file: {python_executable}"
            )

        if not _INSTALL_WHEEL_SCRIPT.is_file():
            raise ExecutionError(
                "wheel installation helper does not exist or is not "
                f"a file: {_INSTALL_WHEEL_SCRIPT}"
            )

        stages.append("install")

    for stage in stages:
        log_path = _stage_log_path(
            log_directory,
            plan.manifest.name,
            stage,
        )

        if (
            log_path is not None
            and log_path.exists()
            and not log_path.is_file()
        ):
            raise ExecutionError(
                "log path exists but is not a file: "
                f"{log_path}"
            )


def _create_variant_build_directory(
    variant_build_directory: Path,
) -> Path:
    resolved = (
        Path(variant_build_directory)
        .expanduser()
        .resolve()
    )

    try:
        resolved.mkdir(
            parents=True,
            exist_ok=True,
        )
    except OSError as exc:
        raise ExecutionError(
            f"could not create variant build directory "
            f"{resolved}: {exc}"
        ) from exc

    if not resolved.is_dir():
        raise ExecutionError(
            f"build directory is not a directory: {resolved}"
        )

    return resolved


def _remove_cmake_cache(
    variant_build_directory: Path,
) -> None:
    cache_path = (
        Path(variant_build_directory)
        .expanduser()
        .resolve()
        / "CMakeCache.txt"
    )

    if not cache_path.exists():
        return

    if not cache_path.is_file():
        raise ExecutionError(
            "CMake cache path exists but is not a file: "
            f"{cache_path}"
        )

    try:
        cache_path.unlink()
    except OSError as exc:
        raise ExecutionError(
            f"could not remove CMake cache {cache_path}: {exc}"
        ) from exc


def _native_quiet_build_command(
    command: Sequence[str],
    variant_build_directory: Path,
    *,
    quiet: bool,
) -> Tuple[str, ...]:
    """Add native Make quiet flags for a Unix Makefiles build."""

    command_tuple = _validated_command(command)

    if not quiet:
        return command_tuple

    cache_path = (
        Path(variant_build_directory)
        .expanduser()
        .resolve()
        / "CMakeCache.txt"
    )

    if not cache_path.is_file():
        return command_tuple

    try:
        cache_lines = cache_path.read_text(
            encoding="utf-8",
            errors="replace",
        ).splitlines()
    except OSError as exc:
        raise ExecutionError(
            f"could not read CMake cache {cache_path}: {exc}"
        ) from exc

    generator = None

    for line in cache_lines:
        if (
            line.startswith("CMAKE_GENERATOR:")
            and "=" in line
        ):
            generator = line.split("=", 1)[1]
            break

    if generator != "Unix Makefiles":
        return command_tuple

    return (
        *command_tuple,
        "--",
        "--quiet",
        "--no-print-directory",
    )


def _install_command(
    plan: VariantPlan,
) -> Tuple[str, ...]:
    """Return the runtime wheel-installation helper command."""

    artifact_manifest = (
        plan.variant_build_directory
        / "src/libs/cgalpy/dist/distribution-artifacts.json"
    ).resolve()

    pip_option_arguments = tuple(
        f"--pip-install-option={option}"
        for option in plan.pip_install_options
    )

    return (
        str(plan.python_executable),
        str(_INSTALL_WHEEL_SCRIPT),
        "--manifest",
        str(artifact_manifest),
        *pip_option_arguments,
    )


def execute_variant(
    plan: VariantPlan,
    *,
    log_directory: Optional[Path] = None,
    output_stream: Optional[TextIO] = None,
    clean: bool = False,
    quiet: bool = False,
) -> VariantExecutionResult:
    """Configure, build, and optionally install one variant."""

    if type(clean) is not bool:
        raise ExecutionError(
            "clean must be a boolean"
        )

    if type(quiet) is not bool:
        raise ExecutionError(
            "quiet must be a boolean"
        )

    resolved_log_directory = _prepare_log_directory(
        log_directory
    )

    _preflight_plan(
        plan,
        resolved_log_directory,
    )

    variant_build_directory = _create_variant_build_directory(
        plan.variant_build_directory
    )

    if clean:
        _remove_cmake_cache(
            variant_build_directory
        )

    configure_result = execute_command(
        plan.configure_command,
        working_directory=variant_build_directory,
        output_stream=output_stream,
        log_path=_stage_log_path(
            resolved_log_directory,
            plan.manifest.name,
            "configure",
        ),
    )

    if not configure_result.succeeded:
        return VariantExecutionResult(
            plan=plan,
            configure_result=configure_result,
            build_result=None,
            install_result=None,
        )

    build_command = _native_quiet_build_command(
        plan.build_command,
        variant_build_directory,
        quiet=quiet,
    )

    build_result = execute_command(
        build_command,
        working_directory=variant_build_directory,
        output_stream=output_stream,
        log_path=_stage_log_path(
            resolved_log_directory,
            plan.manifest.name,
            "build",
        ),
    )

    if (
        not build_result.succeeded
        or not plan.install_wheel
    ):
        return VariantExecutionResult(
            plan=plan,
            configure_result=configure_result,
            build_result=build_result,
            install_result=None,
        )

    install_result = execute_command(
        _install_command(plan),
        working_directory=variant_build_directory,
        output_stream=output_stream,
        log_path=_stage_log_path(
            resolved_log_directory,
            plan.manifest.name,
            "install",
        ),
    )

    return VariantExecutionResult(
        plan=plan,
        configure_result=configure_result,
        build_result=build_result,
        install_result=install_result,
    )


def execute_variants(
    plans: Iterable[VariantPlan],
    *,
    continue_on_error: bool,
    log_directory: Optional[Path] = None,
    output_stream: Optional[TextIO] = None,
    clean: bool = False,
    quiet: bool = False,
) -> Tuple[VariantExecutionResult, ...]:
    """Execute plans in order with optional continuation after failure."""

    if type(continue_on_error) is not bool:
        raise ExecutionError(
            "continue_on_error must be a boolean"
        )

    if type(clean) is not bool:
        raise ExecutionError(
            "clean must be a boolean"
        )

    if type(quiet) is not bool:
        raise ExecutionError(
            "quiet must be a boolean"
        )

    plan_tuple = tuple(plans)
    resolved_log_directory = _prepare_log_directory(
        log_directory
    )

    for plan in plan_tuple:
        _preflight_plan(
            plan,
            resolved_log_directory,
        )

    results = []

    for plan in plan_tuple:
        if quiet and output_stream is not None:
            print(
                "variant: "
                f"{plan.manifest.name}: "
                f"{plan.variant_build_directory}",
                file=output_stream,
                flush=True,
            )

        result = execute_variant(
            plan,
            log_directory=resolved_log_directory,
            output_stream=(
                None
                if quiet
                else output_stream
            ),
            clean=clean,
            quiet=quiet,
        )
        results.append(result)

        if (
            not result.succeeded
            and not continue_on_error
        ):
            break

    return tuple(results)
