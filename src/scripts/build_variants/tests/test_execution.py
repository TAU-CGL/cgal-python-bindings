#!/usr/bin/env python3
"""Tests for isolated build-variant command execution."""

from __future__ import annotations

import io
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from build_variants.execution import (
    CommandExecutionResult,
    ExecutionError,
    VariantExecutionResult,
    execute_command,
    execute_variant,
    execute_variants,
)
from build_variants.manifest import BuildVariantManifest
from build_variants.planning import VariantPlan


class VariantExecutionTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()
        self.build_directory = self.root / "build"
        self.build_directory.mkdir()

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def manifest(
        self,
        name: str,
    ) -> BuildVariantManifest:
        return BuildVariantManifest(
            schema_version=1,
            name=name,
            description=f"Variant {name}",
            cmake_tests=(
                f"cmake/tests/{name}.cmake",
            ),
            path=self.root / f"{name}.json",
        )

    def plan(
        self,
        name: str,
        *,
        configure_code: str,
        build_code: str,
        install_wheel: bool = False,
        pip_install_options=(),
    ) -> VariantPlan:
        return VariantPlan(
            manifest=self.manifest(name),
            operating_system="macos",
            compiler="test-compiler",
            build_variant_directory=(
                self.build_directory / name
            ),
            python_executable=Path(sys.executable).resolve(),
            install_wheel=install_wheel,
            pip_install_options=tuple(pip_install_options),
            configure_command=(
                sys.executable,
                "-c",
                configure_code,
            ),
            build_command=(
                sys.executable,
                "-c",
                build_code,
            ),
        )

    def test_execute_command_streams_combined_output(
        self,
    ) -> None:
        output = io.StringIO()

        result = execute_command(
            (
                sys.executable,
                "-c",
                (
                    "import sys; "
                    "print('stdout', flush=True); "
                    "print('stderr', file=sys.stderr, flush=True); "
                    "raise SystemExit(3)"
                ),
            ),
            working_directory=self.root,
            output_stream=output,
        )

        self.assertEqual(result.return_code, 3)
        self.assertFalse(result.succeeded)
        self.assertEqual(
            output.getvalue().splitlines(),
            ["stdout", "stderr"],
        )

    def test_execute_command_closes_output_pipe(
        self,
    ) -> None:
        process = mock.Mock()
        process.stdout = io.StringIO(
            "captured output\n"
        )
        process.wait.return_value = 0
        process.poll.return_value = 0

        with mock.patch(
            "build_variants.execution.subprocess.Popen",
            return_value=process,
        ):
            result = execute_command(
                ("synthetic-command",),
                working_directory=self.root,
            )

        self.assertTrue(result.succeeded)
        self.assertTrue(process.stdout.closed)
        process.kill.assert_not_called()

    def test_execute_command_writes_log(
        self,
    ) -> None:
        log_path = self.root / "logs/command.log"

        result = execute_command(
            (
                sys.executable,
                "-c",
                "print('logged output')",
            ),
            working_directory=self.root,
            log_path=log_path,
        )

        self.assertTrue(result.succeeded)
        self.assertEqual(
            result.log_path,
            log_path.resolve(),
        )
        self.assertEqual(
            log_path.read_text(encoding="utf-8"),
            "logged output\n",
        )

    def test_missing_working_directory_is_rejected(
        self,
    ) -> None:
        with self.assertRaisesRegex(
            ExecutionError,
            "working directory does not exist",
        ):
            execute_command(
                (
                    sys.executable,
                    "-c",
                    "pass",
                ),
                working_directory=self.root / "missing",
            )

    def test_execute_variant_configures_then_builds(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code=(
                "from pathlib import Path; "
                "Path('order.txt').write_text("
                "'configure\\n', encoding='utf-8')"
            ),
            build_code=(
                "from pathlib import Path; "
                "path = Path('order.txt'); "
                "path.write_text("
                "path.read_text(encoding='utf-8') + "
                "'build\\n', encoding='utf-8')"
            ),
        )

        result = execute_variant(plan)

        self.assertTrue(result.succeeded)
        self.assertTrue(
            result.configure_result.succeeded
        )
        self.assertIsNotNone(result.build_result)
        self.assertEqual(
            (
                plan.build_variant_directory / "order.txt"
            ).read_text(encoding="utf-8"),
            "configure\nbuild\n",
        )

    def test_configure_failure_skips_build(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code="raise SystemExit(5)",
            build_code=(
                "from pathlib import Path; "
                "Path('build-ran.txt').write_text("
                "'yes', encoding='utf-8')"
            ),
        )

        result = execute_variant(plan)

        self.assertFalse(result.succeeded)
        self.assertEqual(
            result.configure_result.return_code,
            5,
        )
        self.assertIsNone(result.build_result)
        self.assertFalse(
            (
                plan.build_variant_directory
                / "build-ran.txt"
            ).exists()
        )

    def test_execute_variants_stops_after_failure(
        self,
    ) -> None:
        first = self.plan(
            "first",
            configure_code="raise SystemExit(2)",
            build_code="pass",
        )
        second = self.plan(
            "second",
            configure_code=(
                "from pathlib import Path; "
                "Path('second-ran.txt').write_text("
                "'yes', encoding='utf-8')"
            ),
            build_code="pass",
        )

        results = execute_variants(
            (first, second),
            continue_on_error=False,
        )

        self.assertEqual(len(results), 1)
        self.assertFalse(results[0].succeeded)
        self.assertFalse(
            second.build_variant_directory.exists()
        )

    def test_execute_variants_can_continue_after_failure(
        self,
    ) -> None:
        first = self.plan(
            "first",
            configure_code="raise SystemExit(2)",
            build_code="pass",
        )
        second = self.plan(
            "second",
            configure_code="pass",
            build_code="pass",
        )

        results = execute_variants(
            (first, second),
            continue_on_error=True,
        )

        self.assertEqual(len(results), 2)
        self.assertFalse(results[0].succeeded)
        self.assertTrue(results[1].succeeded)

    def command_result(
        self,
        command,
        return_code: int = 0,
        log_path=None,
    ) -> CommandExecutionResult:
        return CommandExecutionResult(
            command=tuple(command),
            return_code=return_code,
            log_path=log_path,
        )

    def test_install_runs_after_successful_build(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
            pip_install_options=("--user", "--no-deps"),
        )

        results = [
            self.command_result(
                plan.configure_command
            ),
            self.command_result(
                plan.build_command
            ),
            self.command_result(
                (
                    str(plan.python_executable),
                    "install-helper",
                )
            ),
        ]

        with mock.patch(
            "build_variants.execution.execute_command",
            side_effect=results,
        ) as executor:
            result = execute_variant(plan)

        self.assertTrue(result.succeeded)
        self.assertIsNotNone(result.install_result)
        self.assertEqual(executor.call_count, 3)

        configure_call = executor.call_args_list[0]
        build_call = executor.call_args_list[1]
        install_call = executor.call_args_list[2]

        self.assertEqual(
            configure_call.args[0],
            plan.configure_command,
        )
        self.assertEqual(
            build_call.args[0],
            plan.build_command,
        )

        install_command = install_call.args[0]

        self.assertEqual(
            install_command[0],
            str(plan.python_executable),
        )
        self.assertTrue(
            install_command[1].endswith(
                "build_variants/install_wheel.py"
            )
        )
        self.assertEqual(
            install_command[2],
            "--manifest",
        )
        self.assertEqual(
            Path(install_command[3]),
            (
                plan.build_variant_directory
                / "src/libs/cgalpy/dist/"
                "distribution-artifacts.json"
            ).resolve(),
        )
        self.assertEqual(
            install_command[4:],
            (
                "--pip-install-option=--user",
                "--pip-install-option=--no-deps",
            ),
        )

    def test_build_failure_skips_installation(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code="pass",
            build_code="raise SystemExit(8)",
            install_wheel=True,
        )

        with mock.patch(
            "build_variants.execution.execute_command",
            side_effect=(
                self.command_result(
                    plan.configure_command
                ),
                self.command_result(
                    plan.build_command,
                    return_code=8,
                ),
            ),
        ) as executor:
            result = execute_variant(plan)

        self.assertFalse(result.succeeded)
        self.assertEqual(executor.call_count, 2)
        self.assertIsNotNone(result.build_result)
        self.assertEqual(
            result.build_result.return_code,
            8,
        )
        self.assertIsNone(result.install_result)

    def test_install_failure_fails_variant(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
        )

        with mock.patch(
            "build_variants.execution.execute_command",
            side_effect=(
                self.command_result(
                    plan.configure_command
                ),
                self.command_result(
                    plan.build_command
                ),
                self.command_result(
                    ("install-helper",),
                    return_code=9,
                ),
            ),
        ):
            result = execute_variant(plan)

        self.assertFalse(result.succeeded)
        self.assertIsNotNone(result.install_result)
        self.assertEqual(
            result.install_result.return_code,
            9,
        )

    def test_install_log_uses_deterministic_name(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
        )
        log_directory = self.root / "logs"

        def synthetic_execution(
            command,
            *,
            working_directory,
            output_stream=None,
            log_path=None,
        ):
            return self.command_result(
                command,
                log_path=log_path,
            )

        with mock.patch(
            "build_variants.execution.execute_command",
            side_effect=synthetic_execution,
        ):
            result = execute_variant(
                plan,
                log_directory=log_directory,
            )

        self.assertTrue(result.succeeded)
        self.assertIsNotNone(result.install_result)
        self.assertEqual(
            result.install_result.log_path,
            (
                log_directory
                / "alpha.install.log"
            ).resolve(),
        )

    def test_execute_variants_continues_after_install_failure(
        self,
    ) -> None:
        first = self.plan(
            "first",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
        )
        second = self.plan(
            "second",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
        )

        failed_install = VariantExecutionResult(
            plan=first,
            configure_result=self.command_result(
                first.configure_command
            ),
            build_result=self.command_result(
                first.build_command
            ),
            install_result=self.command_result(
                ("install-helper",),
                return_code=9,
            ),
        )
        successful_install = VariantExecutionResult(
            plan=second,
            configure_result=self.command_result(
                second.configure_command
            ),
            build_result=self.command_result(
                second.build_command
            ),
            install_result=self.command_result(
                ("install-helper",),
            ),
        )

        with mock.patch(
            "build_variants.execution.execute_variant",
            side_effect=(
                failed_install,
                successful_install,
            ),
        ) as executor:
            results = execute_variants(
                (first, second),
                continue_on_error=True,
            )

        self.assertEqual(len(results), 2)
        self.assertFalse(results[0].succeeded)
        self.assertTrue(results[1].succeeded)
        self.assertEqual(executor.call_count, 2)

    def test_execute_variants_stops_after_install_failure(
        self,
    ) -> None:
        first = self.plan(
            "first",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
        )
        second = self.plan(
            "second",
            configure_code="pass",
            build_code="pass",
            install_wheel=True,
        )

        failed_install = VariantExecutionResult(
            plan=first,
            configure_result=self.command_result(
                first.configure_command
            ),
            build_result=self.command_result(
                first.build_command
            ),
            install_result=self.command_result(
                ("install-helper",),
                return_code=9,
            ),
        )

        with mock.patch(
            "build_variants.execution.execute_variant",
            return_value=failed_install,
        ) as executor:
            results = execute_variants(
                (first, second),
                continue_on_error=False,
            )

        self.assertEqual(len(results), 1)
        self.assertFalse(results[0].succeeded)
        executor.assert_called_once()


    def test_variant_logs_use_deterministic_names(
        self,
    ) -> None:
        plan = self.plan(
            "alpha",
            configure_code="print('configure output')",
            build_code="print('build output')",
        )
        log_directory = self.root / "logs"

        result = execute_variant(
            plan,
            log_directory=log_directory,
        )

        self.assertTrue(result.succeeded)
        self.assertEqual(
            result.configure_result.log_path,
            (
                log_directory
                / "alpha.configure.log"
            ).resolve(),
        )
        self.assertEqual(
            result.build_result.log_path,
            (
                log_directory
                / "alpha.build.log"
            ).resolve(),
        )
        self.assertEqual(
            (
                log_directory
                / "alpha.configure.log"
            ).read_text(encoding="utf-8"),
            "configure output\n",
        )
        self.assertEqual(
            (
                log_directory
                / "alpha.build.log"
            ).read_text(encoding="utf-8"),
            "build output\n",
        )

    def test_invalid_log_directory_is_rejected(
        self,
    ) -> None:
        log_directory = self.root / "logs"
        log_directory.write_text(
            "not a directory\n",
            encoding="utf-8",
        )

        with self.assertRaisesRegex(
            ExecutionError,
            "not a directory",
        ):
            execute_variant(
                self.plan(
                    "alpha",
                    configure_code="pass",
                    build_code="pass",
                ),
                log_directory=log_directory,
            )

    def test_batch_preflight_prevents_partial_execution(
        self,
    ) -> None:
        first = self.plan(
            "first",
            configure_code=(
                "from pathlib import Path; "
                "Path('first-ran.txt').write_text("
                "'yes', encoding='utf-8')"
            ),
            build_code="pass",
        )
        second = self.plan(
            "second",
            configure_code="pass",
            build_code="pass",
        )

        second.build_variant_directory.write_text(
            "not a directory\n",
            encoding="utf-8",
        )

        with self.assertRaisesRegex(
            ExecutionError,
            "build variant directory path exists",
        ):
            execute_variants(
                (first, second),
                continue_on_error=False,
            )

        self.assertFalse(
            first.build_variant_directory.exists()
        )


if __name__ == "__main__":
    unittest.main()
