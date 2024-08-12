# #include <CGAL/Simple_cartesian.h>
# #include <CGAL/Surface_mesh.h>
# #include <CGAL/Timer.h>
#
# #include <CGAL/Surface_mesh_simplification/edge_collapse.h>
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_stop_predicate.h>
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_cost.h>
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_placement.h>
#
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Bounded_normal_change_filter.h>
#
# #include <iostream>
# #include <fstream>
#
# typedef CGAL::Simple_cartesian<double> Kernel;
# typedef CGAL::Surface_mesh<Kernel::Point_3> Surface_mesh;
#
# namespace SMS = CGAL::Surface_mesh_simplification;
# struct Dummy_placement {
#
#   template <typename Profile>
#   std::optional<typename Profile::Point> operator()(const Profile&) const
#   {
#     return std::nullopt;
#   }
#
#  template <typename Profile>
#  std::optional<typename Profile::Point> operator()(const Profile&, const std::optional<typename Profile::Point>& op) const
#   {
#     return op;
#   }
#
#
# };
# int main(int argc, char** argv)
# {
#   Surface_mesh surface_mesh;
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/fold.off");
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
#   // This is a stop predicate (defines when the algorithm terminates).
#   // In this example, the simplification stops when the number of undirected edges
#   // left in the surface mesh drops below the specified number
#   const std::size_t stop_n = (argc > 2) ? std::stoi(argv[2]) : num_edges(surface_mesh)/2 - 1;
#   SMS::Edge_count_stop_predicate<Surface_mesh> stop(stop_n);
#
#   typedef SMS::LindstromTurk_placement<Surface_mesh> Placement;
#
#   CGAL::Timer t;
#   t.start();
#   // This the actual call to the simplification algorithm.
#   // The surface mesh and stop conditions are mandatory arguments.
#   // The index maps are needed because the vertices and edges
#   // of this surface mesh lack an "id()" field.
#   std::cout << "Collapsing edges of mesh: " << filename << ", aiming for " << stop_n << " final edges..." << std::endl;
#   SMS::Bounded_normal_change_filter<> filter;
#   SMS::edge_collapse(surface_mesh, stop,
#                      CGAL::parameters::get_cost(SMS::LindstromTurk_cost<Surface_mesh>())
#                                       .filter(filter)
#                                       .get_placement(Placement()));
#
#   std::cout << t.time() << " sec" << std::endl;
#   CGAL::IO::write_polygon_mesh((argc > 3) ? argv[3] : "out.off", surface_mesh, CGAL::parameters::stream_precision(17));
#
#   return EXIT_SUCCESS;
# }


import time
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

filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/fold.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print(f"Failed to read input mesh: {filename}")
    sys.exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

# This is a stop predicate (defines when the algorithm terminates).
# In this example, the simplification stops when the number of undirected edges
# left in the surface mesh drops below the specified number
stop_n = int(sys.argv[i]) if len(sys.argv) > i else Sm.num_edges(surface_mesh) // 2 - 1
i += 1
stop = Sms.Edge_count_stop_predicate(stop_n)

timer = time.perf_counter_ns()
# This the actual call to the simplification algorithm.
# The surface mesh and stop conditions are mandatory arguments.
# The index maps are needed because the vertices and edges
# of this surface mesh lack an "id()" field.
print(f"Collapsing edges of mesh: {filename}, aiming for {stop_n} final edges...")
filter = Sms.Bounded_normal_change_filter()
r = Sms.edge_collapse(surface_mesh, stop,
                      {"cost": Sms.LindstromTurk_cost(),
                       "filter": filter,
                       "get_placement": Sms.LindstromTurk_placement()
                      })
print(f"{(time.perf_counter_ns() - timer) / 1e9} sec")
Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh, {"stream_precision": 17})
i += 1

