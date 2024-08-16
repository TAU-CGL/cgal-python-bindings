# #include <CGAL/Simple_cartesian.h>
# #include <CGAL/Surface_mesh.h>
#  
# // Simplification function
# #include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#  
# // Visitor base
# #include <CGAL/Surface_mesh_simplification/Edge_collapse_visitor_base.h>
#  
# // Stop-condition policy
# #include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Edge_count_ratio_stop_predicate.h>
#  
# #include <iostream>
# #include <fstream>
#  
# typedef CGAL::Simple_cartesian<double>                                  Kernel;
# typedef Kernel::Point_3                                                 Point_3;
#  
# typedef CGAL::Surface_mesh<Point_3>                                     Surface_mesh;
#  
# typedef boost::graph_traits<Surface_mesh>::halfedge_descriptor          halfedge_descriptor;
# typedef boost::graph_traits<Surface_mesh>::vertex_descriptor            vertex_descriptor;
#  
# namespace SMS = CGAL::Surface_mesh_simplification;
#  
# typedef SMS::Edge_profile<Surface_mesh>                                 Profile;
#  
# // The following is a Visitor that keeps track of the simplification process.
# // In this example the progress is printed real-time and a few statistics are
# // recorded (and printed in the end).
# //
# struct Stats
# {
#   std::size_t collected = 0;
#   std::size_t processed = 0;
#   std::size_t collapsed = 0;
#   std::size_t non_collapsable = 0;
#   std::size_t cost_uncomputable = 0;
#   std::size_t placement_uncomputable = 0;
# };
#  
# struct My_visitor : SMS::Edge_collapse_visitor_base<Surface_mesh>
# {
#   My_visitor(Stats* s) : stats(s) {}
#  
#   // Called during the collecting phase for each edge collected.
#   void OnCollected(const Profile&, const std::optional<double>&)
#   {
#     ++(stats->collected);
#     std::cerr << "\rEdges collected: " << stats->collected << std::flush;
#   }
#  
#   // Called during the processing phase for each edge selected.
#   // If cost is absent the edge won't be collapsed.
#   void OnSelected(const Profile&,
#                   std::optional<double> cost,
#                   std::size_t initial,
#                   std::size_t current)
#   {
#     ++(stats->processed);
#     if(!cost)
#       ++(stats->cost_uncomputable);
#  
#     if(current == initial)
#       std::cerr << "\n" << std::flush;
#     std::cerr << "\r" << current << std::flush;
#   }
#  
#   // Called during the processing phase for each edge being collapsed.
#   // If placement is absent the edge is left uncollapsed.
#   void OnCollapsing(const Profile&,
#                     std::optional<Point> placement)
#   {
#     if(!placement)
#       ++(stats->placement_uncomputable);
#   }
#  
#   // Called for each edge which failed the so called link-condition,
#   // that is, which cannot be collapsed because doing so would
#   // turn the surface mesh into a non-manifold.
#   void OnNonCollapsable(const Profile&)
#   {
#     ++(stats->non_collapsable);
#   }
#  
#   // Called after each edge has been collapsed
#   void OnCollapsed(const Profile&, vertex_descriptor)
#   {
#     ++(stats->collapsed);
#   }
#  
#   Stats* stats;
# };
#  
# int main(int argc, char** argv)
# {
#   Surface_mesh surface_mesh;
#   const std::string filename = (argc > 1) ? argv[1] : CGAL::data_file_path("meshes/small_cube.off");
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
#   // In this example, the simplification stops when the number of undirected edges
#   // drops below xx% of the initial count
#   const double ratio = (argc > 2) ? std::stod(argv[2]) : 0.1;
#   SMS::Edge_count_ratio_stop_predicate<Surface_mesh> stop(ratio);
#  
#   Stats stats;
#   My_visitor vis(&stats);
#  
#   // The index maps are not explicitelty passed as in the previous
#   // example because the surface mesh items have a proper id() field.
#   // On the other hand, we pass here explicit cost and placement
#   // function which differ from the default policies, omitted in
#   // the previous example.
#   int r = SMS::edge_collapse(surface_mesh, stop, CGAL::parameters::visitor(vis));
#  
#   std::cout << "\nEdges collected: "  << stats.collected
#             << "\nEdges processed: "  << stats.processed
#             << "\nEdges collapsed: "  << stats.collapsed
#             << std::endl
#             << "\nEdges not collapsed due to topological constraints: "  << stats.non_collapsable
#             << "\nEdge not collapsed due to cost computation constraints: "  << stats.cost_uncomputable
#             << "\nEdge not collapsed due to placement computation constraints: " << stats.placement_uncomputable
#             << std::endl;
#  
#   std::cout << "\nFinished!\n" << r << " edges removed.\n"
#             << surface_mesh.number_of_edges() << " final edges.\n";
#  
#   CGAL::IO::write_polygon_mesh((argc > 3) ? argv[3] : "out.off", surface_mesh, CGAL::parameters::stream_precision(17));
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
Sms = CGALPY.Sms
Sm = CGALPY.Sm


