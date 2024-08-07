#!/usr/bin/python

# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <CGAL/Polygon_mesh_processing/corefinement.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <array>
# #include <iostream>
# #include <string>
# #include <tuple>
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
# typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
# namespace params = CGAL::parameters;
#
# int main(int argc, char* argv[])
# {
#   const std::string filename1 = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/blobby.off");
#   const std::string filename2 = (argc > 2) ? argv[2] : CGAL::data_file_path("meshes/eight.off");
#
#   Mesh mesh1, mesh2;
#   if(!PMP::IO::read_polygon_mesh(filename1, mesh1) || !PMP::IO::read_polygon_mesh(filename2, mesh2))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#
#   Mesh out_union, out_intersection;
#   std::array<std::optional<Mesh*>, 4> output;
#   output[PMP::Corefinement::UNION] = &out_union;
#   output[PMP::Corefinement::INTERSECTION] = &out_intersection;
#
#   // for the example, we explicit the named parameters, this is identical to
#   // PMP::corefine_and_compute_boolean_operations(mesh1, mesh2, output)
#   std::array<bool, 4> res =
#     PMP::corefine_and_compute_boolean_operations(
#       mesh1, mesh2,
#       output,
#       params::default_values(), // mesh1 named parameters
#       params::default_values(), // mesh2 named parameters
#       std::make_tuple(
#         params::vertex_point_map(get(boost::vertex_point, out_union)), // named parameters for out_union
#         params::vertex_point_map(get(boost::vertex_point, out_intersection)), // named parameters for out_intersection
#         params::default_values(), // named parameters for mesh1-mesh2 not used
#         params::default_values() )// named parameters for mesh2-mesh1 not used)
#     );
#
#   if (res[PMP::Corefinement::UNION])
#   {
#     std::cout << "Union was successfully computed\n";
#     CGAL::IO::write_polygon_mesh("union.off", out_union, CGAL::parameters::stream_precision(17));
#   }
#   else
#     std::cout << "Union could not be computed\n";
#
#   if (res[PMP::Corefinement::INTERSECTION])
#   {
#     std::cout << "Intersection was successfully computed\n";
#     CGAL::IO::write_polygon_mesh("intersection.off", out_intersection, CGAL::parameters::stream_precision(17));
#   }
#   else
#     std::cout << "Intersection could not be computed\n";
#
#   return 0;
# }
#

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

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename1 = sys.argv[i] if len(sys.argv) > i else 'meshes/blobby.off'
i += 1
filename2 = sys.argv[i] if len(sys.argv) > i else 'meshes/eight.off'

try:
  mesh1 = Sm.read_polygon_mesh(filename1)
  mesh2 = Sm.read_polygon_mesh(filename2)
except:
  raise ValueError("Invalid input.")

operations = Pmp.corefine_and_compute_boolean_operations(mesh1, mesh2,
                                                         {}, # mesh1 named parameters
                                                         {}, # mesh2 named parameters
                                                         ({}, {}, {},{}) # named parameters for out_union, out_intersection, tm1_minus_tm2, tm2_minus_tm1
                                                         )

union, intersection, tm1_minus_tm2, tm2_minus_tm1 = operations

if union is not None:
  # these values get set to None if the operation was not successful
  print("Union was successfully computed")
  Sm.write_polygon_mesh("union.off", union)
else:
  print("Union could not be computed")

if intersection is not None:
  print("Intersection was successfully computed")
  Sm.write_polygon_mesh("intersection.off", intersection)
else:
  print("Intersection could not be computed")
