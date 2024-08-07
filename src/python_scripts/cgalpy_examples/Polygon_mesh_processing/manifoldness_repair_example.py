#!/usr/bin/python

# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <CGAL/Polygon_mesh_processing/repair.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <CGAL/boost/graph/iterator.h>
#
# #include <iostream>
# #include <iterator>
# #include <string>
# #include <vector>
#
# namespace PMP = CGAL::Polygon_mesh_processing;
# namespace NP = CGAL::parameters;
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel          K;
# typedef CGAL::Surface_mesh<K::Point_3>                               Mesh;
#
# typedef boost::graph_traits<Mesh>::vertex_descriptor                 vertex_descriptor;
# typedef boost::graph_traits<Mesh>::halfedge_descriptor               halfedge_descriptor;
#
# void merge_vertices(vertex_descriptor v_keep, vertex_descriptor v_rm, Mesh& mesh)
# {
#   std::cout << "merging vertices " << v_keep << " and " << v_rm << std::endl;
#
#   for(halfedge_descriptor h : CGAL::halfedges_around_target(v_rm, mesh))
#     set_target(h, v_keep, mesh); // to ensure that no halfedge points at the deleted vertex
#
#   remove_vertex(v_rm, mesh);
# }
#
# int main(int argc, char* argv[])
# {
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path(CGALPY.data_file_path("meshes/blobby.off");)
#
#   Mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh) || CGAL::is_empty(mesh))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#
#   // Artificially create non-manifoldness for the sake of the example by merging some vertices
#   vertex_descriptor v0 = *(vertices(mesh).begin());
#   vertex_descriptor v1 = *(--(vertices(mesh).end()));
#   merge_vertices(v0, v1, mesh);
#
#   // Count non manifold vertices
#   int counter = 0;
#   for(vertex_descriptor v : vertices(mesh))
#   {
#     if(PMP::is_non_manifold_vertex(v, mesh))
#     {
#       std::cout << "vertex " << v << " is non-manifold" << std::endl;
#       ++counter;
#     }
#   }
#
#   std::cout << counter << " non-manifold occurrence(s)" << std::endl;
#
#   // Fix manifoldness by splitting non-manifold vertices
#   std::vector<std::vector<vertex_descriptor> > duplicated_vertices;
#   std::size_t new_vertices_nb = PMP::duplicate_non_manifold_vertices(mesh,
#                                                                      NP::output_iterator(
#                                                                        std::back_inserter(duplicated_vertices)));
#
#   std::cout << new_vertices_nb << " vertices have been added to fix mesh manifoldness" << std::endl;
#
#   for(std::size_t i=0; i<duplicated_vertices.size(); ++i)
#   {
#     std::cout << "Non-manifold vertex " << duplicated_vertices[i].front() << " was fixed by creating";
#     for(std::size_t j=1; j<duplicated_vertices[i].size(); ++j)
#       std::cout << " " << duplicated_vertices[i][j];
#     std::cout << std::endl;
#   }
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

def merge_vertices(v_keep, v_rm, mesh):
  print(f"merging vertices v{v_keep} and v{v_rm}")

  for h in Sm.halfedges_around_target(v_rm, mesh):
    Sm.set_target(h, v_keep, mesh) # to ensure that no halfedge points at the deleted vertex

  Sm.remove_vertex(v_rm, mesh)

filename = sys.argv[i] if len(sys.argv) > i else 'meshes/blobby.off'

try: mesh = Sm.read_polygon_mesh(filename)
except: raise ValueError("Invalid input.")

# Artificially create non-manifoldness for the sake of the example by merging some vertices
v0 = Sm.vertices(mesh)[0]
v1 = Sm.vertices(mesh)[-1]
merge_vertices(v0, v1, mesh)

# Count non manifold vertices
counter = 0
for v in Sm.vertices(mesh):
  if Pmp.is_non_manifold_vertex(v, mesh):
    print(f"vertex v{v} is non-manifold")
    counter += 1

print(f"{counter} non-manifold occurrence(s)")

# Fix manifoldness by splitting non-manifold vertices
new_vertices_nb, duplicated_vertices = Pmp.duplicate_non_manifold_vertices(mesh)

print(f"{new_vertices_nb} vertices have been added to fix mesh manifoldness")

for i in range(len(duplicated_vertices)):
  print(f"Non-manifold vertex v{duplicated_vertices[i][0]} was fixed by creating", end="")
  for j in range(1, len(duplicated_vertices[i])):
    print(f" v{duplicated_vertices[i][j]}", end="")
  print()