class Stats:
  def __init__(self):
    self.collected = 0
    self.processed = 0
    self.collapsed = 0
    self.non_collapsable = 0
    self.cost_uncomputable = 0
    self.placement_uncomputable = 0


def OnCollected(profile: Sms.Edge_profile, cost) -> None:
    stats.collected += 1
    sys.stderr.write(f"\rEdges collected: {stats.collected}")

def OnSelected(profile: Sms.Edge_profile, cost, initial: int, current: int) -> None:
    stats.processed += 1
    if cost is None:
        stats.cost_uncomputable += 1
    if current == initial:
        sys.stderr.write(f"\n")
    sys.stderr.write(f"\r{current}")

def OnCollapsing(profile: Sms.Edge_profile, placement: float | None) -> None:
    if placement is None:
        stats.placement_uncomputable += 1

def OnNonCollapsable(profile: Sms.Edge_profile) -> None:
    stats.non_collapsable += 1

def OnCollapsed(profile: Sms.Edge_profile, vertex: Sm.SM_vertex_index | None) -> None:
    stats.collapsed += 1


filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("meshes/small_cube.off")
i += 1

try:
    surface_mesh = Sm.read_polygon_mesh(filename)
except:
    print("Failed to read input mesh: ", filename)
    sys.exit(1)

if not Sm.is_triangle_mesh(surface_mesh):
    print("Input geometry is not triangulated.")
    sys.exit(1)

stats = Stats()
my_visitor = Sms.Edge_collapse_visitor_base()

Sms.set_OnCollected(my_visitor, OnCollected)
Sms.set_OnSelected(my_visitor, OnSelected)
Sms.set_OnCollapsing(my_visitor, OnCollapsing)
Sms.set_OnNonCollapsable(my_visitor, OnNonCollapsable)
Sms.set_OnCollapsed(my_visitor, OnCollapsed)


ratio = float(sys.argv[i]) if len(sys.argv) > i else 0.1
i += 1

stop = Sms.Edge_count_ratio_stop_predicate(ratio)

r = Sms.edge_collapse(surface_mesh, stop, {"visitor": my_visitor})

print(f"\nEdges collected: {stats.collected}",
      f"\nEdges processed: {stats.processed}",
      f"\nEdges collapsed: {stats.collapsed}",
      f"\n",
      f"\nEdges not collapsed due to topological constraints: {stats.non_collapsable}",
      f"\nEdge not collapsed due to cost computation constraints: {stats.cost_uncomputable}",
      f"\nEdge not collapsed due to placement computation constraints: {stats.placement_uncomputable}",
      f"\n")

print(f"Finished!\n{r} edges removed.\n{surface_mesh.number_of_edges()} final edges.\n")

Sm.write_polygon_mesh(sys.argv[i] if len(sys.argv) > i else "out.off", surface_mesh, {"stream_precision": 17})

del my_visitor



