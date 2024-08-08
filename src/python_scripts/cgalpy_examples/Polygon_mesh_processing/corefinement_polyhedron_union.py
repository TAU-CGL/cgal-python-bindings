#!/usr/bin/python

# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Polyhedron_3.h>
# #include <CGAL/Polyhedron_items_with_id_3.h>
#
# #include <CGAL/Polygon_mesh_processing/corefinement.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <fstream>
# #include <iostream>
# #include <string>
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
# typedef CGAL::Polyhedron_3<K, CGAL::Polyhedron_items_with_id_3> Mesh;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# int main(int argc, char* argv[])
# {
#   const std::string filename1 = (argc > 1) ? argv[1] : CGAL::data_file_path(CGALPY.data_file_path("meshes/blobby.off");))
#   const std::string filename2 = (argc > 2) ? argv[2] : CGAL::data_file_path(CGALPY.data_file_path("meshes/eight.off");))
#
#   Mesh mesh1, mesh2;
#   if(!PMP::IO::read_polygon_mesh(filename1, mesh1) || !PMP::IO::read_polygon_mesh(filename2, mesh2))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#
#   Mesh out;
#   bool valid_union = PMP::corefine_and_compute_union(mesh1,mesh2, out);
#
#   if (valid_union)
#   {
#     std::cout << "Union was successfully computed\n";
#     std::ofstream output("union.off");
#     output.precision(17);
#     output << out;
#     return 0;
#   }
#   std::cout << "Union could not be computed\n";
#   return 1;
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

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Pmp = CGALPY.Pmp
Pol3 = CGALPY.Pol3

filename1 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/blobby.off")
i += 1
filename2 = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/eight.off")

try: mesh1 = Pol3.read_polygon_mesh(filename1)
except: raise ValueError("Invalid input 1.")

try: mesh2 = Pol3.read_polygon_mesh(filename2)
except: raise ValueError("Invalid input 2.")

try: out = Pmp.corefine_and_compute_union(mesh1, mesh2)
except: raise ValueError("Union could not be computed.")

print("Union was successfully computed")
Pol3.write_polygon_mesh("union.off", out, {"stream_precision": 17})
