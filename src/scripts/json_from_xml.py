#!/usr/bin/env python3
"""
Convert CGAL Doxygen XML output into compact structured JSON.

Input:
  Doxygen XML directory generated with CGAL_GENERATE_XML=ON.

Output:
  One JSON file containing compounds, members, descriptions, params,
  return types, and source locations.

This script is intentionally independent from stale generated C++ files.
"""

from __future__ import annotations

import argparse
import json
import sys
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import Any


KEEP_COMPOUND_KINDS = {"class", "struct", "namespace", "group", "concept"}


def normalize_ws(text: str) -> str:
    return " ".join(text.split())


def text_of(node: ET.Element | None) -> str:
    """Extract readable text recursively from a Doxygen XML node."""
    if node is None:
        return ""

    parts: list[str] = []

    if node.text:
        parts.append(node.text)

    for child in list(node):
        child_text = text_of(child)
        if child_text:
            parts.append(child_text)
        if child.tail:
            parts.append(child.tail)

    return normalize_ws(" ".join(parts))


def parse_param(param_node: ET.Element) -> dict[str, str]:
    return {
        "type": text_of(param_node.find("type")),
        "name": text_of(param_node.find("declname")) or text_of(param_node.find("defname")),
        "default": text_of(param_node.find("defval")),
        "brief": text_of(param_node.find("briefdescription")),
        "detailed": text_of(param_node.find("detaileddescription")),
    }


def parse_memberdef(member_node: ET.Element, section_kind: str, section_header: str) -> dict[str, Any]:
    location = member_node.find("location")

    entry: dict[str, Any] = {
        "kind": member_node.get("kind", ""),
        "name": text_of(member_node.find("name")),
        "qualified_name": text_of(member_node.find("qualifiedname")),
        "brief": text_of(member_node.find("briefdescription")),
        "detailed": text_of(member_node.find("detaileddescription")),
        "return_type": text_of(member_node.find("type")),
        "definition": text_of(member_node.find("definition")),
        "argsstring": text_of(member_node.find("argsstring")),
        "params": [parse_param(p) for p in member_node.findall("param")],
        "section_kind": section_kind,
        "section": section_header,
    }

    if location is not None:
        entry["file"] = location.get("file", "")
        entry["line"] = location.get("line", "")

    return entry


def parse_compound(xml_file: Path) -> dict[str, Any] | None:
    try:
        root = ET.parse(xml_file).getroot()
    except ET.ParseError as exc:
        print(f"[WARN] Could not parse {xml_file}: {exc}", file=sys.stderr)
        return None

    compounddef = root.find("compounddef")
    if compounddef is None:
        return None

    kind = compounddef.get("kind", "")
    if kind not in KEEP_COMPOUND_KINDS:
        return None

    name = text_of(compounddef.find("compoundname"))
    if not name:
        return None

    location = compounddef.find("location")

    members: list[dict[str, Any]] = []
    for sectiondef in compounddef.findall("sectiondef"):
        section_kind = sectiondef.get("kind", "")
        section_header = text_of(sectiondef.find("header"))
        for memberdef in sectiondef.findall("memberdef"):
            members.append(parse_memberdef(memberdef, section_kind, section_header))

    compound: dict[str, Any] = {
        "kind": kind,
        "name": name,
        "brief": text_of(compounddef.find("briefdescription")),
        "detailed": text_of(compounddef.find("detaileddescription")),
        "members": members,
        "xml_file": xml_file.name,
    }

    if location is not None:
        compound["file"] = location.get("file", "")
        compound["line"] = location.get("line", "")

    return compound


def compound_files_from_index(xml_dir: Path) -> list[Path]:
    index_file = xml_dir / "index.xml"
    if not index_file.exists():
        return sorted(xml_dir.glob("*.xml"))

    root = ET.parse(index_file).getroot()
    files: list[Path] = []

    for compound in root.findall("compound"):
        kind = compound.get("kind", "")
        if kind not in KEEP_COMPOUND_KINDS:
            continue
        refid = compound.get("refid", "")
        if not refid:
            continue
        candidate = xml_dir / f"{refid}.xml"
        if candidate.exists():
            files.append(candidate)

    return files


def build_json(xml_dir: Path, package: str) -> dict[str, Any]:
    if not xml_dir.is_dir():
        raise SystemExit(f"ERROR: XML directory not found: {xml_dir}")

    compounds: dict[str, Any] = {}
    skipped = 0

    for xml_file in compound_files_from_index(xml_dir):
        compound = parse_compound(xml_file)
        if compound is None:
            skipped += 1
            continue

        name = compound["name"]
        if name in compounds:
            print(f"[WARN] Duplicate compound name ignored: {name}", file=sys.stderr)
            skipped += 1
            continue

        compounds[name] = compound

    return {
        "schema_version": 1,
        "package": package,
        "source": {
            "kind": "CGAL Doxygen XML",
            "generated_with": "CGAL_GENERATE_XML=ON",
        },
        "compound_count": len(compounds),
        "skipped_count": skipped,
        "compounds": compounds,
    }


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--package", required=True, help="CGAL package/module name")
    parser.add_argument("--xml-dir", required=True, type=Path, help="Doxygen XML directory")
    parser.add_argument("--output", required=True, type=Path, help="Output JSON file")
    args = parser.parse_args()

    data = build_json(args.xml_dir, args.package)

    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_text(json.dumps(data, indent=2, ensure_ascii=False), encoding="utf-8")

    print(f"package: {args.package}", file=sys.stderr)
    print(f"xml_dir: {args.xml_dir}", file=sys.stderr)
    print(f"output: {args.output}", file=sys.stderr)
    print(f"compounds: {data['compound_count']}", file=sys.stderr)
    print(f"skipped: {data['skipped_count']}", file=sys.stderr)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
