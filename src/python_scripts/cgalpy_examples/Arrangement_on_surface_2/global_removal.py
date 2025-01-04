#!/usr/bin/python

import os
import sys
import importlib
from arr_print import *

if len(sys.argv) < 2:
  sys.path.append(os.path.abspath('../precompiled'))
  lib = 'CGALPY'
else:
  lib = sys.argv[1]

CGALPY = importlib.import_module(lib)
Aos2 = CGALPY.Aos2
Arrangement = Aos2.Arrangement_2
Point = Arrangement.Geometry_traits_2.Point_2
Segment = Arrangement.Geometry_traits_2.X_monotone_curve_2

# Create an arrangement of four line segments forming an H-shape:
arr = Arrangement()

s1 = Segment(Point(1, 3), Point(5, 3))
e1 = arr.insert_in_face_interior(s1, arr.unbounded_face())
s2 = Segment(Point(1, 4), Point(5, 4))
e2 = arr.insert_in_face_interior (s2, arr.unbounded_face())
Aos2.insert(arr, Segment(Point(1, 1), Point(1, 6)))
Aos2.insert(arr, Segment(Point(5, 1), Point(5, 6)))

print("The initial arrangement:")
print_arrangement_size(arr)

# Remove e1 and its incident vertices using the function remove_edge().
v1 = e1.source()
v2 = e1.target()
arr.remove_edge(e1)
Aos2.remove_vertex(arr, v1)
Aos2.remove_vertex(arr, v2)

# Remove e2 using the free remove_edge() function.
Aos2.remove_edge(arr, e2)

print("The final arrangement:")
print_arrangement_size(arr)
