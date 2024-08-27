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
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY
Sm = CGALPY.Sm
Smsk = CGALPY.Smsk
Skeletonization = Smsk.Mean_curvature_flow_skeletonization
Skeleton = Skeletonization.Skeleton

tmesh = Sm.read_polygon_mesh(CGALPY.data_file_path("meshes/elephant.off"))

if not Sm.is_triangle_mesh(tmesh):
  print("Input geometry is not triangulated.")
  sys.exit(1)

skeleton = Skeleton()

Smsk.extract_mean_curvature_flow_skeleton(tmesh, skeleton)

print("Number of vertices of the skeleton:", Smsk.num_vertices(skeleton))
print("Number of edges of the skeleton:", Smsk.num_edges(skeleton))

# Output all the edges of the skeleton.
with open("skel-sm.polylines.txt", "w") as output:
  for e in skeleton.m_edges:
    s = e.m_source
    t = e.m_target
    output.write("2 {} {}\n".format(s, t))


# #include <CGAL/Simple_cartesian.h>
# #include <CGAL/Surface_mesh.h>
# #include <CGAL/extract_mean_curvature_flow_skeleton.h>
#  
# #include <fstream>
#  
#  
# typedef CGAL::Simple_cartesian<double>                        Kernel;
# typedef Kernel::Point_3                                       Point;
# typedef CGAL::Surface_mesh<Point>                             Triangle_mesh;
#  
# typedef boost::graph_traits<Triangle_mesh>::vertex_descriptor    vertex_descriptor;
#  
# typedef CGAL::Mean_curvature_flow_skeletonization<Triangle_mesh> Skeletonization;
# typedef Skeletonization::Skeleton                             Skeleton;
#  
# typedef Skeleton::vertex_descriptor                           Skeleton_vertex;
# typedef Skeleton::edge_descriptor                             Skeleton_edge;
#  
#  
# // This example extracts a medially centered skeleton from a given mesh.
# int main(int argc, char* argv[])
# {
#   std::ifstream input((argc>1)?argv[1]:CGAL::data_file_path("meshes/elephant.off"));
#   Triangle_mesh tmesh;
#   input >> tmesh;
#   if (!CGAL::is_triangle_mesh(tmesh))
#   {
#     std::cout << "Input geometry is not triangulated." << std::endl;
#     return EXIT_FAILURE;
#   }
#  
#   Skeleton skeleton;
#  
#   CGAL::extract_mean_curvature_flow_skeleton(tmesh, skeleton);
#  
#   std::cout << "Number of vertices of the skeleton: " << boost::num_vertices(skeleton) << "\n";
#   std::cout << "Number of edges of the skeleton: " << boost::num_edges(skeleton) << "\n";
#  
#   // Output all the edges of the skeleton.
#   std::ofstream output("skel-sm.polylines.txt");
#   for(Skeleton_edge e : CGAL::make_range(edges(skeleton)))
#   {
#     const Point& s = skeleton[source(e, skeleton)].point;
#     const Point& t = skeleton[target(e, skeleton)].point;
#     output << "2 " << s << " " << t << "\n";
#   }
#   output.close();
#  
#   // Output skeleton points and the corresponding surface points
#   output.open("correspondance-sm.polylines.txt");
#   for(Skeleton_vertex v : CGAL::make_range(vertices(skeleton)))
#     for(vertex_descriptor vd : skeleton[v].vertices)
#       output << "2 " << skeleton[v].point << "  " << get(CGAL::vertex_point, tmesh, vd)  << "\n";
#  
#  
#   return EXIT_SUCCESS;
# }

