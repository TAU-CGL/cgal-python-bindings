# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Surface_mesh.h>
#
# #include <CGAL/Polygon_mesh_processing/detect_features.h>
# #include <CGAL/Polygon_mesh_processing/IO/polygon_mesh_io.h>
#
# #include <iostream>
# #include <string>
#
# typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
# typedef CGAL::Surface_mesh<K::Point_3>                      Mesh;
# typedef boost::graph_traits<Mesh>::face_descriptor          face_descriptor;
#
# namespace PMP = CGAL::Polygon_mesh_processing;
#
# int main(int argc, char* argv[])
# {
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/P.off");
#
#   Mesh mesh;
#   if(!PMP::IO::read_polygon_mesh(filename, mesh))
#   {
#     std::cerr << "Invalid input." << std::endl;
#     return 1;
#   }
#
#   typedef boost::property_map<Mesh, CGAL::edge_is_feature_t>::type EIFMap;
#   typedef boost::property_map<Mesh, CGAL::face_patch_id_t<int> >::type PIMap;
#   typedef boost::property_map<Mesh, CGAL::vertex_incident_patches_t<int> >::type VIMap;
#
#   EIFMap eif = get(CGAL::edge_is_feature, mesh);
#   PIMap pid = get(CGAL::face_patch_id_t<int>(), mesh);
#   VIMap vip = get(CGAL::vertex_incident_patches_t<int>(), mesh);
#
#   std::size_t number_of_patches
#     = PMP::sharp_edges_segmentation(mesh, 90, eif, pid,
#                                     CGAL::parameters::vertex_incident_patches_map(vip));
#
#   std::size_t nb_sharp_edges = 0;
#   for(boost::graph_traits<Mesh>::edge_descriptor e : edges(mesh))
#   {
#     if(get(eif, e))
#       ++nb_sharp_edges;
#   }
#
#   std::cout << "This mesh contains " << nb_sharp_edges << " sharp edges" << std::endl;
#   std::cout << " and " << number_of_patches << " surface patches." << std::endl;
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
print(dir(CGALPY.CGALPY.Ker.Kernel))
Ker = CGALPY.Ker
Pmp = CGALPY.Pmp
Sm = CGALPY.Sm

filename = "meshes/P.off" if len(sys.argv) < 2 else sys.argv[1]

try:
    mesh = Sm.read_polygon_mesh(filename)
except:
    print("Invalid input.")
    exit(1)

eif = mesh.add_property_map_edge_bool("e:is_feature")[0]
pid = mesh.add_property_map_face_int("f:patch_id")[0]
vip = mesh.add_property_map_vertex_set_int("v:incident_patches")[0]

number_of_patches = Pmp.sharp_edges_segmentation(mesh, 90, eif, pid,
                                                 {"vertex_incident_patches_map": vip})

nb_sharp_edges = 0
for e in mesh.edges():
    if Sm.get(eif, e):
        nb_sharp_edges += 1

