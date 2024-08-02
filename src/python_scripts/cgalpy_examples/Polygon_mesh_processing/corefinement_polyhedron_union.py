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
if len(sys.argv) < 4:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[3]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Pmp = CGALPY.Pmp
Pol3 = CGALPY.Pol3

filename1 = "meshes/blobby.off" if len(sys.argv) < 2 else sys.argv[1]
filename2 = "meshes/eight.off" if len(sys.argv) < 3 else sys.argv[2]

try:
    mesh1 = Pol3.read_polygon_mesh(filename1)
    mesh2 = Pol3.read_polygon_mesh(filename2)
except:
    print("Invalid input.")
    exit(1)

try:
    out = Pmp.corefine_and_compute_union(mesh1, mesh2)
except:
    print("Union could not be computed")
    exit(1)

print("Union was successfully computed")
Pol3.write_polygon_mesh("union.off", out, {"stream_precision": 17})

