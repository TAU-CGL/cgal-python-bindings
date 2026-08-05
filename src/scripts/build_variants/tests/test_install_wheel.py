#!/usr/bin/env python3
# Author(s): Utkarsh Khajuria <utkarshkhajuria55@gmail.com>

"""Tests for exact wheel validation and installation."""

from __future__ import annotations

import hashlib
import io
import json
import subprocess
import sys
import tempfile
import unittest
from contextlib import redirect_stderr
from pathlib import Path
from unittest import mock

from build_variants.install_wheel import (
    build_pip_command,
    main,
    resolve_wheel_path,
)


class InstallWheelTests(unittest.TestCase):
    def setUp(self) -> None:
        self.temporary_directory = tempfile.TemporaryDirectory()
        self.root = Path(
            self.temporary_directory.name
        ).resolve()
        self.dist_directory = self.root / "dist"
        self.dist_directory.mkdir()
        self.manifest_file = (
            self.dist_directory
            / "distribution-artifacts.json"
        )
        self.wheel_file = (
            self.dist_directory
            / "example_package-1.0-py3-none-any.whl"
        )
        self.wheel_file.write_bytes(b"synthetic wheel\n")

    def tearDown(self) -> None:
        self.temporary_directory.cleanup()

    def payload(self):
        wheel_bytes = self.wheel_file.read_bytes()

        return {
            "distribution_name": "example-package",
            "version": "1.0",
            "wheel": {
                "filename": self.wheel_file.name,
                "sha256": hashlib.sha256(
                    wheel_bytes
                ).hexdigest(),
                "size_bytes": len(wheel_bytes),
                "tags": ["py3-none-any"],
            },
            "sdist": {
                "filename": "example-package-1.0.tar.gz",
                "sha256": "0" * 64,
                "size_bytes": 0,
            },
        }

    def write_manifest(self, payload=None) -> None:
        selected_payload = (
            self.payload()
            if payload is None
            else payload
        )

        self.manifest_file.write_text(
            json.dumps(
                selected_payload,
                indent=2,
            )
            + "\n",
            encoding="utf-8",
        )

    def test_valid_manifest_resolves_exact_wheel(self) -> None:
        self.write_manifest()

        resolved = resolve_wheel_path(
            self.manifest_file
        )

        self.assertEqual(
            resolved,
            self.wheel_file.resolve(),
        )

    def test_valid_manifest_installs_exact_wheel(self) -> None:
        self.write_manifest()
        completed = subprocess.CompletedProcess(
            args=(),
            returncode=0,
        )

        with mock.patch(
            "build_variants.install_wheel.subprocess.run",
            return_value=completed,
        ) as runner:
            exit_code = main(
                [
                    "--manifest",
                    str(self.manifest_file),
                ]
            )

        self.assertEqual(exit_code, 0)
        runner.assert_called_once_with(
            build_pip_command(
                self.wheel_file.resolve()
            ),
            check=False,
        )
        self.assertEqual(
            runner.call_args.args[0],
            (
                sys.executable,
                "-m",
                "pip",
                "install",
                "--force-reinstall",
                str(self.wheel_file.resolve()),
            ),
        )

    def test_malformed_json_is_rejected_without_pip(self) -> None:
        self.manifest_file.write_text(
            "{not valid json\n",
            encoding="utf-8",
        )
        stderr = io.StringIO()

        with mock.patch(
            "build_variants.install_wheel.subprocess.run"
        ) as runner:
            with redirect_stderr(stderr):
                exit_code = main(
                    [
                        "--manifest",
                        str(self.manifest_file),
                    ]
                )

        self.assertEqual(exit_code, 1)
        self.assertIn(
            "not valid JSON",
            stderr.getvalue(),
        )
        runner.assert_not_called()

    def test_unsafe_wheel_filename_is_rejected(self) -> None:
        payload = self.payload()
        payload["wheel"]["filename"] = "../unsafe.whl"
        self.write_manifest(payload)

        with self.assertRaisesRegex(
            ValueError,
            "plain filename",
        ):
            resolve_wheel_path(
                self.manifest_file
            )

    def test_missing_wheel_is_rejected(self) -> None:
        self.write_manifest()
        self.wheel_file.unlink()

        with self.assertRaisesRegex(
            ValueError,
            "recorded wheel does not exist",
        ):
            resolve_wheel_path(
                self.manifest_file
            )

    def test_wheel_size_mismatch_is_rejected(self) -> None:
        payload = self.payload()
        payload["wheel"]["size_bytes"] += 1
        self.write_manifest(payload)

        with self.assertRaisesRegex(
            ValueError,
            "recorded wheel size",
        ):
            resolve_wheel_path(
                self.manifest_file
            )

    def test_wheel_sha256_mismatch_is_rejected(self) -> None:
        payload = self.payload()
        payload["wheel"]["sha256"] = "0" * 64
        self.write_manifest(payload)

        with self.assertRaisesRegex(
            ValueError,
            "SHA-256 does not match",
        ):
            resolve_wheel_path(
                self.manifest_file
            )

    def test_build_pip_command_preserves_option_order(self) -> None:
        command = build_pip_command(
            self.wheel_file.resolve(),
            (
                "--user",
                "--no-deps",
            ),
        )

        self.assertEqual(
            command,
            (
                sys.executable,
                "-m",
                "pip",
                "install",
                "--force-reinstall",
                "--user",
                "--no-deps",
                str(self.wheel_file.resolve()),
            ),
        )


    def test_main_forwards_repeated_pip_install_options(self) -> None:
        self.write_manifest()
        completed = subprocess.CompletedProcess(
            args=(),
            returncode=0,
        )

        with mock.patch(
            "build_variants.install_wheel.subprocess.run",
            return_value=completed,
        ) as runner:
            exit_code = main(
                [
                    "--manifest",
                    str(self.manifest_file),
                    "--pip-install-option=--user",
                    "--pip-install-option=--no-deps",
                ]
            )

        self.assertEqual(exit_code, 0)
        runner.assert_called_once_with(
            (
                sys.executable,
                "-m",
                "pip",
                "install",
                "--force-reinstall",
                "--user",
                "--no-deps",
                str(self.wheel_file.resolve()),
            ),
            check=False,
        )


if __name__ == "__main__":
    unittest.main()
