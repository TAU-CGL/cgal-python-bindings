#!/usr/bin/python3
# export PYTHONPATH=...
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

p1 = Point(1, 3)
p2 = Point(3, 5)
p3 = Point(5, 3)
p4 = Point(3, 1)
s1 = Segment(p1, p2)
s2 = Segment(p2, p3)
s3 = Segment(p3, p4)
s4 = Segment(p4, p1)
s5 = Segment(p1, p3)

arr = Arrangement()
e1 = arr.insert_in_face_interior(s1, arr.unbounded_face())
v1 = e1.source()
v2 = e1.target()
e2 = arr.insert_from_left_vertex(s2, v2)
v3 = e2.target()
e3 = arr.insert_from_right_vertex(s3, v3)
v4 = e3.target()
arr.insert_at_vertices(s4, v4, v1)   # return e4
arr.insert_at_vertices(s5, v1, v3)   # return e5

print_arrangement(arr)
