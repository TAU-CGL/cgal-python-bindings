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

input_filename = sys.argv[i] if len(sys.argv) > i else CGALPY.data_file_path("points_3/before_upsample.xyz")
i += 1
output_filename = sys.argv[i] if len(sys.argv) > i else "data/before_upsample_UPSAMPLED.xyz"
i += 1

# Reads a .xyz point set file in points[], *with normals*.
success, points = CGALPY.read_points_with_normals(input_filename)
if not success:
  print("Error: cannot read file " + input_filename)
  sys.exit(1)

#Algorithm parameters
sharpness_angle = 25   # control sharpness of the result.
edge_sensitivity = 0    # higher values will sample more points near the edges
neighbor_radius = 0.25  # initial size of neighborhood.
number_of_output_points = len(points) * 4

   #Run algorithm
CGALPY.edge_aware_upsample_point_set(points, sharpness_angle=sharpness_angle, edge_sensitivity=edge_sensitivity, neighbor_radius=neighbor_radius, number_of_output_points=number_of_output_points)

# Saves point set.
success = CGALPY.write_points(output_filename, points)
if not success:
  print("Error: cannot write file " + output_filename)
  sys.exit(1)










# #include <CGAL/Simple_cartesian.h>
#
# #include <CGAL/edge_aware_upsample_point_set.h>
# #include <CGAL/IO/read_points.h>
# #include <CGAL/IO/write_points.h>
#
# #include <vector>
# #include <fstream>
#
# // types
# typedef CGAL::Simple_cartesian<double> Kernel;
# typedef Kernel::Point_3 Point;
# typedef Kernel::Vector_3 Vector;
#
# // Point with normal vector stored in a std::pair.
# typedef std::pair<Point, Vector> PointVectorPair;
#
# // Concurrency
# typedef CGAL::Parallel_if_available_tag Concurrency_tag;
#
# int main(int argc, char* argv[])
# {
#   const std::string input_filename = (argc>1) ? argv[1] : CGAL::data_file_path("points_3/before_upsample.xyz");
#   const char* output_filename = (argc>2) ? argv[2] : "data/before_upsample_UPSAMPLED.xyz";
#
#   // Reads a .xyz point set file in points[], *with normals*.
#   std::vector<PointVectorPair> points;
#   if(!CGAL::IO::read_points(input_filename,
#                             std::back_inserter(points),
#                             CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
#                                              .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())))
#   {
#     std::cerr << "Error: cannot read file " << input_filename << std::endl;
#     return EXIT_FAILURE;
#   }
#
#   //Algorithm parameters
#   const double sharpness_angle = 25;   // control sharpness of the result.
#   const double edge_sensitivity = 0;    // higher values will sample more points near the edges
#   const double neighbor_radius = 0.25;  // initial size of neighborhood.
#   const std::size_t number_of_output_points = points.size() * 4;
#
#    //Run algorithm
#   CGAL::edge_aware_upsample_point_set<Concurrency_tag>(
#     points,
#     std::back_inserter(points),
#     CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>()).
#     normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>()).
#     sharpness_angle(sharpness_angle).
#     edge_sensitivity(edge_sensitivity).
#     neighbor_radius(neighbor_radius).
#     number_of_output_points(number_of_output_points));
#
#   // Saves point set.
#   if(!CGAL::IO::write_points(output_filename, points,
#                              CGAL::parameters::point_map(CGAL::First_of_pair_property_map<PointVectorPair>())
#                                               .normal_map(CGAL::Second_of_pair_property_map<PointVectorPair>())
#                                               .stream_precision(17)))
#     return EXIT_FAILURE;
#
#   return EXIT_SUCCESS;
# }
