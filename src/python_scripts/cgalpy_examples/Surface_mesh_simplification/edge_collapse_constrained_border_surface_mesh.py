# #include <CGAL/Simple_cartesian.h>
# #include <CGAL/Surface_mesh.h>
#
# // Simplification function
# #include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#
# // Midpoint placement policy
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
#
# //Placement wrapper
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>
#
# // Stop-condition policy
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_stop_predicate.h>
#
# #include <iostream>
# #include <fstream>
#
# typedef CGAL::Simple_cartesian<double>                          Kernel;
# typedef Kernel::Point_3                                         Point_3;
#
# typedef CGAL::Surface_mesh<Point_3>                             Surface_mesh;
# typedef boost::graph_traits<Surface_mesh>::halfedge_descriptor  halfedge_descriptor;
# typedef boost::graph_traits<Surface_mesh>::edge_descriptor      edge_descriptor;
#
# namespace SMS = CGAL::Surface_mesh_simplification;
#
# // BGL property map which indicates whether an edge is marked as non-removable
# struct Border_is_constrained_edge_map
# {
#   const Surface_mesh* sm_ptr;
#   typedef edge_descriptor                                       key_type;
#   typedef bool                                                  value_type;
#   typedef value_type                                            reference;
#   typedef boost::readable_property_map_tag                      category;
#
#   Border_is_constrained_edge_map(const Surface_mesh& sm) : sm_ptr(&sm) {}
#
#   friend value_type get(const Border_is_constrained_edge_map& m, const key_type& edge) {
#     return CGAL::is_border(edge, *m.sm_ptr);
#   }
# };
#
# // Placement class
# typedef SMS::Constrained_placement<SMS::Midpoint_placement<Surface_mesh>,
#                                    Border_is_constrained_edge_map > Placement;
#
# int main(int argc, char** argv)
# {
#   Surface_mesh surface_mesh;
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/mesh_with_border.off");
#   std::ifstream is(filename);
#   if(!is || !(is >> surface_mesh))
#   {
#     std::cerr << "Failed to read input mesh: " << filename << std::endl;
#     return EXIT_FAILURE;
#   }
#
#   if(!CGAL::is_triangle_mesh(surface_mesh))
#   {
#     std::cerr << "Input geometry is not triangulated." << std::endl;
#     return EXIT_FAILURE;
#   }
#
#   Surface_mesh::Property_map<halfedge_descriptor, std::pair<Point_3, Point_3> > constrained_halfedges;
#   constrained_halfedges = surface_mesh.add_property_map<halfedge_descriptor,std::pair<Point_3, Point_3> >("h:vertices").first;
#
#   std::size_t nb_border_edges=0;
#   for(halfedge_descriptor hd : halfedges(surface_mesh))
#   {
#     if(CGAL::is_border(hd, surface_mesh))
#     {
#       constrained_halfedges[hd] = std::make_pair(surface_mesh.point(source(hd, surface_mesh)),
#                                                  surface_mesh.point(target(hd, surface_mesh)));
#       ++nb_border_edges;
#     }
#   }
#
#   // Contract the surface mesh as much as possible
#   SMS::Edge_count_stop_predicate<Surface_mesh> stop(0);
#   Border_is_constrained_edge_map bem(surface_mesh);
#
#   // This the actual call to the simplification algorithm.
#   // The surface mesh and stop conditions are mandatory arguments.
#   std::cout << "Collapsing as many edges of mesh: " << filename << " as possible..." << std::endl;
#   int r = SMS::edge_collapse(surface_mesh, stop,
#                              CGAL::parameters::edge_is_constrained_map(bem)
#                                               .get_placement(Placement(bem)));
#
#   std::cout << "\nFinished!\n" << r << " edges removed.\n"
#             << surface_mesh.number_of_edges() << " final edges.\n";
#
#   CGAL::IO::write_polygon_mesh((argc > 2) ? argv[2] : "out.off", surface_mesh, CGAL::parameters::stream_precision(17));
#
#   // now check!
#   for(halfedge_descriptor hd : halfedges(surface_mesh))
#   {
#     if(CGAL::is_border(hd,surface_mesh))
#     {
#       --nb_border_edges;
#       if(constrained_halfedges[hd] != std::make_pair(surface_mesh.point(source(hd, surface_mesh)),
#                                                      surface_mesh.point(target(hd, surface_mesh))))
#       {
#         std::cerr << "oops. send us a bug report\n";
#       }
#     }
#   }
#   assert(nb_border_edges==0);
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
Sm = CGALPY.Sm
Sms = CGALPY.Sms

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/mesh_with_border.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    sys.exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

constrained_halfedges = surface_mesh.add_property_map_halfedge_tuple("h:vertices")[0]

nb_border_edges = 0
for hd in surface_mesh.halfedges():
    if Sm.is_border(hd, surface_mesh):
        constrained_halfedges[hd] = (surface_mesh.point(surface_mesh.source(hd)),
                                     surface_mesh.point(surface_mesh.target(hd)))
        nb_border_edges += 1

 # Contract the surface mesh as much as possible
stop = Sms.Edge_count_stop_predicate(0)
bem = surface_mesh.add_property_map_edge_bool("e:is_border")[0]
for e in surface_mesh.edges():
    bem[e] = Sm.is_border(e, surface_mesh)

# This the actual call to the simplification algorithm.
# The surface mesh and stop conditions are mandatory arguments.
print(f"Collapsing as many edges of mesh: {filename} as possible...")
r = Sms.edge_collapse(surface_mesh, stop,
                      {"edge_is_constrained_map": bem,
                       "placement": Sms.Constrained_placement_Midpoint_placement_Edge_bool_map(bem)
                      })

print(f"\nFinished!\n{r} edges removed.\n",
      f"{surface_mesh.number_of_edges()} final edges.\n")

Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh, {"stream_precision": 17})
i += 1

# now check!
for hd in surface_mesh.halfedges():
    if Sm.is_border(hd, surface_mesh):
        nb_border_edges -= 1
        if constrained_halfedges[hd] != (surface_mesh.point(surface_mesh.source(hd)),
                                         surface_mesh.point(surface_mesh.target(hd))):
            print("oops. send us a bug report")
assert nb_border_edges == 0

