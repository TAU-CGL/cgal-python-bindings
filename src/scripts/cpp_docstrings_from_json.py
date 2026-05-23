#!/usr/bin/env python3
"""
cpp_docstrings_from_json.py  —  CGAL GSoC 2026: Python Bindings Enhancement
Converts JSON docstring data into two C++ files per module:
  - <module>_docstrings.hpp  (extern declarations, included in bindings)
  - <module>_docstrings.cpp  (constexpr definitions)

Usage:
    python3 cpp_docstrings_from_json.py
        --library <library_name>
        --module <module_name>
        --input-path <json_dir>
        --output-inc-path <include_output_dir>
        --output-lib-path <lib_output_dir>

Example:
    python3 cpp_docstrings_from_json.py \
        --library cgalpy \
        --module Arrangement_on_surface_2 \
        --input-path src/libs/cgalpy/lib/docstrings/json \
        --output-inc-path build/include/generated/cgalpy \
        --output-lib-path build/lib/generated
"""

import sys
import json
import re
import argparse
from pathlib import Path


def escape_cpp(s):
    return s.replace('\\', '\\\\').replace('"', '\\"').replace('\n', '\\n')


def make_identifier(name):
    operator_map = {
        "operator==": "operator_eq",
        "operator!=": "operator_ne",
        "operator<=": "operator_le",
        "operator>=": "operator_ge",
        "operator<": "operator_lt",
        "operator>": "operator_gt",
        "operator()": "operator_op",
        "operator[]": "operator",
        "operator+": "operator_plus",
        "operator-": "operator_minus",
        "operator*": "operator_mul",
        "operator/": "operator_div",
        "operator%": "operator_mod",
        "operator<<": "operator_op",
        "operator>>": "operator_op",
        "operator=": "operator_op",
    }
    for old, new in sorted(operator_map.items(), key=lambda kv: len(kv[0]), reverse=True):
        name = name.replace(old, new)
    s = re.sub(r'[^A-Za-z0-9_]', '_', name)
    s = re.sub(r'_+', '_', s)
    s = s.strip('_')
    return "operator_op" if s == "operator" else s


def strip_cgal_namespace(name):
    return name.replace("CGAL::", "")


def collect_entries(data):
    """Return list of (ident, docstring) pairs from JSON data."""
    entries = []
    seen = {}
    for compound_name, compound in data.items():
        kind = compound.get("kind", "unknown")
        
        # Handle namespaces: emit each member function as a top-level entry
        if kind == "namespace":
            for member in compound.get("members", []):
                brief = member.get("brief", "").strip()
                detailed = member.get("detailed", "").strip()

                qname = member.get("qualified_name") or                         f"{compound_name}__{member.get('name', 'unknown')}"
                qname = strip_cgal_namespace(qname)
                ident = make_identifier(qname)

                # Disambiguate overloads
                if ident in seen:
                    seen[ident] += 1
                    ident = f"{ident}_{seen[ident]}"
                else:
                    seen[ident] = 0

                doc_parts = []
                if brief:
                    doc_parts.append(brief)
                if detailed and detailed != brief:
                    doc_parts.append(detailed)
                
                # Synthetic docstring from signature if no docs available
                if not doc_parts:
                    func_name = member.get("name", "unknown")
                    params = member.get("params", [])
                    param_strs = []
                    for p in params:
                        ptype = p.get("type", "")
                        pname = p.get("name", "")
                        if ptype and pname:
                            param_strs.append(f"{ptype} {pname}")
                        elif pname:
                            param_strs.append(pname)
                    sig = f"{func_name}({', '.join(param_strs)})"
                    ret = member.get("return_type", "").strip()
                    if ret and ret not in ("void", ""):
                        sig += f" -> {ret}"
                    doc_parts.append(sig)
                else:
                    ret = member.get("return_type", "").strip()
                    if ret and ret not in ("void", ""):
                        doc_parts.append(f"Returns: {ret}")
                
                doc = escape_cpp(" ".join(doc_parts))
                entries.append((ident, doc))
            continue  # Don't process namespace as a class

        if compound.get("brief"):
            ident = make_identifier(strip_cgal_namespace(compound_name)) + "__class__"
            doc = escape_cpp(compound["brief"].strip())
            entries.append((ident, doc))

        for member in compound.get("members", []):
            brief = member.get("brief", "").strip()
            detailed = member.get("detailed", "").strip()
            if not brief and not detailed:
                continue

            qname = member.get("qualified_name") or \
                    f"{compound_name}__{member.get('name', 'unknown')}"
            qname = strip_cgal_namespace(qname)
            ident = make_identifier(qname)

            # Disambiguate overloads
            if ident in seen:
                seen[ident] += 1
                ident = f"{ident}_{seen[ident]}"
            else:
                seen[ident] = 0

            doc_parts = []
            if brief:
                doc_parts.append(brief)
            if detailed and detailed != brief:
                doc_parts.append(detailed)
            ret = member.get("return_type", "").strip()
            if ret and ret not in ("void", ""):
                doc_parts.append(f"Returns: {ret}")
            doc = escape_cpp(" ".join(doc_parts))
            entries.append((ident, doc))

    return entries


