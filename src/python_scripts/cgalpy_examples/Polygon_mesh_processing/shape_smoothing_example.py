# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <CGAL/Polygon_mesh_processing/smooth_shape.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <iostream>
# #include <set>
# #include <string>
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
# typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# int main(int argc, char* argv[])
# {
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/pig.off");
#
#   Mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#
#   const unsigned int nb_iterations = (argc > 2) ? std::atoi(argv[2]) : 10;
#   const double time = (argc > 3) ? std::atof(argv[3]) : 0.0001;
#
#   std::set<Mesh::Vertex_index> constrained_vertices;
#   for(Mesh::Vertex_index v : vertices(mesh))
#   {
#     if(is_border(v, mesh))
#       constrained_vertices.insert(v);
#   }
#
#   std::cout << "Constraining: " << constrained_vertices.size() << " border vertices" << std::endl;
#   CGAL::Boolean_property_map<std::set<Mesh::Vertex_index> > vcmap(constrained_vertices);
#
#   std::cout << "Smoothing shape... (" << nb_iterations << " iterations)" << std::endl;
#   PMP::smooth_shape(mesh, time, CGAL::parameters::number_of_iterations(nb_iterations)
#                                                  .vertex_is_constrained_map(vcmap));
#
#   CGAL::IO::write_polygon_mesh("mesh_shape_smoothed.off", mesh, CGAL::parameters::stream_precision(17));
#
#   std::cout << "Done!" << std::endl;
#   return EXIT_SUCCESS;
# }

import time
import os
import sys
import importlib
if len(sys.argv) < 5:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[4]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    exit(1)

vcmap, success = mesh.add_property_map_Vertex_bool("v:bool")
if not success:
    print("Failed to add property map.")
    exit(1)

nb_iterations = 1000 if len(sys.argv) < 3 else int(sys.argv[2])
time = 0.0001 if len(sys.argv) < 4 else float(sys.argv[3])

constrained_vertices = 0

for v in Sm.vertices(mesh):
    if Sm.is_border(v, mesh):
        vcmap[v] = True
        constrained_vertices += 1

print(f"Constraining: {constrained_vertices} border vertices")

print(f"Smoothing shape... ({nb_iterations} iterations)")
# Pmp.smooth_shape(mesh, time, {"number_of_iterations": nb_iterations, "vertex_is_constrained_map": vcmap})
Pmp.smooth_shape(mesh, time)

Sm.write_polygon_mesh("mesh_shape_smoothed_py.off", mesh, {"stream_precision": 17})

print("Done!")

