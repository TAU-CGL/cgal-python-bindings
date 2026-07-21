from arr_print import *
from read_objects import *

if len(sys.argv) < 2: lib = 'CGALPY'
else: lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Traits = Aos2.Arr_linear_traits_2
Xcv = Traits.X_monotone_curve_2
Point = Traits.Point_2
Line = Traits.Line_2
Ker = CGALPY.Ker

#include <CGAL/Arr_curve_data_traits_2.h>
#include "arr_linear.h"

# typedef CGAL::Arr_curve_data_traits_2<Traits, size_t>   Data_traits;
# typedef Data_traits::X_monotone_curve_2                 Data_x_monotone_curve_2;
# typedef CGAL::Arrangement_2<Data_traits>                Data_arrangement;

# Get the name of the input file from the command line, or use the default
# points.dat file if no command-line parameters are given.
if len(sys.argv) > 1:
    filename = sys.argv[1]
else:
    filename = 'coll_points.dat'

points = read_objects(Point, filename)
dual_lines = []
k = 0
# std::transform(points.begin(), points.end(), dual_lines.begin(),
#                [&](const Point& p) {
#                  Line dual_line(p.x(), -1, -(p.y()))
#                  return Data_x_monotone_curve_2(dual_line, k++)
#                })
for p in points:
  dual_lines.append(Xcv(Line(p.x(), -1, -(p.y())), k))
  k += 1

# Construct the dual arrangement by aggregately inserting the lines.
arr = Arrangement()
Aos2.insert(arr, dual_lines)

# Look for vertices whose degree is greater than 4.
v: Arrangement.Vertex
for v in arr.vertices():
  if vit.degree() > 4:
    # There should be vit.degree()/2 lines intersecting at the current
    # vertex. We print their primal points and their indices.
    circ = v.incident_halfedges()
    for d in range(v.degree() / 2):
      k = next(circ).curve().data()  # The index of the primal point.
      print('Point no. {}: ({}), '.format(k+1, points[k]), end='')
    print("are collinear.")