def generate_hpp(library, module, entries, out_path):
    guard = f"{library.upper()}_{make_identifier(module).upper()}_DOCSTRINGS_HPP"
    ns = make_identifier(module)
    lines = [
        f"// AUTO-GENERATED by cpp_docstrings_from_json.py — DO NOT EDIT",
        f"// Library: {library}  Module: {module}",
        f"#ifndef {guard}",
        f"#define {guard}",
        f"",
        f"namespace {library} {{",
        f"namespace docstrings {{",
        f"namespace {ns} {{",
        f"",
    ]
    for ident, _ in entries:
        lines.append(f"extern const char* const {ident};")
    lines += [
        f"",
        f"}} // namespace {ns}",
        f"}} // namespace docstrings",
        f"}} // namespace {library}",
        f"",
        f"#endif // {guard}",
        "",
    ]
    out_path.write_text("\n".join(lines), encoding="utf-8")


def generate_cpp(library, module, entries, out_path):
    hpp_name = f"{module}_docstrings.hpp"
    ns = make_identifier(module)
    lines = [
        f"// AUTO-GENERATED by cpp_docstrings_from_json.py — DO NOT EDIT",
        f"// Library: {library}  Module: {module}",
        f'#include "{hpp_name}"',
        f"",
        f"namespace {library} {{",
        f"namespace docstrings {{",
        f"namespace {ns} {{",
        f"",
    ]
    for ident, doc in entries:
        lines.append(f'const char* const {ident} = "{doc}";')
    lines += [
        f"",
        f"}} // namespace {ns}",
        f"}} // namespace docstrings",
        f"}} // namespace {library}",
        "",
    ]
    out_path.write_text("\n".join(lines), encoding="utf-8")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--library",         required=True)
    parser.add_argument("--module",          required=True)
    parser.add_argument("--input-path",      required=True)
    parser.add_argument("--output-inc-path", required=True)
    parser.add_argument("--output-lib-path", required=True)
    args = parser.parse_args()

    json_file = Path(args.input_path) / f"{args.module}.json"
    if not json_file.exists():
        print(f"ERROR: JSON not found: {json_file}", file=sys.stderr)
        sys.exit(1)

    data = json.loads(json_file.read_text(encoding="utf-8"))
    entries = collect_entries(data)

    inc_dir = Path(args.output_inc_path)
    lib_dir = Path(args.output_lib_path)
    inc_dir.mkdir(parents=True, exist_ok=True)
    lib_dir.mkdir(parents=True, exist_ok=True)

    hpp_path = inc_dir / f"{args.module}_docstrings.hpp"
    cpp_path = lib_dir / f"{args.module}_docstrings.cpp"

    generate_hpp(args.library, args.module, entries, hpp_path)
    generate_cpp(args.library, args.module, entries, cpp_path)

    print(f"Written: {hpp_path}", file=sys.stderr)
    print(f"Written: {cpp_path}", file=sys.stderr)
    print(f"  {len(entries)} docstring entries for module '{args.module}'",
          file=sys.stderr)


if __name__ == "__main__":
    main()
