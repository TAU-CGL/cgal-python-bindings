#!/usr/bin/python
#
# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Polygon_mesh_processing/interpolated_corrected_curvatures.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <boost/graph/graph_traits.hpp>
#
# #include <iostream>
# #include <string>
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Epic_kernel;
# typedef CGAL::Surface_mesh<Epic_kernel::Point_3>            Mesh;
# typedef boost::graph_traits<Mesh>::vertex_descriptor        vertex_descriptor;
#
# int main(int argc, char* argv[])
# {
#   // instantiating and reading mesh
#   Mesh smesh;
#   const std::string filename = (argc > 1) ?
#     argv[1] :
#     CGAL::data_file_path("meshes/sphere.off");
#
#   if (!CGAL::IO::read_polygon_mesh(filename, smesh))
#   {
#     std::cerr << "Invalid input file." << std::endl;
#     return EXIT_FAILURE;
#   }
#
#   // loop over vertices and use vertex_descriptor to compute a curvature on one vertex
#   for (vertex_descriptor v : vertices(smesh))
#   {
#     double h, g;
#     PMP::Principal_curvatures_and_directions<Epic_kernel> p;
#
#     PMP::interpolated_corrected_curvatures(v,
#       smesh,
#       CGAL::parameters::vertex_mean_curvature(std::ref(h))
#                        .vertex_Gaussian_curvature(std::ref(g))
#                        .vertex_principal_curvatures_and_directions(std::ref(p)));
#
#     // we can also specify a ball radius for expansion and a user defined vertex normals map using
#     // named parameters. Refer to interpolated_corrected_curvatures_SM.cpp to see example usage.
#
#
#     std::cout << v.idx() << ": HC = " << h
#       << ", GC = " << g << "\n"
#       << ", PC = [ " << p.min_curvature << " , " << p.max_curvature << " ]\n";
#   }
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

filename = "meshes/sphere.off" if len(sys.argv) < 2 else sys.argv[1]

mesh = Sm.read_polygon_mesh(filename)

for v in Sm.vertices(mesh):
  # h, g = 0.0, 0.0
  p = Pmp.Principal_curvatures_and_directions()
  h, g = Pmp.interpolated_corrected_curvatures(v, mesh,
                                               {'vertex_mean_curvature': True,
                                                'vertex_Gaussian_curvature': True,
                                                'vertex_principal_curvatures_and_directions': p})

  print(f"{v.id()}: HC = {h}, GC = {g}, PC = [ {p.min_curvature} , {p.max_curvature} ]")
