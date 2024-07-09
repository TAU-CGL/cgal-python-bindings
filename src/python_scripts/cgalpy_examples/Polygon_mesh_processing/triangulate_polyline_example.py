# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Polygon_mesh_processing/triangulate_hole.h>
# #include <CGAL/utility.h>
# #include <vector>
# #include <iterator>
# #include <cassert>
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
# typedef Kernel::Point_3                                     Point;
# int main()
# {
#   std::vector<Point> polyline;
#   polyline.push_back(Point( 1.,0.,0.));
#   polyline.push_back(Point( 0.,1.,0.));
#   polyline.push_back(Point(-1.,0.,0.));
#   polyline.push_back(Point( 1.,1.,0.));
#   // repeating first point (i.e. polyline.push_back(Point(1.,0.,0.)) ) is optional
#   // any type, having Type(int, int, int) constructor available, can be used to hold output triangles
#   typedef CGAL::Triple<int, int, int> Triangle_int;
#   std::vector<Triangle_int> patch;
#   patch.reserve(polyline.size() -2); // there will be exactly n-2 triangles in the patch
#   CGAL::Polygon_mesh_processing::triangulate_hole_polyline(
#           polyline,
#           std::back_inserter(patch));
#   for(std::size_t i = 0; i < patch.size(); ++i)
#   {
#     std::cout << "Triangle " << i << ": "
#       << patch[i].first << " " << patch[i].second << " " << patch[i].third
#       << std::endl;
#   }
#   // note that no degenerate triangles are generated in the patch
#   std::vector<Point> polyline_collinear;
#   polyline_collinear.push_back(Point(1.,0.,0.));
#   polyline_collinear.push_back(Point(2.,0.,0.));
#   polyline_collinear.push_back(Point(3.,0.,0.));
#   polyline_collinear.push_back(Point(4.,0.,0.));
#   std::vector<Triangle_int> patch_will_be_empty;
#   CGAL::Polygon_mesh_processing::triangulate_hole_polyline(polyline_collinear,
#                                                            back_inserter(patch_will_be_empty));
#   assert(patch_will_be_empty.empty());
#   return 0;
# }

import os
import sys
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp

polyline = [Point_3(0, 0, 0), Point_3(1, 0, 0), Point_3(1, 1, 0), Point_3(0, 1, 0)]
# repeating the first point (i.e. polyline.append(Point_3(0, 0, 0)) ) is optional

# a Triangle is a tuple of three integers
patch = Pmp.triangulate_hole_polyline(polyline)

for i, triangle in enumerate(patch):
    print(f"Triangle {i}: {triangle[0]} {triangle[1]} {triangle[2]}")

# note that no degenerate triangles are generated in the patch
polyline_collinear = [Point_3(0, 0, 0), Point_3(1, 0, 0), Point_3(2, 0, 0), Point_3(3, 0, 0), Point_3(4, 0, 0)]

patch_will_be_empty = Pmp.triangulate_hole_polyline(polyline_collinear)
if patch_will_be_empty:
    raise ValueError("Patch should be empty")

