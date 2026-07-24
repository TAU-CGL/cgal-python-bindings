#!/usr/bin/env python3
"""Tests for list and dry-run application orchestration."""

from __future__ import annotations

import io
import json
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from build_variants.application import (
    prepare_application_output,
    run_application,
)
from build_variants.cli import RunnerArguments
from build_variants.execution import (
    CommandExecutionResult,
    ExecutionError,
    VariantExecutionResult,
)


class RunnerApplicationTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()

        self.source_directory = self.root / "source"
        self.build_directory = self.root / "build"
        self.manifest_directory = (
            self.source_directory
            / "src/scripts/build_variants/manifests"
        )
        self.cmake_test_directory = (
            self.source_directory / "cmake/tests"
        )

        self.manifest_directory.mkdir(parents=True)
        self.cmake_test_directory.mkdir(parents=True)
        self.build_directory.mkdir()

        self.runner_path = (
            self.source_directory / "src/scripts/run"
        )
        self.runner_path.parent.mkdir(
            parents=True,
            exist_ok=True,
        )
        self.runner_path.write_text(
            "",
            encoding="utf-8",
        )

        self.python_executable = self.root / "python"
        self.python_executable.write_text(
            "",
            encoding="utf-8",
        )

        self.write_variant("alpha")
        self.write_variant("beta")

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def write_variant(self, name: str) -> None:
        reference = f"cmake/tests/{name}.cmake"

        (
            self.source_directory / reference
        ).write_text(
            f'set(VARIANT "{name}" CACHE STRING "" FORCE)\n',
            encoding="utf-8",
        )

        (
            self.manifest_directory / f"{name}.json"
        ).write_text(
            json.dumps(
                {
                    "schema_version": 1,
                    "name": name,
                    "description": f"Variant {name}",
                    "cmake_tests": [reference],
                },
                indent=2,
            )
            + "\n",
            encoding="utf-8",
        )

    def arguments(
        self,
        *,
        manifests=(),
        all_manifests=False,
        list_manifests=False,
        dry_run=False,
        generate_run=False,
        abort_after_run_generation=False,
        continue_on_error=False,
        clean=False,
        quiet=False,
        install_wheel=False,
        pip_install_options=(),
        log_directory=None,
        manifest_directory=None,
    ) -> RunnerArguments:
        return RunnerArguments(
            manifests=tuple(manifests),
            all_manifests=all_manifests,
            build_type="Release",
            fixed_library_name=False,
            operating_system="macos",
            compiler="/usr/bin/c++",
            generate_run=generate_run,
            abort_after_run_generation=(
                abort_after_run_generation
            ),
            manifest_directory=(
                manifest_directory
                if manifest_directory is not None
                else self.manifest_directory
            ),
            source_directory=self.source_directory,
            build_directory=self.build_directory,
            cgal_dir=None,
            python_executable=self.python_executable,
            install_wheel=install_wheel,
            pip_install_options=tuple(pip_install_options),
            nanobind_dir=None,
            jobs=4,
            list_manifests=list_manifests,
            dry_run=dry_run,
            continue_on_error=continue_on_error,
            log_directory=log_directory,
            clean=clean,
            quiet=quiet,
        )

    def run_case(self, arguments):
        stdout = io.StringIO()
        stderr = io.StringIO()

        exit_code = run_application(
            arguments,
            stdout=stdout,
            stderr=stderr,
        )

        return (
            exit_code,
            stdout.getvalue(),
            stderr.getvalue(),
        )

    def test_list_mode_renders_catalog(self) -> None:
        exit_code, stdout, stderr = self.run_case(
            self.arguments(
                list_manifests=True,
            )
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout.splitlines(),
            [
                (
                    "alpha: Variant alpha "
                    "[cmake/tests/alpha.cmake]"
                ),
                (
                    "beta: Variant beta "
                    "[cmake/tests/beta.cmake]"
                ),
            ],
        )

    def test_list_mode_does_not_require_planning(self) -> None:
        exit_code, stdout, stderr = self.run_case(
            self.arguments(
                manifests=("missing",),
                list_manifests=True,
                dry_run=True,
            )
        )

        self.assertEqual(exit_code, 0)
        self.assertIn(
            "alpha: Variant alpha",
            stdout,
        )
        self.assertEqual(stderr, "")
        self.assertNotIn(
            "unknown manifest",
            stdout,
        )

    def test_dry_run_preserves_requested_order(self) -> None:
        exit_code, stdout, stderr = self.run_case(
            self.arguments(
                manifests=("beta", "alpha"),
                dry_run=True,
            )
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")

        beta_index = stdout.index("manifest: beta")
        alpha_index = stdout.index("manifest: alpha")

        self.assertLess(beta_index, alpha_index)
        self.assertIn(
            "configure-command: cmake ",
            stdout,
        )
        self.assertIn(
            "build-command: cmake --build ",
            stdout,
        )

    def test_all_dry_run_uses_catalog_order(self) -> None:
        exit_code, stdout, stderr = self.run_case(
            self.arguments(
                all_manifests=True,
                dry_run=True,
            )
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertLess(
            stdout.index("manifest: alpha"),
            stdout.index("manifest: beta"),
        )

    def test_unknown_dry_run_manifest_returns_error(self) -> None:
        exit_code, stdout, stderr = self.run_case(
            self.arguments(
                manifests=("missing",),
                dry_run=True,
            )
        )

        self.assertEqual(exit_code, 2)
        self.assertEqual(stdout, "")
        self.assertIn(
            "error: unknown manifest 'missing'",
            stderr,
        )
        self.assertIn(
            "available manifests: alpha, beta",
            stderr,
        )

    def test_missing_manifest_directory_returns_error(self) -> None:
        exit_code, stdout, stderr = self.run_case(
            self.arguments(
                list_manifests=True,
                manifest_directory=(
                    self.root / "missing-manifests"
                ),
            )
        )

        self.assertEqual(exit_code, 2)
        self.assertEqual(stdout, "")
        self.assertIn(
            "manifest directory does not exist",
            stderr,
        )

    def run_generation_case(self, arguments):
        stdout = io.StringIO()
        stderr = io.StringIO()

        exit_code = run_application(
            arguments,
            stdout=stdout,
            stderr=stderr,
            current_directory=self.root,
        )

        return (
            exit_code,
            stdout.getvalue(),
            stderr.getvalue(),
        )

    def execution_result(
        self,
        plan,
        *,
        configure_return_code=0,
        build_return_code=0,
        install_return_code=None,
    ):
        configure_result = CommandExecutionResult(
            command=plan.configure_command,
            return_code=configure_return_code,
            log_path=None,
        )

        build_result = (
            None
            if build_return_code is None
            else CommandExecutionResult(
                command=plan.build_command,
                return_code=build_return_code,
                log_path=None,
            )
        )

        install_result = (
            None
            if install_return_code is None
            else CommandExecutionResult(
                command=("install-helper",),
                return_code=install_return_code,
                log_path=None,
            )
        )

        return VariantExecutionResult(
            plan=plan,
            configure_result=configure_result,
            build_result=build_result,
            install_result=install_result,
        )

    def successful_execution(
        self,
        plans,
        **unused_arguments,
    ):
        return tuple(
            self.execution_result(plan)
            for plan in plans
        )

    def test_generate_and_abort_writes_launcher(
        self,
    ) -> None:
        exit_code, stdout, stderr = (
            self.run_generation_case(
                self.arguments(
                    manifests=("alpha",),
                    generate_run=True,
                    abort_after_run_generation=True,
                )
            )
        )

        launcher = self.root / "run_alpha"

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout,
            f"generated-launcher: {launcher}\n",
        )
        self.assertTrue(launcher.is_file())

    def test_generate_all_writes_catalog_launchers(
        self,
    ) -> None:
        exit_code, stdout, stderr = (
            self.run_generation_case(
                self.arguments(
                    all_manifests=True,
                    generate_run=True,
                    abort_after_run_generation=True,
                )
            )
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout.splitlines(),
            [
                (
                    "generated-launcher: "
                    f"{self.root / 'run_alpha'}"
                ),
                (
                    "generated-launcher: "
                    f"{self.root / 'run_beta'}"
                ),
            ],
        )
        self.assertTrue(
            (self.root / "run_alpha").is_file()
        )
        self.assertTrue(
            (self.root / "run_beta").is_file()
        )

    def test_generate_and_dry_run_preserves_order(
        self,
    ) -> None:
        exit_code, stdout, stderr = (
            self.run_generation_case(
                self.arguments(
                    manifests=("beta", "alpha"),
                    generate_run=True,
                    dry_run=True,
                )
            )
        )

        lines = stdout.splitlines()

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            lines[0:2],
            [
                (
                    "generated-launcher: "
                    f"{self.root / 'run_beta'}"
                ),
                (
                    "generated-launcher: "
                    f"{self.root / 'run_alpha'}"
                ),
            ],
        )
        self.assertLess(
            lines.index("manifest: beta"),
            lines.index("manifest: alpha"),
        )
        self.assertTrue(
            (self.root / "run_beta").is_file()
        )
        self.assertTrue(
            (self.root / "run_alpha").is_file()
        )

    def test_unknown_generation_manifest_has_no_side_effects(
        self,
    ) -> None:
        exit_code, stdout, stderr = (
            self.run_generation_case(
                self.arguments(
                    manifests=("alpha", "missing"),
                    generate_run=True,
                    abort_after_run_generation=True,
                )
            )
        )

        self.assertEqual(exit_code, 2)
        self.assertEqual(stdout, "")
        self.assertIn(
            "unknown manifest 'missing'",
            stderr,
        )
        self.assertFalse(
            (self.root / "run_alpha").exists()
        )

    def test_list_mode_does_not_generate_launchers(
        self,
    ) -> None:
        exit_code, stdout, stderr = (
            self.run_generation_case(
                self.arguments(
                    manifests=("missing",),
                    list_manifests=True,
                    generate_run=True,
                    abort_after_run_generation=True,
                )
            )
        )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertIn("alpha:", stdout)
        self.assertFalse(
            (self.root / "run_missing").exists()
        )

    def test_generate_then_live_success_reports_launcher(
        self,
    ) -> None:
        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=self.successful_execution,
        ):
            exit_code, stdout, stderr = (
                self.run_generation_case(
                    self.arguments(
                        manifests=("alpha",),
                        generate_run=True,
                    )
                )
            )

        launcher = self.root / "run_alpha"

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout.splitlines(),
            [
                f"generated-launcher: {launcher}",
                "variant-result: alpha: success",
            ],
        )
        self.assertTrue(launcher.is_file())

    def test_live_execution_success_returns_zero(
        self,
    ) -> None:
        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=self.successful_execution,
        ) as executor:
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha",),
                )
            )

        self.assertEqual(exit_code, 0)
        self.assertEqual(
            stdout,
            "variant-result: alpha: success\n",
        )
        self.assertEqual(stderr, "")
        self.assertFalse(
            executor.call_args.kwargs[
                "continue_on_error"
            ]
        )
        self.assertIsNone(
            executor.call_args.kwargs[
                "log_directory"
            ]
        )
        self.assertFalse(
            executor.call_args.kwargs[
                "clean"
            ]
        )
        self.assertFalse(
            executor.call_args.kwargs[
                "quiet"
            ]
        )

    def test_live_execution_forwards_clean(
        self,
    ) -> None:
        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=self.successful_execution,
        ) as executor:
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha",),
                    clean=True,
                )
            )

        self.assertEqual(exit_code, 0)
        self.assertEqual(
            stdout,
            "variant-result: alpha: success\n",
        )
        self.assertEqual(stderr, "")
        self.assertTrue(
            executor.call_args.kwargs[
                "clean"
            ]
        )

    def test_live_execution_forwards_quiet(
        self,
    ) -> None:
        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=self.successful_execution,
        ) as executor:
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha",),
                    quiet=True,
                )
            )

        self.assertEqual(exit_code, 0)
        self.assertEqual(
            stdout,
            "variant-result: alpha: success\n",
        )
        self.assertEqual(stderr, "")
        self.assertTrue(
            executor.call_args.kwargs[
                "quiet"
            ]
        )

    def test_live_failure_marks_remaining_plan_skipped(
        self,
    ) -> None:
        def failing_execution(plans, **unused_arguments):
            return (
                self.execution_result(
                    plans[0],
                    configure_return_code=5,
                    build_return_code=None,
                ),
            )

        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=failing_execution,
        ):
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha", "beta"),
                )
            )

        self.assertEqual(exit_code, 1)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout.splitlines(),
            [
                (
                    "variant-result: alpha: "
                    "configure-failed (exit 5)"
                ),
                "variant-result: beta: skipped",
            ],
        )

    def test_install_failure_is_reported(
        self,
    ) -> None:
        def failing_install(plans, **unused_arguments):
            return (
                self.execution_result(
                    plans[0],
                    install_return_code=9,
                ),
            )

        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=failing_install,
        ):
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha",),
                    install_wheel=True,
                )
            )

        self.assertEqual(exit_code, 1)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout,
            (
                "variant-result: alpha: "
                "install-failed (exit 9)\n"
            ),
        )

    def test_successful_install_is_reported_as_success(
        self,
    ) -> None:
        def successful_install(plans, **unused_arguments):
            return (
                self.execution_result(
                    plans[0],
                    install_return_code=0,
                ),
            )

        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=successful_install,
        ):
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha",),
                    install_wheel=True,
                )
            )

        self.assertEqual(exit_code, 0)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout,
            "variant-result: alpha: success\n",
        )


    def test_continue_on_error_reports_all_results(
        self,
    ) -> None:
        log_directory = self.root / "logs"

        def mixed_execution(plans, **unused_arguments):
            return (
                self.execution_result(
                    plans[0],
                    configure_return_code=6,
                    build_return_code=None,
                ),
                self.execution_result(plans[1]),
            )

        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=mixed_execution,
        ) as executor:
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha", "beta"),
                    continue_on_error=True,
                    log_directory=log_directory,
                )
            )

        self.assertEqual(exit_code, 1)
        self.assertEqual(stderr, "")
        self.assertEqual(
            stdout.splitlines(),
            [
                (
                    "variant-result: alpha: "
                    "configure-failed (exit 6)"
                ),
                "variant-result: beta: success",
            ],
        )
        self.assertTrue(
            executor.call_args.kwargs[
                "continue_on_error"
            ]
        )
        self.assertEqual(
            executor.call_args.kwargs[
                "log_directory"
            ],
            log_directory,
        )

    def test_execution_error_returns_two(
        self,
    ) -> None:
        with mock.patch(
            "build_variants.application.execute_variants",
            side_effect=ExecutionError(
                "synthetic execution failure"
            ),
        ):
            exit_code, stdout, stderr = self.run_case(
                self.arguments(
                    manifests=("alpha",),
                )
            )

        self.assertEqual(exit_code, 2)
        self.assertEqual(stdout, "")
        self.assertEqual(
            stderr,
            (
                "error: synthetic execution failure\n"
            ),
        )

    def test_prepare_output_has_no_execution_side_effects(
        self,
    ) -> None:
        build_directories_before = tuple(
            self.build_directory.iterdir()
        )

        lines = prepare_application_output(
            self.arguments(
                manifests=("alpha",),
                dry_run=True,
            )
        )

        build_directories_after = tuple(
            self.build_directory.iterdir()
        )

        self.assertTrue(lines)
        self.assertEqual(
            build_directories_before,
            build_directories_after,
        )


if __name__ == "__main__":
    unittest.main()
