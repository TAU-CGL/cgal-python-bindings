# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/Point_set_3.h>
#  
# #include <fstream>
# #include <limits>
#  
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
# typedef Kernel::FT FT;
# typedef Kernel::Point_3 Point;
# typedef Kernel::Vector_3 Vector;
#  
# typedef CGAL::Point_set_3<Point> Point_set;
#  
# void print_point_set (const Point_set& point_set)
# {
#   std::cerr << "Content of point set:" << std::endl;
#   for (Point_set::const_iterator it = point_set.begin();
#        it != point_set.end(); ++ it)
#     std::cerr << "* Point " << *it
#               << ": " << point_set.point(*it) // or point_set[it]
#               << " with normal " << point_set.normal(*it)
#               << std::endl;
# }
#  
# int main (int, char**)
# {
#   Point_set point_set;
#  
#   // Add points
#   point_set.insert (Point (0., 0., 0.));
#   point_set.insert (Point (0., 0., 1.));
#   point_set.insert (Point (0., 1., 0.));
#  
#   point_set.add_normal_map();
#  
#   print_point_set(point_set); // Normals have default values
#  
#   // Change normal values
#   Point_set::iterator it = point_set.begin();
#   point_set.normal(*(it++)) = Vector (1., 0., 0.);
#   point_set.normal(*(it++)) = Vector (0., 1., 0.);
#   point_set.normal(*(it++)) = Vector (0., 0., 1.);
#  
#   // Add point + normal
#   point_set.insert (Point (1., 2., 3.), Vector (4., 5., 6.));
#  
#   print_point_set(point_set);
#  
#   // Add new item
#   Point_set::iterator new_item = point_set.insert(Point (7., 8., 9.));
#   point_set.normal(*new_item) = Vector (10., 11., 12.);
#  
#   print_point_set(point_set); // New item has default values
#  
#   point_set.remove (point_set.begin() + 2,
#                     point_set.begin() + 4);
#  
#   print_point_set(point_set); // New item has default values
#  
#   // Display information
#   std::cerr << "Number of removed points: " <<point_set.number_of_removed_points() << std::endl;
#  
#   point_set.collect_garbage();
#  
#   // Display information (garbage should be gone)
#   std::cerr << "After garbage collection: " <<point_set.number_of_removed_points() << std::endl;
#  
#   return 0;
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
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY
Ker = CGALPY.Ker.Kernel

def print_point_set (point_set: CGALPY.Point_set_3):
    sys.stderr.write("Content of point set:\n")
    for pt in point_set:
        sys.stderr.write(f"* Point {pt}: {point_set.point(pt)} with normal {point_set.normal(pt)}\n")

point_set = CGALPY.Point_set_3()

# Add points
point_set.insert (Ker.Point_3(0., 0., 0.))
point_set.insert (Ker.Point_3(0., 0., 1.))
point_set.insert (Ker.Point_3(0., 1., 0.))

print_point_set(point_set) # Normals have default values

# Change normal values
it = next(iter(point_set))
