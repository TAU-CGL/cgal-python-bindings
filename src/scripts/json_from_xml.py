#!/usr/bin/env python3
"""
json_from_xml.py  —  CGAL GSoC 2026: Python Bindings Enhancement
Converts Doxygen XML output (CGAL_GENERATE_XML=ON) into structured JSON
suitable for generating C++ nanobind docstring headers.

Usage:
    python3 json_from_xml.py <xml_dir> <output.json>

Example:
    python3 json_from_xml.py \
        ~/build/cgal_doc/doc_output/Arrangement_on_surface_2/xml \
        aos2_docs.json
"""

import sys
import json
import xml.etree.ElementTree as ET
from pathlib import Path


def text_of(node):
    """Recursively extract all text from an XML node, stripping excess whitespace."""
    if node is None:
        return ""
    parts = []
    if node.text:
        parts.append(node.text.strip())
    for child in node:
        t = text_of(child)
        if t:
            parts.append(t)
        if child.tail:
            parts.append(child.tail.strip())
    return " ".join(p for p in parts if p)


def parse_param(param_node):
    """Parse a <param> element into a dict."""
    return {
        "type": text_of(param_node.find("type")),
        "name": text_of(param_node.find("declname")) or text_of(param_node.find("defname")),
        "default": text_of(param_node.find("defval")),
        "brief": text_of(param_node.find("briefdescription")),
    }


def parse_memberdef(m):
    """Parse a single <memberdef> into a dict."""
    kind = m.get("kind", "")
    entry = {
        "kind": kind,
        "name": text_of(m.find("name")),
        "qualified_name": text_of(m.find("qualifiedname")),
        "brief": text_of(m.find("briefdescription")),
        "detailed": text_of(m.find("detaileddescription")),
        "return_type": text_of(m.find("type")),
        "argsstring": text_of(m.find("argsstring")),
        "params": [parse_param(p) for p in m.findall("param")],
    }
    loc = m.find("location")
    if loc is not None:
        entry["file"] = loc.get("file", "")
        entry["line"] = loc.get("line", "")
    return entry


def parse_compound(xml_file):
    """Parse one class/struct/namespace XML file into a compound dict."""
    try:
        tree = ET.parse(xml_file)
    except ET.ParseError as e:
        print(f"  [WARN] Could not parse {xml_file}: {e}", file=sys.stderr)
        return None

    root = tree.getroot()
    compounddef = root.find("compounddef")
    if compounddef is None:
        return None

    kind = compounddef.get("kind", "")
    # Only process classes, structs, namespaces
    if kind not in ("class", "struct", "namespace"):
        return None

    name = text_of(compounddef.find("compoundname"))
    brief = text_of(compounddef.find("briefdescription"))
    detailed = text_of(compounddef.find("detaileddescription"))

    members = []
    for sectiondef in compounddef.findall("sectiondef"):
        section_header = text_of(sectiondef.find("header"))
        for memberdef in sectiondef.findall("memberdef"):
            m = parse_memberdef(memberdef)
            m["section"] = section_header
            members.append(m)

    return {
        "kind": kind,
        "name": name,
        "brief": brief,
        "detailed": detailed,
        "members": members,
    }


def build_json(xml_dir):
    xml_dir = Path(xml_dir)
    if not xml_dir.exists():
        print(f"ERROR: XML directory not found: {xml_dir}", file=sys.stderr)
        sys.exit(1)

    # Use index.xml to get ordered list of compounds
    index_file = xml_dir / "index.xml"
    compound_files = []
    if index_file.exists():
        idx = ET.parse(index_file)
        for compound in idx.getroot().findall("compound"):
            kind = compound.get("kind", "")
            if kind in ("class", "struct", "namespace"):
                refid = compound.get("refid", "")
                f = xml_dir / f"{refid}.xml"
                if f.exists():
                    compound_files.append(f)
    else:
        # Fallback: glob all class/struct xml files
        compound_files = sorted(xml_dir.glob("class_*.xml")) + \
                         sorted(xml_dir.glob("struct_*.xml")) + \
                         sorted(xml_dir.glob("namespace_*.xml"))

    result = {}
    skipped = 0
    for f in compound_files:
        compound = parse_compound(f)
        if compound is None:
            skipped += 1
            continue
        name = compound["name"]
        result[name] = compound

    print(f"  Parsed {len(result)} compounds, skipped {skipped}", file=sys.stderr)
    return result


def main():
    if len(sys.argv) != 3:
        print(__doc__)
        sys.exit(1)

    xml_dir = sys.argv[1]
    out_file = sys.argv[2]

    print(f"Parsing XML from: {xml_dir}", file=sys.stderr)
    data = build_json(xml_dir)

    with open(out_file, "w", encoding="utf-8") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)

    print(f"Written: {out_file}  ({len(data)} compounds)", file=sys.stderr)


if __name__ == "__main__":
    main()
