# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
# #include <CGAL/Polygon_mesh_processing/remesh.h>
# #include <CGAL/Polygon_mesh_processing/border.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <boost/iterator/function_output_iterator.hpp>
#
# #include <iostream>
# #include <string>
# #include <vector>
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
# typedef CGAL::Surface_mesh<K::Point_3>                        Mesh;
#
# typedef boost::graph_traits<Mesh>::halfedge_descriptor        halfedge_descriptor;
# typedef boost::graph_traits<Mesh>::edge_descriptor            edge_descriptor;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# struct halfedge2edge
# {
#   halfedge2edge(const Mesh& m, std::vector<edge_descriptor>& edges)
#     : m_mesh(m), m_edges(edges)
#   {}
#   void operator()(const halfedge_descriptor& h) const
#   {
#     m_edges.push_back(edge(h, m_mesh));
#   }
#   const Mesh& m_mesh;
#   std::vector<edge_descriptor>& m_edges;
# };
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
#   double target_edge_length = (argc > 2) ? std::stod(std::string(argv[2])) : 0.04;
#   unsigned int nb_iter = (argc > 3) ? std::stoi(std::string(argv[3])) : 10;
#
#   std::cout << "Split border...";
#
#   std::vector<edge_descriptor> border;
#   PMP::border_halfedges(faces(mesh), mesh, boost::make_function_output_iterator(halfedge2edge(mesh, border)));
#   PMP::split_long_edges(border, target_edge_length, mesh);
#
#   std::cout << "done." << std::endl;
#   std::cout << "Start remeshing of " << filename
#     << " (" << num_faces(mesh) << " faces)..." << std::endl;
#
#   PMP::isotropic_remeshing(faces(mesh), target_edge_length, mesh,
#                            CGAL::parameters::number_of_iterations(nb_iter)
#                                             .protect_constraints(true)); //i.e. protect border, here
#
#   CGAL::IO::write_polygon_mesh("out.off", mesh, CGAL::parameters::stream_precision(17));
#
#   std::cout << "Remeshing done." << std::endl;
#
#   return 0;
# }


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
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

def halfedge2edge(mesh, edges):
  return [Sm.edge(h, mesh) for h in edges]


filename = "meshes/pig.off" if len(sys.argv) < 2 else sys.argv[1]

mesh = Sm.read_polygon_mesh(filename)

target_edge_length = 0.04 if len(sys.argv) < 3 else float(sys.argv[2])
nb_iter = 10 if len(sys.argv) < 4 else int(sys.argv[3])

print("Split border...", end="")

border = halfedge2edge(mesh, Pmp.border_halfedges(Sm.faces(mesh), mesh))
Pmp.split_long_edges(border, target_edge_length, mesh)

print("done.")
print(f"Start remeshing of {filename} ({Sm.num_faces(mesh)} faces)...")
Pmp.isotropic_remeshing(Sm.faces(mesh), target_edge_length, mesh,
                        {"number_of_iterations": nb_iter, "protect_constraints": True})

Sm.write_polygon_mesh("out.off", mesh, {"stream_precision": 17})

print("Remeshing done.")

