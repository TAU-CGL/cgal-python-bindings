# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Polyhedral_envelope.h>
# #include <CGAL/Polygon_mesh_processing/remesh.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <algorithm>
# #include <iostream>
# #include <fstream>
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# int main(int argc, char* argv[])
# {
#   typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
#   typedef Kernel::Point_3 Point_3;
#   typedef CGAL::Surface_mesh<Point_3> Mesh;
#
#   typedef CGAL::Polyhedral_envelope<Kernel> Envelope;
#
#   std::ifstream in((argc>1) ? argv[1] : CGAL::data_file_path("meshes/blobby.off"));
#   Mesh tmesh;
#   in >> tmesh;
#
#   // remesh the input using the longest edge size as target edge length
#   Mesh query = tmesh;
#   Mesh::Edge_iterator longest_edge_it =
#     std::max_element(edges(query).begin(), edges(query).end(),
#                      [&query](Mesh::Edge_index e1, Mesh::Edge_index e2)
#                      {
#                         return PMP::edge_length(halfedge(e1, query), query) <
#                                PMP::edge_length(halfedge(e2, query), query);
#                      });
#   PMP::isotropic_remeshing(faces(tmesh), PMP::edge_length(halfedge(*longest_edge_it, query), query), query);
#
#   // construct the polyhedral envelope
#   const double eps = (argc>2) ? std::stod(std::string(argv[2])) : 0.01;
#   Envelope envelope(tmesh, eps);
#
#   // check is the remeshed version is inside the polyhedral envelope of the input mesh
#   if ( envelope(query) )
#     std::cout << "Remeshing is inside the polyhedral envelope\n";
#   else
#     std::cout << "Remeshing is not inside the polyhedral envelope\n";
#
#   std::ofstream("remeshed.off") << query;
#
#   return 0;
# }


import os
import sys
import importlib

lib = 'CGALPY'
a = 1
if len(sys.argv) > 1:
  str = sys.argv[1]
  if str.startswith('CGALPY'):
    lib = str
    a = 2
if lib == 'CGALPY':
  sys.path.append(os.path.abspath('../precompiled'))

CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

tmesh = Sm.read_polygon_mesh("meshes/blobby.off")

query = tmesh
longest_edge = max(query.edges(), key=lambda e: Pmp.edge_length(query.halfedge(e), query))

target_edge_length = Pmp.edge_length(query.halfedge(longest_edge), query)
Pmp.isotropic_remeshing(list(query.faces()), Pmp.Uniform_sizing_field(target_edge_length, query), query)

# construct the polyhedral envelope
eps = float(sys.argv[a]) if len(sys.argv) > a else 0.01
a += 1
envelope = Pmp.Polyhedral_envelope(tmesh, eps)

if envelope.inside(query):
  print("Remeshing is inside the polyhedral envelope")
else:
  print("Remeshing is not inside the polyhedral envelope")

Sm.write_polygon_mesh("remeshed.off", query)

