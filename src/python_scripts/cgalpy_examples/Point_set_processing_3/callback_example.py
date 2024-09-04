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
# CGALPY = importlib.import_module(lib)
import CGALPY.CGALPY as CGALPY

class Progress_to_std_cerr_callback:
    def __init__(self, name):
        self.nb = 0
        self.timer = time.perf_counter()
        self.t_start = self.timer
        self.t_latest = self.t_start
        self.name = name

progress = Progress_to_std_cerr_callback("Computing average spacing")

def callback(advancement):
    print(f"advancement: {advancement}")
    # Avoid calling time() at every single iteration, which could
    # impact performances very badly
    progress.nb += 1
    if advancement != 1 and progress.nb % 100 != 0:
        return True
    
    t = time.perf_counter()
    if advancement == 1 or (t - progress.t_latest) > 0.1: # Update every 1/10th of second
        sys.stderr.write(f"\r{progress.name}: {int(advancement * 100)}%")
    
        if advancement == 1:
            sys.stderr.write("\n")
        progress.t_latest = t
    
    return True

N = int(sys.argv[i]) if len(sys.argv) > i else 1000
i += 1

# Generate N points on a sphere of radius 100.
points = [CGALPY.Ker.Kernel.Point_3(0, 0, 0) for _ in range(N)] # TODO: use Random_points_on_sphere_3

average_spacing = CGALPY.compute_average_spacing(points, 6, callback)



# #include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# #include <CGAL/point_generators_3.h>
# #include <CGAL/Real_timer.h>
#  
# #include <CGAL/compute_average_spacing.h>
# #include <CGAL/grid_simplify_point_set.h>
# #include <CGAL/jet_smooth_point_set.h>
#  
# #include <boost/lexical_cast.hpp>
#  
# #include <vector>
# #include <fstream>
#  
# // Types
# typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
# typedef Kernel::FT FT;
# typedef Kernel::Point_3 Point;
# typedef CGAL::Random_points_on_sphere_3<Point> Generator;
#  
# // Concurrency
# typedef CGAL::Parallel_if_available_tag Concurrency_tag;
#  
# // instance of std::function<bool(double)>
# struct Progress_to_std_cerr_callback
# {
#   mutable std::size_t nb;
#   CGAL::Real_timer timer;
#   double t_start;
#   mutable double t_latest;
#   const std::string name;
#  
#   Progress_to_std_cerr_callback (const char* name)
#     : name (name)
#   {
#     timer.start();
#     t_start = timer.time();
#     t_latest = t_start;
#   }
#  
#   bool operator()(double advancement) const
#   {
#     // Avoid calling time() at every single iteration, which could
#     // impact performances very badly
#     ++ nb;
#     if (advancement != 1 && nb % 100 != 0)
#       return true;
#  
#     double t = timer.time();
#     if (advancement == 1 || (t - t_latest) > 0.1) // Update every 1/10th of second
#     {
#       std::cerr << "\r" // Return at the beginning of same line and overwrite
#                 << name << ": " << int(advancement * 100) << "%";
#  
#       if (advancement == 1)
#         std::cerr << std::endl;
#       t_latest = t;
#     }
#  
#     return true;
#   }
# };
#  
# int main (int argc, char* argv[])
# {
#   int N = (argc > 1) ? boost::lexical_cast<int>(argv[1]) : 1000;
#  
#   // Generate N points on a sphere of radius 100.
#   std::vector<Point> points;
#   points.reserve(N);
#   Generator generator(100.);
#   std::copy_n(generator, N, std::back_inserter(points));
#  
#   // Compute average spacing
#   FT average_spacing = CGAL::compute_average_spacing<Concurrency_tag>
#     (points, 6,
#      CGAL::parameters::callback(Progress_to_std_cerr_callback("Computing average spacing")));
#  
#   // Simplify on a grid with a size of twice the average spacing
#   points.erase(CGAL::grid_simplify_point_set(points, 2. * average_spacing,
#                                              CGAL::parameters::callback(Progress_to_std_cerr_callback("Grid simplification"))),
#                points.end());
#  
#   // Smooth simplified point set
#   CGAL::jet_smooth_point_set<Concurrency_tag>(points, 6,
#                                               CGAL::parameters::callback(Progress_to_std_cerr_callback("Jet smoothing")));
#  
#   return EXIT_SUCCESS;
# }

