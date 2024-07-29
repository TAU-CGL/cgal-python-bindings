# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
# #include <CGAL/Polyhedron_3.h>
# #include <CGAL/property_map.h>
#
# #include <boost/graph/graph_traits.hpp>
#
# #include <iostream>
# #include <string>
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Epic_kernel;
# typedef CGAL::Polyhedron_3<Epic_kernel>                     Mesh;
# typedef boost::graph_traits<Mesh>::vertex_descriptor        vertex_descriptor;
#
# int main(int argc, char* argv[])
# {
#   Mesh polyhedron;
#   const std::string filename = (argc > 1) ?
#     argv[1] :
#     CGAL::data_file_path("meshes/sphere.off");
#
#   if (!CGAL::IO::read_polygon_mesh(filename, polyhedron))
#   {
#     std::cerr << "Invalid input file." << std::endl;
#     return EXIT_FAILURE;
#   }
#
#   // define property map to store curvature value and directions
#   boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<Epic_kernel::FT>>::type
#     mean_curvature_map = get(CGAL::dynamic_vertex_property_t<Epic_kernel::FT>(), polyhedron),
#     Gaussian_curvature_map = get(CGAL::dynamic_vertex_property_t<Epic_kernel::FT>(), polyhedron);
#
#   boost::property_map<Mesh, CGAL::dynamic_vertex_property_t<PMP::Principal_curvatures_and_directions<Epic_kernel>>>::type
#     principal_curvatures_and_directions_map =
#     get(CGAL::dynamic_vertex_property_t<PMP::Principal_curvatures_and_directions<Epic_kernel>>(), polyhedron);
#
#   PMP::interpolated_corrected_curvatures(polyhedron,
#     CGAL::parameters::vertex_mean_curvature_map(mean_curvature_map)
#                      .vertex_Gaussian_curvature_map(Gaussian_curvature_map)
#                      .vertex_principal_curvatures_and_directions_map(principal_curvatures_and_directions_map)
#                      .vertex_point_map(get(CGAL::vertex_point, polyhedron))
#   // uncomment to use an expansion ball radius of 0.5 to estimate the curvatures
#   //                 .ball_radius(0.5)
#   );
#
#   int i = 0;
#   for (vertex_descriptor v : vertices(polyhedron))
#   {
#     auto PC = get(principal_curvatures_and_directions_map, v);
#     std::cout << i << ": HC = " << get(mean_curvature_map, v)
#       << ", GC = " << get(Gaussian_curvature_map, v) << "\n"
#       << ", PC = [ " << PC.min_curvature << " , " << PC.max_curvature << " ]\n";
#     i++;
#   }
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
Pol3 = CGALPY.Pol3
Pmp = CGALPY.Pmp


filename = "meshes/sphere.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    polyhedron = Pol3.read_polygon_mesh(filename)
except:
    print("Invalid input file.")
    exit(1)

mean_curvature_map = Pol3.get(Pol3.dynamic_property_vertex_size_t(), polyhedron)
Gaussian_curvature_map = Pol3.get(Pol3.dynamic_property_vertex_FT(), polyhedron)
principal_curvatures_and_directions_map = Pol3.get(Pol3.dynamic_property_vertex_PC(), polyhedron)

Pmp.interpolated_corrected_curvatures(polyhedron,
                                      {"vertex_mean_curvature_map": mean_curvature_map,
                                      #  "vertex_Gaussian_curvature_map": Gaussian_curvature_map,
                                      #  "vertex_principal_curvatures_and_directions_map": principal_curvatures_and_directions_map,
                                       # "vertex_point_map": Pol3.get_vertex_point_map(polyhedron),
# uncomment to use an expansion ball radius of 0.5 to estimate the curvatures
                                       # "ball_radius": 0.5
                                       })

i = 0

for v in Pol3.vertices(polyhedron):
    PC = Pol3.get(principal_curvatures_and_directions_map, v)
    print(f"{i}: HC = {Pol3.get(mean_curvature_map, v)}, GC = {Pol3.get(Gaussian_curvature_map, v)}\n"
          f"PC = [ {PC.min_curvature} , {PC.max_curvature} ]")
    i += 1

