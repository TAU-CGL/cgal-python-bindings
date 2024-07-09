# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
# #include <CGAL/Polygon_mesh_processing/triangulate_faces.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
# #include <CGAL/boost/graph/helpers.h>
# #include <iostream>
# #include <string>
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
# typedef Kernel::Point_3                                     Point;
# typedef CGAL::Surface_mesh<Point>                           Surface_mesh;
# namespace PMP = CGAL::Polygon_mesh_processing;
# int main(int argc, char* argv[])
# {
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/P.off");
#   const char* outfilename = (argc > 2) ? argv[2] : "P_tri.off";
#   Surface_mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh))
#   {
#     std::cerr << "Error: Invalid input." << std::endl;
#     return EXIT_FAILURE;
#   }
#   if(is_empty(mesh))
#   {
#     std::cerr << "Warning: empty file?" << std::endl;
#     return EXIT_SUCCESS;
#   }
#   if(!CGAL::is_triangle_mesh(mesh))
#     std::cout << "Input mesh is not triangulated." << std::endl;
#   else
#     std::cout << "Input mesh is triangulated." << std::endl;
#   PMP::triangulate_faces(mesh);
#   // Confirm that all faces are triangles.
#   for(boost::graph_traits<Surface_mesh>::face_descriptor f : faces(mesh))
#   {
#     if(!CGAL::is_triangle(halfedge(f, mesh), mesh))
#       std::cerr << "Error: non-triangular face left in mesh." << std::endl;
#   }
#   CGAL::IO::write_polygon_mesh(outfilename, mesh, CGAL::parameters::stream_precision(17));
#   return EXIT_SUCCESS;
# }


import os
import sys
import importlib
if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]
CGALPY = importlib.import_module(lib)
Ker = CGALPY.Ker
Point_3 = Ker.Point_3
Sm = CGALPY.Sm
Pmp = CGALPY.Pmp

filename = 'meshes/P.off' if len(sys.argv) < 2 else sys.argv[1]
outfilename = 'P_tri.off' if len(sys.argv) < 3 else sys.argv[2]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    raise ValueError("Error: Invalid input.")
if mesh.is_empty():
    raise ValueError("Warning: empty file?")
if not mesh.is_triangle_mesh():
    print("Input mesh is not triangulated.")

try:
    triangulated_mesh = Pmp.triangulate_faces(mesh)
except:
    raise ValueError("Error: Could not triangulate faces.")

# Confirm that all faces are triangles.
faces = mesh.faces()
for f in faces:
    he = Sm.halfedge(f, triangulated_mesh)
    if not Sm.is_triangle(he, triangulated_mesh):
        raise ValueError("Error: non-triangular face left in mesh.")

Sm.write_polygon_mesh(outfilename, triangulated_mesh)


