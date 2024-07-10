# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Polyhedron_3.h>
#  
# #include <CGAL/Polygon_mesh_processing/compute_normal.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
# // #include <CGAL/Unique_hash_map.h>
# // #include <boost/unordered_map.hpp>
#  
# #include <boost/property_map/property_map.hpp>
#  
# #include <iostream>
# #include <map>
# #include <string>
#  
# typedef CGAL::Exact_predicates_inexact_constructions_kernel   K;
# typedef K::Point_3                                            Point;
# typedef K::Vector_3                                           Vector;
#  
# typedef CGAL::Polyhedron_3<K>                                 Mesh;
# typedef boost::graph_traits<Mesh>::vertex_descriptor          vertex_descriptor;
# typedef boost::graph_traits<Mesh>::face_descriptor            face_descriptor;
#  
# namespace PMP = CGAL::Polygon_mesh_processing;
#  
# int main(int argc, char* argv[])
# {
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/eight.off");
#  
#   Mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#  
#   std::map<face_descriptor,Vector> fnormals;
#   std::map<vertex_descriptor,Vector> vnormals;
#   // Instead of std::map you may use std::unordered_map, boost::unordered_map
#   // or CGAL::Unique_hash_map
#   // CGAL::Unique_hash_map<face_descriptor,Vector> fnormals;
#   // boost::unordered_map<vertex_descriptor,Vector> vnormals;
#  
#   PMP::compute_normals(mesh, boost::make_assoc_property_map(vnormals),
#                              boost::make_assoc_property_map(fnormals));
#  
#   std::cout << "Face normals :" << std::endl;
#   for(face_descriptor fd: faces(mesh))
#     std::cout << fnormals[fd] << std::endl;
#  
#   std::cout << "Vertex normals :" << std::endl;
#   for(vertex_descriptor vd: vertices(mesh))
#     std::cout << vnormals[vd] << std::endl;
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
Point_3 = Ker.Point_3
Pmp = CGALPY.Pmp
Pol3 = CGALPY.Pol3

filename = "meshes/eight.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Pol3.read_polygon_mesh(filename)
except:
    raise ValueError("Invalid input.")

a = Pmp.compute_face_normals(mesh)
print(a)

print("Face normals :")
for fd in mesh.faces():
    print(fnormals[fd])

print("Vertex normals :")
for vd in mesh.vertices():
    print(vnormals[vd])


