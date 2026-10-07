#!/usr/bin/env python3
#
# Author(s): Utkarsh Khajuria  <utkarshkhajuria55@gmail.com>

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from dataclasses import asdict, dataclass
from pathlib import Path


TIMING_VALUE_PATTERN = (
    r"[-+]?(?:\d+(?:\.\d*)?|\.\d+)"
    r"(?:[eE][-+]?\d+)?"
)


@dataclass(frozen=True)
class ExamplePair:
    name: str
    python_relpath: str
    cpp_relpath: str
    python_workdir_relpath: str
    cpp_include_relpath: str
    executable: str
    data_relpaths: tuple[str, ...] = ()
    cpp_extra_include_relpaths: tuple[str, ...] = ()
    cpp_source_in_repo: bool = False
    cpp_needs_qt: bool = False
    normalize_timing: bool = False
    normalize_gog_cube_d: bool = False
    comparison_kind: str = "stdout"
    output_filename: str = ""
    raster_abs_tolerance: int = 0
    raster_max_bad_fraction: float = 0.0


@dataclass(frozen=True)
class ComparisonResult:
    kind: str
    passed: bool
    exact_match: bool
    metrics: dict[str, object]
    reason: str = ""


@dataclass(frozen=True)
class RasterImage:
    width: int
    height: int
    channels: int
    pixels: bytes

    def __post_init__(self):
        if self.width <= 0 or self.height <= 0:
            raise ValueError("Raster dimensions must be positive")
        if self.channels not in (1, 3, 4):
            raise ValueError("Raster channels must be 1, 3, or 4")
        expected = self.width * self.height * self.channels
        if len(self.pixels) != expected:
            raise ValueError(
                f"Raster byte count mismatch: expected {expected}, got {len(self.pixels)}"
            )


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
    "aos2_aggregated_insertion": ExamplePair(
        name="aos2_aggregated_insertion",
        python_relpath="Arrangement_on_surface_2/aggregated_insertion.py",
        cpp_relpath="Arrangement_on_surface_2/examples/Arrangement_on_surface_2/aggregated_insertion.cpp",
        python_workdir_relpath="Arrangement_on_surface_2",
        cpp_include_relpath="Arrangement_on_surface_2/examples/Arrangement_on_surface_2",
        executable="aggregated_insertion",
    ),
    "aos2_visual_buffer_summary": ExamplePair(
        name="aos2_visual_buffer_summary",
        python_relpath="Arrangement_on_surface_2/aos2_visual_buffer_summary.py",
        cpp_relpath="src/python_examples/cgalpy_examples/Arrangement_on_surface_2/aos2_visual_buffer_summary.cpp",
        python_workdir_relpath="Arrangement_on_surface_2",
        cpp_include_relpath="Arrangement_on_surface_2/examples/Arrangement_on_surface_2",
        executable="aos2_visual_buffer_summary",
        cpp_source_in_repo=True,
        cpp_needs_qt=True,
    ),
    "aos2_visual_buffer_raster": ExamplePair(
        name="aos2_visual_buffer_raster",
        python_relpath="Arrangement_on_surface_2/aos2_visual_buffer_raster.py",
        cpp_relpath="src/python_examples/cgalpy_examples/Arrangement_on_surface_2/aos2_visual_buffer_raster.cpp",
        python_workdir_relpath="Arrangement_on_surface_2",
        cpp_include_relpath="Arrangement_on_surface_2/examples/Arrangement_on_surface_2",
        executable="aos2_visual_buffer_raster",
        cpp_source_in_repo=True,
        cpp_needs_qt=True,
        comparison_kind="raster",
        output_filename="visual.ppm",
        raster_abs_tolerance=0,
        raster_max_bad_fraction=0.0,
    ),
    "pol3_visual_buffer_raster": ExamplePair(
        name="pol3_visual_buffer_raster",
        python_relpath="Polyhedron_3/pol3_visual_buffer_raster.py",
        cpp_relpath="src/python_examples/cgalpy_examples/Polyhedron_3/pol3_visual_buffer_raster.cpp",
        python_workdir_relpath="Polyhedron_3",
        cpp_include_relpath=".",
        executable="pol3_visual_buffer_raster",
        cpp_source_in_repo=True,
        cpp_needs_qt=True,
        comparison_kind="raster",
        output_filename="visual.ppm",
        raster_abs_tolerance=0,
        raster_max_bad_fraction=0.0,
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
    "vis2_general_polygon_example": ExamplePair(
        name="vis2_general_polygon_example",
        python_relpath="Visibility_2/general_polygon_example.py",
        cpp_relpath="Visibility_2/examples/Visibility_2/general_polygon_example.cpp",
        python_workdir_relpath="Visibility_2",
        cpp_include_relpath="Visibility_2/examples/Visibility_2",
        executable="general_polygon_example",
    ),
    "ker_exact": ExamplePair(
        name="ker_exact",
        python_relpath="Kernel_23/exact.py",
        cpp_relpath="Kernel_23/examples/Kernel_23/exact.cpp",
        python_workdir_relpath="Kernel_23",
        cpp_include_relpath="Kernel_23/examples/Kernel_23",
        executable="exact",
    ),
    "ker_points_and_segment": ExamplePair(
        name="ker_points_and_segment",
        python_relpath="Kernel_23/points_and_segment.py",
        cpp_relpath="Kernel_23/examples/Kernel_23/points_and_segment.cpp",
        python_workdir_relpath="Kernel_23",
        cpp_include_relpath="Kernel_23/examples/Kernel_23",
        executable="points_and_segment",
    ),
    "ker_intersection_get": ExamplePair(
        name="ker_intersection_get",
        python_relpath="Kernel_23/intersection_get.py",
        cpp_relpath="Kernel_23/examples/Kernel_23/intersection_get.cpp",
        python_workdir_relpath="Kernel_23",
        cpp_include_relpath="Kernel_23/examples/Kernel_23",
        executable="intersection_get",
    ),
    "ker_intersection_visitor": ExamplePair(
        name="ker_intersection_visitor",
        python_relpath="Kernel_23/intersection_visitor.py",
        cpp_relpath="Kernel_23/examples/Kernel_23/intersection_visitor.cpp",
        python_workdir_relpath="Kernel_23",
        cpp_include_relpath="Kernel_23/examples/Kernel_23",
        executable="intersection_visitor",
    ),
    "sm_iterators": ExamplePair(
        name="sm_iterators",
        python_relpath="Surface_mesh/sm_iterators.py",
        cpp_relpath="Surface_mesh/examples/Surface_mesh/sm_iterators.cpp",
        python_workdir_relpath="Surface_mesh",
        cpp_include_relpath="Surface_mesh/examples/Surface_mesh",
        executable="sm_iterators",
    ),
    "sm_circulators": ExamplePair(
        name="sm_circulators",
        python_relpath="Surface_mesh/sm_circulators.py",
        cpp_relpath="Surface_mesh/examples/Surface_mesh/sm_circulators.cpp",
        python_workdir_relpath="Surface_mesh",
        cpp_include_relpath="Surface_mesh/examples/Surface_mesh",
        executable="sm_circulators",
    ),
    "pol3_polyhedron_prog_tetra": ExamplePair(
        name="pol3_polyhedron_prog_tetra",
        python_relpath="Polyhedron_3/polyhedron_prog_tetra.py",
        cpp_relpath="Polyhedron/examples/Polyhedron/polyhedron_prog_tetra.cpp",
        python_workdir_relpath="Polyhedron_3",
        cpp_include_relpath="Polyhedron/examples/Polyhedron",
        executable="polyhedron_prog_tetra",
    ),
    "pol3_polyhedron_prog_vector": ExamplePair(
        name="pol3_polyhedron_prog_vector",
        python_relpath="Polyhedron_3/polyhedron_prog_vector.py",
        cpp_relpath="Polyhedron/examples/Polyhedron/polyhedron_prog_vector.cpp",
        python_workdir_relpath="Polyhedron_3",
        cpp_include_relpath="Polyhedron/examples/Polyhedron",
        executable="polyhedron_prog_vector",
    ),
    "pmp_sample_example": ExamplePair(
        name="pmp_sample_example",
        python_relpath="Polygon_mesh_processing/sample_example.py",
        cpp_relpath="Polygon_mesh_processing/examples/Polygon_mesh_processing/sample_example.cpp",
        python_workdir_relpath="Polygon_mesh_processing",
        cpp_include_relpath="Polygon_mesh_processing/examples/Polygon_mesh_processing",
        executable="sample_example",
        data_relpaths=(
            "Data/data/meshes/eight.off",
        ),
    ),
    "tri2_for_loop_2": ExamplePair(
        name="tri2_for_loop_2",
        python_relpath="Triangulation_2/for_loop_2.py",
        cpp_relpath="Triangulation_2/examples/Triangulation_2/for_loop_2.cpp",
        python_workdir_relpath="Triangulation_2",
        cpp_include_relpath="Triangulation_2/examples/Triangulation_2",
        executable="for_loop_2",
    ),
    "tri3_for_loop": ExamplePair(
        name="tri3_for_loop",
        python_relpath="Triangulation_3/for_loop.py",
        cpp_relpath="Triangulation_3/examples/Triangulation_3/for_loop.cpp",
        python_workdir_relpath="Triangulation_3",
        cpp_include_relpath="Triangulation_3/examples/Triangulation_3",
        executable="for_loop",
    ),
    "trid_delaunay_triangulation": ExamplePair(
        name="trid_delaunay_triangulation",
        python_relpath="Triangulation_d/delaunay_triangulation.py",
        cpp_relpath="Triangulation/examples/Triangulation/delaunay_triangulation.cpp",
        python_workdir_relpath="Triangulation_d",
        cpp_include_relpath="Triangulation/examples/Triangulation",
        executable="delaunay_triangulation",
    ),
    "as3_alpha_shapes_exact_alpha": ExamplePair(
        name="as3_alpha_shapes_exact_alpha",
        python_relpath="Alpha_shape_3/as3_alpha_shapes_exact_alpha.py",
        cpp_relpath="Alpha_shapes_3/examples/Alpha_shapes_3/ex_alpha_shapes_exact_alpha.cpp",
        python_workdir_relpath="Alpha_shape_3",
        cpp_include_relpath="Alpha_shapes_3/examples/Alpha_shapes_3",
        executable="ex_alpha_shapes_exact_alpha",
    ),
    "bgl_normals": ExamplePair(
        name="bgl_normals",
        python_relpath="BGL/bgl_normals.py",
        cpp_relpath="BGL/examples/BGL_polyhedron_3/normals.cpp",
        python_workdir_relpath="BGL",
        cpp_include_relpath="BGL/examples/BGL_polyhedron_3",
        executable="normals",
        data_relpaths=(
            "Data/data/meshes/cube_poly.off",
        ),
    ),
    "ch2_vector_convex_hull_2": ExamplePair(
        name="ch2_vector_convex_hull_2",
        python_relpath="Convex_hull_2/vector_convex_hull_2.py",
        cpp_relpath="Convex_hull_2/examples/Convex_hull_2/vector_convex_hull_2.cpp",
        python_workdir_relpath="Convex_hull_2",
        cpp_include_relpath="Convex_hull_2/examples/Convex_hull_2",
        executable="vector_convex_hull_2",
    ),
    "ch2_array_convex_hull_2": ExamplePair(
        name="ch2_array_convex_hull_2",
        python_relpath="Convex_hull_2/array_convex_hull_2.py",
        cpp_relpath="Convex_hull_2/examples/Convex_hull_2/array_convex_hull_2.cpp",
        python_workdir_relpath="Convex_hull_2",
        cpp_include_relpath="Convex_hull_2/examples/Convex_hull_2",
        executable="array_convex_hull_2",
    ),
    "ch3_graph_hull_3": ExamplePair(
        name="ch3_graph_hull_3",
        python_relpath="Convex_hull_3/graph_hull_3.py",
        cpp_relpath="Convex_hull_3/examples/Convex_hull_3/graph_hull_3.cpp",
        python_workdir_relpath="Convex_hull_3",
        cpp_include_relpath="Convex_hull_3/examples/Convex_hull_3",
        executable="graph_hull_3",
        data_relpaths=(
            "Data/data/meshes/star.off",
        ),
    ),
    "smsk_simple_mcfskel_sm_example": ExamplePair(
        name="smsk_simple_mcfskel_sm_example",
        python_relpath="Surface_mesh_skeletonization/simple_mcfskel_sm_example.py",
        cpp_relpath="Surface_mesh_skeletonization/examples/Surface_mesh_skeletonization/simple_mcfskel_sm_example.cpp",
        python_workdir_relpath="Surface_mesh_skeletonization",
        cpp_include_relpath="Surface_mesh_skeletonization/examples/Surface_mesh_skeletonization",
        executable="simple_mcfskel_sm_example",
        data_relpaths=(
            "Data/data/meshes/elephant.off",
        ),
    ),
    "sn2_create_offset_polygons_2": ExamplePair(
        name="sn2_create_offset_polygons_2",
        python_relpath="Straight_skeleton_2/create_offset_polygons_2.py",
        cpp_relpath="Straight_skeleton_2/examples/Straight_skeleton_2/create_offset_polygons_2.cpp",
        python_workdir_relpath="Straight_skeleton_2",
        cpp_include_relpath="Straight_skeleton_2/examples/Straight_skeleton_2",
        executable="create_offset_polygons_2",
    ),
    "st_my_point": ExamplePair(
        name="st_my_point",
        python_relpath="Spatial_sorting/my_point.py",
        cpp_relpath="Spatial_sorting/examples/Spatial_sorting/myPoint.cpp",
        python_workdir_relpath="Spatial_sorting",
        cpp_include_relpath="Spatial_sorting/examples/Spatial_sorting",
        executable="myPoint",
    ),
    "ss_nearest_neighbor_searching": ExamplePair(
        name="ss_nearest_neighbor_searching",
        python_relpath="Spatial_searching/nearest_neighbor_searching.py",
        cpp_relpath="Spatial_searching/examples/Spatial_searching/nearest_neighbor_searching.cpp",
        python_workdir_relpath="Spatial_searching",
        cpp_include_relpath="Spatial_searching/examples/Spatial_searching",
        executable="nearest_neighbor_searching",
    ),
    "kerd_intersection_test": ExamplePair(
        name="kerd_intersection_test",
        python_relpath="Kernel_d/intersection_test.py",
        cpp_relpath="Kernel_d/test/Kernel_d/intersection-test.cpp",
        python_workdir_relpath="Kernel_d",
        cpp_include_relpath="Kernel_d/test/Kernel_d",
        cpp_extra_include_relpaths=("Kernel_d/test/Kernel_d/include",),
        executable="intersection_test",
    ),
    "gog_cube_d": ExamplePair(
        name="gog_cube_d",
        python_relpath="Geometric_object_generators/cube_d.py",
        cpp_relpath="Generator/examples/Generator/cube_d.cpp",
        python_workdir_relpath="Geometric_object_generators",
        cpp_include_relpath="Generator/examples/Generator",
        executable="cube_d",
        normalize_gog_cube_d=True,
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
    if pair.normalize_gog_cube_d:
        normalized_lines = []
        point_line_re = re.compile(
            r"^\s*5(?:\s+[-+]?(?:\d+(?:\.\d*)?|\.\d+)(?:[eE][-+]?\d+)?){5}\s*$"
        )
        for line in text.splitlines():
            if point_line_re.match(line):
                normalized_lines.append(" <RANDOM_POINT_D_5>")
            else:
                normalized_lines.append(line)
        text = "\n".join(normalized_lines)
        if text:
            text += "\n"

    if not pair.normalize_timing:
        return text
    text = re.sub(
        rf"Constructed the input polygons in {TIMING_VALUE_PATTERN} seconds\.",
        "Constructed the input polygons in <TIME> seconds.",
        text,
    )
    text = re.sub(
        rf"The intersection computation took {TIMING_VALUE_PATTERN} seconds\.",
        "The intersection computation took <TIME> seconds.",
        text,
    )
    text = re.sub(
        rf"Offset computation took {TIMING_VALUE_PATTERN} seconds\.",
        "Offset computation took <TIME> seconds.",
        text,
    )
    text = re.sub(
        rf"Inset computation took {TIMING_VALUE_PATTERN} seconds\.",
        "Inset computation took <TIME> seconds.",
        text,
    )
    text = re.sub(
        rf"Construction took {TIMING_VALUE_PATTERN} seconds\.",
        "Construction took <TIME> seconds.",
        text,
    )
    return text


def write_comparison_result(path, result):
    write_text(path, json.dumps(asdict(result), indent=2, sort_keys=True) + "\n")


def compare_stdout(pair, cpp_stdout, python_stdout):
    comparable_cpp = comparable_stdout(pair, cpp_stdout)
    comparable_python = comparable_stdout(pair, python_stdout)
    exact_match = comparable_cpp == comparable_python
    normalized_match = (
        " ".join(comparable_cpp.split())
        == " ".join(comparable_python.split())
    )
    return ComparisonResult(
        kind="stdout",
        passed=exact_match,
        exact_match=exact_match,
        metrics={
            "normalized_match": normalized_match,
        },
    )


def _normalize_transparent_rgb(image):
    if image.channels != 4:
        return image

    pixels = bytearray(image.pixels)
    for offset in range(0, len(pixels), 4):
        if pixels[offset + 3] == 0:
            pixels[offset] = 0
            pixels[offset + 1] = 0
            pixels[offset + 2] = 0

    return RasterImage(
        width=image.width,
        height=image.height,
        channels=image.channels,
        pixels=bytes(pixels),
    )


def compare_raster_images(
    cpp_image,
    python_image,
    *,
    abs_tolerance=0,
    max_bad_fraction=0.0,
    normalize_transparent_rgb=True,
):
    if abs_tolerance < 0 or abs_tolerance > 255:
        raise ValueError("abs_tolerance must be in [0, 255]")
    if max_bad_fraction < 0.0 or max_bad_fraction > 1.0:
        raise ValueError("max_bad_fraction must be in [0, 1]")

    cpp_shape = (
        cpp_image.width,
        cpp_image.height,
        cpp_image.channels,
    )
    python_shape = (
        python_image.width,
        python_image.height,
        python_image.channels,
    )

    if cpp_shape != python_shape:
        return ComparisonResult(
            kind="raster",
            passed=False,
            exact_match=False,
            metrics={
                "cpp_width": cpp_image.width,
                "cpp_height": cpp_image.height,
                "cpp_channels": cpp_image.channels,
                "python_width": python_image.width,
                "python_height": python_image.height,
                "python_channels": python_image.channels,
            },
            reason="raster shape mismatch",
        )

    if normalize_transparent_rgb:
        cpp_image = _normalize_transparent_rgb(cpp_image)
        python_image = _normalize_transparent_rgb(python_image)

    differences = [
        abs(left - right)
        for left, right in zip(cpp_image.pixels, python_image.pixels)
    ]
    compared_values = len(differences)
    compared_pixels = cpp_image.width * cpp_image.height
    exact_differences = sum(value != 0 for value in differences)
    bad_values = sum(value > abs_tolerance for value in differences)
    bad_pixels = sum(
        any(
            value > abs_tolerance
            for value in differences[offset:offset + cpp_image.channels]
        )
        for offset in range(0, compared_values, cpp_image.channels)
    )
    bad_fraction = bad_pixels / compared_pixels
    value_bad_fraction = bad_values / compared_values
    max_abs_error = max(differences, default=0)
    mean_abs_error = sum(differences) / compared_values

    return ComparisonResult(
        kind="raster",
        passed=bad_fraction <= max_bad_fraction,
        exact_match=exact_differences == 0,
        metrics={
            "width": cpp_image.width,
            "height": cpp_image.height,
            "channels": cpp_image.channels,
            "compared_values": compared_values,
            "compared_pixels": compared_pixels,
            "exact_differences": exact_differences,
            "values_over_tolerance": bad_values,
            "pixels_over_tolerance": bad_pixels,
            "value_bad_fraction": value_bad_fraction,
            "bad_fraction": bad_fraction,
            "max_abs_error": max_abs_error,
            "mean_abs_error": mean_abs_error,
            "abs_tolerance": abs_tolerance,
            "max_bad_fraction": max_bad_fraction,
            "normalize_transparent_rgb": normalize_transparent_rgb,
        },
    )


def _raster_rgb_bytes(image):
    if image.channels == 3:
        return image.pixels

    out = bytearray()
    if image.channels == 1:
        for value in image.pixels:
            out.extend((value, value, value))
        return bytes(out)

    for offset in range(0, len(image.pixels), 4):
        red, green, blue, alpha = image.pixels[offset:offset + 4]
        inverse_alpha = 255 - alpha
        out.extend(
            (
                (red * alpha + 255 * inverse_alpha + 127) // 255,
                (green * alpha + 255 * inverse_alpha + 127) // 255,
                (blue * alpha + 255 * inverse_alpha + 127) // 255,
            )
        )
    return bytes(out)


def write_ppm(path, image):
    rgb = _raster_rgb_bytes(image)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("wb") as output:
        output.write(
            f"P6\n{image.width} {image.height}\n255\n".encode("ascii")
        )
        output.write(rgb)


def read_ppm(path):
    data = path.read_bytes()
    parts = data.split(b"\n", 3)
    if len(parts) != 4:
        raise ValueError(f"Invalid PPM header: {path}")
    magic, dimensions, max_value, pixels = parts
    if magic != b"P6":
        raise ValueError(f"Expected P6 PPM: {path}")
    dimension_tokens = dimensions.split()
    if len(dimension_tokens) != 2:
        raise ValueError(f"Invalid PPM dimensions: {path}")
    try:
        width = int(dimension_tokens[0])
        height = int(dimension_tokens[1])
        maximum = int(max_value)
    except ValueError as exc:
        raise ValueError(f"Invalid PPM numeric header: {path}") from exc
    if maximum != 255:
        raise ValueError(f"Only 8-bit PPM is supported: {path}")
    return RasterImage(
        width=width,
        height=height,
        channels=3,
        pixels=pixels,
    )


def _difference_heatmap(cpp_image, python_image):
    pixels = bytearray()
    channels = cpp_image.channels

    for offset in range(0, len(cpp_image.pixels), channels):
        channel_differences = [
            abs(left - right)
            for left, right in zip(
                cpp_image.pixels[offset:offset + channels],
                python_image.pixels[offset:offset + channels],
            )
        ]

        if channels == 1:
            value = channel_differences[0]
            pixels.extend((value, value, value))
        elif channels == 3:
            pixels.extend(channel_differences)
        else:
            alpha_difference = channel_differences[3]
            pixels.extend(
                max(value, alpha_difference)
                for value in channel_differences[:3]
            )

    return RasterImage(
        width=cpp_image.width,
        height=cpp_image.height,
        channels=3,
        pixels=bytes(pixels),
    )


def _side_by_side(cpp_image, python_image):
    cpp_rgb = _raster_rgb_bytes(cpp_image)
    python_rgb = _raster_rgb_bytes(python_image)
    row_bytes = cpp_image.width * 3
    pixels = bytearray()

    for row in range(cpp_image.height):
        begin = row * row_bytes
        end = begin + row_bytes
        pixels.extend(cpp_rgb[begin:end])
        pixels.extend(python_rgb[begin:end])

    return RasterImage(
        width=cpp_image.width * 2,
        height=cpp_image.height,
        channels=3,
        pixels=bytes(pixels),
    )


def write_raster_diagnostics(
    output_dir,
    result,
    cpp_image,
    python_image,
):
    output_dir.mkdir(parents=True, exist_ok=True)
    write_comparison_result(
        output_dir / "comparison.json",
        result,
    )
    write_ppm(output_dir / "cpp.ppm", cpp_image)
    write_ppm(output_dir / "python.ppm", python_image)

    cpp_shape = (
        cpp_image.width,
        cpp_image.height,
        cpp_image.channels,
    )
    python_shape = (
        python_image.width,
        python_image.height,
        python_image.channels,
    )

    if cpp_shape == python_shape:
        normalize_transparent_rgb = bool(
            result.metrics.get("normalize_transparent_rgb", False)
        )
        if normalize_transparent_rgb:
            comparison_cpp = _normalize_transparent_rgb(cpp_image)
            comparison_python = _normalize_transparent_rgb(python_image)
        else:
            comparison_cpp = cpp_image
            comparison_python = python_image

        write_ppm(
            output_dir / "diff_heatmap.ppm",
            _difference_heatmap(
                comparison_cpp,
                comparison_python,
            ),
        )
        write_ppm(
            output_dir / "side_by_side.ppm",
            _side_by_side(
                comparison_cpp,
                comparison_python,
            ),
        )


def build_cpp(pair, *, cgal_source, cgal_dir, work_dir, build_type, osx_architectures, cmake_prefix_path):
    repo_root = Path(__file__).resolve().parents[3]
    cpp_source_root = repo_root if pair.cpp_source_in_repo else cgal_source
    cpp_source = cpp_source_root / pair.cpp_relpath
    cpp_include = cgal_source / pair.cpp_include_relpath
    cpp_extra_includes = [cgal_source / relpath for relpath in pair.cpp_extra_include_relpaths]

    if not cpp_source.is_file():
        raise FileNotFoundError(f"C++ source not found: {cpp_source}")
    for include_dir in cpp_extra_includes:
        if not include_dir.is_dir():
            raise FileNotFoundError(f"C++ extra include directory not found: {include_dir}")

    cmake_source_dir = work_dir / pair.name / "cpp_source"
    cmake_build_dir = work_dir / pair.name / "cpp_build"
    shutil.rmtree(cmake_source_dir, ignore_errors=True)
    shutil.rmtree(cmake_build_dir, ignore_errors=True)
    cmake_source_dir.mkdir(parents=True)

    extra_include_lines = "".join(f'  "{include_dir}"\n' for include_dir in cpp_extra_includes)
    qt_find_lines = ""
    qt_link_lines = ""
    if pair.cpp_needs_qt:
        qt_find_lines = """find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGL OpenGLWidgets Svg)
include(CGAL_SetupCGAL_Qt6Dependencies)
"""
        qt_link_lines = f"""
target_compile_definitions({pair.executable} PRIVATE CGAL_USE_BASIC_VIEWER)
target_link_libraries({pair.executable} PRIVATE
  CGAL::Qt6_moc_and_resources
  Qt6::Widgets
  Qt6::OpenGL
  Qt6::OpenGLWidgets
  Qt6::Svg
)
"""

    cmake_lists = f"""cmake_minimum_required(VERSION 3.12)
project({pair.name}_compare)

find_package(CGAL REQUIRED)
find_package(Eigen3 QUIET)
include(CGAL_Eigen3_support)
{qt_find_lines}
add_executable({pair.executable}
  "{cpp_source}"
)
target_include_directories({pair.executable} PRIVATE
  "{cpp_include}"
{extra_include_lines})
target_link_libraries({pair.executable} PRIVATE CGAL::CGAL)
if(TARGET CGAL::Eigen3_support)
  target_link_libraries({pair.executable} PRIVATE CGAL::Eigen3_support)
endif()
{qt_link_lines}
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

    if cmake_prefix_path:
        configure_cmd.append(f"-DCMAKE_PREFIX_PATH={cmake_prefix_path}")

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


def prepare_pair_dir(work_dir, pair_name):
    pair_dir = work_dir / pair_name
    shutil.rmtree(pair_dir, ignore_errors=True)
    pair_dir.mkdir(parents=True)
    return pair_dir


def run_pair(pair, args, examples_root):
    pair_dir = prepare_pair_dir(args.work_dir, pair.name)

    build_result, cpp_exe = build_cpp(
        pair,
        cgal_source=args.cgal_source,
        cgal_dir=args.cgal_dir,
        work_dir=args.work_dir,
        build_type=args.build_type,
        osx_architectures=args.osx_architectures,
        cmake_prefix_path=args.cmake_prefix_path,
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

    cpp_output_path = None
    python_output_path = None
    cpp_command = [str(cpp_exe), *data_args]
    if pair.comparison_kind == "raster":
        if not pair.output_filename:
            raise ValueError(
                f"Raster pair {pair.name} must define output_filename"
            )
        cpp_output_dir = pair_dir / "cpp_output"
        python_output_dir = pair_dir / "python_output"
        cpp_output_dir.mkdir(parents=True, exist_ok=True)
        python_output_dir.mkdir(parents=True, exist_ok=True)
        cpp_output_path = cpp_output_dir / pair.output_filename
        python_output_path = python_output_dir / pair.output_filename
        cpp_command.append(str(cpp_output_path))

    cpp_run = run_command(cpp_command, cwd=cpp_exe.parent)
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

    python_command = [
        str(args.python_executable),
        str(python_script.name),
        args.library,
        *data_args,
    ]
    if pair.comparison_kind == "raster":
        python_command.append(str(python_output_path))

    py_run = run_command(
        python_command,
        cwd=python_workdir,
        env=env,
    )
    write_text(pair_dir / "python.stdout", py_run.stdout)
    write_text(pair_dir / "python.stderr", py_run.stderr)

    normalized_match = None
    if pair.comparison_kind == "stdout":
        comparison = compare_stdout(
            pair,
            cpp_run.stdout,
            py_run.stdout,
        )
        normalized_match = bool(
            comparison.metrics["normalized_match"]
        )
    elif pair.comparison_kind == "raster":
        if cpp_run.returncode != 0 or py_run.returncode != 0:
            comparison = ComparisonResult(
                kind="raster",
                passed=False,
                exact_match=False,
                metrics={},
                reason="producer process failed",
            )
        elif not cpp_output_path.is_file() or not python_output_path.is_file():
            comparison = ComparisonResult(
                kind="raster",
                passed=False,
                exact_match=False,
                metrics={
                    "cpp_output_exists": cpp_output_path.is_file(),
                    "python_output_exists": python_output_path.is_file(),
                },
                reason="raster producer did not create its output",
            )
        else:
            cpp_image = read_ppm(cpp_output_path)
            python_image = read_ppm(python_output_path)
            comparison = compare_raster_images(
                cpp_image,
                python_image,
                abs_tolerance=pair.raster_abs_tolerance,
                max_bad_fraction=pair.raster_max_bad_fraction,
            )
            write_raster_diagnostics(
                pair_dir / "raster_diagnostics",
                comparison,
                cpp_image,
                python_image,
            )
    else:
        raise ValueError(
            f"Unsupported comparison kind for {pair.name}: "
            f"{pair.comparison_kind}"
        )

    write_comparison_result(
        pair_dir / "comparison.json",
        comparison,
    )

    print(f"PAIR: {pair.name}")
    print(f"CPP_RC: {cpp_run.returncode}")
    print(f"PYTHON_RC: {py_run.returncode}")
    print(f"EXACT_MATCH: {comparison.exact_match}")
    if normalized_match is not None:
        print(f"NORMALIZED_MATCH: {normalized_match}")
    print(f"COMPARISON_KIND: {comparison.kind}")
    print(f"COMPARISON_RESULT: {comparison.passed}")
    if comparison.kind == "raster":
        print(
            "RASTER_METRICS: "
            + json.dumps(comparison.metrics, sort_keys=True)
        )
        if comparison.reason:
            print(f"COMPARISON_REASON: {comparison.reason}")
    print(f"OUTPUT_DIR: {pair_dir}")

    if (
        cpp_run.returncode != 0
        or py_run.returncode != 0
        or not comparison.passed
    ):
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
    parser.add_argument("--cmake-prefix-path", default=os.environ.get("CMAKE_PREFIX_PATH", ""))
    args = parser.parse_args()

    repo_root = Path(__file__).resolve().parents[3]
    examples_root = repo_root / "src/python_examples/cgalpy_examples"

    ok = run_pair(PAIRS[args.pair], args, examples_root)
    return 0 if ok else 1


if __name__ == "__main__":
    raise SystemExit(main())
