#!/usr/bin/env python3
"""
Convert all CGAL Doxygen XML package directories under a doc_output tree
into JSON files.

Expected input layout:
  <doc-output>/<Package>/xml/index.xml

Example:
  python3 src/scripts/json_from_doc_output.py \
    --doc-output ~/build/cgal/doc_xml_aos2/doc_output \
    --output-dir src/libs/cgalpy/lib/docstrings/json
"""

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


def find_package_xml_dirs(doc_output: Path) -> list[tuple[str, Path]]:
    if not doc_output.is_dir():
        raise SystemExit(f"ERROR: doc_output directory not found: {doc_output}")

    packages: list[tuple[str, Path]] = []

    for package_dir in sorted(p for p in doc_output.iterdir() if p.is_dir()):
        xml_dir = package_dir / "xml"
        index_xml = xml_dir / "index.xml"
        if index_xml.is_file():
            packages.append((package_dir.name, xml_dir))

    return packages


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--doc-output", required=True, type=Path)
    parser.add_argument("--output-dir", required=True, type=Path)
    parser.add_argument(
        "--json-from-xml",
        type=Path,
        default=Path(__file__).with_name("json_from_xml.py"),
        help="Path to single-package JSON converter",
    )
    args = parser.parse_args()

    packages = find_package_xml_dirs(args.doc_output)
    if not packages:
        raise SystemExit(f"ERROR: no package XML dirs found under {args.doc_output}")

    args.output_dir.mkdir(parents=True, exist_ok=True)

    print(f"doc_output: {args.doc_output}", file=sys.stderr)
    print(f"output_dir: {args.output_dir}", file=sys.stderr)
    print(f"packages: {len(packages)}", file=sys.stderr)

    failures: list[tuple[str, int]] = []

    for package, xml_dir in packages:
        output = args.output_dir / f"{package}.json"
        cmd = [
            sys.executable,
            str(args.json_from_xml),
            "--package",
            package,
            "--xml-dir",
            str(xml_dir),
            "--output",
            str(output),
        ]

        print(f"== {package} ==", file=sys.stderr)
        result = subprocess.run(cmd)
        if result.returncode != 0:
            failures.append((package, result.returncode))

    if failures:
        print("FAILED packages:", file=sys.stderr)
        for package, code in failures:
            print(f"  {package}: exit {code}", file=sys.stderr)
        return 1

    print(f"converted packages: {len(packages)}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
