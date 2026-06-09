#!/usr/bin/env python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import argparse
import os
import re
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
    data_relpaths: tuple[str, ...] = ()
    normalize_timing: bool = False


PAIRS = {
    "pol2_polygon": ExamplePair(
        name="pol2_polygon",
        python_relpath="Polygon/Polygon.py",
        cpp_relpath="Polygon/examples/Polygon/Polygon.cpp",
        python_workdir_relpath="Polygon",
        cpp_include_relpath="Polygon/examples/Polygon",
        executable="Polygon",
    ),
    "pol2_polygon_algorithms": ExamplePair(
        name="pol2_polygon_algorithms",
        python_relpath="Polygon/polygon_algorithms.py",
        cpp_relpath="Polygon/examples/Polygon/polygon_algorithms.cpp",
        python_workdir_relpath="Polygon",
        cpp_include_relpath="Polygon/examples/Polygon",
        executable="polygon_algorithms",
    ),
    "pol2_ranges": ExamplePair(
        name="pol2_ranges",
        python_relpath="Polygon/ranges.py",
        cpp_relpath="Polygon/examples/Polygon/ranges.cpp",
        python_workdir_relpath="Polygon",
        cpp_include_relpath="Polygon/examples/Polygon",
        executable="ranges",
    ),
    "pol2_example": ExamplePair(
        name="pol2_example",
        python_relpath="Polygon/Example.py",
        cpp_relpath="Polygon/examples/Polygon/Example.cpp",
        python_workdir_relpath="Polygon",
        cpp_include_relpath="Polygon/examples/Polygon",
        executable="Example",
    ),
    "ms2_sum_triangle_square": ExamplePair(
        name="ms2_sum_triangle_square",
        python_relpath="Minkowski_sum_2/sum_triangle_square.py",
        cpp_relpath="Minkowski_sum_2/examples/Minkowski_sum_2/sum_triangle_square.cpp",
        python_workdir_relpath="Minkowski_sum_2",
        cpp_include_relpath="Minkowski_sum_2/examples/Minkowski_sum_2",
        executable="sum_triangle_square",
    ),
    "ms2_sum_with_holes": ExamplePair(
        name="ms2_sum_with_holes",
        python_relpath="Minkowski_sum_2/sum_with_holes.py",
        cpp_relpath="Minkowski_sum_2/examples/Minkowski_sum_2/sum_with_holes.cpp",
        python_workdir_relpath="Minkowski_sum_2",
        cpp_include_relpath="Minkowski_sum_2/examples/Minkowski_sum_2",
        executable="sum_with_holes",
        data_relpaths=(
            "Minkowski_sum_2/examples/Minkowski_sum_2/rooms_star.dat",
        ),
    ),
    "ms2_sum_by_decomposition": ExamplePair(
        name="ms2_sum_by_decomposition",
        python_relpath="Minkowski_sum_2/sum_by_decomposition.py",
        cpp_relpath="Minkowski_sum_2/examples/Minkowski_sum_2/sum_by_decomposition.cpp",
        python_workdir_relpath="Minkowski_sum_2",
        cpp_include_relpath="Minkowski_sum_2/examples/Minkowski_sum_2",
        executable="sum_by_decomposition",
        data_relpaths=(
            "Minkowski_sum_2/examples/Minkowski_sum_2/rooms_star.dat",
        ),
    ),
    "ms2_sum_of_holes": ExamplePair(
        name="ms2_sum_of_holes",
        python_relpath="Minkowski_sum_2/sum_of_holes.py",
        cpp_relpath="Minkowski_sum_2/examples/Minkowski_sum_2/sum_of_holes.cpp",
        python_workdir_relpath="Minkowski_sum_2",
        cpp_include_relpath="Minkowski_sum_2/examples/Minkowski_sum_2",
        executable="sum_of_holes",
        data_relpaths=(
            "Minkowski_sum_2/examples/Minkowski_sum_2/holes.dat",
        ),
    ),
    "ms2_approx_offset": ExamplePair(
        name="ms2_approx_offset",
        python_relpath="Minkowski_sum_2/approx_offset.py",
        cpp_relpath="Minkowski_sum_2/examples/Minkowski_sum_2/approx_offset.cpp",
        python_workdir_relpath="Minkowski_sum_2",
        cpp_include_relpath="Minkowski_sum_2/examples/Minkowski_sum_2",
        executable="approx_offset",
        data_relpaths=(
            "Minkowski_sum_2/examples/Minkowski_sum_2/spiked.dat",
        ),
        normalize_timing=True,
    ),
    "ms2_approx_inset": ExamplePair(
        name="ms2_approx_inset",
        python_relpath="Minkowski_sum_2/approx_inset.py",
        cpp_relpath="Minkowski_sum_2/examples/Minkowski_sum_2/approx_inset.cpp",
        python_workdir_relpath="Minkowski_sum_2",
        cpp_include_relpath="Minkowski_sum_2/examples/Minkowski_sum_2",
        executable="approx_inset",
        data_relpaths=(
            "Minkowski_sum_2/examples/Minkowski_sum_2/tight.dat",
        ),
        normalize_timing=True,
    ),
    "ss2_plane_sweep": ExamplePair(
        name="ss2_plane_sweep",
        python_relpath="Surface_sweep_2/plane_sweep.py",
        cpp_relpath="Surface_sweep_2/examples/Surface_sweep_2/plane_sweep.cpp",
        python_workdir_relpath="Surface_sweep_2",
        cpp_include_relpath="Surface_sweep_2/examples/Surface_sweep_2",
        executable="plane_sweep",
    ),
    "env2_envelope_segments": ExamplePair(
        name="env2_envelope_segments",
        python_relpath="Envelope_2/envelope_segments.py",
        cpp_relpath="Envelope_2/examples/Envelope_2/envelope_segments.cpp",
        python_workdir_relpath="Envelope_2",
        cpp_include_relpath="Envelope_2/examples/Envelope_2",
        executable="envelope_segments",
    ),
    "env2_convex_hull_2": ExamplePair(
        name="env2_convex_hull_2",
        python_relpath="Envelope_2/convex_hull_2.py",
        cpp_relpath="Envelope_2/examples/Envelope_2/convex_hull_2.cpp",
        python_workdir_relpath="Envelope_2",
        cpp_include_relpath="Envelope_2/examples/Envelope_2",
        executable="convex_hull_2",
        data_relpaths=(
            "Envelope_2/examples/Envelope_2/ch_points.dat",
        ),
    ),
    "env3_envelope_spheres": ExamplePair(
        name="env3_envelope_spheres",
        python_relpath="Envelope_3/envelope_spheres.py",
        cpp_relpath="Envelope_3/examples/Envelope_3/envelope_spheres.cpp",
        python_workdir_relpath="Envelope_3",
        cpp_include_relpath="Envelope_3/examples/Envelope_3",
        executable="envelope_spheres",
        data_relpaths=(
            "Envelope_3/examples/Envelope_3/spheres.dat",
        ),
        normalize_timing=True,
    ),
    "env3_envelope_triangles": ExamplePair(
        name="env3_envelope_triangles",
        python_relpath="Envelope_3/envelope_triangles.py",
        cpp_relpath="Envelope_3/examples/Envelope_3/envelope_triangles.cpp",
        python_workdir_relpath="Envelope_3",
        cpp_include_relpath="Envelope_3/examples/Envelope_3",
        executable="envelope_triangles",
    ),
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
    "bso2_oriented_side": ExamplePair(
        name="bso2_oriented_side",
        python_relpath="Boolean_set_operations_2/oriented_side.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/oriented_side.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="oriented_side",
    ),
    "bso2_circle_segment": ExamplePair(
        name="bso2_circle_segment",
        python_relpath="Boolean_set_operations_2/circle_segment.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/circle_segment.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="circle_segment",
    ),
    "bso2_set_union": ExamplePair(
        name="bso2_set_union",
        python_relpath="Boolean_set_operations_2/set_union.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/set_union.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="set_union",
    ),
    "bso2_connect_polygon": ExamplePair(
        name="bso2_connect_polygon",
        python_relpath="Boolean_set_operations_2/connect_polygon.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/connect_polygon.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="connect_polygon",
        data_relpaths=(
            "Boolean_set_operations_2/examples/Boolean_set_operations_2/pgn_holes.dat",
        ),
    ),
    "bso2_conic_traits_adapter": ExamplePair(
        name="bso2_conic_traits_adapter",
        python_relpath="Boolean_set_operations_2/conic_traits_adapter.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/conic_traits_adapter.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="conic_traits_adapter",
    ),
    "bso2_bezier_traits_adapter": ExamplePair(
        name="bso2_bezier_traits_adapter",
        python_relpath="Boolean_set_operations_2/bezier_traits_adapter.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/bezier_traits_adapter.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="bezier_traits_adapter",
        data_relpaths=(
            "Boolean_set_operations_2/examples/Boolean_set_operations_2/char_g.dat",
            "Boolean_set_operations_2/examples/Boolean_set_operations_2/char_m.dat",
        ),
        normalize_timing=True,
    ),
    "bso2_bezier_traits_adapter2": ExamplePair(
        name="bso2_bezier_traits_adapter2",
        python_relpath="Boolean_set_operations_2/bezier_traits_adapter2.py",
        cpp_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2/bezier_traits_adapter2.cpp",
        python_workdir_relpath="Boolean_set_operations_2",
        cpp_include_relpath="Boolean_set_operations_2/examples/Boolean_set_operations_2",
        executable="bezier_traits_adapter2",
        data_relpaths=(
            "Boolean_set_operations_2/examples/Boolean_set_operations_2/char_g.bps",
            "Boolean_set_operations_2/examples/Boolean_set_operations_2/char_m.bps",
        ),
        normalize_timing=True,
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


def comparable_stdout(pair, text):
    if not pair.normalize_timing:
        return text
    text = re.sub(
        r"Constructed the input polygons in [0-9.]+ seconds\.",
        "Constructed the input polygons in <TIME> seconds.",
        text,
    )
    text = re.sub(
        r"The intersection computation took [0-9.]+ seconds\.",
        "The intersection computation took <TIME> seconds.",
        text,
    )
    text = re.sub(
        r"Offset computation took [0-9.]+ seconds\.",
        "Offset computation took <TIME> seconds.",
        text,
    )
    text = re.sub(
        r"Inset computation took [0-9.]+ seconds\.",
        "Inset computation took <TIME> seconds.",
        text,
    )
    text = re.sub(
        r"Construction took [0-9.]+ seconds\.",
        "Construction took <TIME> seconds.",
        text,
    )
    return text


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

    data_args = [str(args.cgal_source / relpath) for relpath in pair.data_relpaths]
    for data_arg in data_args:
        if not Path(data_arg).is_file():
            raise FileNotFoundError(f"Example data file not found: {data_arg}")

    for data_arg in data_args:
        shutil.copy2(data_arg, cpp_exe.parent / Path(data_arg).name)

    cpp_run = run_command([str(cpp_exe), *data_args], cwd=cpp_exe.parent)
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
        [str(args.python_executable), str(python_script.name), args.library, *data_args],
        cwd=python_workdir,
        env=env,
    )
    write_text(pair_dir / "python.stdout", py_run.stdout)
    write_text(pair_dir / "python.stderr", py_run.stderr)

    comparable_cpp_stdout = comparable_stdout(pair, cpp_run.stdout)
    comparable_py_stdout = comparable_stdout(pair, py_run.stdout)
    exact_match = comparable_cpp_stdout == comparable_py_stdout
    normalized_match = " ".join(comparable_cpp_stdout.split()) == " ".join(comparable_py_stdout.split())

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
