# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Kinetic_surface_reconstruction_3.h>
# #include <CGAL/Point_set_3.h>
# #include <CGAL/Point_set_3/IO.h>
# #include <CGAL/IO/polygon_soup_io.h>
#
# using Kernel    = CGAL::Exact_predicates_inexact_constructions_kernel;
# using FT        = typename Kernel::FT;
# using Point_3   = typename Kernel::Point_3;
# using Vector_3  = typename Kernel::Vector_3;
# using Segment_3 = typename Kernel::Segment_3;
#
# using Point_set    = CGAL::Point_set_3<Point_3>;
# using Point_map    = typename Point_set::Point_map;
# using Normal_map   = typename Point_set::Vector_map;
#
# using KSR = CGAL::Kinetic_surface_reconstruction_3<Kernel, Point_set, Point_map, Normal_map>;
#
# int main(const int, const char**) {
#   // Input.
#
#   Point_set point_set;
#   CGAL::IO::read_point_set(CGAL::data_file_path("points_3/building.ply"), point_set);
#
#   auto param = CGAL::parameters::maximum_distance(0.1)
#     .maximum_angle(10)
#     .minimum_region_size(100)
#     .reorient_bbox(true)
#     .regularize_parallelism(true)
#     .regularize_coplanarity(true)
#     .angle_tolerance(5)
#     .maximum_offset(0.02);
#
#   // Algorithm.
#   KSR ksr(point_set, param);
#
#   ksr.detection_and_partition(2, param);
#
#   std::vector<Point_3> vtx;
#   std::vector<std::vector<std::size_t> > polylist;
#
#   std::vector<FT> lambdas{0.3, 0.5, 0.7, 0.8, 0.9, 0.95, 0.99};
#
#   bool non_empty = false;
#
#   for (FT l : lambdas) {
#     vtx.clear();
#     polylist.clear();
#
#     ksr.reconstruct_with_ground(l, std::back_inserter(vtx), std::back_inserter(polylist));
#
#     if (polylist.size() > 0) {
#       non_empty = true;
#       CGAL::IO::write_polygon_soup("polylist_" + std::to_string(l) + ".ply", vtx, polylist);
#     }
#   }
#
#   return (non_empty) ? EXIT_SUCCESS : EXIT_FAILURE;
# }

import os
import sys
import importlib
lib = 'CGALPY'
i = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    i = 2
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY
Ker = CGALPY.Ker

point_set = CGALPY.Point_set_3()
CGALPY.read_point_set(CGALPY.data_file_path("points_3/building.ply"), point_set)

param = {"maximum_distance": 0.1,
         "maximum_angle": 10,
         "minimum_region_size": 100,
         "reorient_bbox": True,
         "regularize_parallelism": True,
         "regularize_coplanarity": True,
         "angle_tolerance": 5,
         "maximum_offset": 0.02}

# Algorithm.
ksr = CGALPY.Kinetic_surface_reconstruction_3(point_set, param)

ksr.detection_and_partition(2, param)

vtx = []
polylist = []

lambdas = [0.3, 0.5, 0.7, 0.8, 0.9, 0.95, 0.99]

non_empty = False

for l in lambdas:
    vtx.clear()
    polylist.clear()

    vtx, polylist = ksr.reconstruct_with_ground(l)

    if len(polylist) > 0:
        non_empty = True
        Ker.write_polygon_soup(f"polylist_{l}.ply", vtx, polylist)

exit(1 if non_empty else 0)

