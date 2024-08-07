#!/usr/bin/python

# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <CGAL/Polygon_mesh_processing/angle_and_area_smoothing.h>
# #include <CGAL/Polygon_mesh_processing/detect_features.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <iostream>
# #include <string>
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel     K;
# typedef CGAL::Surface_mesh<K::Point_3>                          Mesh;
#
# typedef boost::graph_traits<Mesh>::edge_descriptor              edge_descriptor;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# int main(int argc, char** argv)
# {
#   const std::string filename = argc > 1 ? argv[1] : CGAL::data_file_path(CGALPY.data_file_path("meshes/anchor_dense.off");))
#
#   Mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#
#!/usr/bin/python

#   // Constrain edges with a dihedral angle over 60°
#   typedef boost::property_map<Mesh, CGAL::edge_is_feature_t>::type EIFMap;
#   EIFMap eif = get(CGAL::edge_is_feature, mesh);
#   PMP::detect_sharp_edges(mesh, 60, eif);
#
#   int sharp_counter = 0;
#   for(edge_descriptor e : edges(mesh))
#     if(get(eif, e))
#       ++sharp_counter;
#
#   std::cout << sharp_counter << " sharp edges" << std::endl;
#
#   const unsigned int nb_iterations = (argc > 2) ? std::atoi(argv[2]) : 10;
#
#   std::cout << "Smoothing mesh... (" << nb_iterations << " iterations)" << std::endl;
#
#   // Smooth with both angle and area criteria + Delaunay flips
#   PMP::angle_and_area_smoothing(mesh, CGAL::parameters::number_of_iterations(nb_iterations)
#                                                        .use_safety_constraints(false) // authorize all moves
#                                                        .edge_is_constrained_map(eif));
#
#   CGAL::IO::write_polygon_mesh("mesh_smoothed.off", mesh, CGAL::parameters::stream_precision(17));
#
#   std::cout << "Done!" << std::endl;
#
#   return EXIT_SUCCESS;
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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/anchor_dense.off')
i += 1
nb_iterations = int(sys.argv[i]) if len(sys.argv) > i else 10

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

# Constrain edges with a dihedral angle over 60°
eif = mesh.add_property_map_edge_bool("e:is_feature")[0]
Pmp.detect_sharp_edges(mesh, 60, eif)

sharp_counter = 0
for e in mesh.edges():
  if Sm.get(eif, e):
    sharp_counter += 1

print(f"{sharp_counter} sharp edges")
print(f"Smoothing mesh... ({nb_iterations} iterations)")

# Smooth with both angle and area criteria + Delaunay flips
Pmp.angle_and_area_smoothing(mesh, {"number_of_iterations": nb_iterations,
                                    "use_safety_constraints": False,
                                    "edge_is_constrained_map": eif})

Sm.write_polygon_mesh("mesh_smoothed.off", mesh, {"stream_precision": 17})

print("Done!")
