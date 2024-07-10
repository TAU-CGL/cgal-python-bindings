# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
# #include <CGAL/Polygon_mesh_processing/tangential_relaxation.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#  
# #include <fstream>
#  
# typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
# typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
#  
#  
# namespace PMP = CGAL::Polygon_mesh_processing;
#  
#  
# int main(int argc, char* argv[])
# {
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/pig.off");
#  
#   Mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh) || !CGAL::is_triangle_mesh(mesh))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#  
#   unsigned int nb_iter = (argc > 2) ? std::stoi(std::string(argv[2])) : 10;
#  
#   std::cout << "Relax...";
#  
#   PMP::tangential_relaxation(mesh, CGAL::parameters::number_of_iterations(nb_iter));
#  
#   std::cout << "done." << std::endl;
#  
#   return 0;
# }

import os
import sys
import importlib
if len(sys.argv) < 3:
    sys.path.append(os.path.abspath('../precompiled'))
    lib = 'CGALPY'
else:
    lib = sys.argv[2]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    raise ValueError("Invalid input.")

if not Sm.is_triangle_mesh(mesh):
    raise ValueError("Invalid input.")

nb_iter = 10 if len(sys.argv) < 3 else int(sys.argv[2])

print("Relax...")

Pmp.tangential_relaxation(mesh, {'number_of_iterations': nb_iter})

print("done.")

