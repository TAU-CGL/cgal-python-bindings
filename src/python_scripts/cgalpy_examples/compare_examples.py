#!/usr/bin/env python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import argparse
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass(frozen=True)
class ExamplePair:
    name: str
    python_relpath: str
    cpp_relpath: str
    python_workdir_relpath: str
    cpp_include_relpath: str
    executable: str


PAIRS = {
    "bso2_simple_join_intersect": ExamplePair(
        name="bso2_simple_join_intersect",
        python_relpath="Boolean_set_operations_2/simple_join_intersect.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/simple_join_intersect.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="simple_join_intersect",
    ),
    "bso2_symmetric_difference": ExamplePair(
        name="bso2_symmetric_difference",
        python_relpath="Boolean_set_operations_2/symmetric_difference.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/symmetric_difference.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="symmetric_difference",
    ),
    "bso2_sequence": ExamplePair(
        name="bso2_sequence",
        python_relpath="Boolean_set_operations_2/sequence.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/sequence.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="sequence",
    ),
    "bso2_do_intersect": ExamplePair(
        name="bso2_do_intersect",
        python_relpath="Boolean_set_operations_2/do_intersect.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/do_intersect.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="do_intersect",
    ),
}


def run_command(cmd, *, cwd, env=None):
    return subprocess.run(
        cmd,
        cwd=cwd,
        env=env,
        text=True,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )


def write_text(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


def build_cpp(pair, *, cgal_source, cgal_dir, work_dir, build_type, osx_architectures):
    cpp_source = cgal_source / pair.cpp_relpath
    cpp_include = cgal_source / pair.cpp_include_relpath

    if not cpp_source.is_file():
        raise FileNotFoundError(f"C++ source not found: {cpp_source}")

    cmake_source_dir = work_dir / pair.name / "cpp_source"
    cmake_build_dir = work_dir / pair.name / "cpp_build"
    shutil.rmtree(cmake_source_dir, ignore_errors=True)
    shutil.rmtree(cmake_build_dir, ignore_errors=True)
    cmake_source_dir.mkdir(parents=True)

    cmake_lists = f"""cmake_minimum_required(VERSION 3.12)
project({pair.name}_compare)

find_package(CGAL REQUIRED)

add_executable({pair.executable}
  "{cpp_source}"
)
target_include_directories({pair.executable} PRIVATE "{cpp_include}")
target_link_libraries({pair.executable} PRIVATE CGAL::CGAL)
"""
    write_text(cmake_source_dir / "CMakeLists.txt", cmake_lists)

    configure_cmd = [
        "cmake",
        "-S",
        str(cmake_source_dir),
        "-B",
        str(cmake_build_dir),
        f"-DCMAKE_BUILD_TYPE={build_type}",
        f"-DCGAL_DIR={cgal_dir}",
    ]

    if osx_architectures:
        configure_cmd.append(f"-DCMAKE_OSX_ARCHITECTURES={osx_architectures}")

    configure = run_command(configure_cmd, cwd=cmake_source_dir)
    write_text(work_dir / pair.name / "cpp_configure.stdout", configure.stdout)
    write_text(work_dir / pair.name / "cpp_configure.stderr", configure.stderr)
    if configure.returncode != 0:
        return configure, None

    build = run_command(["cmake", "--build", str(cmake_build_dir), "-j4"], cwd=cmake_source_dir)
    write_text(work_dir / pair.name / "cpp_build.stdout", build.stdout)
    write_text(work_dir / pair.name / "cpp_build.stderr", build.stderr)
    if build.returncode != 0:
        return build, None

    return build, cmake_build_dir / pair.executable


def run_pair(pair, args, examples_root):
    pair_dir = args.work_dir / pair.name
    pair_dir.mkdir(parents=True, exist_ok=True)

    build_result, cpp_exe = build_cpp(
        pair,
        cgal_source=args.cgal_source,
        cgal_dir=args.cgal_dir,
        work_dir=args.work_dir,
        build_type=args.build_type,
        osx_architectures=args.osx_architectures,
    )

    if cpp_exe is None:
        print(f"[FAIL] {pair.name}: C++ configure/build failed")
        print(build_result.stderr)
        return False

    cpp_run = run_command([str(cpp_exe)], cwd=cpp_exe.parent)
    write_text(pair_dir / "cpp.stdout", cpp_run.stdout)
    write_text(pair_dir / "cpp.stderr", cpp_run.stderr)

    python_script = examples_root / pair.python_relpath
    python_workdir = examples_root / pair.python_workdir_relpath
    if not python_script.is_file():
        raise FileNotFoundError(f"Python example not found: {python_script}")

    env = os.environ.copy()
    old_pythonpath = env.get("PYTHONPATH", "")
    env["PYTHONPATH"] = str(args.python_build / "src/libs/cgalpy")
    if old_pythonpath:
        env["PYTHONPATH"] += os.pathsep + old_pythonpath

    py_run = run_command(
        [str(args.python_executable), str(python_script.name), args.library],
        cwd=python_workdir,
        env=env,
    )
    write_text(pair_dir / "python.stdout", py_run.stdout)
    write_text(pair_dir / "python.stderr", py_run.stderr)

    exact_match = cpp_run.stdout == py_run.stdout
    normalized_match = " ".join(cpp_run.stdout.split()) == " ".join(py_run.stdout.split())

    print(f"PAIR: {pair.name}")
    print(f"CPP_RC: {cpp_run.returncode}")
    print(f"PYTHON_RC: {py_run.returncode}")
    print(f"EXACT_MATCH: {exact_match}")
    print(f"NORMALIZED_MATCH: {normalized_match}")
    print(f"OUTPUT_DIR: {pair_dir}")

    if cpp_run.returncode != 0 or py_run.returncode != 0 or not exact_match:
        print()
        print("C++ stdout:")
        print(cpp_run.stdout)
        print("Python stdout:")
        print(py_run.stdout)
        print("C++ stderr:")
        print(cpp_run.stderr)
        print("Python stderr:")
        print(py_run.stderr)
        return False

    return True


def main():
    parser = argparse.ArgumentParser(description="Compare curated CGAL C++ examples with CGALPY Python translations.")
    parser.add_argument("--pair", choices=sorted(PAIRS), default="bso2_simple_join_intersect")
    parser.add_argument("--cgal-source", type=Path, required=True)
    parser.add_argument("--cgal-dir", type=Path, required=True)
    parser.add_argument("--python-build", type=Path, required=True)
    parser.add_argument("--library", required=True)
    parser.add_argument("--python-executable", type=Path, default=Path(sys.executable))
    parser.add_argument("--work-dir", type=Path, default=Path.home() / "build/cgalpy/example_compare_curated")
    parser.add_argument("--build-type", default="Release")
    parser.add_argument("--osx-architectures", default="")
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parents[3]
    examples_root = repo_root / "src/python_scripts/cgalpy_examples"

    ok = run_pair(PAIRS[args.pair], args, examples_root)
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
